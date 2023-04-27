#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
using namespace sf;
 
const int M = 20;
const int N = 10;

int field[M][N] = {0};

struct Point
{
    int x, y;
} a[4], b[4];

class Point1
{
public:
    bool check()
    {
        // checks the po
        for (int i = 0; i < 4; i++)
            if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
                return false;
            else if (field[a[i].y][a[i].x])
                return false;

        return true;
    };
};

class Point2 : public Point1
{
    int figures[7][4] =
        {
            1, 3, 5, 7, // I
            2, 4, 5, 7, // Z
            3, 5, 4, 6, // S
            3, 5, 4, 7, // T
            2, 3, 5, 7, // L
            3, 5, 7, 6, // J
            2, 3, 4, 5, // O
        };

public:
    // getter //keeps the shape of figure in that position allocated by the pointer
    int returnFigures(int ptr[][4])
    {
        for (int i = 0; i < 7; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                *(*(ptr + i) + j) = *(*(figures + i) + j);
            }
        }
        return 0;
    }
};
int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(640, 480), "The Game!");

    Texture t1, t2, t3;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/pulback.png");
    t3.loadFromFile("images/frame.png");

    Sprite s(t1), background(t2), frame(t3);

    int dx = 0;
    bool rotate = 0;
    int colorNum = 1;
    float timer = 0, delay = 0.3;
    Point2 p1;
    int point2[7][4];
    p1.returnFigures(point2);
    Clock clock;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::Up)
                    rotate = true;
                else if (e.key.code == Keyboard::Left)
                    dx = -1;
                else if (e.key.code == Keyboard::Right)
                    dx = 1;
        }

        if (Keyboard::isKeyPressed(Keyboard::Down))
            delay = 0.05;

        //// <- Move -> ///
        for (int i = 0; i < 4; i++)
        {
            b[i] = a[i];
            a[i].x += dx;
        }
        if (!p1.check())
            for (int i = 0; i < 4; i++)
                a[i] = b[i];

        //////Rotate//////
        if (rotate)
        {
            Point p = a[1]; // center of rotation
            for (int i = 0; i < 4; i++)
            {
                int x = a[i].y - p.y;
                int y = a[i].x - p.x;
                a[i].x = p.x - x;
                a[i].y = p.y + y;
            }
            if (!p1.check())
                for (int i = 0; i < 4; i++)
                    a[i] = b[i];
        }

        ///////Tick//////
        if (timer > delay)
        {
            for (int i = 0; i < 4; i++)
            {
                b[i] = a[i];
                a[i].y += 1;
            }

            if (!p1.check())
            {
                for (int i = 0; i < 4; i++)
                    field[b[i].y][b[i].x] = colorNum;

                colorNum = 1 + rand() % 7;
                int n = rand() % 7;
                for (int i = 0; i < 4; i++)
                {
                    a[i].x = point2[n][i] % 2;
                    a[i].y = point2[n][i] / 2;
                }
            }

            timer = 0;
        }

        ///////check lines//////////
        int k = M - 1, score;
        for (int i = M - 1; i > 0; i--)
        {
            int count = 0;
            for (int j = 0; j < N; j++)
            {
                if (field[i][j])
                    count++;
                field[k][j] = field[i][j];
            }
            if (count < N)
            {
                k--;
            }
            else
            {
                score++;
            }
        }
        if (!p1.check())
        {
            window.draw(background);
            sf::Font font;
            font.loadFromFile("arial.ttf");
            sf::Text gameOver("Game Over", font, 80);
            gameOver.setPosition(100, 200);
            gameOver.setFillColor(sf::Color::Red);
            window.draw(gameOver);
            sf::Text scoreText;
            scoreText.setFont(font);
            scoreText.setCharacterSize(50);
            scoreText.setPosition(200, 300);
            scoreText.setFillColor(sf::Color::Green);
            scoreText.setString("Score: " + std::to_string(score));
            window.draw(scoreText);
            window.display();
            sf::sleep(sf::seconds(3)); // Wait for 2 seconds
            break;
        }
        sf::Font font;
        if (!font.loadFromFile("arial.ttf"))
        {
            std::cerr << "Failed to load font" << std::endl;
            return 1;
        }
        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(20);
        scoreText.setPosition(75, 0);
        scoreText.setFillColor(sf::Color::Blue);
        scoreText.setString("Score: " + std::to_string(score));
        dx = 0;
        rotate = 0;
        delay = 0.3;
        /////////draw//////////
        window.draw(background);
        window.draw(scoreText);

        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
            {
                if (field[i][j] == 0)
                    continue;
                s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
                s.setPosition(j * 18, i * 18);
                s.move(28, 31); // offset
                window.draw(s);
            }

        for (int i = 0; i < 4; i++)
        {
            s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
            s.setPosition(a[i].x * 18, a[i].y * 18);
            s.move(28, 31); // offset
            window.draw(s);
        }

        window.draw(frame);
        window.display();
    }
    return 0;
}
