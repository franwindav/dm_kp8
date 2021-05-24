#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new QGraphicsScene(this))

{
    ui->setupUi(this);
    setWindowIcon(QIcon("./img/icon.svg"));
    mainAlgorithm = new MainAlgorithm(ui->plainTextEdit, ui->label_for_result);
    graph = new Graph(scene, ui->is_visible_path->isChecked());
    ui->graphicsView->setScene(scene); // установка сцены
    ui->graphicsView->setRenderHint(QPainter::Antialiasing); // сглаживание

    connect(mainAlgorithm, SIGNAL(endAlgorithm(int, std::vector<std::vector<int>>const*, int, std::vector<int>const*)), graph, SLOT(drawGraph(int, std::vector<std::vector<int>>const*, int, std::vector<int>const*)));
    connect(ui->calculateButton, SIGNAL(clicked()), mainAlgorithm, SLOT(startAlgorithm()));
    connect(mainAlgorithm, SIGNAL(emptyData()), this, SLOT(clearScene()));
    connect(mainAlgorithm, SIGNAL(emptyData()), graph, SLOT(clearGraph()));
    connect(ui->is_visible_path, SIGNAL(stateChanged(int)), graph, SLOT(display_max_path(int)));
    connect(ui->reduce, SIGNAL(clicked()), this, SLOT(reduceScale()));
    connect(ui->increase, SIGNAL(clicked()), this, SLOT(increaseScale()));
}

void MainWindow::clearScene()
{
    ui->label_for_result->setText("");
    scene->clear();
}

void MainWindow::reduceScale()
{
    if(scale > 0.3)
        scale -= 0.1;
    ui->graphicsView->resetTransform();
    ui->graphicsView->scale(scale, scale);
    scene->update();
}
void MainWindow::increaseScale()
{
    if(scale < 4)
        scale += 0.1;
    ui->graphicsView->resetTransform();
    ui->graphicsView->scale(scale, scale);
    scene->update();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
}

