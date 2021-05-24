#include "arrow.h"

Arrow::Arrow(qreal x1, qreal y1, qreal x2, qreal y2, qreal op, bool start, Qt::GlobalColor color)
{
    this->x1 = x1;
    this->x2 = x2;
    this->y1 = y1;
    this->y2 = y2;
    this->start = start;
    this->arrow_color = this->curent_color = color;
    this->arrow_opacity = op;
}

QRectF Arrow::boundingRect() const
{
    qreal px = x2 - x1, py = y2 - y1;
    return QRectF(-length_petal + (px > 0 ? 0 : px), -length_petal + (py > 0 ? 0 : py), std::abs(px) + 2*length_petal, std::abs(py) + 2*length_petal);
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(curent_color); pen.setWidth(pen_width);
    painter->setPen(pen);
    painter->setOpacity(arrow_opacity);
    QLineF line = QLineF(0, 0, x2 - x1, y2 - y1), tmp;
    line.setLength(line.length() - radius);
    line.setLine(line.x2(), line.y2(), line.x1(), line.y1());
    line.setLength(line.length() - (start ? radius : 0));
    tmp = line;
    painter->drawLine(line);
    line.setAngle(line.angle() - angale);
    line.setLength(length_petal);
    painter->drawLine(line);
    line = tmp;
    line.setAngle(line.angle() + angale);
    line.setLength(length_petal);
    painter->drawLine(line);
    setPos(x1, y1);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Arrow::setWidth(int w)
{
    this->pen_width = w;
}

void Arrow::setColor(Qt::GlobalColor color)
{
    this->curent_color = color;
}

void Arrow::setNormalColor()
{
    this->curent_color = arrow_color;
}
