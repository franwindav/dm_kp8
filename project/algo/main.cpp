#include <iostream>
#include <vector>

using namespace std;

struct Node
{
    vector<Node*> son, father;
    int unused_father = 0;
    int id, power = 0;
    bool used = false;
};

void function(Node* cur_node, vector<vector<int>>* adjacency_matrix, int* max_power)
{
    if (cur_node->unused_father > 0)
        return;
    cur_node->used = true;
    if (cur_node->son.empty())
    {
        *max_power = max(cur_node->power, *max_power);
        return;
    }
    for (Node* i : cur_node->son)
    {
        i->unused_father ^= 1 << cur_node->id;
        i->power = max(i->power, cur_node->power + (*adjacency_matrix)[cur_node->id][i->id]);
        function(i, adjacency_matrix, max_power);
    }
}


int main()
{
    int n, m;
    cin >> n;

    vector<Node> nodes(n);
    for (int i = 0; i < n; i++)
        nodes[i].id = i;

    vector<vector<int>> adjacency_matrix(n, vector<int>(n, -1));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            cin >> adjacency_matrix[i][j];
            if (adjacency_matrix[i][j] > 0)
            {
                nodes[i].son.push_back(&nodes[j]);
                nodes[j].father.push_back(&nodes[i]);
                nodes[j].unused_father ^= 1 << i;
            }
        }
    int max_power = 0;
    for (int i = 0; i < n; i++)
        if (nodes[i].unused_father == 0 && !nodes[i].used)
        {
            function(&nodes[i], &adjacency_matrix, &max_power);
            i = -1;
            continue;
        }

    cout << max_power;

    return 0;
}