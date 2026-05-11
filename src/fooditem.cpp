#include "fooditem.h"
#include <QPainter>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QImage>

static QImage loadAndScale(const QString &filename)
{
    QStringList dirs = {
        QCoreApplication::applicationDirPath() + "/",
        QCoreApplication::applicationDirPath() + "/../src/",
        QCoreApplication::applicationDirPath() + "/../../src/",
        "D:/Qt/eating-battle/src/",
        "D:/Qt/eating-battle/build/",
    };

    for (const auto &dir : dirs) {
        QString path = dir + filename;
        if (QFile::exists(path)) {
            QImage img(path);
            if (!img.isNull())
                return img.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }
    }
    return QImage();
}

struct FoodImageCache {
    QImage paomian, mianzi, banmian, kele;
    bool loaded = false;

    void ensure()
    {
        if (loaded) return;
        loaded = true;
        banmian = loadAndScale("banmian.png");
        paomian = loadAndScale("paomian.png");
        mianzi  = loadAndScale("mianzi.png");
        kele    = loadAndScale("kele.png");
    }
};

static FoodImageCache s_cache;

QPixmap FoodItem::pixmapForType(FoodType type)
{
    s_cache.ensure();
    switch (type) {
    case FoodType::BanMian: return QPixmap::fromImage(s_cache.banmian);
    case FoodType::PaoMian: return QPixmap::fromImage(s_cache.paomian);
    case FoodType::MianZi:  return QPixmap::fromImage(s_cache.mianzi);
    case FoodType::KeLe:    return QPixmap::fromImage(s_cache.kele);
    }
    return QPixmap();
}

FoodItem::FoodItem(FoodType type, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_type(type)
{
    QPixmap pix = pixmapForType(type);
    if (!pix.isNull())
        m_pixmap = pix;
}

FoodType FoodItem::foodType() const { return m_type; }

int FoodItem::scoreValue() const
{
    switch (m_type) {
    case FoodType::PaoMian: return 10;
    case FoodType::MianZi:  return 15;
    case FoodType::BanMian: return 20;
    case FoodType::KeLe:    return 5;
    }
    return 0;
}

QString FoodItem::label() const
{
    switch (m_type) {
    case FoodType::PaoMian: return "泡面";
    case FoodType::MianZi:  return "焖子";
    case FoodType::BanMian: return "板面";
    case FoodType::KeLe:    return "可乐";
    }
    return "";
}

QRectF FoodItem::boundingRect() const
{
    if (!m_pixmap.isNull())
        return QRectF(-m_pixmap.width() / 2.0, -m_pixmap.height() / 2.0,
                       m_pixmap.width(), m_pixmap.height());
    return QRectF(-15, -15, 30, 30);
}

void FoodItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!m_pixmap.isNull()) {
        painter->drawPixmap(
            -m_pixmap.width() / 2, -m_pixmap.height() / 2,
            m_pixmap);
    } else {
        painter->setRenderHint(QPainter::Antialiasing);
        QColor color;
        switch (m_type) {
        case FoodType::PaoMian: color = QColor(255, 200, 50); break;
        case FoodType::MianZi:  color = QColor(139, 90, 43); break;
        case FoodType::BanMian: color = QColor(245, 230, 200); break;
        case FoodType::KeLe:    color = QColor(220, 50, 50); break;
        }
        painter->setBrush(color);
        painter->setPen(QPen(color.darker(150), 2));
        painter->drawEllipse(boundingRect());
        painter->setPen(Qt::black);
        painter->setFont(QFont("SimHei", 8));
        painter->drawText(boundingRect(), Qt::AlignCenter, label());
    }
}
