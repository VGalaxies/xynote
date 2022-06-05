#include "../forms/ui_sider.h"
#include <widgets/sider.h>

#include <QDebug>

Sider::Sider(QWidget *parent) : QWidget(parent), ui(new Ui::Sider) {
  ui->setupUi(this);

  model = new QFileSystemModel(this);
  model->setRootPath(QDir::homePath());
  ui->directory->setModel(model);

  ui->pushButton->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));
}

Sider::~Sider() {
  delete ui;
  delete model;
}
