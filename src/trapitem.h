#ifndef TRAPITEM_H
#define TRAPITEM_H

#include <QGraphicsItem>

enum class TrapType {
    Vegetable,  // 蔬菜 -10分
    DietPill    // 减肥药 -1命
};

class TrapItem : public QGraphicsItem
{
public:
    explicit TrapItem(TrapType type, QGraphicsItem *parent = nullptr);

    TrapType trapType() const;
    int scorePenalty() const;
    bool isFatal() const;
    QString label() const;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

private:
    TrapType m_type;
};

#endif // TRAPITEM_H
