#include "gameManager.h"
#include "obstacleGround.h"
#include "obstacleAir.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <stdexcept>
#include <iostream>

GameManager::GameManager()
    : _window(sf::VideoMode(WIN_W, WIN_H), "Hide & Run"),
      _menu(_font),
      _state(GameState::Menu),
      _timer(GAME_DURATION),
      _spawnTimer(0.f),
      _nextSpawnIn(1.5f),
      _bgOffset(0.f)
{
    _window.setFramerateLimit(60);
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    try {
        if (!_font.loadFromFile(FONT_PATH))
            throw std::runtime_error(
                std::string("Impossible de charger : ") + FONT_PATH);
    } catch (const std::exception& e) {
        std::cerr << "[ERREUR] " << e.what() << std::endl;
    }

    _menu = Menu(_font);

    _txtTimer.setFont(_font);
    _txtTimer.setCharacterSize(28);
    _txtTimer.setFillColor(sf::Color::White);
    _txtTimer.setPosition(WIN_W / 2.f - 60.f, 16.f);

    _txtMessage.setFont(_font);
    _txtMessage.setCharacterSize(48);
    _txtMessage.setPosition(WIN_W / 2.f - 250.f, WIN_H / 2.f - 60.f);

    _ground.setSize({(float)WIN_W, 20.f});
    _ground.setPosition(0.f, GROUND_Y);
    _ground.setFillColor(sf::Color(60, 60, 80));

    _progressBar.setSize({(float)WIN_W - 40.f, 14.f});
    _progressBar.setPosition(20.f, WIN_H - 24.f);
    _progressBar.setFillColor(sf::Color(40, 40, 60));

    _progressFill.setSize({0.f, 14.f});
    _progressFill.setPosition(20.f, WIN_H - 24.f);
    _progressFill.setFillColor(sf::Color(70, 200, 100));
}

void GameManager::run() {
    sf::Clock clock;
    while (_window.isOpen()) {
        float dt = clock.restart().asSeconds();
        _processEvents();
        if (_state == GameState::Playing)
            _update(dt);
        _render();
    }
}

void GameManager::_resetGame() {
    _player      = Player();
    _obstacles.clear();
    _timer       = GAME_DURATION;
    _spawnTimer  = 0.f;
    _nextSpawnIn = 1.5f;
    _bgOffset    = 0.f;
    _state       = GameState::Playing;
}

void GameManager::_processEvents() {
    sf::Event event;
    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            _window.close();

        if (_state == GameState::Menu) {
            _menu.handleInput(event);
            MenuChoice c = _menu.getChoice();
            if (c == MenuChoice::Play)  { _menu.resetChoice(); _resetGame(); }
            if (c == MenuChoice::Quit)    _window.close();
        }
        else if (_state == GameState::Playing) {
            _player.handleInput(event);
        }
        else {
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Space)
                _state = GameState::Menu;
        }
    }
}

void GameManager::_update(float dt) {
    _bgOffset -= 200.f * dt;
    _player.update(dt);

    _timer -= dt;
    if (_timer <= 0.f) { _timer = 0.f; _state = GameState::GameOver; return; }
    if (_timer <= 0.5f) { _state = GameState::Victory; return; }

    _spawnTimer += dt;
    if (_spawnTimer >= _nextSpawnIn) {
        _spawnTimer  = 0.f;
        _nextSpawnIn = _randomFloat(OBS_INTERVAL_MIN, OBS_INTERVAL_MAX);
        _spawnObstacle();
    }

    for (auto& obs : _obstacles) obs->update(dt);

    _obstacles.erase(
        std::remove_if(_obstacles.begin(), _obstacles.end(),
            [](const std::unique_ptr<Obstacle>& o){
                return o->isOffScreen();
            }),
        _obstacles.end()
    );

    _checkCollisions();

    if (_player.isDead()) _state = GameState::GameOver;

    float prog = 1.f - (_timer / GAME_DURATION);
    _progressFill.setSize({(WIN_W - 40.f) * prog, 14.f});
}

void GameManager::_spawnObstacle() {
    float x = WIN_W + 60.f;
    if (std::rand() % 2 == 0)
        _obstacles.push_back(std::make_unique<ObstacleGround>(x));
    else
        _obstacles.push_back(std::make_unique<ObstacleAir>(x));
}

