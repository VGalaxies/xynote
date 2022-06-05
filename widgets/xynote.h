#pragma once

#include <QLabel>
#include <QMainWindow>
#include <QTimer>
#include <widgets/sider.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class XYNote;
}
QT_END_NAMESPACE

class XYNote : public QMainWindow {
  Q_OBJECT

public:
  explicit XYNote(QWidget *parent = nullptr);
  ~XYNote() override;

signals:
  void pathChanged(const QString &path);

private slots:
  void render();
  void open();
  void openFile(const QString &path);

private:
  Ui::XYNote *ui{};
  QLabel *syncState;
  QLabel *wordCount;
  QTimer *syncTimer;
  Sider *sider;
};
