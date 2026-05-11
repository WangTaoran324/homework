#include "player.h"
#include <QPainter>
#include <QCoreApplication>
#include <QFile>

Player::Player(QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , m_sceneWidth(800), m_width(80), m_height(80), m_speed(12)
{
    QStringList paths = {
        QCoreApplication::applicationDirPath() + "/liangzi.png",
        QCoreApplication::applicationDirPath() + "/../src/liangzi.png",
        "D:/Qt/eating-battle/src/liangzi.png",
    };
    for (const auto &path : paths) {
        if (QFile::exists(path)) {
            m_pixmap.load(path);
            break;
        }
    }
    setPos(360, 500);
}

QRectF Player::boundingRect() const
{
    return QRectF(-m_width / 2, -m_height / 2, m_width, m_height);
}

void Player::setSceneWidth(int w) { m_sceneWidth = w; }

void Player::setFatness(int score)
{
    prepareGeometryChange();
    m_width = 80 + score / 10;
    if (m_width > 140) m_width = 140;
    m_height = m_width;
}

void Player::moveLeft()
{
    int nx = x() - m_speed;
    if (nx - m_width / 2 < 0) nx = m_width / 2;
    setPos(nx, y());
}

void Player::moveRight()
{
    int nx = x() + m_speed;
    if (nx + m_width / 2 > m_sceneWidth) nx = m_sceneWidth - m_width / 2;
    setPos(nx, y());
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!m_pixmap.isNull()) {
        painter->drawPixmap(
            -m_width / 2, -m_height / 2,
            m_width, m_height, m_pixmap
        );
    }
}
