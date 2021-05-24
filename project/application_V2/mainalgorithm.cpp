#include "mainalgorithm.h"

MainAlgorithm::MainAlgorithm(QPlainTextEdit *plainTextEdit, QLabel *label)
{
    text = plainTextEdit;
    label_for_result = label;
}

void MainAlgorithm::startAlgorithm()
{
    if(!getVector(text->toPlainText()))
    {
        adjacency_matrix.clear();
        max_path.clear();
        last_element_path = nullptr;
        nodes.clear();
        result = 0;
        return;
    }
    if(!n)
    {
        emit emptyData();
        return;
    }
    if(n > (int)sizeof(unsigned long long) * 8)
    {
        showError("Граф не должен содержать больше " + QString::number(sizeof(unsigned long long) * 8) + " вершин!");
        return;
    }
    nodes = std::vector<Node>(n);
    for (int i = 0; i < n; i++)
    {
        nodes[i].id = i;
        for (int j = 0; j < n; j++)
            if (adjacency_matrix[i][j] > 0)
            {
                nodes[i].son.push_back(&nodes[j]);
                nodes[j].father.push_back(&nodes[i]);
                nodes[j].unused_father ^= 1 << i;
            }
    }
    bool e = true;
    for (int i = 0; i < n; i++)
    {
        e &= nodes[i].used;
        if (nodes[i].unused_father == 0 && !nodes[i].used)
        {
            nodes[i].cnt = 1;
            algorithm(&nodes[i]);
            i = -1;
            e = true;
            continue;
        }
    }
    if(!e)
    {
        showError("В графе содержится цикл!");
        return;
    }
    max_path = std::vector<int>(last_element_path->cnt);
    Node *tmp = last_element_path;
    while(tmp)
    {
        max_path[tmp->cnt - 1] = tmp->id;
        tmp = tmp->prev_node;
    }
    label_for_result->setText(QString::number(result));
    emit endAlgorithm(n, &adjacency_matrix, last_element_path->cnt, &max_path);

    adjacency_matrix.clear();
    max_path.clear();
    last_element_path = nullptr;
    nodes.clear();
    result = 0;
}

bool MainAlgorithm::getVector(QString str)
{
    n = 0;
    bool e = false, o = false;
    for(QChar i : str)
    {
        if(i == ' ')
        {
            if(e) n++;
            e = false;
            continue;
        }
        else if(i == '\n')
        {
            if(e) n++;
            if(o) break;
        }
        else if(!i.isDigit())
        {
            showError("Недопустимые символы!");
            return false;
        }
        e = true, o = true;
    }
    adjacency_matrix = std::vector<std::vector<int>>(n, std::vector<int>(n, 0));
    int j = 0, i = 0;
    e = false, o = false;
    for(QChar t : str)
    {
        if(t == ' ')
        {
            if(j < n - 1)
            {
                if(e) j++;
                e = false;
            }
            continue;
        }
        else if(t == '\n')
        {
            if(o && j != n - 1)
            {
                showError("Некорректный размер матрицы!");
                return false;
            }
            if(e && o) i++;
            j = 0, e = false, o = false;
            continue;
        }
        else if(!t.isDigit())
        {
            showError("Недопустимые символы!");
            return false;
        }
        e = true, o = true;
        adjacency_matrix[i][j] = adjacency_matrix[i][j] * 10 + t.digitValue();
    }
    if(j != n - 1 && i != n)
    {
        showError("Некорректный размер матрицы!");
        return false;
    }
    return true;
}

void MainAlgorithm::algorithm(Node *cur_node)
{
    if (cur_node->unused_father > 0)
        return;
    cur_node->used = true;
    if (cur_node->son.empty())
    {
        if(cur_node->power > result  || last_element_path == nullptr || (result == cur_node->power && last_element_path->cnt >= cur_node->cnt))
        {
            result = cur_node->power;
            last_element_path = cur_node;
        }
        result = std::max(cur_node->power, result);
        return;
    }
    for (Node* i : cur_node->son)
    {
        i->unused_father ^= 1 << cur_node->id;
        if(i->power == cur_node->power + adjacency_matrix[cur_node->id][i->id] && i->cnt >= cur_node->cnt + 1)
        {
            i->cnt = cur_node->cnt + 1;
            i->prev_node = cur_node;
        }
        else if(i->power < cur_node->power + adjacency_matrix[cur_node->id][i->id])
        {
            i->cnt = cur_node->cnt + 1;
            i->power = cur_node->power + adjacency_matrix[cur_node->id][i->id];
            i->prev_node = cur_node;
        }
        algorithm(i);
    }
}

void MainAlgorithm::showError(QString str)
{
    emit emptyData();
    QMessageBox warning;
    warning.setWindowIcon(QIcon("./img/icon.svg"));
    warning.setWindowTitle("Warning");
    warning.setIcon(QMessageBox::Warning);
    warning.setText(str);
    warning.exec();
}
