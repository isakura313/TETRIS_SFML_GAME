#include <SFML/Graphics.hpp>
#include <time.h>
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
#include <unistd.h>


using namespace sf;

const int M = 20; // size  Y
const int N = 10; // size X

int field[M][N] = {0};
int countLine = 0;
int allCountLine = 0;

struct Point {
    int x, y;
} a[4], b[4];

int figures[7][4] =
{
    1, 3, 5, 7, // I
    2, 4, 5, 7, // Z
    3, 5, 4, 6, // S
    3, 5, 4, 7, // T
    2, 3, 5, 7, // L
    3, 5, 7, 6, // J
    2, 3, 4, 5, // O
}; // define figures

bool check() {
    for (int i = 0; i < 4; i++) {
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) {
            return 0;
        }
        // }else if (field[a[i].y][a[i].x]) {
        //     if(field[a[i].y][a[i].x] ==field[b[i].y][b[i].x] ) {
        //         std::cout << "Hello World!";
        //     }
        //
        //     return 0;
        // }
        else if (field[a[i].y][a[i].x]) return 0;
    }
    return 1;
};



int main() {
    srand(time(0));

    RenderWindow window(VideoMode(320, 480), "Tetris");
    Music music;
    //
    // if (!music.openFromFile("back_sound.wav"))
    //     return -1; // error
    // music.play();
    music.openFromFile("backgroundSound.wav");
    music.setVolume(5.f);
    music.setPitch(1.f);
    music.play();
    Music musicLose;
    musicLose.openFromFile("looseSound.wav");
    musicLose.setVolume(5.f);
    musicLose.setPitch(1.f);
    Music musicLine;
    musicLine.openFromFile("happySound.wav");
    musicLine.setVolume(5.f);
    musicLine.setPitch(1.f);
    Font font;
    font.loadFromFile("playFont.ttf");
    Text text;
    text.setFont(font);
    text.setString(std::to_string(countLine));
    text.setCharacterSize(50);
    text.setFillColor(Color::White);
    text.setPosition(260,8);
    Texture t1, t2, t3;
    t1.loadFromFile("tiles.png");
    t2.loadFromFile("background1.png");
    t3.loadFromFile("frame.png");

    Sprite tiles(t1), background(t2), frame(t3);

    int dx = 0; //move horizontally
    bool rotate = 0;
    int colorNum = 1;
    float timer = 0, delay = 0.3;

    Clock clock;

    while (window.isOpen()) {
        // float time = clock.getElapsedTime().asSeconds();
        // clock.restart();
        timer += clock.restart().asSeconds();
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                window.close();
            }
            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Up) {
                    rotate = true;
                } else if (e.key.code == Keyboard::Left) {
                    dx = -1;
                } else if (e.key.code == Keyboard::Right) {
                    dx = 1;
                }
            }
        }
            if (Keyboard::isKeyPressed(Keyboard::Down)) {
                delay = 0.05;
            }

        // move
        for (int i = 0; i < 4; i++) {

            b[i] = a[i];
            a[i].x += dx;

        }
        if (!check()) {
            for (int i = 0; i < 4; i++) {
                a[i] = b[i];

            }
        }
        // rotate
        if (rotate) {
            Point center = a[1];
            for (int i = 0; i < 4; i = i + 1) {
                int x = a[i].y - center.y;
                int y = a[i].x - center.x;
                a[i].x = center.x - x;
                a[i].y = center.y + y;
            }
            if (!check()) {
                for (int i = 0; i < 4; i = i + 1) {
                    a[i] = b[i];
                }
            }
        }
        //tick
        if (timer > delay) {
            {
                for (int i = 0; i < 4; i++) {
                    b[i] = a[i];
                    a[i].y += 1;
                }
                for (int i = 0; i < 4; i++) {
                    if (field[a[i].y][a[i].x]) {
                        if(field[a[i].y][a[i].x] ==field[b[i].y][b[i].x] ) {
                            std::cout << "Game over!";
                            // timer = 1000;
                            musicLose.play();
                            music.stop();
                            // window.clear();
                            sleep(3);
                            window.close();
                            break;

                        }
                    }

                }
                if (!check()) {
                    for (int i = 0; i < 4; i++) {
                        field[b[i].y][b[i].x] = colorNum;
                    }
                    colorNum = 1 + rand() % 7;
                    int n = rand() % 7;
                    for (int i = 0; i < 4; i++) {
                        // TODO обсудить появление фигуры
                        a[i].x = figures[n][i] % 2;
                        a[i].y = figures[n][i] / 2;
                    }
                }


                timer = 0;
            }

            /////check lose///////
            // for (int i= M-1; i> 0; i--) {
            //         if(field[i][19] != 0) {
            //             std::cout << std::to_string(field[i][19])+ ";";
            //
            //     }
            //
            // }
            ///////check lines//////////
            int k = M - 1;
            for (int i = M - 1; i > 0; i--) {
                int count = 0;
                for (int j = 0; j < N; j++) {
                    if (field[i][j]) {
                        count++;
                    }
                    field[k][j] = field[i][j]; // TODO позже удалить

                }
                if (count < N) {
                    k = k - 1;
                }
                else {
                    musicLine.play();
                    countLine = countLine + 1;
                    text.setString(std::to_string(countLine));

                }

            }


        }
        dx = 0;
        rotate = 0;
        delay = 0.3;
        // отрисовка?
        window.clear(Color::White);
        window.draw(background);
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++) {
                if (field[i][j] == 0) {
                    continue;
                }
                tiles.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
                tiles.setPosition(j * 18, i * 18);
                tiles.move(28, 31); //offset
                window.draw(tiles);
            }
        for (int i = 0; i < 4; i++) {
            tiles.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
            tiles.setPosition(a[i].x * 18, a[i].y * 18);
            tiles.move(28, 31); //offset
            window.draw(tiles);
        }
        window.draw(frame);
        window.draw(text);
        window.display();
    }
    return 0;
}
