#include "pixelart.h"
#include <QPainter>
#include <QCoreApplication>
#include <QDir>
#include <QFile>

PixelArt::PixelArt(QWidget *parent)
    : QWidget(parent)
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

    if (!m_pixmap.isNull()) {
        setFixedSize(m_pixmap.size());
    } else {
        setFixedSize(452, 452);
    }
}

void PixelArt::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), QColor(30, 30, 35));

    if (!m_pixmap.isNull()) {
        painter.drawPixmap(0, 0, m_pixmap);
    }
}
