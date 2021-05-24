#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsObject>
#include <QPainter>

#include <ConstantVariables.h>

#include <iostream>

class Arrow : public QGraphicsObject
{
    Q_OBJECT

public:
    Arrow(qreal, qreal, qreal, qreal, qreal = 1, bool = true, Qt::GlobalColor = Main_Color);
    void setNormalColor();
    void setColor(Qt::GlobalColor);
    void setWidth(int);


protected:
    QRectF boundingRect() const;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

private:
    qreal x1, x2, y1, y2;
    qreal arrow_opacity;
    int pen_width = Pen_Width;
    const qreal radius = Radius;
    const qreal angale = Arrow_Angle;
    const qreal length_petal = Length_Petal;
    bool start;
    Qt::GlobalColor arrow_color;
    Qt::GlobalColor curent_color;
};


#endif // ARROW_H
