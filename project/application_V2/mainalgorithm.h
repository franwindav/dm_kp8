#ifndef MAINALGORITHM_H
#define MAINALGORITHM_H

#include <QObject>
#include <QPlainTextEdit>
#include <QLabel>
#include <QMessageBox>
#include <QIcon>

#include <vector>

class MainAlgorithm : public QObject
{
    Q_OBJECT

public:
    MainAlgorithm(QPlainTextEdit*, QLabel*);

private:
    struct Node
    {
        std::vector<Node*> son, father; // ссылки на исходящие и входящие вершины
        unsigned long long unused_father = 0; // маска, отвечающая за то какие отцы были пройдены
        int id, power = 0, cnt; // id --- номер, power --- максимальный путь до этой вершины, cnt --- какая по чету в максимальном пути
        bool used = false; // была ли обработана уже
        Node *prev_node = nullptr; // нужна для востановления пути

    };

    void algorithm(Node*); // функция для нахождения максимального пути
    bool getVector(QString); // позвляет получить исходные данные
    void showError(QString);

    QPlainTextEdit *text;
    QLabel *label_for_result;
    std::vector<std::vector<int>> adjacency_matrix; // матрицыа смежности
    std::vector<Node> nodes; // вершины
    std::vector<int> max_path; // максимальный путь по id
    int n = 0; // кол-во вершин в графе
    Node *last_element_path = nullptr; // крайний элемент в найденом пути
    int result = 0; // результат

signals:
    void endAlgorithm(int, std::vector<std::vector<int>>const*, int, std::vector<int>const*);
    void emptyData();

private slots:
    void startAlgorithm(); // сигнал, который начинает вычислять максимльный путь


};

#endif // MAINALGORITHM_H
