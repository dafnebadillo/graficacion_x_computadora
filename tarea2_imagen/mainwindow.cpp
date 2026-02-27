#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <algorithm>
#include "imagecanvas.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto *canvas = new ImageCanvas(this);
    setCentralWidget(canvas);

    canvas -> setFixedSize(W,H);
    resize(W,H);

    // leer la imaghen con qimage
    QImage img("/Users/dafnebadillo/tarea2_imagen/Pixar_UP.png"); //aqui va la direccion depende dde donde este la imagen
    if (img.isNull()) {
        // por sii falla la lectura deja la ventana negra
        std::fill(&rgb[0][0][0], &rgb[0][0][0] + (W*H*3), 0);
        QImage black(&rgb[0][0][0], W, H, W*3, QImage::Format_RGB888);
        canvas->setPixmap(QPixmap::fromImage(black));
        return;
    }

    //resize de la ventana
    img = img.scaled(W, H);

    //formato rgb
    img = img.convertToFormat(QImage::Format_RGB888);

    for (int y = 0; y < H; ++y) {
        const unsigned char* line = img.constScanLine(y);
        for (int x = 0; x < W; ++x) {
            int i = x * 3;
            rgb[y][x][0] = line[i + 0]; // R
            //rgb[y][x][1] = line[i + 1]; // G
            //rgb[y][x][2] = line[i + 2]; // B
            // hacer la imagen rojiza
            rgb[y][x][1] = 0;
            rgb[y][x][2] = 0;
        }
    }

    QImage imgFromArray(&rgb[0][0][0], W, H, W * 3, QImage::Format_RGB888);
    QPixmap pm = QPixmap::fromImage(imgFromArray); //dibujar con qpixmap
    canvas->setPixmap(pm);
    setWindowTitle(pm.isNull() ? "Pixmap VACIO" : "OK");

}

MainWindow::~MainWindow()
{
    delete ui;
}


