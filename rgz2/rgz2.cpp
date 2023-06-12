
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
#include<conio.h>
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
    void User_Input(); //нужно дописать ввод матрицы юзером
    void File_Input();//ввод матрицы из файла
    void Set_Mode() { std::cin >> mode; }//ну тут понятно вроде
    void Print_Console();//принт матрицы в консоль
    ~Field()//деструктор удаляет двумерные динамические массивы
    {
        for (int i = 0; i < width; i++)
        {
            delete[] world[i];
            //delete[] nextStep[i];
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
    std::cout << "Enter 1 to input matrix manualy or 2 to take it from file\n";
    char c;
    std::cin >> c;
    if (c == '1') this->User_Input();
    else if (c == '2') this->File_Input();// ввод матрицы из файла, можно заменить на ввод юзером
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
    int width, height;
    std::cout << "Enter field size\n";
    std::cout << "Width=";
    std::cin >> width;
    std::cout << "Height=";
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
void Field::Game_Cycle_Graphic()
{
    window.create(VideoMode(width * sizeTile, height * sizeTile), "Life"); //размеры окна=размер тайла умноженный на высоту и ширину карты
    while (window.isOpen())
    {
        time = clock.getElapsedTime().asSeconds();

        Event event;

        while (window.pollEvent(event)) if (event.type == Event::Closed) window.close();

        Vector2i pixelPos = Mouse::getPosition(window); //позиция мышки относительно окна
        Vector2f pos = window.mapPixelToCoords(pixelPos);

        if (Mouse::isButtonPressed(Mouse::Left)) // рисование "живых" тайлов по лкм. 
        {
            world[int(pos.x / sizeTile)][int(pos.y / sizeTile)] = true;
            nextStep[int(pos.x / sizeTile)][int(pos.y / sizeTile)] = true;
        }

        if (Keyboard::isKeyPressed(Keyboard::LAlt))//ввод "задержки" на левый альт. Можно поменять на что-то другое. Список можно увидеть написав Keyboard:: 
        {
            std::cout << "Enter interval (as seconds): ";
            std::cin >> interval;
        }
        if (Keyboard::isKeyPressed(Keyboard::Space) && interval <= time)// основной цикл переопределения "живых" тайлов по нажатию пробела
        {
            clock.restart();
            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                { //в переменную neighbours складываем 8 соседских клеток(0 или 1), (i+width+1)%width а не просто i+1 потому что карта замкнутая
                    neighbours = world[(i + width - 1) % width][j] + world[(i + width + 1) % width][j] + world[i][(j + height - 1) % height] + world[i][(j + height + 1) % height] + world[(i + width - 1) % width][(j + height - 1) % height] + world[(i + width + 1) % width][(j + height - 1) % height] + world[(i + width - 1) % width][(j + height + 1) % height] + world[(i + width + 1) % width][(j + height + 1) % height];
                    if (world[i][j] == true)
                    { //если в текущем поколений клетка жива
                        if (neighbours < 2 || neighbours > 3)
                            nextStep[i][j] = false; //если соседей больше 3 или меньше 2 то клетка в следющем ходу умирает
                    }
                    else if (neighbours == 3)
                        nextStep[i][j] = true; //если 3 живых соседей то в следующем шаге рождается клетка
                }
            }
            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {
                    world[i][j] = nextStep[i][j]; //копируем массив nextStep в world
                }
            }
        }
        this->Clear_Window();
    }
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
