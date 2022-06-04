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
  auto *sync_button = new QPushButton("sync");
  toolbar->addWidget(sync_button);

  // text editor
  auto *text = new QTextEdit(this);
  text->setFontPointSize(16);
  setCentralWidget(text);
  connect(text, &QTextEdit::textChanged, this, [=]() {
    sync_button->setText("sync");
  });

  // dock
  auto *dock = new QDockWidget;
  addDockWidget(Qt::RightDockWidgetArea, dock);

  // view
  auto *view = new QWebEngineView;
  dock->setWidget(view);
  dock->setFixedSize(600, 900);
  dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
  dock->setWindowTitle("preview");

  auto sync = [=]() {
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
      sync_button->setText("synced");
    } else {
      QMessageBox::information(this, "info", "internal error");
    }
  };

  // open connect function
  connect(open_button, &QPushButton::clicked, this, [=]() {
    QString ori_file_name = QFileDialog::getOpenFileName(this, "open");
    QFileInfo file_info(ori_file_name);
    QString suffix = file_info.suffix();
    if (suffix == "md") {
      QFile ori_file(ori_file_name);
      ori_file.open(QIODevice::ReadOnly);
      text->setText(ori_file.readAll());
    } else {
      QMessageBox::information(this, "info", "only support markdown file");
    }
    sync();
  });

  // timer
  auto *timer = new QTimer(this);
  timer->start(10000);

  // preview connect function
  connect(timer, &QTimer::timeout, this, sync);
  connect(sync_button, &QPushButton::clicked, this, sync);
}

MainWindow::~MainWindow() = default;
