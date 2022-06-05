#pragma once

#include <QFileSystemModel>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Sider;
}
QT_END_NAMESPACE

class Sider : public QWidget {
  Q_OBJECT

public:
  explicit Sider(QWidget *parent = nullptr);
  ~Sider() override;

private:
  Ui::Sider *ui;
  QFileSystemModel *model;
};
