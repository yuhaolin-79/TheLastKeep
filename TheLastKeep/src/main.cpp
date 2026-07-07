#include <QApplication>

#include <ui/Mainwindow.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec(); //进入Qt事件循环
}
