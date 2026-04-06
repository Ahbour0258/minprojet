#pragma once
#include <SFML/Graphics.hpp>
#include "constants.h"

enum class PlayerState { Alive, Injured, Dead };

class Player {
public:
    Player();

    void handleInput(const sf::Event& event);
    void update(float dt);
    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getBounds()    const;
    PlayerState   getState()     const { return _state; }
    bool          isDead()       const { return _state == PlayerState::Dead; }
    bool          isInvincible() const { return _invTimer > 0.f; }
    void          hit();

private:
    sf::RectangleShape _body;
    sf::CircleShape    _head;
    sf::Vector2f       _velocity;
    PlayerState        _state;
    float              _invTimer;
    bool               _onGround;
    bool               _crouching;

    void _jump();
    void _setCrouch(bool active);
    void _updateColor();
};