#include "Player.hpp"


void Player::setTexture(const sf::Texture& texture)
{
  this->mSprite.setTexture(texture);
}

void Player::setScale(float factorX, float factorY)
{
  this->mSprite.setScale(factorX, factorY);
}
void Player::move(float offsetX, float offsetY)
{
  this->mSprite.move(offsetX, offsetY);
}
sf::FloatRect Player::getPlayerGlobalBounds() const
{
    return mSprite.getGlobalBounds();
}

sf::Vector2f Player::getPlayerPosition() const
{
  return mSprite.getPosition();
}
void Player::setPlayerPosition(float x, float y)
{
  this->mSprite.setPosition(x,y);
}

void Player::setPlayerOrigin(float x, float y)
{
  this->mSprite.setOrigin(x,y);
}
void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}
