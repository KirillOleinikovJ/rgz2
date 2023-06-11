#include<iostream>
using namespace std;
int main()
{
    int n, m;
    cout << "Enter field size" << endl;
    cout << "N=";
    cin >> n;
    cout << "M=";
    cin >> m;
    cout << endl << "Enter cell states(1 if alive, 0 if dead, if you enter any other number-it will be count  dead):\n";
    n += 2;
    m += 2;
    int** a = new int* [n];
    for (int i = 0; i < n; i++) {
        a[i] = new int[m];
    }
    for (int i = 0, j = n - 1, s = 0; s < m; s++) //заполнение по горизонтальнім краям
    {
        a[i][s] = 0;
        a[j][s] = 0;
    }
    for (int i = 0, j = m - 1, s = 0; s < n; s++) //заполнение по вертикальным караям
    {
        a[s][i] = 0;
        a[s][j] = 0;
    }
    for (int i = 1; i < n - 1; i++) { // ввод с первого из за неживых соседей
        for (int j = 1; j < m - 1; j++)
        {
            cin >> a[i][j];
            if (a[i][j] != 1 && a[i][j] != 0)
                a[i][j] = 0;
        }
    }
    cout << "\nGame starts:\n";
    for (int i = 0; i < n; i++, cout << endl) {
        for (int j = 0; j < m; j++)
            cout << a[i][j] << "\t";
    }
    cout << endl;
    while (true) //логика
    {
        int c = 0;//условный флаг
        for (int i = 1; i < n - 1; i++) //перебор без соседей
            for (int j = 1; j < m - 1; j++)
            {
                int an = 0; //счетчик соседей дальше идет сравнение их по жизнеспособности
                if (a[i - 1][j] == 1)
                    an++;
                if (a[i + 1][j] == 1)
                    an++;
                if (a[i][j - 1] == 1)
                    an++;
                if (a[i][j + 1] == 1)
                    an++;
                if (a[i - 1][j - 1] == 1)
                    an++;
                if (a[i - 1][j + 1] == 1)
                    an++;
                if (a[i + 1][j - 1] == 1)
                    an++;
                if (a[i + 1][j + 1] == 1)
                    an++;
                if (an != 2 && an != 3 && a[i][j] == 1) // проверка по условию и если что клетка становиться не живой
                {
                    c++;
                    a[i][j] = 0;
                }
                else if ((an == 2 || an == 3) && a[i][j] == 0) // тут наоборот
                {
                    c++;
                    a[i][j] = 1;
                }
            }
        if (c == 0)//выход из цикла
        {
            cout << "Game ended!";
            break;
        }
        for (int i = 0; i < n; i++, cout << endl) // и последний вывод
            for (int j = 0; j < m; j++)
                cout << a[i][j] << "\t";
        cout << endl;
    }
    return 0;
}
