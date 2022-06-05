#include "../forms/ui_sider.h"
#include <widgets/sider.h>

void Sider::changeDir(const QString &path) {
  currentPath = path.isEmpty() ? QDir::currentPath() : path;
  ui->path->setText(currentPath.right(36));
  QDir::setCurrent(currentPath);
  auto index = model->setRootPath(currentPath);
  ui->directory->setRootIndex(index);
}

void Sider::onUpper() {
  auto info = QFileInfo(currentPath);
  changeDir(info.path());
}

void Sider::onDoubleClicked(const QModelIndex &index) {
  auto fileName = model->filePath(index);
  auto fileInfo = QFileInfo(fileName);
  if (fileInfo.isDir()) {
    changeDir(fileInfo.filePath());
  } else {
    emit fileChanged(fileInfo.filePath());
  }
}

Sider::Sider(QWidget *parent)
    : QWidget(parent), ui(new Ui::Sider), model(new QFileSystemModel(this)),
      currentPath(QDir::homePath()) {
  // ui
  ui->setupUi(this);

  // set model
  QDir::setCurrent(currentPath);
  ui->path->setText(currentPath.right(36));
  model->setRootPath(currentPath);
  ui->directory->setModel(model);

  // set index
  auto index = model->index(currentPath);
  ui->directory->setRootIndex(index);

  // hidden
  ui->directory->setHeaderHidden(true);
  for (auto i = 1; i <= 3; i++) {
    ui->directory->setColumnHidden(i, true);
  }

  // button icon
  ui->upper->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));

  // connections
  connect(ui->upper, &QPushButton::clicked, this, &Sider::onUpper);
  connect(ui->directory, &QTreeView::doubleClicked, this,
          &Sider::onDoubleClicked);
}

Sider::~Sider() {
  delete ui;
  delete model;
}
