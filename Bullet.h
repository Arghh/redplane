#include <SFML/Graphics.hpp>

class Bullet : public sf::Drawable
{
private:
  sf::Sprite sprite;
  bool directionLeft;
  int speedPerSecond;
  float distanceTravelled;
  int maximumDistance;

protected:
  virtual void draw (sf::RenderTarget &target, sf::RenderStates states) const
  {
    target.draw(sprite, states);
  }

public:
  Bullet(const sf::Texture& texture, bool directionLeft, int speedPerSecond, int distance, const sf::Vector2f& position) : sprite(texture)
  {
    this->directionLeft = directionLeft;
    this->speedPerSecond = speedPerSecond;
    this->distanceTravelled = 0;
    this->maximumDistance = distance;

    this->sprite.setPosition(position);
  }

  void Update(float timeDelta)
  {
    auto distanceTravelledThisUpdate = speedPerSecond * timeDelta;

    this->distanceTravelled += distanceTravelledThisUpdate;

    if(directionLeft) 
    {
        this->sprite.move(-distanceTravelledThisUpdate,0);
    }
    else
    {
    this->sprite.move(distanceTravelledThisUpdate, 0);
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
