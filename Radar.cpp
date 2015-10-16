#include "Radar.hpp"

Radar::Radar()
{
  texture.create(260, 80);
}

// draws an object onto the radar screen
void Radar::draw (Bullet& bullet)
{
  sf::RectangleShape b;
  b.setSize(sf::Vector2f(4,4));
  b.setFillColor(sf::Color::Cyan);
  b.setPosition(bullet.getBulletPosition().x/10.f+80.f,bullet.getBulletPosition().y/10.f+18.f);
  texture.draw(bullet);
}

void Radar::draw(Player& player)
{
  
  sf::CircleShape asd;
  asd.setRadius(8);
  asd.setFillColor(sf::Color::White);
  asd.setPosition(player.getPlayerPosition().x/10.f+80.f,player.getPlayerPosition().y/10.f+18.f);
  texture.draw(asd);
}
// draws an object onto the radar screen
void Radar::draw (Enemy& enemy)
{
  sf::RectangleShape e;
  e.setSize(sf::Vector2f(5,5));
  e.setFillColor(sf::Color::Red);
  e.setPosition(enemy.getEnemyPosition().x/10.f+80.f,enemy.getEnemyPosition().y/10.f+18.f);
  texture.draw(e);
}

// draws the radar screen onto another target
void Radar::draw(sf::RenderTarget &target,sf::RenderStates states) const
{
  // OPTIMIZE THIS
  sf::Sprite sprite(texture.getTexture());
  sprite.setPosition(target.getView().getCenter()+sf::Vector2f(-150,180));
  target.draw(sprite, states);
}

void Radar::clear (const sf::Color &color=sf::Color())
{
  texture.clear(color);
}

void Radar::display()
{
  texture.display();
}
