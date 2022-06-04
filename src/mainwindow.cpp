#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  this->setFixedSize(800, 600);

  auto *toolbar = new QToolBar(this);
  this->addToolBar(Qt::LeftToolBarArea, toolbar);

  auto *button = new QPushButton("open");
  toolbar->addWidget(button);

  auto *text = new QTextEdit(this);
  setCentralWidget(text);

  auto *dock = new QDockWidget;
  auto *view = new QWebEngineView;
  dock->setWidget(view);
  addDockWidget(Qt::RightDockWidgetArea, dock);

  connect(button, &QPushButton::clicked, this, [=]() {
    this->file_name = QFileDialog::getOpenFileName(this, "open");
    QFileInfo file_info(this->file_name);
    QString suffix = file_info.suffix();
    //    qDebug() << suffix;
    if (suffix == "md") {
      QFile ori_file(this->file_name);
      ori_file.open(QIODevice::ReadOnly);
      text->setText(ori_file.readAll());
    } else {
      QMessageBox::information(this, "info", "only support markdown file");
    }
  });

  connect(text, &QTextEdit::textChanged, this, [=]() {
    QString tmp_file_name = this->file_name + ".tmp";

    QFile tmp_file(tmp_file_name);
    tmp_file.open(QFile::NewOnly);
    tmp_file.write(text->toPlainText().toUtf8());

    QProcess process;
    process.start("pandoc", QStringList() << "-t"
                                          << "html" << tmp_file_name);
    process.waitForFinished();

    view->setHtml(process.readAllStandardOutput());
  });
}

MainWindow::~MainWindow() = default;
