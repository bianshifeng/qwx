// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include "circleimage.h"

CircleImage::CircleImage(QQuickItem* parent)
  : QQuickPaintedItem(parent), 
    m_color(Qt::white), 
    m_imageSource("") 
{
}

CircleImage::~CircleImage() 
{
}

QColor CircleImage::color() const { return m_color; }
void CircleImage::setColor(const QColor & color) 
{
    if (m_color != color) {
        m_color = color;
        Q_EMIT colorChanged();
        update();
    }
}

QString CircleImage::imageSource() const { return m_imageSource; }
void CircleImage::setImageSource(const QString & imageSource) 
{
    if (m_imageSource != imageSource) {
        m_imageSource = imageSource;
        if (m_imageSource.contains("qrc:/qml")) 
            m_imageSource.replace("qrc:/qml", ":");
        else if (m_imageSource.contains("file://")) 
            m_imageSource.replace("file://", "");
        Q_EMIT imageSourceChanged();
        update();
    }
}

void CircleImage::paint(QPainter* painter) 
{
    QFile file(m_imageSource);
    if (!file.exists())
        return;
    QImage image(m_imageSource);
    if (image.isNull()) return;
	QPainterPath path;
    path.addEllipse(0, 0, width(), height());
    painter->setRenderHints(QPainter::Antialiasing);
    painter->setRenderHints(QPainter::SmoothPixmapTransform);
	painter->setClipping(true);
	painter->setClipPath(path);
	painter->drawImage(0, 0, image.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	painter->clipPath();
}
