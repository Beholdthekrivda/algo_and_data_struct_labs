#include <iostream>
#include <time.h>
#include <iomanip>

using namespace std;

template <typename T>
class Sort
{
private:
    static void merge_series(T** A, int b, int c, int e, T** D, int &count_comp)
    {
        int i = b, j = c + 1, k;

        for (k = b; k <= e; k++)
        {
            if (j > e)
                D[k] = A[i++];
            else if (i > c)
                D[k] = A[j++];
            else if (*A[i] < *A[j])
            {
                D[k] = A[i++];
                count_comp++;
            }
            else
            {
                D[k] = A[j++];
                count_comp++;
            }
        }
    }
public:
    static int exchange_sort(T** &ptrs, int n) // сортировка обменом
    {
        int count_comp = 0;

        for (int i = 1; i < n; i++)
        {
            for (int j = i - 1; j >= 0 && (++count_comp, *ptrs[j + 1] < *ptrs[j]); j--)
            {
                swap(ptrs[j], ptrs[j + 1]);
            }
        }

        return count_comp;
    }

    static int merge_sort(T** &ptrs, int n)
    {
        int s, b, c, e, count_comp = 0;
        T** ptrs_D = new T*[n];

        for (s = 1; s < n; s *= 2)
        {
            for (b = 0; b < n; b += s*2)
            {
                c = min(b + s - 1, n - 1);
                e = min(c + s, n - 1);
                merge_series(ptrs, b, c, e, ptrs_D, count_comp);
            }

            for (b = 0; b < n; b++) ptrs[b] = ptrs_D[b];
        }

        delete[] ptrs_D;

        return count_comp;
    }
    
    static int shell_sort(T** &ptrs, int n)
    {
        int i, j, h, count_comp = 0;

        for (h = 1; h <= n / 9; h = h * 3 + 1);
        
        while (h >= 1)
        {
            for (i = h; i < n; i++)
            {
                for (j = i - h; j >= 0 && (++count_comp, *ptrs[j + h] < *ptrs[j]); j -= h)
                {
                    swap(ptrs[j], ptrs[j + h]);
                }
            }
            h = (h - 1) / 3;
        }

        return count_comp;
    }
};

class Participant
{
private:
    int id, solved, time, score, fails;
    static int ID;
public:
    Participant()
    {
        id = ID++;
        solved = 0 + rand() % 6;
        time = 60 + rand() % 241;
        score = 0 + rand() % 26;
        fails = 0 + rand() % 21;
    }

    bool operator<(const Participant& other) const
    {
        if (score != other.score) return score > other.score;
        if (solved != other.solved) return solved > other.solved;
        if (time != other.time) return time < other.time;
        if (fails != other.fails) return fails < other.fails;
        return id < other.id;
    }

    friend ostream& operator<<(ostream& r, Participant& obj)
    {
        r << setw(3) << obj.id << " | "
           << setw(7) << obj.solved << " | "
           << setw(5) << obj.time << " | "
           << setw(5) << obj.score << " | "
           << setw(5) << obj.fails;
        return r;
    }

    static void printHeader() 
    {
        cout << setw(3) << "ID" << " | "
            << setw(7) << "Solved" << " | "
            << setw(5) << "Time" << " | "
            << setw(5) << "Score" << " | "
            << setw(5) << "Fails" << '\n';
    }
};

int Participant::ID = 1;

template <typename T>
bool check_sort(T** ptrs, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        if (*ptrs[i + 1] < *ptrs[i]) 
        {
            cout << "Массив не отсортирован" << endl;
            return false;
        }
    }
    cout << "Массив прошел проверку на упорядоченность" << endl;
    return true;
}

void test1(int n) // целые числа
{
    int mass[n];
    for (int i = 0, j = n / 2; i < n; i++, j--)
    {
        mass[i] = j; 
    }
    
    int** ptrs = new int*[n];
    for (int i = 0; i < n; i++) 
    {
        ptrs[i] = &mass[i];
    }

    Sort<int>::shell_sort(ptrs, n);

    for (int i = 0; i < n; i++)
    {
        cout << *ptrs[i] << " ";
    }
    cout << endl;

    check_sort(ptrs, n);

    delete[] ptrs;
}

void test2(int n) // учатсники
{
    Participant* mass = new Participant[n]();

    Participant** ptrs = new Participant*[n];
    for (int i = 0; i < n; i++) 
    {
        ptrs[i] = &mass[i];
    }

    Sort<Participant>::shell_sort(ptrs, n);

    Participant::printHeader();
    for (int i = 0; i < n; i++)
    {
        cout << *ptrs[i] << endl;
    }

    check_sort(ptrs, n);

    delete[] ptrs; delete[] mass;
}

int main()
{
    srand(time(NULL));

    int n;

    cout << "Введите n: "; cin >> n;
    
    test1(n); 
    cout << endl;
    test2(n);

    return 0;
}