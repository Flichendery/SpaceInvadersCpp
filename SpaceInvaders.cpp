#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <vector>
#include <chrono>
#include <thread>
#include <random>
#include <iostream>
#include <algorithm>

#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Barrier.h"

using namespace ftxui;
using namespace std;

// Объявление размера игры, количество жизней и прочего
const int width = 100;
const int height = 20;

int playerLives = 3;

enum Difficulty { EASY, MEDIUM, NIGHTMARE };
Difficulty difficulty = MEDIUM;

Player player(width / 2, height - 2);
Bullet playerBullet;
vector<Bullet> enemyBullets;
vector<Enemy> enemies;
vector<Barrier> barriers;
int score = 0;
int enemyDir = 1;
int tick = 0;
bool gameOver = false;
int playerVelocityX = 0;
std::mt19937 rng(random_device{}());

void Setup() {
    enemies.clear();
    int enemyCols = 10; // Количество столбцов противников
    int spacing = 8; // Расстояние между ними
    int offsetX = width / 2 - spacing * (enemyCols - 1) / 2;
    for (int row = 0; row < 4; ++row)
        for (int col = 0; col < enemyCols; ++col)
            enemies.emplace_back(offsetX + col * spacing, 2 + row * 2);
    barriers.clear();
    int barrierCount = 5;
    int gap = width / (barrierCount + 1);
    for (int i = 1; i <= barrierCount; ++i)
        barriers.emplace_back(i * gap - 1, height - 5);
    enemyBullets.clear();
    playerBullet.active = false;
    player.x = width / 2;
    player.y = height - 2;
    playerLives = 3;
    score = 0;
    enemyDir = 1;
    tick = 0;
    gameOver = false;
}

void CheckPlayerCollision() {
    for (auto& b : enemyBullets) {
        if (!b.active) continue;
        if (b.x == player.x && b.y == player.y) {
            b.active = false;
            playerLives--;
            if (playerLives <= 0) gameOver = true;
        }
    }
}

void Logic() {
    tick++;
    int moveRate = (difficulty == EASY) ? 25 : (difficulty == MEDIUM ? 17 : 10); // Настройка скорости — чем ниже значение, тем быстрее скорость
    if (tick % moveRate == 0) {
        bool switchDir = false;
        for (auto& e : enemies) {
            if (!e.alive) continue;
            e.x += enemyDir;
            if (e.x <= 0 || e.x >= width - 2)
                switchDir = true;
        }
        if (switchDir) {
            enemyDir *= -1;
            for (auto& e : enemies)
                e.y++;
        }
    }

    int fireChance = (difficulty == EASY) ? 10 : (difficulty == MEDIUM ? 7 : 4); // Частота выстрелов, чем ниже значение, тем чаще противники стреляют
    std::uniform_int_distribution<int> dist(0, fireChance - 1);
    if (dist(rng) == 0) {
        int idx = std::uniform_int_distribution<int>(0, enemies.size() - 1)(rng);
        if (enemies[idx].alive) {
            const Enemy& shooter = enemies[idx];
            int dy = player.y - shooter.y;
            if (dy <= 0) return;
            int predictedX = player.x + playerVelocityX * dy;
            predictedX = std::clamp(predictedX, 0, width - 1);
            int dx = (predictedX > shooter.x) ? 1 : (predictedX < shooter.x ? -1 : 0);
            Bullet b;
            b.Shoot(shooter.x, shooter.y + 1, dx);
            enemyBullets.push_back(b);
        }
    }

    playerBullet.Update();
    for (auto& b : enemyBullets)
        b.Update(true);

    if (playerBullet.active) {
        for (auto& e : enemies) {
            if (e.alive && playerBullet.y == e.y &&
                (playerBullet.x == e.x || playerBullet.x == e.x + 1)) {
                e.alive = false;
                playerBullet.active = false;
                score += 10;
                break;
            }
        }
        for (auto& b : barriers) {
            if (b.hp > 0 &&
                (playerBullet.x >= b.x && playerBullet.x < b.x + 3) &&
                playerBullet.y == b.y) {
                b.hp--;
                playerBullet.active = false;
                break;
            }
        }
    }

    for (auto& b : enemyBullets) {
        if (!b.active) continue;
        if (b.x == player.x && b.y == player.y) {
            b.active = false;
            playerLives--;
            if (playerLives <= 0) gameOver = true;
        }
        for (auto& br : barriers) {
            if (br.hp > 0 &&
                (b.x >= br.x && b.x < br.x + 3) &&
                b.y == br.y) {
                br.hp--;
                b.active = false;
                break;
            }
        }
    }

    for (auto& e : enemies)
        if (e.alive && e.y >= height - 2)
            gameOver = true;
}

