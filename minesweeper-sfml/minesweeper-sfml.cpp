#include <SFML/Graphics.hpp>
#include "Field.h"
#include <ctime>

using namespace sf;

std::string formatTime(int timerValue) {
    int mins = timerValue / 60;
    int secs = timerValue % 60;

    std::string minsStr = (mins < 10) ? "0" + std::to_string(mins) : std::to_string(mins);
    std::string secsStr = (secs < 10) ? "0" + std::to_string(secs) : std::to_string(secs);

    return minsStr + ":" + secsStr;
}

void drawText(sf::RenderWindow& window, int resolution, std::string text, sf::Font font, double fontSize, sf::Color color, double posX, double posY){
    sf::Text Text(text, font, fontSize);
    Text.setFillColor(color);
    Text.setOrigin(Text.getLocalBounds().width / 2, Text.getLocalBounds().height / 2);
    Text.setPosition(posX * resolution, posY * resolution);
    window.draw(Text);
}

void drawRectangle(sf::RenderWindow& window, int resolution, double width, double height, sf::Color color, bool center, double posX, double posY) {
    sf::RectangleShape Rect(sf::Vector2f(width * resolution, height * resolution));
    Rect.setFillColor(color);
    if (center){
        Rect.setOrigin(Rect.getLocalBounds().width / 2, Rect.getLocalBounds().height / 2);
    }
    Rect.setPosition(posX * resolution, posY * resolution);
    window.draw(Rect);
}

void showSidePannel(sf::RenderWindow& window, int resolution, int flagsCount, int timerValue, sf::Font font) {
    //фон
    drawRectangle(window, resolution, 0.3, 1, sf::Color(85, 22, 116), false, 1, 0);

    //надписи флаги и таймер
    drawText(window, resolution, "Flags:", font, 0.3 * resolution / 10, sf::Color(153, 204, 237), 1.15, 0.25);
    drawText(window, resolution, "Timer:", font, 0.3 * resolution / 10, sf::Color(153, 204, 237), 1.15, 0.55);

    //рамки
    drawRectangle(window, resolution, 0.24, 0.1, sf::Color(197, 254, 254), true, 1.15, 0.35);
    drawRectangle(window, resolution, 0.24, 0.1, sf::Color(197, 254, 254), true, 1.15, 0.65);

    //количество флагов и таймер
    drawText(window, resolution, std::to_string(flagsCount), font, 0.3 * resolution / 8, sf::Color(126, 57, 182), 1.15, 0.35);
    drawText(window, resolution, formatTime(timerValue), font, 0.3 * resolution / 8, sf::Color(126, 57, 182), 1.15, 0.65);
}

void showRestartButton(sf::RenderWindow& window, int resolution, sf::Font font) {
    drawRectangle(window, resolution, 0.6, 0.1, sf::Color(126, 57, 182), true, 0.5, 0.55);
    drawText(window, resolution, "Restart", font, 0.9 * resolution / 20, sf::Color(197, 254, 254), 0.5, 0.55);
}

void restart(Field& field, bool& firstClick, int& flags, int mines, bool& lose, bool& win, int& timerValue, std::time_t& startTime) {
    firstClick = true;
    flags = mines;
    lose = false;
    win = false;
    timerValue = 0;
    startTime = std::time(nullptr);
    field.clear();
}

void showWin(sf::RenderWindow& window, int resolution, sf::Font font) {
    drawRectangle(window, resolution, 1, 1, sf::Color(197, 254, 254), false, 0, 0);
    drawText(window, resolution, "You won!", font, 0.9 * resolution / 20, sf::Color(126, 57, 182), 0.5, 0.45);
    showRestartButton(window, resolution, font);
}

void showGameOver(sf::RenderWindow& window, int resolution, sf::Font font) {
    drawRectangle(window, resolution, 1, 1, sf::Color(197, 254, 254), false, 0, 0);
    drawText(window, resolution, "Oops, you exploded!", font, 0.9 * resolution / 20, sf::Color(126, 57, 182), 0.5, 0.45);
    showRestartButton(window, resolution, font);
}

int main()
{
    sf::Font font;
    font.loadFromFile("assets/fonts/mine-sweeper.TTF");

    //можно менять разрешение и размер поля 
    int resolution = 800;
    int fieldSize = 15;
    int mines = 20;
    RenderWindow window(VideoMode(resolution * 1.3, resolution), "Minesweeper");

    //поле сгенерируется по нажатию
    Field f1(fieldSize);

    //исключаем возможность проигрыша на первом ходу
    bool firstClick = true;
    int firstClickX, firstClickY;
    int flags = mines;
    bool lose = false;
    bool win = false;

    std::time_t startTime = std::time(nullptr);
    int timerValue = 0;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed)
            {
                Vector2i mousePos = Mouse::getPosition(window);

                if (event.mouseButton.button == Mouse::Left)
                {
                    if (win || lose) 
                    {
                        double x = (double)(mousePos.x) / resolution;
                        double y = (double)(mousePos.y) / resolution;

                        if (x > 0.2 && x < 0.8 && y > 0.5 && y < 0.65) 
                        {
                            restart(f1, firstClick, flags, mines, lose, win, timerValue, startTime);
                        }
                    }
                    
                    else {
                        int y = (int)(std::ceil((double)(mousePos.x) * fieldSize / resolution)) - 1;
                        int x = (int)(std::ceil((double)(mousePos.y) * fieldSize / resolution)) - 1;

                        if (f1.isInField(x, y) && !f1.getCell(x, y).getIsFlagged()) 
                        {
                            if (firstClick) 
                            {
                                firstClick = false;
                                f1.generateField(mines, x, y);
                                f1.getCell(x, y).setOpen();
                                //открываем область, даже если попали на значущую клетку
                                f1.showOthers(x, y);
                            }
                            else f1.openCell(x, y, lose);
                        }
                    }
                }
                else if (event.mouseButton.button == Mouse::Right && !win && !lose)
                {
                    int y = (int)(std::ceil(static_cast<double>(mousePos.x) * fieldSize / resolution)) - 1;
                    int x = (int)(std::ceil(static_cast<double>(mousePos.y) * fieldSize / resolution)) - 1;

                    if (f1.isInField(x, y) && !f1.getCell(x, y).getIsOpen()&&!firstClick) {
                        if (!f1.getCell(x, y).getIsFlagged()) {
                            f1.getCell(x, y).setFlag();
                            --flags;
                        }
                        else if (flags < mines) {
                            f1.getCell(x, y).unflag();
                            ++flags;
                        }
                    }

                }
            }
        }
        if (!firstClick) {
            win = (f1.checkWinConditionFlags(flags) || f1.checkWinConditionOpenCells());

            if (!win && !lose) {
                std::time_t currentTime = std::time(nullptr);
                if (currentTime - startTime >= 1) {
                    startTime = currentTime;
                    ++timerValue;
                }
            }
        }

 

        window.clear();

        if (win) {
            showWin(window, resolution, font);
        }
        else if (lose) {
            showGameOver(window, resolution, font);
        }
        else {
            f1.render(window, resolution, font);
        }

        showSidePannel(window, resolution, flags, timerValue, font);
        //showGameOver(window, resolution, font);
        window.display();
    }
}
//сделать экран победы полупрозрачным и открыть все клетки

//настройка сложность (може бiть), изменить условие выигрыша с клетками, сделать так, чтобы флаг не ставился на открытую клетку и нельзя открыть клетку с флагом