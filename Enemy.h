#include <SFML/Graphics.hpp>

class Enemy : public sf::Drawable
{
private:
  sf::Sprite sprite;
  int speedPerSecond;
  float distanceTravelled;
  int maximumDistance;

protected:
  virtual void draw (sf::RenderTarget &target, sf::RenderStates states) const
  {
    target.draw(sprite, states);
  }

public:
  Enemy(const sf::Texture& texture, int speedPerSecond, int posx, int posy) : sprite(texture)
  {
    this->speedPerSecond = speedPerSecond;
    this->distanceTravelled = 0;
    this->maximumDistance = 472;
    this->sprite.setPosition(posx, posy);
  }

  void Update(float speedX, float timeDelta)
  {
    auto distanceTravlledThisUpdate = speedPerSecond * timeDelta;
    this->distanceTravelled += distanceTravlledThisUpdate;
    this->sprite.move(speedX*timeDelta, distanceTravlledThisUpdate);
    if(this->sprite.getPosition().x > 1600)
    {
      this->sprite.setPosition(-800, sprite.getPosition().y);
    }
    if(this->sprite.getPosition().x < -800)
    {
      this->sprite.setPosition(1600, sprite.getPosition().y);
    }
  }

  sf::FloatRect getGlobalBounds() const
  {
    return this->sprite.getGlobalBounds();
  }

  bool IsAlive() const
  {
    return this->distanceTravelled < this->maximumDistance;
  }
};
