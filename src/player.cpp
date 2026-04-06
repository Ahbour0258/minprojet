#include "player.h"

Player::Player()
    : _velocity(0.f, 0.f),
      _state(PlayerState::Alive),
      _invTimer(0.f),
      _onGround(true),
      _crouching(false)
{
    _body.setSize({40.f, 70.f});
    _body.setOrigin(20.f, 70.f);
    _body.setPosition(PLAYER_X, GROUND_Y);
    _body.setFillColor(sf::Color(70, 130, 255));
    _body.setOutlineColor(sf::Color::White);
    _body.setOutlineThickness(2.f);

    _head.setRadius(18.f);
    _head.setOrigin(18.f, 18.f);
    _head.setFillColor(sf::Color(255, 200, 120));
    _head.setOutlineColor(sf::Color::White);
    _head.setOutlineThickness(2.f);
}

void Player::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if ((event.key.code == sf::Keyboard::Up ||
             event.key.code == sf::Keyboard::Space)
             && !_crouching)
            _jump();
        if (event.key.code == sf::Keyboard::Down ||
            event.key.code == sf::Keyboard::S)
            _setCrouch(true);
    }
    if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Down ||
            event.key.code == sf::Keyboard::S)
            _setCrouch(false);
    }
}

void Player::update(float dt) {
    if (!_onGround)
        _velocity.y += GRAVITY * dt;

    _body.move(0.f, _velocity.y * dt);

    if (_body.getPosition().y >= GROUND_Y) {
        _body.setPosition(_body.getPosition().x, GROUND_Y);
        _velocity.y = 0.f;
        _onGround   = true;
    }

    _head.setPosition(
        _body.getPosition().x,
        _body.getPosition().y - _body.getSize().y - 14.f
    );

    if (_invTimer > 0.f) _invTimer -= dt;
    _updateColor();
}

void Player::draw(sf::RenderWindow& window) const {
    if (_invTimer > 0.f &&
        static_cast<int>(_invTimer * 8) % 2 == 0)
        return;
    window.draw(_body);
    window.draw(_head);
}

sf::FloatRect Player::getBounds() const {
    return _body.getGlobalBounds();
}

void Player::hit() {
    if (_invTimer > 0.f) return;
    if (_state == PlayerState::Alive) {
        _state    = PlayerState::Injured;
        _invTimer = 2.f;
    } else if (_state == PlayerState::Injured) {
        _state = PlayerState::Dead;
    }
}

void Player::_jump() {
    if (_onGround) {
        _velocity.y = JUMP_FORCE;
        _onGround   = false;
    }
}

void Player::_setCrouch(bool active) {
    _crouching = active;
    if (active) {
        _body.setSize({40.f, 35.f});
        _body.setOrigin(20.f, 35.f);
    } else {
        _body.setSize({40.f, 70.f});
        _body.setOrigin(20.f, 70.f);
    }
}

void Player::_updateColor() {
    if (_state == PlayerState::Injured)
        _body.setFillColor(sf::Color(255, 140, 0));
    else if (_state == PlayerState::Dead)
        _body.setFillColor(sf::Color(180, 30, 30));
    else
        _body.setFillColor(sf::Color(70, 130, 255));
}