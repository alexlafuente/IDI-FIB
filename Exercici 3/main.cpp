#include <QApplication>
#include "Sinus.h"
int main (int argc, char **argv) {
    QApplication app(argc,argv);
    Sinus sinus;
    sinus.show();
    return app.exec();
}
