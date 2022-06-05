#include "xynote.h"
#include "../forms/ui_xynote.h"

XYNote::XYNote(QWidget *parent) : QMainWindow(parent), ui(new Ui::XYNote) {
  // main
  ui->setupUi(this);

  // status
  auto *syncState = new QLabel("unsynced"); // mem leak
  ui->statusBar->addPermanentWidget(syncState);
  auto *wordCount = new QLabel("0 words"); // mem leak
  ui->statusBar->addWidget(wordCount);

  // sync
  auto sync = [=]() {
    QProcess process;
    process.start("pandoc", QStringList() << "-f"
                                          << "markdown"
                                          << "-t"
                                          << "html");

    process.write(ui->text->toPlainText().toUtf8());
    process.closeWriteChannel();

    if (process.waitForFinished()) {
      ui->view->setHtml(process.readAllStandardOutput());
      syncState->setText("synced");
    } else {
      QMessageBox::warning(this, "warning", "internal error");
    }
  };

  // open connect function
  connect(ui->actionOpen, &QAction::triggered, this, [=]() {
    QString fileName = QFileDialog::getOpenFileName(this, "open");
    QFileInfo fileInfo(fileName);
    QString suffix = fileInfo.suffix();
    if (suffix == "md") {
      QFile ori_file(fileName);
      ori_file.open(QIODevice::ReadOnly);
      ui->text->setPlainText(ori_file.readAll());
    } else {
      QMessageBox::information(this, "info", "only support markdown file");
    }
    sync();
  });

  // timer
  auto *timer = new QTimer(this);
  timer->start(3000);

  // preview connect function
  connect(timer, &QTimer::timeout, this, sync);
  connect(ui->actionSync, &QAction::triggered, this, sync);
  connect(ui->text, &QPlainTextEdit::textChanged, this, [=]() {
    syncState->setText("unsynced");
    wordCount->setText(tr("%1 words").arg(ui->text->toPlainText().size()));
  });
}

XYNote::~XYNote() {
  delete ui;
}
