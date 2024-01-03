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

void showSidePannel(sf::RenderWindow& window, int resolution, int flagsCount, int timerValue, sf::Font font) {
    //фон
    sf::RectangleShape backgroundRect(sf::Vector2f(0.3 * resolution, resolution));
    backgroundRect.setFillColor(sf::Color(85, 22, 116));
    backgroundRect.setPosition(resolution, 0);

    //надпись сверху
    sf::Text flagsText("Flags:", font, 0.3 * resolution / 10);
    flagsText.setFillColor(sf::Color(153, 204, 237));
    flagsText.setOrigin(flagsText.getLocalBounds().width / 2, flagsText.getLocalBounds().height / 2);
    flagsText.setPosition(1.15 * resolution, 0.25 * resolution);

    //прямоугольник для флагов
    sf::RectangleShape flagsRect(sf::Vector2f(0.24 * resolution, 0.1 * resolution));
    flagsRect.setFillColor(sf::Color(197, 254, 254));
    flagsRect.setOrigin(flagsRect.getLocalBounds().width / 2, flagsRect.getLocalBounds().height / 2);
    flagsRect.setPosition(1.15 * resolution, 0.35 * resolution);
    //flagsRect.setPosition(1.03 * resolution, resolution / 5 + (textBounds.height * 1.5));

    //количество флагов
    sf::Text flags(std::to_string(flagsCount), font, 0.3 * resolution / 8);
    flags.setFillColor(sf::Color(126, 57, 182));
    flags.setOrigin(flags.getLocalBounds().width / 2, flags.getLocalBounds().height / 2);
    flags.setPosition(1.15 * resolution, 0.35 * resolution);

    //надпись сверху
    sf::Text timerText("Timer:", font, 0.3 * resolution / 10);
    timerText.setFillColor(sf::Color(153, 204, 237));
    timerText.setOrigin(timerText.getLocalBounds().width / 2, timerText.getLocalBounds().height / 2);
    timerText.setPosition(1.15 * resolution, 0.55 * resolution);

    //прямоугольник для таймера
    sf::RectangleShape timerRect(sf::Vector2f(0.24 * resolution, 0.1 * resolution));
    timerRect.setFillColor(sf::Color(197, 254, 254));
    timerRect.setOrigin(timerRect.getLocalBounds().width / 2, timerRect.getLocalBounds().height / 2);
    timerRect.setPosition(1.15 * resolution, 0.65 * resolution);

    //таймер
    sf::Text timer(formatTime(timerValue), font, 0.3 * resolution / 8);
    timer.setFillColor(sf::Color(126, 57, 182));
    timer.setOrigin(timer.getLocalBounds().width / 2, timer.getLocalBounds().height / 2);
    timer.setPosition(1.15 * resolution, 0.65 * resolution);

    window.draw(backgroundRect);
    window.draw(flagsRect);
    window.draw(timerRect);
    window.draw(flagsText);
    window.draw(flags);
    window.draw(timer);
    window.draw(timerText);
}

void showWin(sf::RenderWindow& window, int resolution, sf::Font font) {

    sf::Text gameOverText("You won!", font, 0.9 * resolution / 20);
    gameOverText.setFillColor(sf::Color(126, 57, 182));

    sf::RectangleShape backgroundRect(sf::Vector2f(resolution, resolution));
    backgroundRect.setFillColor(sf::Color(197, 254, 254));

    sf::FloatRect textBounds = gameOverText.getLocalBounds();
    gameOverText.setPosition((resolution - textBounds.width) / 2, (resolution - textBounds.height) / 2);

    window.draw(backgroundRect);
    window.draw(gameOverText);
}

void showGameOver(sf::RenderWindow& window, int resolution, sf::Font font) {

    sf::Text gameOverText("Oops, you exploded!", font, 0.9 * resolution / 20);
    gameOverText.setFillColor(sf::Color(126, 57, 182));

    sf::RectangleShape backgroundRect(sf::Vector2f(resolution, resolution));
    backgroundRect.setFillColor(sf::Color(197, 254, 254));

    sf::FloatRect textBounds = gameOverText.getLocalBounds();
    gameOverText.setPosition((resolution - textBounds.width) / 2, (resolution - textBounds.height) / 2);

    window.draw(backgroundRect);
    window.draw(gameOverText);
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
    Field f1(fieldSize, 0, 0, 0);

    //исключаем возможность проигрыша на первом ходу
    bool firstClick = true;
    int firstClickX, firstClickY;
    int flags = mines;
    bool lose = false;
    bool win = false;

    std::time_t startTime = std::time(nullptr);
    int timerValue = 0;

    //sf::Font font;
    //font.loadFromFile("assets/fonts/mine-sweeper.TTF");
    //sf::Text text("Hello world", font, 100);
    //text.setPosition(20,20);
    //text.setFillColor(sf::Color::Cyan);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(window);
                    int y = static_cast<int>(std::ceil(static_cast<double>(mousePos.x) * fieldSize / resolution));
                    int x = static_cast<int>(std::ceil(static_cast<double>(mousePos.y) * fieldSize / resolution));

                    if (f1.isInField(x - 1, y - 1)) {
                        if (firstClick) {
                            firstClick = false;
                            Field gen(fieldSize, mines, x - 1, y - 1);
                            f1 = gen;
                            f1.openCell(x - 1, y - 1);
                            f1.showOthers(x - 1, y - 1);
                        }
                        else if (f1.getCell(x - 1, y - 1).getIsMine()) {
                            lose = true;
                        }
                        else if (!f1.getCell(x - 1, y - 1).getIsOpen()) {
                            f1.openCell(x - 1, y - 1);
                            if (!f1.getCell(x - 1, y - 1).getMinesAround()) {
                                f1.showOthers(x - 1, y - 1);
                            }
                        }
                    }
                }
                else if (event.mouseButton.button == Mouse::Right)
                {
                    Vector2i mousePos = Mouse::getPosition(window);
                    int y = static_cast<int>(std::ceil(static_cast<double>(mousePos.x) * fieldSize / resolution));
                    int x = static_cast<int>(std::ceil(static_cast<double>(mousePos.y) * fieldSize / resolution));

                    if (f1.isInField(x - 1, y - 1)) {
                        if (!f1.getCell(x - 1, y - 1).getIsFlagged()) {
                            f1.getCell(x - 1, y - 1).setFlag();
                            --flags;
                        }
                        else if (flags < mines) {
                            f1.getCell(x - 1, y - 1).unflag();
                            ++flags;
                        }
                    }
                }
            }
        }

        if (!win && !lose) {
            std::time_t currentTime = std::time(nullptr);
            if (currentTime - startTime >= 1) {
                startTime = currentTime;
                ++timerValue;
            }
        }

        window.clear();

        if (win) {}
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
