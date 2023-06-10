

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
#include<SFML/Graphics.hpp>//ссылка на либу: https://www.sfml-dev.org/index.php 
// туториал по подключению либы в visual studio: https://www.sfml-dev.org/tutorials/2.5/start-vc.php
using namespace sf;
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
    RenderWindow window;//графическое окно
    Clock clock; float time;// я хз что это, вроде для интервала надо
    RectangleShape tileBlack; //тайлы клеток
    char mode;//выбор версии игры (консольная/графическая)
    Field();
    void Game_Cycle_Console();//консольная версия игры
    void Game_Cycle_Graphic();//графическая
    void Clear_Window();//перерисовка графического окна (на каждом ходу)
    void User_Input_Param(); //нужно дописать ввод матрицы юзером
    void File_Input_Param();//ввод матрицы из файла
    void Set_Mode() { std::cin >> mode; }//ну тут понятно вроде
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
    this->tileBlack.setSize(Vector2f(sizeTile, sizeTile));//черный квадрат будет рисоватся где живые тоесть заполненные клетки
    tileBlack.setFillColor(Color::Black);
}
void Field::Game_Cycle_Console()
{
    this->File_Input_Param();// ввод матрицы из файла, можно заменить на ввод юзером
    //TODO консольную версию игры
}
void Field::File_Input_Param()
{
    std::ifstream fin;
    fin.open("Input.txt", std::ios::in);//можно переделать чтобы был string filename; fin.open(filename,in)
    fin >> width >> height;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
            fin >> world[i][j];
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
void Field::Game_Cycle_Graphic()
{
    window.create(VideoMode(width * sizeTile, height * sizeTile), "Life"); //размеры окна=размер тайла умноженный на высоту и ширину карты
    // сюда вставить код из ссылки от while(window.isOpen())... до window.clear()
}
void Field::Clear_Window()//по сути это не clear a print, можно переименовать
{
    window.clear(Color::White); //очищаем окно в белый цвет
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            if (world[i][j] == true)
            {
                tileBlack.setPosition(Vector2f(i * sizeTile, j * sizeTile));
                window.draw(tileBlack); //рисуем только живые клетки черным цветом,, а остальные останутся белого цвета
            }
        }
    }
    window.display();
}
int main()
{
    Field game;
    std::cout << "Enter 'c' to play in console mode or 'g' to play in graphic mode\n";
    game.Set_Mode();//если юзер вводит с - режим консольный, g - графический. Стоит добавить обработку ошибки если юзер ввёл ни то ни другое
    if (game.mode == 'c')
        game.Game_Cycle_Console();
    else if (game.mode == 'g')
        game.Game_Cycle_Graphic();
    return 0;
}