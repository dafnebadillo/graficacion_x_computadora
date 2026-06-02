#include <QApplication>
#include <QSurfaceFormat>
#include "glwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    GLWindow window;
    window.setWindowTitle("Ray Tracing con Shaders");
    window.resize(960, 496);
    window.show();

    return app.exec();
}