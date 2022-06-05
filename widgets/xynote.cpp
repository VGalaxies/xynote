#include "../forms/ui_xynote.h"
#include <widgets/xynote.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>

void XYNote::render() {
  QProcess process;
  process.start("pandoc", QStringList() << "-f"
                                        << "markdown"
                                        << "-t"
                                        << "html");

  process.write(ui->text->toPlainText().toUtf8());
  process.closeWriteChannel(); // EOF

  if (process.waitForFinished()) {
    ui->view->setHtml(process.readAllStandardOutput());
    syncState->setText("synced");
  } else {
    QMessageBox::warning(this, "warning", "internal error");
  }
}

void XYNote::openFile(const QString &path) {
  QFileInfo fileInfo(path);
  QString suffix = fileInfo.suffix();
  if (suffix == "md") {
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    ui->text->setPlainText(file.readAll());
    this->render();
    emit pathChanged(fileInfo.path());
  } else {
    QMessageBox::information(this, "info", "only support markdown file");
  }
}

void XYNote::open() {
  QString fileName = QFileDialog::getOpenFileName(this, "open");
  this->openFile(fileName);
}

XYNote::XYNote(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::XYNote),
      syncState(new QLabel("unsynced")), wordCount(new QLabel("0 words")),
      syncTimer(new QTimer), sider(new Sider) {
  // ui
  ui->setupUi(this);

  // status
  ui->statusBar->addPermanentWidget(syncState);
  ui->statusBar->addWidget(wordCount);
  connect(ui->text, &QPlainTextEdit::textChanged, this, [=]() {
    syncState->setText("unsynced");
    wordCount->setText(tr("%1 words").arg(ui->text->toPlainText().size()));
  });

  // open
  connect(ui->actionOpen, &QAction::triggered, this, &XYNote::open);

  // render
  syncTimer->start(10000);
  connect(syncTimer, &QTimer::timeout, this, &XYNote::render);
  connect(ui->actionSync, &QAction::triggered, this, &XYNote::render);
  ui->actionSync->setShortcut(tr("Ctrl+S"));

  // connect sider
  connect(this, &XYNote::pathChanged, ui->sider, &Sider::changeDir);
  connect(ui->sider, &Sider::fileChanged, this, &XYNote::openFile);
}

XYNote::~XYNote() {
  delete ui;
  delete syncState;
  delete wordCount;
  delete syncTimer;
  delete sider;
}
