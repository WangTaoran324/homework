#ifndef PIXELART_H
#define PIXELART_H

#include <QWidget>
#include <QPixmap>

class PixelArt : public QWidget
{
    Q_OBJECT

public:
    explicit PixelArt(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap m_pixmap;
};

#endif
