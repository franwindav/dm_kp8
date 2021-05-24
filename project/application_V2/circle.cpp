#include "circle.h"


Circle::Circle(qreal x, qreal y, int id)
{
    this->x = x;
    this->y = y;
    this->id = id;
}

QRectF Circle::boundingRect() const
{
    return QRectF(-radius, -radius, 2*radius, 2*radius);
}

void Circle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(color_circle); pen.setWidth(pen_width);
    painter->setPen(pen);
    painter->setBrush(color_brush);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    QString str = QString::number(id);
    QSize s = QFontMetrics(painter->font()).size(0, str);
    painter->drawText(-s.width()/2, -s.height()/2, s.width(), s.height(), 0, str);

    setPos(x, y);
    setZValue(10);

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Circle::setCircleColor(Qt::GlobalColor color, Qt::GlobalColor brush)
{
    this->color_circle = color;
    this->color_brush = brush;
}

void Circle::setWith(int w)
{
    pen_width = w;
}
