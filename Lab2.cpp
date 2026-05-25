#include <iostream>
#include "time.h"
#include <algorithm>
#include <vector>

using namespace std;

struct Node
{
    int val;
    Node *left, *right;
    Node(int v)
    {
        val = v; left = right = NULL;
    }
};

class Tree
{
private:
    Node *root, *current, *parent;
    int max_level;
    double count_level = 0, sum_level = 0;

    Node* search_private(int target)
    {
        if (root == NULL) return root;

        parent = NULL;
        current = root;

        while (current != NULL && current->val != target)
        {
            parent = current;
            current = (target > current->val) ? current->right : current->left;
        }

        return current;
    }

    void add_private(int v)
    {
        if (search_private(v)) return;

        Node* newnode = new Node(v);
        if (root == NULL)
        {
            root = newnode;
            return;
        }

        parent = NULL;
        current = root;

        while (current != NULL)
        {
            parent = current;
            current = (v > current->val) ? current->right : current-> left;
        }
        if (v > parent->val) parent->right = newnode;
        else parent->left = newnode;
    }

    void print_recursive(Node* node, int level)
    {
        if (node == NULL) return;

        if (node->val != root->val)
        {
            for (int i = 0; i < level; i++) cout << " ";
            for (int i = level; i < level + 1; i++)
            {
                cout << "|"; cout << "_";
            }
        }

        cout << node->val << " " << endl;

        print_recursive(node->left, level + 1);
        print_recursive(node->right, level + 1);
    }

    void get_vector_private(Node* node, int left, int right, vector<int> &v)
    {
        if (node == NULL) return;

        if (node->val > left) get_vector_private(node->left, left, right, v);

        if (node->val >= left && node->val <= right) v.push_back(node->val);

        if (node->val < right) get_vector_private(node->right, left, right, v);
    }

    void build_opt_tree_private(int *arr, int left, int right)
    {
        if (left > right) return;

        int mid = (left + right) / 2;
        add_private(arr[mid]);
        if (mid > left) build_opt_tree_private(arr, left, mid - 1);
        if (mid < right) build_opt_tree_private(arr, mid + 1, right);
    }

    void clear(Node* node)
    {
        if (node == NULL) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

    int get_level_private(Node* node, int target, int level)
    {
        if (node == NULL) return -1;

        if (node->val == target) return level;

        if (node->val > target) return get_level_private(node->left, target, level + 1);

        return get_level_private(node->right, target, level + 1);
    }

    void find_max_avg_private(Node* node, int level)
    {
        if (node == NULL) return;

        if (max_level < level) max_level = level;

        count_level++;
        sum_level += level;

        find_max_avg_private(node->left, level + 1);
        find_max_avg_private(node->right, level + 1);
    }

public:
    Tree(): root(NULL), current(NULL), parent(NULL) {};
    Tree(int v)
    {
        root = new Node(v);
        current = parent = NULL;
    }

    void print()
    {
        print_recursive(root, 0);
        cout << endl;
    }

    void get_vector(int left, int right, vector<int> &v)
    {
        get_vector_private(root, left, right, v);
        cout << endl;
    }

    bool search(int v)
    {
        if (search_private(v)) return true;
        return false;
    }

    void add(int *arr, int n) // для массива
    {
        for (int i = 0; i < n; i++)
        {
            add_private(arr[i]);
        }
    }

    void add(int v) // для одного элемента
    {
        add_private(v);
    }

    void remove(int v)
    {
        if (!search(v)) return;

        if (!current->left && !current->right)
        {
            if (current == parent->left) parent->left = NULL;
            else parent->right = NULL;
        }
        else if (!current->left)
        {
            if (current == parent->left) parent->left = current->right;
            else parent->right = current->right;
        }
        else if (!current->right)
        {
            if (current == parent->left) parent->left = current->left;
            else parent->right = current->left;
        }
        else
        {
            Node* found = current;
            parent = current; current = current->left;
            while (current->right != NULL)
            {
                parent = current;
                current = current->right;
            }
            // после цикла current максимальное значение левого поддерева корня
            if (current == parent->left) parent->left = current->left;
            else parent->right = current->left;
            found->val = current->val;
        }
        delete(current);
    }

    void build_tree(int* arr, int n)
    {
        clear(root);
        root = NULL;
        add(arr, n);
        cout << "root: " << root->val << endl;
    }

    void build_opt_tree(int* arr, int n)
    {
        clear(root);
        root = NULL;
        sort(arr, arr + n);

        build_opt_tree_private(arr, 0, n - 1);
        cout << "root: " << root->val << endl;
    }

    int get_level(int v)
    {
        return get_level_private(root, v, 1);
    }

    void find_max_avg_level()
    {
        max_level = 0;
        count_level = 0;
        sum_level = 0;

        find_max_avg_private(root, 1);

        cout << "Максимальный уровень: " << max_level << endl;
        cout << "Кол-во уровней: " << count_level << " Сумма уровней: " << sum_level << endl;
        cout << "Среднее значение: " << sum_level / count_level << endl;
    }

    ~Tree()
    {
        clear(root);
    }
};

void test_basics()
{
    Tree t;
    t.add(10);
    t.add(10); // не должно добавиться
    t.add(5);
    t.add(15);

    cout << "Ожидаемый вывод (5 10 15): " <<endl;
    t.print();

    cout << "Поиск 10: " << (t.search(10) ? "Найдено" : "Нет") << endl;
    cout << "Поиск 100: " << (t.search(100) ? "Найдено" : "Нет") << endl;
}

void test_range()
{
    Tree t;
    int arr[] = {20, 10, 30, 5, 15, 25, 35};
    t.add(arr, 7);

    vector<int> res;
    t.get_vector(10, 25, res); // Ожидаем: 10, 15, 20, 25

    cout << "Элементы в диапазоне [10, 25]: ";
    for(int val : res) cout << val << " ";
    cout << endl;
}

void test_efficiency()
{
    int n = 15;
    int* arr = new int[n];
    for(int i = 0; i < n; i++) arr[i] = i + 1; // Отсортированный массив

    Tree t_normal;
    t_normal.build_tree(arr, n);
    cout << "Обычное дерево (из отсортированного массива):" << endl;
    t_normal.find_max_avg_level();
    cout << endl;

    Tree t_opt;
    t_opt.build_opt_tree(arr, n);
    cout << "Оптимальное дерево:" << endl;
    t_opt.find_max_avg_level();

    delete[] arr;
}

void test_levels()
{
    Tree t;
    t.add(10);
    t.add(5);
    t.add(15);

    cout << "Уровень узла 15: " << t.get_level(15) << " (ожидаем 2)" << endl;
    t.find_max_avg_level(); // Max: 2, Avg: (1+2+2)/3 = 1.66
}

int main()
{
    srand(time(NULL));

    int n = 10;
    int *arr = new int[n];

    for (int i = 0; i < n; i++)
    {
        arr[i] = i + 1;
    }

    Tree tree;

    tree.build_opt_tree(arr, n);

    tree.print();

    // test_basics();
    // test_range();
    // cout << endl;
    // test_efficiency();
    // cout << endl;
    // test_levels();

    return 0;
}
