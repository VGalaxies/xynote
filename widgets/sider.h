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

public slots:
  void changeDir(const QString &path);

private slots:
  void onUpper();
  void onDoubleClicked(const QModelIndex &index);

signals:
  void fileChanged(const QString &path);

private:
  Ui::Sider *ui;
  QFileSystemModel *model;
  QString currentPath;
};
