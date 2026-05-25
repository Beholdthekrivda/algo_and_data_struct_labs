#include <iostream>
#include <vector>
#include <time.h>
#include <cmath>
#include <iomanip>
#define INF 100000000

using namespace std;

struct point {double x, y;};

class graph
{
private:
    int n; // кол-во вершин
    vector<vector<double>> C; // матрица весов
    vector<vector<double>> V; // массив ребер минимального остова
    vector<vector<int>> M; // матрица смежности ребер минимального остова
    vector<point> points_coord;

    void sort_V()
    {
        for (int i = 1; i < V.size(); i++)
        {
            for (int j = i - 1; j >= 0 && V[j + 1][2] < V[j][2]; j--)
            {
                swap(V[j], V[j + 1]);
            }
        }
    }

    void cdeep(int CurrVer, int *R, int CurrNum)
    {
        R[CurrVer] = CurrNum;
        for (int i = 0; i < n; i++)
        {
            if (M[CurrVer][i] && !R[i]) cdeep(i, R, CurrNum);
        }
    }

    void klaster_stat(int* result, int num_comp)
    {
        for (int c = 1; c <= num_comp; c++) 
        {
            double minX = INF, maxX = -1, sumX = 0;
            double minY = INF, maxY = -1, sumY = 0;
            int count = 0;
            for (int i = 0; i < n; i++) 
            {
                if (result[i] == c) 
                {
                    count++;
                    sumX += points_coord[i].x;
                    sumY += points_coord[i].y;
                    if (minX > points_coord[i].x) minX = points_coord[i].x;
                    if (maxX < points_coord[i].x) maxX = points_coord[i].x;
                    if (minY > points_coord[i].y) minY = points_coord[i].y;
                    if (maxY < points_coord[i].y) maxY = points_coord[i].y;
                }
            }
            cout << "Кластер " << c << endl;
            cout << "Точка с минимальными координатами: " << minX << " " << minY << endl;
            cout << "Точка с максимальными координатами: " << maxX << " " << maxY << endl;
            cout << "Центроид: " << sumX / count << " " << sumY / count << endl;
        }
    }
public:
    graph(vector<point> &points) : points_coord(points)
    {
        n = points.size();
        C.resize(n, vector<double>(n)); 

        for (int i = 0; i < n; i++) 
        {
            for (int j = 0; j < n; j++) 
            {
                if (i == j) 
                {
                    C[i][j] = 0;
                } 
                else 
                {
                    double dist = sqrt(pow(points[i].x - points[j].x, 2) + pow(points[i].y - points[j].y, 2));
                    C[i][j] = dist;
                }
            }
        } 
    }

    void get_span_tree() // алгоритм Прима
    {
        vector<double> B(n);
        B[0] = -1;
        for (int i = 1; i < n; i++) B[i] = 0;
        
        double wmin, vm;
        for (int i = 1; i < n; i++)
        {
            wmin = INF; 
            vm = 0;
            for (int j = 1; j < n; j++)
            {
                if (B[j] != -1 && wmin > C[j][B[j]])
                {
                    vm = j; wmin = C[j][B[j]];
                }
            }
            if (!vm) return;
            
            V.push_back({B[vm], vm, C[vm][B[vm]]});

            B[vm] = -1;
            for (int j = 1; j < n; j++)
            {
                if (B[j] != -1 && C[j][B[j]] > C[j][vm]) B[j] = vm;
            }
        }

        sort_V();
    }

    int* get_comp(int& CurrNum)
    {
        int *R = new int[n](), i;
        for (CurrNum = i = 0; i < n; i++)
        {
            if (!R[i])
            {
                CurrNum++;
                cdeep(i, R, CurrNum);
            }
        }
        return R;
    }

    void klaster(int k)
    {
        get_span_tree();

        M.resize(n, vector<int>(n));
        for (int i = 0; i < n - k; i++)
        {
            M[V[i][0]][V[i][1]] = 1;
            M[V[i][1]][V[i][0]] = 1;
        }
        
        int num_comp;
        int* result = get_comp(num_comp);

        cout << "Принадлежность вершин к кластерам: ";
        for (int i = 0; i < n; i++)
        {
            cout << result[i] << " ";
        }
        cout << endl;

        klaster_stat(result, num_comp);

        delete[] result;
    }

    void print_weight_matrix()
    {
        cout << "Матрица весов (" << n << "x" << n << "):" << endl;
        for (int i = 0; i < n; i++) 
        {
            for (int j = 0; j < n; j++) 
            {
                cout << setprecision(8) << setw(10) << C[i][j] << " ";
            }
            cout << endl;
        }
    }

    void print_span_tree() 
    {
        cout << "Минимальный остов (ребра):" << endl;
        for (const auto& edge : V) 
        {
            // edge[0] – u, edge[1] – v, edge[2] – weight
            cout << edge[0] + 1 << " - " << edge[1] + 1 << " : " << edge[2] << endl;
        }
    }
};

int main()
{	
    srand(time(NULL));

    int n = 20;
    vector<point> points;
    srand48(time(NULL));

    for (int i = 0; i < n; i++)
    {
        point p;
        p.x = drand48() * 10; p.y = drand48() * 10;

        points.push_back(p);
    }
    
    cout << "Массив точек" << endl;
    for (point p : points)
    {
        cout << p.x << " " << p.y << endl;
    }
    cout << endl;

    graph pgraph(points);

    pgraph.print_weight_matrix();
    cout << endl;
    pgraph.klaster(3);
    cout << endl;
    pgraph.print_span_tree();

    return 0;
}