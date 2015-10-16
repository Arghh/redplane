#include "Bullet.hpp"


Bullet::Bullet(const sf::Texture& texture, bool directionLeft, const sf::Vector2f& position) :
  mSprite(texture),
  mSpeedPerSecond(800.f),
  mDistanceTravelled(0),
  mMaximumDistance(1000.f)
{
  this->direction = directionLeft;
  this->mSprite.setPosition(position);
}

void Bullet::updateBullet(float timeDelta) 
{
  auto mDistanceTravelledThisUpdate = mSpeedPerSecond * timeDelta;

  mDistanceTravelled += mDistanceTravelledThisUpdate;
  if(direction) 
  {
    mSprite.move(-mDistanceTravelledThisUpdate, 0);
  }
  else
  {
    mSprite.move(mDistanceTravelledThisUpdate, 0);
  }
}

sf::FloatRect Bullet::getBulletGlobalBounds() const
{
  return mSprite.getGlobalBounds();
}
sf::Vector2f Bullet::getBulletPosition() const
{
  return mSprite.getPosition();
}
bool Bullet::isBulletAlive() const
{
  return mDistanceTravelled < mMaximumDistance;
}

void Bullet::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}
