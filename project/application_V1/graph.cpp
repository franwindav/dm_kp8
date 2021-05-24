#include "graph.h"

Graph::Graph(QGraphicsScene *scene, bool is)
{
    this->scene = scene;
    this->visible_max_path = is;
}

void Graph::drawGraph(int n, std::vector<std::vector<int>> const*matrix, int length, std::vector<int> const*path)
{
    scene->clear();
    scene->setSceneRect(0, 0, 0, 0);
    clearGraph();
    this->adjacency_matrix = *matrix; this->n = n; this->max_path_index = *path; this->length_path = length;
    distributionLevels();
    for(int i = 0; i < max_level; i++)
        settingCoordinates(i);
    for(int k = 0; k < length_path - 1; k++)
    {
        nodes[max_path_index[k]]->in_max_path = nodes[max_path_index[k + 1]];
        max_path_circles.push_back(nodes[max_path_index[k]]->circle);
    }
    max_path_circles.push_back(nodes[max_path_index[length_path - 1]]->circle);
    for(int i = 0; i < max_level; i++)
        renderingGraph(i);
    scene->setSceneRect(-radius, -max_top, max_right + 70, max_top * 2);
    if(visible_max_path)
        addMaxPath();
    for(Node *i : nodes)
        delete i;
    nodes.clear();
    levels.clear();
    adjacency_matrix.clear();
    max_path_index.clear();
    max_top = 0, max_right = -radius * 2, max_level = 0, this->n = 0;
}

void Graph::settingCoordinates(int id)
{
    if(id == 0)
        levels[id].center_x =  -radius * 2;
    else if(levels[id - 1].nodes.size() == 1)
        levels[id].center_x = levels[id-1].center_x + 4*radius;
    else
        levels[id].center_x = levels[id-1].center_x + levels[id-1].radius + radius;

    levels[id].radius = radius * 2 + 4*radius * levels[id].nodes.size() / 2;
    max_right = std::max(levels[id].center_x + levels[id].radius, max_right);
    if(levels[id].nodes.size() == 1)
    {
        levels[id].nodes[0]->centre = {levels[id].center_x + levels[id].radius, 0};
        levels[id].right = levels[id].center_x + levels[id].radius;
        levels[id].nodes[0]->circle = new Circle(levels[id].nodes[0]->centre.x, levels[id].nodes[0]->centre.y, levels[id].nodes[0]->id);
        return;
    }
    double t = std::acos(-1) / (levels[id].nodes.size() + 1);
    double k = std::asin(-1) + t;
    for(unsigned long long j = 0; j < levels[id].nodes.size(); j++, k += t)
    {
        levels[id].nodes[j]->centre = {levels[id].center_x + levels[id].radius * cos(k), levels[id].radius * sin(k)};
        max_top = std::max(std::abs(levels[id].radius * sin(k)) + 30, max_top);
        levels[id].nodes[j]->circle = new Circle(levels[id].nodes[j]->centre.x, levels[id].nodes[j]->centre.y, levels[id].nodes[j]->id);
        levels[id].right = std::max(levels[id].nodes[j]->centre.x, levels[id].right);
    }

}

void Graph::distributionLevels()
{
    nodes = std::vector<Node*>(n);
    for(int i = 0; i < n; i++)
        nodes[i] = new Node;
    for (int i = 0; i < n; i++)
    {
        nodes[i]->id = i;
        for (int j = 0; j < n; j++)
            if (adjacency_matrix[i][j] > 0)
                nodes[i]->sons.push_back(nodes[j]);
    }
    nodes[max_path_index[0]]->level = 0;
    bfs(nodes[max_path_index[0]]);
    for(int i = 0; i < n; i++)
        if(nodes[i]->level == -1)
        {
           for(Node* t : nodes[i]->sons)
               if(t->level != -1)
               {
                   nodes[i]->level = t->level - 1;
                   break;
               }
           bfs(nodes[i]);
        }
    max_level++;
    levels = std::vector<Level>(max_level);
    for(Node* i : nodes)
    {
        levels[i->level].id = i->level;
        levels[i->level].nodes.push_back(i);
    }
}

void Graph::bfs(Node *node)
{
    max_level = std::max(max_level, node->level);
    std::vector<Node*> next;
    for(Node* i : node->sons)
        if(i->level >= node->level + 1 || i->level == -1)
        {
            i->level = node->level + 1;
            next.push_back(i);
        }
    for(Node *i : next)
        bfs(i);
}

