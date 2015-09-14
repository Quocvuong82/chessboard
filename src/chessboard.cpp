#include <QApplication>
#include <QtWidgets>
#include <QLabel>
#include <QGridLayout>
#include <QGroupBox>
#include <QVector>
#include <QString>
#include <fen.h>
#include <board.h>
#include <iostream>
#include <mainwindow.h>

#include <chessdatabase.h>
#include <vector>
#include <string>

#include <boost/thread.hpp>
#include<fstream>
#include<cstdlib>
#include<sstream>
#include <sys/types.h>  // mkfifo
#include <sys/stat.h>   // mkfifo

using namespace std;

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    //QMainWindow window;
    MainWindow window;
    window.show();
    return app.exec();
}
