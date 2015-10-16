#ifndef BULLET_INCLUDE
#define BULLET_INCLUDE
#include <SFML/Graphics.hpp>

class Bullet : public sf::Drawable
{
public:
    Bullet(const sf::Texture& texture, bool direction, const sf::Vector2f& position);
    void updateBullet(float timeDelta);
    sf::FloatRect getBulletGlobalBounds() const;
    sf::Vector2f getBulletPosition() const;
    bool isBulletAlive() const;

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    sf::Sprite      mSprite;
    const float     mSpeedPerSecond;
    float           mDistanceTravelled;
    const float     mMaximumDistance;
    bool            direction;
};
#endif
