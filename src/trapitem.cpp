#include "trapitem.h"
#include <QPainter>

TrapItem::TrapItem(TrapType type, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_type(type)
{
}

TrapType TrapItem::trapType() const { return m_type; }

int TrapItem::scorePenalty() const
{
    return (m_type == TrapType::Vegetable) ? 10 : 0;
}

bool TrapItem::isFatal() const
{
    return m_type == TrapType::DietPill;
}

QString TrapItem::label() const
{
    switch (m_type) {
    case TrapType::Vegetable: return "蔬菜";
    case TrapType::DietPill:  return "减肥药";
    }
    return "";
}

QRectF TrapItem::boundingRect() const
{
    return QRectF(-15, -15, 30, 30);
}

void TrapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing);
    QColor color = (m_type == TrapType::Vegetable)
        ? QColor(80, 200, 80)
        : QColor(255, 105, 180);
    painter->setBrush(color);
    painter->setPen(QPen(color.darker(150), 2));
    painter->drawRect(boundingRect());

    painter->setPen(Qt::black);
    painter->setFont(QFont("SimHei", 7));
    painter->drawText(boundingRect(), Qt::AlignCenter, label());
}
