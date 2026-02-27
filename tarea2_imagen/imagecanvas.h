#pragma once
#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QRect>

class ImageCanvas : public QWidget {
    Q_OBJECT
public:
    explicit ImageCanvas(QWidget *parent=nullptr) : QWidget(parent) {}

    void setPixmap(const QPixmap &pm) { pixmap = pm; update(); }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter p(this);
        p.drawPixmap(QRect(0, 0, width(), height()), pixmap);
    }

private:
    QPixmap pixmap;
};
