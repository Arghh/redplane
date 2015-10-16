#ifndef PLAYER_INCLUDE
#define PLAYER_INCLUDE

#include <SFML/Graphics.hpp>

class Player : public sf::Drawable
{
public:
   sf::FloatRect getPlayerGlobalBounds() const;
   sf::Vector2f getPlayerPosition() const;
   void setPlayerPosition(float x, float y);
   void setPlayerOrigin(float x, float y);
   void setScale (float factorX, float factorY);
   void move(float offsetX, float offsetY);
   void setTexture(const sf::Texture& texture);
private:
    sf::Sprite      mSprite;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif // PLAYER_INCLUDE
