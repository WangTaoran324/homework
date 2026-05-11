#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsItem>
#include <QPixmap>

class Player : public QGraphicsItem
{
public:
    explicit Player(QGraphicsItem *parent = nullptr);

    void moveLeft();
    void moveRight();
    void setSceneWidth(int width);
    void setFatness(int score);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

private:
    QPixmap m_pixmap;
    int m_sceneWidth;
    int m_width;
    int m_height;
    int m_speed;
};

#endif
