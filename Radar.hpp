#ifndef RADAR_INCLUDE
#define RADAR_INCLUDE
#include <SFML/Graphics.hpp>
#include<SFML/Window/Window.hpp>
#include <SFML/System.hpp>
#include "Bullet.hpp"
#include "Enemy.hpp"
#include "Player.hpp"

class Radar : public sf::Drawable
{
private:
  sf::RenderTexture texture;
  sf::Sprite sprite;
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
  
public:
  Radar::Radar();
  void draw (Bullet& bullet);
  void draw (Enemy& enemy);
  void draw (Player& player);
  void clear (const sf::Color &color);
  void display();
};
#endif
