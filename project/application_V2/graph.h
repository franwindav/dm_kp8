#ifndef GRAPH_H
#define GRAPH_H

#include <QObject>
#include <QGraphicsScene>

#include <iostream>
#include <vector>
#include <math.h>

#include <circle.h>
#include <arrow.h>
#include <ConstantVariables.h>


class Graph : public QObject
{
    Q_OBJECT
public:
    Graph(QGraphicsScene*, bool);

private:
    struct Coordinates
    {
        qreal x, y;
    };
    struct Node
    {
       std::vector<Node*> sons;
       Coordinates centre;
       int id;
       int level = -1;
       Circle *circle;
       Node *in_max_path = nullptr;
    };
    struct Level
    {
        int id;
        std::vector<Node*> nodes;
        qreal radius = 0;
        int center_x = 0;
        qreal right = 0;
    };

    void bfs(Node*);
    void distributionLevels();
    void settingCoordinates(int);
    void renderingGraph(int);
    void addMaxPath();
    void removeMaxPath();

    std::vector<Node*> nodes;
    QGraphicsScene *scene;
    std::vector<std::vector<int>> adjacency_matrix; // матрицыа смежности
    std::vector<int> max_path_index; // максимальный путь по id
    std::vector<Arrow*> max_path_arrows;
    std::vector<Circle*> max_path_circles;
    std::vector<QGraphicsEllipseItem*> max_path_dots;
    std::vector<QGraphicsLineItem*> max_path_lines;
    std::vector<Level> levels;
    int n = 0; // кол-во вершин в графе
    int length_path;
    int max_level = 0;
    qreal max_right = -40;
    qreal max_top = 0;
    qreal radius = Radius;
    qreal opacity = Opacity;
    qreal error_rate = 5;
    bool visible_max_path;
    Qt::GlobalColor ordinary_color = Main_Color;
    Qt::GlobalColor path_color = Path_Color;
    Qt::GlobalColor optional_color = Optional_Color;


private slots:
    void drawGraph(int, std::vector<std::vector<int>>const*, int, std::vector<int>const*);
    void display_max_path(int);
    void clearGraph();
};

#endif // GRAPH_H