Element RenderGame() {
    bool playerWon = std::none_of(enemies.begin(), enemies.end(), [](const Enemy& e) {
        return e.alive;
    });
    if (!gameOver && playerWon) gameOver = true;
    if (!gameOver && playerLives <= 0) gameOver = true;

    if (gameOver) {
        std::vector<std::wstring> message_lines;
        Color message_color;

        if (playerWon) {
            message_lines = {
                L"__     ______  _    _  __          _______ _   _",
                L" \\ \\   / / __ \\| |  | | \\ \\        / /_   _| \\ | |",
                L"  \\ \\_/ / |  | | |  | |  \\ \\  /\\  / /  | | |  \\| |",
                L"   \\   /| |  | | |  | |   \\ \\/  \\/ /   | | | . ` |",
                L"    | | | |__| | |__| |    \\  /\\  /   _| |_| |\\  |",
                L"    |_|  \\____/ \\____/      \\/  \\/   |_____|_| \\_|"
            };
            message_color = Color::SeaGreen2;
        } else {
            message_lines = {
                L" __     ______  _    _   _      ____   _____ ______ ",
                L" \\ \\   / / __ \\| |  | | | |    / __ \\ / ____|  ____|",
                L"  \\ \\_/ / |  | | |  | | | |   | |  | | (___ | |__   ",
                L"   \\   /| |  | | |  | | | |   | |  | |\\___ \\|  __|  ",
                L"    | | | |__| | |__| | | |___| |__| |____) | |____ ",
                L"    |_|  \\____/ \\____/  |______\\____/|_____/|______|",
                L"", L"T R Y   A G A I N !"
            };
            message_color = Color::OrangeRed1;
        }

        std::vector<Element> ascii_lines;
        for (const auto& line : message_lines)
            ascii_lines.push_back(text(line) | center | color(message_color));

        return hbox({
            window(
                text(" GAME OVER ") | center,
                vbox(std::move(ascii_lines)) | size(WIDTH, EQUAL, width)
            )
        }) | center;
    }

    vector<vector<wstring>> screen(height, vector<wstring>(width, L" "));
    vector<vector<Color>> colors(height, vector<Color>(width, Color::Purple4));

    if (playerLives > 0) {
        screen[player.y][player.x] = L"A";
        colors[player.y][player.x] = Color::SeaGreen3;
    }

    if (playerBullet.active && playerBullet.y >= 0 && playerBullet.y < height &&
        playerBullet.x >= 0 && playerBullet.x < width) {
        screen[playerBullet.y][playerBullet.x] = L"*";
        colors[playerBullet.y][playerBullet.x] = Color::SpringGreen2Bis;
    }

    for (auto& e : enemies)
        if (e.alive && e.y >= 0 && e.y < height && e.x >= 0 && e.x + 1 < width) {
            screen[e.y][e.x] = L"W";
            screen[e.y][e.x + 1] = L"W";
            colors[e.y][e.x] = Color::Red1;
            colors[e.y][e.x + 1] = Color::Red1;
        }

    for (auto& b : barriers)
        if (b.hp > 0 && b.y >= 0 && b.y < height)
            for (int i = 0; i < 3; ++i)
                if (b.x + i < width) {
                    screen[b.y][b.x + i] = L"▄";
                    colors[b.y][b.x + i] = Color::Yellow2;
                }

    for (auto& b : enemyBullets)
        if (b.active && b.y >= 0 && b.y < height && b.x >= 0 && b.x < width) {
            screen[b.y][b.x] = L"*";
            colors[b.y][b.x] = Color::Red3;
        }

    vector<Element> rows;
    for (int y = 0; y < height; ++y) {
        vector<Element> line;
        for (int x = 0; x < width; ++x)
            line.push_back(text(screen[y][x]) | color(colors[y][x]));
        rows.push_back(hbox(std::move(line)));
    }

    return hbox({
        window(
            text(" Space Invaders by Flichendery") | center,
            vbox({
                vbox(std::move(rows)) | size(WIDTH, EQUAL, width),
                separator(),
                hbox({
                    text("Score: " + to_string(score)),
                    filler(),
                    text("Lives: " + to_string(playerLives))
                }) | size(WIDTH, EQUAL, width)
            })
        )
    }) | center;
}

int main() {
    system("cls");
    cout << "Choose difficulty: (1) Easy  (2) Medium  (3) Nightmare\n> ";
    int d = 2;
    cin >> d;
    if (d == 1) difficulty = EASY;
    else if (d == 3) difficulty = NIGHTMARE;

    Setup();
    auto screen = ScreenInteractive::TerminalOutput();

    Component game = Renderer([&] {
        return RenderGame();
    });

    game = CatchEvent(game, [&](Event e) {
        if (gameOver) {
            if (e == Event::Character('q')) screen.Exit();
            return false;
        }
        if (e == Event::Character('a')) {
            player.x = max(0, player.x - 1);
            playerVelocityX = -1;
            CheckPlayerCollision();
        } else if (e == Event::Character('d')) {
            player.x = min(width - 1, player.x + 1);
            playerVelocityX = 1;
            CheckPlayerCollision();
        } else if (e == Event::Character('j'))
            playerBullet.Shoot(player.x, player.y - 1);
        else if (e == Event::Character('q')) {
            gameOver = true;
            return true;
        } else playerVelocityX = 0;

        return false;
    });

    std::thread logicThread([&] {
        while (!gameOver) {
            Logic();
            screen.PostEvent(Event::Custom);
            std::this_thread::sleep_for(50ms);
        }
    });

    screen.Loop(game);
    logicThread.join();

    std::cout << "\033[2J\033[1;1H";
    std::cout << "GAME OVER! Final Score: " << score << std::endl;
}
