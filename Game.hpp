#ifndef GAME_INCLUDE
#define GAME_INCLUDE
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <algorithm>
#include <memory>
#include "Enemy.hpp"
#include "Bullet.hpp"
#include "Radar.hpp"
#include "Animation.hpp"
#include "AnimatedSprite.hpp"

class Game
{
public:
  Game();
  void Run();

private:
  sf::RenderWindow window;
  sf::View gameview;
  sf::Texture treepic;
  sf::Texture grasspic;
  sf::Texture mountainpic;
  sf::Texture plane;
  sf::Texture enemy;
  sf::Texture shoot;
  sf::Texture explosion;
  sf::Texture bomb;
  Animation boom;
  sf::Sprite mountain;
  sf::Sprite mountainRight;
  sf::Sprite mountainLeft;
  sf::Sprite tree;
  sf::Sprite treeRight;
  sf::Sprite treeLeft;
  sf::Sprite grass;
  sf::Sprite grassRight;
  sf::Sprite grassLeft;
  sf::Image icon;
  std::vector<std::shared_ptr<Enemy>> enemies;
  std::vector<std::shared_ptr<Bullet>> bullets;
  std::vector<std::shared_ptr<AnimatedSprite>> explosions;
  sf::Clock playerDeltaTime;
  sf::Clock enemyDeltaTime;
  sf::Clock bulletDeltaTimer;
  sf::Clock bulletSpawnTimer;
  sf::Clock enemySpawnTimer;
  sf::Clock explosionDeltaTimer;
  sf::Clock timeLeft;
  float speedX;
  float enemyMoveSpeed;
  int lives;
  int hitTheGround;
  int level;
  int countDown;
  sf::Text showEnemiesLeft;
  sf::Text livesLeft;
  sf::Text bombsExploded;
  sf::Font font;
  sf::Text gameOver;
  sf::Text timeRemaining;
  sf::Text showLevel;
  sf::Text tryAgain;
  sf::Text speedometer;
  sf::Time timerDifficulty;
  sf::Time levelTime;
  Radar radar;
  Player player;
  bool directionLeft;
  void processEvent();
  void playerMove();
  void loadExplosion();
  void animateExplosion();
  void collision();  
  void backgroundLoop();
  bool checkGameOver();
  bool checkWinner();
  void changeView();
  void render();
  void createPlayer();
  void createBullet();
  void createEnemies();
};
#endif // GAME_INCLUDE
