#include "xynote.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app{argc, argv};
  XYNote xynote{};

  QApplication::setWindowIcon(QIcon{":icon.svg"});
  xynote.show();
  return QApplication::exec();
}
