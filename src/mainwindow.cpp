#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  // main
  this->setWindowTitle("xynote");
  this->setFixedSize(1400, 900);

  // toolbar
  auto *toolbar = new QToolBar(this);
  this->addToolBar(Qt::TopToolBarArea, toolbar);

  // button
  auto *open_button = new QPushButton("open");
  toolbar->addWidget(open_button);

  // text editor
  auto *text = new QTextEdit(this);
  text->setFontPointSize(16);
  setCentralWidget(text);

  // dock
  auto *dock = new QDockWidget;
  addDockWidget(Qt::RightDockWidgetArea, dock);

  // view
  auto *view = new QWebEngineView;
  dock->setWidget(view);
  dock->setFixedSize(500, 900);
  dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
  dock->setWindowTitle("preview");

  // open connect function
  connect(open_button, &QPushButton::clicked, this, [=]() {
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

  // timer
  auto *timer = new QTimer(this);
  timer->start(500);

  // preview connect function
  connect(timer, &QTimer::timeout, this, [=]() {
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
