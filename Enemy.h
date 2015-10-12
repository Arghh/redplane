#include <SFML/Graphics.hpp>

class Enemy : public sf::Drawable
{
public:
    Enemy(const sf::Texture& texture,  float enemyMoveSpeed, float posx, float posy);
    void updateEnemy(float speedX, float timeDelta);
    sf::FloatRect getEnemyGlobalBounds() const;
    bool isEnemyAlive() const;

private:
    sf::Sprite      mSprite;
    float           mSpeedPerSecond;
    float           mDistanceTravelled;
    const float     mMaximumDistance;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
