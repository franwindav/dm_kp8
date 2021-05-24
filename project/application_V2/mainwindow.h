#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <graph.h>

#include<iostream>

#include "circle.h"
#include "arrow.h"
#include "mainalgorithm.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    MainAlgorithm *mainAlgorithm;
    Graph *graph;
    qreal scale = 1;

private slots:
    void clearScene();
    void reduceScale();
    void increaseScale();
};
#endif // MAINWINDOW_H
