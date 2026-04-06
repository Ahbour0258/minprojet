#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "player.h"
#include "obstacle.h"
#include "menu.h"
#include "constants.h"

enum class GameState { Menu, Playing, GameOver, Victory };

class GameManager {
public:
    GameManager();
    void run();

private:
    sf::RenderWindow                       _window;
    sf::Font                               _font;
    sf::Text                               _txtTimer;
    sf::Text                               _txtMessage;
    sf::RectangleShape                     _ground;
    sf::RectangleShape                     _progressBar;
    sf::RectangleShape                     _progressFill;

    Player                                          _player;
    std::vector<std::unique_ptr<Obstacle>>          _obstacles;
    Menu                                            _menu;
    GameState                                       _state;

    float _timer;
    float _spawnTimer;
    float _nextSpawnIn;
    float _bgOffset;

    void  _processEvents();
    void  _update(float dt);
    void  _render();
    void  _spawnObstacle();
    void  _checkCollisions();
    void  _drawHUD();
    void  _drawBackground();
    void  _resetGame();
    float _randomFloat(float min, float max);
};