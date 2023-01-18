#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    MainWindow mainWindow;
    //mainWindow.showFullScreen();
    return application.exec();
}