void GameManager::_checkCollisions() {
    for (auto& obs : _obstacles) {
        if (_player.getBounds().intersects(obs->getBounds())) {
            _player.hit();
            break;
        }
    }
}

void GameManager::_drawBackground() {
    _window.clear(sf::Color(20, 20, 40));

    for (int i = 0; i < 10; i++) {
        float x = std::fmod(_bgOffset + i * 130.f, WIN_W + 130.f);
        if (x < 0) x += WIN_W + 130.f;
        sf::RectangleShape line({2.f, 60.f});
        line.setPosition(x, GROUND_Y - 80.f);
        line.setFillColor(sf::Color(40, 40, 70));
        _window.draw(line);
    }

    sf::RectangleShape bunker({80.f, 80.f});
    bunker.setPosition(WIN_W - 120.f, GROUND_Y - 80.f);
    bunker.setFillColor(sf::Color(60, 60, 60));
    bunker.setOutlineColor(sf::Color(100, 200, 100));
    bunker.setOutlineThickness(3.f);
    _window.draw(bunker);

    sf::Text txtBunker;
    txtBunker.setFont(_font);
    txtBunker.setString("BUNKER");
    txtBunker.setCharacterSize(14);
    txtBunker.setFillColor(sf::Color(100, 255, 100));
    txtBunker.setPosition(WIN_W - 118.f, GROUND_Y - 95.f);
    _window.draw(txtBunker);
}

void GameManager::_drawHUD() {
    int sec = static_cast<int>(_timer);
    _txtTimer.setString("Temps : " + std::to_string(sec) + "s");
    _txtTimer.setFillColor(sec <= 10 ? sf::Color::Red : sf::Color::White);
    _window.draw(_txtTimer);

    sf::Text txtState;
    txtState.setFont(_font);
    txtState.setCharacterSize(22);
    txtState.setPosition(20.f, 16.f);

    switch (_player.getState()) {
        case PlayerState::Alive:
            txtState.setString("Etat : VIVANT");
            txtState.setFillColor(sf::Color(100, 255, 100));
            break;
        case PlayerState::Injured:
            txtState.setString("Etat : BLESSE");
            txtState.setFillColor(sf::Color(255, 165, 0));
            break;
        case PlayerState::Dead:
            txtState.setString("Etat : MORT");
            txtState.setFillColor(sf::Color::Red);
            break;
    }
    _window.draw(txtState);

    _window.draw(_progressBar);
    _window.draw(_progressFill);

    sf::Text txtProg;
    txtProg.setFont(_font);
    txtProg.setString("Bunker");
    txtProg.setCharacterSize(13);
    txtProg.setFillColor(sf::Color(100, 200, 100));
    txtProg.setPosition(WIN_W - 70.f, WIN_H - 26.f);
    _window.draw(txtProg);
}

void GameManager::_render() {
    _drawBackground();

    if (_state == GameState::Menu) {
        _menu.draw(_window);
    }
    else if (_state == GameState::Playing) {
        _window.draw(_ground);
        for (auto& obs : _obstacles) obs->draw(_window);
        _player.draw(_window);
        _drawHUD();
    }
    else {
        _window.draw(_ground);
        for (auto& obs : _obstacles) obs->draw(_window);
        _player.draw(_window);
        _drawHUD();

        sf::RectangleShape overlay({(float)WIN_W, (float)WIN_H});
        overlay.setFillColor(sf::Color(0, 0, 0, 160));
        _window.draw(overlay);

        if (_state == GameState::GameOver) {
            _txtMessage.setString("GAME OVER !");
            _txtMessage.setFillColor(sf::Color::Red);
        } else {
            _txtMessage.setString("VICTOIRE !");
            _txtMessage.setFillColor(sf::Color(100, 255, 100));
        }
        _window.draw(_txtMessage);

        sf::Text hint;
        hint.setFont(_font);
        hint.setString("Appuyez sur ESPACE pour revenir au menu");
        hint.setCharacterSize(22);
        hint.setFillColor(sf::Color(200, 200, 200));
        hint.setPosition(WIN_W / 2.f - 280.f, WIN_H / 2.f + 40.f);
        _window.draw(hint);
    }

    _window.display();
}

float GameManager::_randomFloat(float min, float max) {
    return min + static_cast<float>(std::rand()) /
           (static_cast<float>(RAND_MAX / (max - min)));
}