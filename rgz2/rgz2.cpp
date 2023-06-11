//Гра відбувається на прямокутному полі. Розмір поля вибирається гравцем. Поле складається з клітин. У кожної клітини є 8 сусідів. Правила такі:
//-жива клітина, у якій є менше ніж дві живі клітини серед сусідів, вмирає.
//- жива клітина, у якій більш ніж три живі клітини серед сусідів, також вмирає.
//- жива клітина, у якій дві або три живі клітини серед сусідів, продовжує жити.
//- мертва клітина, у якій рівно три живі клітини серед сусідів, стає живою.
//Початкова конфігурація задається гравцем.Гра закінчується тоді, 
//коли на полі немає більше живих клітин, інакше розподіл клітин повторюється по кроках.
//На кожному кроці відображається поле, що складається з клітин.
//Кожна клітина відображається цифрою або кольором(якщо готові зробити графічне представлення), 
//що відповідає стану клітини.

#include <iostream>
#include<fstream>
#include<conio.h>
class Field
{
public:
    int width; //ширина карты
    int height; //высота карты
    int sizeTile; //размер каждого тайла
    bool** world; //двумерный массив с клетками в этом ходе
    bool** nextStep; //двумерный массив с клетками которые будут в следующем ходе
    int neighbours; //счетчик отвечающий за количество соседей
    float interval;//интервал между прорисовками окна
    Field();
    void Game_Cycle_Console();//консольная версия игры
    void User_Input(); //нужно дописать ввод матрицы юзером
    void File_Input();//ввод матрицы из файла
    void Print_Console();//принт матрицы в консоль
    //можно добавить больше сеттеров для отдельніх полей класса а так я хз что ещё тут надо
    ~Field()//деструктор удаляет двумерные динамические массивы
    {
        for (int i = 0; i < width; i++)
        {
            delete[] world[i];
            delete[] nextStep[i];
        }
        delete[]world;
        delete[]nextStep;
    }
};
Field::Field()//конструктор, задаёт динамические массивы и дефолтные значения полей, задаёт размер и цвет тайлам
{
    width = 100;
    height = 50;
    sizeTile = 10;
    interval = 0;
    world = new bool* [width];
    nextStep = new bool* [width];
    for (int i = 0; i < width; i++)
    {
        world[i] = new bool[height];
        nextStep[i] = new bool[height];
    }
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            world[i][j] = 0;
            nextStep[i][j] = 0;
        }
    }
}
void Field::Game_Cycle_Console()
{
    char inp;
    std::cout << "Enter 1 to input calls manualy or 2 to take them from file\n";
    std::cin >> inp;
    if (inp == '1')
        this->User_Input();
    else if (inp == '2')
        this->File_Input();

        std::cout << "\nGame starts:\n";
        for (int i = 0; i < width; i++, std::cout << std::endl) {
            for (int j = 0; j < height; j++)
                std::cout << world[i][j] << "\t";
        }
        std::cout << std::endl;
        while (true) //логика
        {
            char ch = _getch();
            std::cout << "Press 0 to stop the game or any other button to continue\n";
            if (ch == '0') break;
            int c = 0;//условный флаг
            for (int i = 1; i < width - 1; i++) //перебор без соседей
                for (int j = 1; j < height - 1; j++)
                {
                    int neighbours = 0; //счетчик соседей дальше идет сравнение их по жизнеспособности
                    if (world[i - 1][j] == 1)
                        neighbours++;
                    if (world[i + 1][j] == 1)
                        neighbours++;
                    if (world[i][j - 1] == 1)
                        neighbours++;
                    if (world[i][j + 1] == 1)
                        neighbours++;
                    if (world[i - 1][j - 1] == 1)
                        neighbours++;
                    if (world[i - 1][j + 1] == 1)
                        neighbours++;
                    if (world[i + 1][j - 1] == 1)
                        neighbours++;
                    if (world[i + 1][j + 1] == 1)
                        neighbours++;
                    if (neighbours != 2 && neighbours != 3 && world[i][j] == 1) // проверка по условию и если что клетка становиться не живой
                    {
                        c++;
                        world[i][j] = 0;
                    }
                    else if ((neighbours == 2 || neighbours == 3) && world[i][j] == 0) // тут наоборот
                    {
                        c++;
                        world[i][j] = 1;
                    }
                }
            if (c == 0)//выход из цикла
            {
                std::cout << "Game ended!";
                break;
            }
            for (int i = 0; i < width; i++, std::cout << std::endl) // и последний вывод
                for (int j = 0; j < height; j++)
                    std::cout << world[i][j] << "\t";
            std::cout << std::endl;
        }
}
void Field::User_Input()
{
    std::cout << "Enter field size\n";
    std::cout << "Width=";
    std::cin >> width;
    std::cout << "\nHeight=";
    std::cin >> height;
    std::cout << "\nEnter cell states(1 if alive, 0 if dead, if you enter any other number-it will be count  dead):\n";
    width += 2;
    height += 2;
    for (int i = 0, j = width - 1, s = 0; s < height; s++) //заполнение по горизонтальнім краям
    {
        world[i][s] = 0;
        world[j][s] = 0;
    }
    for (int i = 0, j = height - 1, s = 0; s < width; s++) //заполнение по вертикальным караям
    {
        world[s][i] = 0;
        world[s][j] = 0;
    }
    for (int i = 1; i < width - 1; i++) { // ввод с первого из за неживых соседей
        for (int j = 1; j < height - 1; j++)
        {
            std::cin >> world[i][j];
            if (world[i][j] != 1 && world[i][j] != 0)
                world[i][j] = 0;
        }
    }
}
void Field::File_Input()
{
    std::ifstream fin;
    fin.open("Input.txt", std::ios::in);//можно переделать чтобы был string filename; fin.open(filename,in)
    fin >> width >> height;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
            fin >> world[i][j];
    }
    for (int i = 1; i < width; i++)
    {
        for (int j = 1; j < height; j++)
        {
            //std::cin >> world[i][j];
            if (world[i][j] != 1 && world[i][j] != 0)
                world[i][j] = 0;
        }
    }
}
void Field::Print_Console()
{
    for (int i = 0; i < this->width; i++)
    {
        for (int j = 0; j < this->height; j++)
            std::cout << this->world[i][j] << " ";
        std::cout << std::endl;
    }
}
int main()
{
    Field game;
    game.Game_Cycle_Console();
    return 0;
}

