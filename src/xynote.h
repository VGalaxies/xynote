#pragma once

#include <QMainWindow>
#include <QFileDialog>
#include <QMenuBar>
#include <QPushButton>
#include <QTextEdit>
#include <QToolBar>
#include <QDebug>
#include <QMessageBox>
#include <QtWebEngineWidgets>

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

private:
  Ui::XYNote *ui{};
};
