#include "MainWindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QSplashScreen>
#include <QTimer>
#include "Sqlite.h"

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());


    if (!Sqlite::isDBExists())
    {
        Sqlite sqlite;
        sqlite.Install();
    }

    QApplication app(argc, argv);
    app.setWindowIcon(MainWindow::createIcon("house"));

    //QSplashScreen *splash = new QSplashScreen(QPixmap(":/Images/images/myhouse.jpg"),
    //                                          Qt::WindowStaysOnTopHint);
    //splash->show();

    MainWindow mainWindow;
    mainWindow.showMaximized();
    mainWindow.show();

    //QTimer::singleShot(1000, splash, SLOT(close()));

    return app.exec();
}
