#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QPainter>
#include <QRect>
#include <QSize>
#include <QImage>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private:
    Ui::MainWindow *ui;
    static constexpr int H = 457;
    static constexpr int W = 600;
    unsigned char rgb[H][W][3];   // arreglo
    QPixmap pixmap;
};
#endif // MAINWINDOW_H
