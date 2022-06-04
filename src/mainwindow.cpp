#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  this->setWindowTitle("xynote");
  this->setFixedSize(1400, 900);

  auto *toolbar = new QToolBar(this);
  this->addToolBar(Qt::TopToolBarArea, toolbar);

  auto *button = new QPushButton("open");
  toolbar->addWidget(button);

  auto *text = new QTextEdit(this);
  setCentralWidget(text);

  auto *dock = new QDockWidget;
  auto *view = new QWebEngineView;
  dock->setWidget(view);
  dock->setFixedSize(500, 900);
  dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
  dock->setWindowTitle("preview");
  addDockWidget(Qt::RightDockWidgetArea, dock);

  connect(button, &QPushButton::clicked, this, [=]() {
    this->file_name = QFileDialog::getOpenFileName(this, "open");
    QFileInfo file_info(this->file_name);
    QString suffix = file_info.suffix();
    if (suffix == "md") {
      QFile ori_file(this->file_name);
      ori_file.open(QIODevice::ReadOnly);
      text->setText(ori_file.readAll());
    } else {
      QMessageBox::information(this, "info", "only support markdown file");
    }
  });

  connect(text, &QTextEdit::textChanged, this, [=]() {
    char temp[] = "/tmp/xynote/XXXXXX.tmp";
    int fd = mkstemps(temp, 4);

    QFile tmp_file;
    tmp_file.open(fd, QFile::NewOnly); // note flag
    tmp_file.write(text->toPlainText().toUtf8());
    tmp_file.close();

    QProcess process;
    process.start("pandoc", QStringList() << "-f"
                                          << "markdown"
                                          << "-t"
                                          << "html" << temp);

    if (process.waitForFinished()) {
      QByteArray array = process.readAllStandardOutput();
      view->setHtml(array);
    } else {
      QMessageBox::information(this, "info", "internal error");
    }
  });
}

MainWindow::~MainWindow() = default;
