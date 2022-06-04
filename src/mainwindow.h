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
#include <unistd.h>

class MainWindow : public QMainWindow {
  Q_OBJECT

private:
  QString file_name{};

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;
};
