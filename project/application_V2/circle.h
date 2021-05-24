#ifndef CIRCLE_H
#define CIRCLE_H

#include <QGraphicsObject>
#include <QPainter>

#include <ConstantVariables.h>

class Circle : public QGraphicsObject
{
    Q_OBJECT

public:
    Circle(qreal, qreal, int);
    void setCircleColor(Qt::GlobalColor = Main_Color, Qt::GlobalColor = Main_Circle_Brush);
    void setWith(int = Pen_Width);

protected:
    QRectF boundingRect() const;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

private:
    qreal x, y;
    int id = 0;
    int pen_width = Pen_Width;
    qreal radius = Radius;
    Qt::GlobalColor color_circle = Main_Color;
    Qt::GlobalColor color_brush = Main_Circle_Brush;
};

#endif // CIRCLE_H
