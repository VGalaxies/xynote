#include "../forms/ui_xynote.h"
#include <widgets/xynote.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>

XYNote::XYNote(QWidget *parent) : QMainWindow(parent), ui(new Ui::XYNote) {
  // main
  ui->setupUi(this);

  // status
  syncState = new QLabel("unsynced");
  ui->statusBar->addPermanentWidget(syncState);
  wordCount = new QLabel("0 words");
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
      QFile file(fileName);
      file.open(QIODevice::ReadOnly);
      ui->text->setPlainText(file.readAll());
    } else {
      QMessageBox::information(this, "info", "only support markdown file");
    }
    sync();
  });

  // timer
  syncTimer = new QTimer(this);
  syncTimer->start(10000);

  // preview connect function
  connect(syncTimer, &QTimer::timeout, this, sync);
  connect(ui->actionSync, &QAction::triggered, this, sync);
  ui->actionSync->setShortcut(tr("Ctrl+S"));
  connect(ui->text, &QPlainTextEdit::textChanged, this, [=]() {
    syncState->setText("unsynced");
    wordCount->setText(tr("%1 words").arg(ui->text->toPlainText().size()));
  });

  // sider
  sider = new Sider;
}

XYNote::~XYNote() {
  delete ui;
  delete syncState;
  delete wordCount;
  delete syncTimer;
  delete sider;
}
