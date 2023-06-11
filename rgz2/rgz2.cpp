

#include <iostream>

int main()
{
    std::cout << "Hello World!\n";
}

    std::cout << std::endl;
    while (true)
    {
        char ch = _getch();
        std::cout << "Press 0 to stop the game or any other button to continue\n";
        if (ch == '0') break;
        int c = 0;
        for (int i = 1; i < width - 1; i++)
            for (int j = 1; j < height - 1; j++)
            {
                neighbours = 0;
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
                if (neighbours != 2 && neighbours != 3 && world[i][j] == 1)
                {
                    c++;
                    world[i][j] = 0;
                }
                else if ((neighbours == 2 || neighbours == 3) && world[i][j] == 0)
                {
                    c++;
                    world[i][j] = 1;
                }
            }
        if (c == 0)
        {
            std::cout << "Game ended!\n";
            break;
        }
        for (int i = 0; i < width; i++, std::cout << std::endl)
            for (int j = 0; j < height; j++)
                std::cout << world[i][j] << "\t";
        std::cout << std::endl;
    }
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
void Field::Game_Cycle_Graphic()
{
    window.create(VideoMode(width * sizeTile, height * sizeTile), "Life"); //размеры окна=размер тайла умноженный на высоту и ширину карты
    // сюда вставить код из ссылки от while(window.isOpen())... до window.clear()
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
