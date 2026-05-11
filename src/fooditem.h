#ifndef FOODITEM_H
#define FOODITEM_H

#include <QGraphicsItem>
#include <QPixmap>

enum class FoodType {
    PaoMian,   // 泡面 +10
    MianZi,    // 焖子 +15
    BanMian,   // 板面 +20
    KeLe       // 可乐 +5
};

class FoodItem : public QGraphicsItem
{
public:
    explicit FoodItem(FoodType type, QGraphicsItem *parent = nullptr);

    FoodType foodType() const;
    int scoreValue() const;
    QString label() const;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    static QPixmap pixmapForType(FoodType type);

private:
    FoodType m_type;
    QPixmap m_pixmap;
};

#endif // FOODITEM_H
