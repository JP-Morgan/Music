#include "My_Music.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MyMusic window;
    window.show();
    return app.exec();
}
