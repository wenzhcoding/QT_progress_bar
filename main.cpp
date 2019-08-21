#include "RulerProgress.h"
#include "WanderEdit.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RulerProgress w;
    w.show();
    return a.exec();
}
