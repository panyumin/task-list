#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QFile File(":style/all/style.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());

    //Initialize resources
    Q_INIT_RESOURCE(images);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    qApp->setStyleSheet(StyleSheet);
    return a.exec();
}
