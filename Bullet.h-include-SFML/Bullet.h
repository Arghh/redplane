#include <SFML/Graphics.hpp>

class Bullet : public sf::Drawable
{
private:
  sf::Sprite sprite;
  bool directionLeft;
  int speedPerSecond;
  double distanceTravelled;
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

  void Update(double timeDelta)
  {
    auto distanceTravlledThisUpdate = speedPerSecond * timeDelta;

    this->distanceTravelled += distanceTravlledThisUpdate;

    if(directionLeft) 
    {
        distanceTravelled *= -1;
    }

    this->sprite.move(distanceTravelled, 0.0f);
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