void  Graph::renderingGraph(int id)
{
    for(Node *i : levels[id].nodes)
    {
        int y = 0;
        QGraphicsLineItem *li = nullptr;
        QGraphicsEllipseItem *ei = nullptr;
        scene->addItem(i->circle);
        for(Node *j : i->sons)
        {
            if(j->level == i->level)
            {
                Arrow *arrow = nullptr;
                scene->addItem(arrow = new Arrow(i->centre.x, i->centre.y, j->centre.x, j->centre.y));
                if(i->in_max_path == j)
                    max_path_arrows.push_back(arrow);
            }
            else if(levels[id].right - i->centre.x > error_rate)
            {
                Arrow *arrow = nullptr;
                if(j->level > i->level)
                {
                    y++;
                    if(y == 1)
                        li = scene->addLine(QLineF(i->centre.x + radius, i->centre.y, levels[id].right + radius, i->centre.y));
                    if(i->in_max_path == j && li)
                    {
                        max_path_lines.push_back(li);
                        li = nullptr;
                    }
                    if(y == 2)
                    {
                        QPen pen(ordinary_color); pen.setWidth(Pen_Width);
                        ei = scene->addEllipse(levels[id].right + radius - Dot_Radius, i->centre.y - Dot_Radius, Dot_Radius * 2, Dot_Radius * 2, pen, QBrush(Main_Dote_Brush));
                        ei->setZValue(10);
                    }
                    if(i->in_max_path == j && ei)
                    {
                        max_path_dots.push_back(ei);
                        ei = nullptr;
                    }
                    scene->addItem(arrow = new Arrow(levels[id].right + radius, i->centre.y, j->centre.x, j->centre.y, 1, false));
                }
                if(j->level < i->level)
                    scene->addItem(arrow = new Arrow(i->centre.x, i->centre.y, j->centre.x, j->centre.y, Opacity, true, optional_color));
                if(i->in_max_path == j && arrow)
                    max_path_arrows.push_back(arrow);
            }
            else
            {
                Arrow *arrow = nullptr;
                if(j->level > i->level)
                    scene->addItem(arrow = new Arrow(i->centre.x, i->centre.y, j->centre.x, j->centre.y));
                if(j->level < i->level)
                    scene->addItem(arrow = new Arrow(i->centre.x, i->centre.y, j->centre.x, j->centre.y, Opacity, true, optional_color));
                if(i->in_max_path == j && arrow)
                    max_path_arrows.push_back(arrow);
            }
        }
    }
}

void Graph::display_max_path(int is)
{
    if(is > 0)
        addMaxPath();
    else
        removeMaxPath();
    scene->update();
}

void Graph::addMaxPath()
{
    visible_max_path = true;
    for(Circle *i : max_path_circles)
    {
        i->setCircleColor(path_color);
        i->setWith(Path_Pen_Width);
        i->update();
    }
    for(Arrow *i : max_path_arrows)
    {
        i->setColor(path_color);
        i->setWidth(Path_Pen_Width);
        i->update();
    }
    for(QGraphicsLineItem *i : max_path_lines)
    {
        QPen pen = QPen(path_color); pen.setWidth(Path_Pen_Width);
        i->setPen(pen);
        i->update();
    }
    for(QGraphicsEllipseItem *i : max_path_dots)
    {
        QPen pen = QPen(path_color); pen.setWidth(Path_Pen_Width);
        i->setPen(pen);
        i->update();
    }
}

void Graph::removeMaxPath()
{
    visible_max_path = false;;
    for(Circle *i : max_path_circles)
    {
        i->setCircleColor();
        i->setWith();
        i->update();
    }
    for(Arrow *i : max_path_arrows)
    {
        i->setNormalColor();
        i->setWidth(Pen_Width);
        i->update();
    }
    for(QGraphicsLineItem *i : max_path_lines)
    {
        QPen pen = QPen(ordinary_color); pen.setWidth(Pen_Width);
        i->setPen(pen);
        i->update();
    }
    for(QGraphicsEllipseItem *i : max_path_dots)
    {
        QPen pen = QPen(ordinary_color); pen.setWidth(Pen_Width);
        i->setPen(pen);
        i->update();
    }
}

void Graph::clearGraph()
{
    max_path_arrows.clear();
    max_path_lines.clear();
    max_path_dots.clear();
    max_path_circles.clear();
}
