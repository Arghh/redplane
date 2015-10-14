#include "Game.h"

//TODO - cleanup code, gameradar, tune the parallex scrolling, make gameplay more enjoyable
sf::RenderWindow window;
sf::Texture treepic;
sf::Texture grasspic;
sf::Texture mountainpic;
sf::Texture plane;
sf::Texture bomb;
sf::Texture shoot;
sf::Texture explosion;
sf::Image icon;
sf::Sprite mountain;
sf::Sprite mountainRight;
sf::Sprite mountainLeft;
sf::Sprite tree;
sf::Sprite treeRight;
sf::Sprite treeLeft;
sf::Sprite grass;
sf::Sprite grassRight;
sf::Sprite grassLeft;
sf::Sprite player;
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
sf::Clock bulletTime;
float speedX;
float enemyMoveSpeed;
int lives;
int hitTheGround;
int level;
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
Animation boom;
bool directionLeft;

void Game::minimap()
{
  sf::View minimap (sf::Vector2f(player.getPosition().x, player.getPosition().y), sf::Vector2f(80,60));
  window.setView(minimap);
}
void Game::processEvent()
{
  sf::Event event;
  //events handler bad and slow. just using for closing the window
  while (window.pollEvent(event))
    if (event.type == sf::Event::Closed)
      window.close();
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    window.close();
}

void Game::playerMove()
{
  //timer
  const auto elapsed = playerDeltaTime.getElapsedTime();
  const float maxSpeed = 500.f;
  int realSpeed = abs(speedX);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
  {
    //cant move out of the map
    if (player.getPosition().y > 45)
    {
      /*if moving left. meaning speed is < 0*/
      if (speedX < 0)
      {
        if (speedX < -maxSpeed)
        {
          speedX = -maxSpeed;
        }
        else
        {
          speedX -= 2.f;
        }
        player.move(0, speedX * elapsed.asSeconds());
      }
      else
      {
        if (speedX > maxSpeed)
        {
          speedX = maxSpeed;
        }
        else
        {
          speedX += 2.f;
        }
        player.move(0, -speedX * elapsed.asSeconds());
      }

    }
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
  {
    ////cant move any lower
    if (player.getPosition().y < 450)
    {
      if (speedX < 0)
      {
        if (speedX < -maxSpeed)
        {
          speedX = -maxSpeed;
        }
        else
        {
          speedX -= 2.f;
        }
        player.move(0, -speedX*elapsed.asSeconds());
      }
      else
      {
        if (speedX > maxSpeed)
        {
          speedX = maxSpeed;
        }
        else
        {
          speedX += 2.f;
        }
        player.move(0, speedX * elapsed.asSeconds());
      }
    }
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
  {
    directionLeft= true;
    //change sprite to face left.
    player.setScale(-1,1);
    //starting speed if i turn around
    if(speedX < 0)
    {
      speedX /= 1.4f;
      speedX+=20.f;
    }
    if (speedX < maxSpeed&& speedX>-10)
    {
      speedX += (maxSpeed/(speedX+100))*10.f;
    }
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
  {
    directionLeft= false;
    player.setScale(1,1);
    if(speedX > 0)
    {
      speedX /= 1.4f;
      speedX-=20.f;
    }
    if (speedX > -maxSpeed && speedX<10)
    {
      speedX -= (-maxSpeed/(speedX-100))*10.f;
    }
  }
  else
  {
    if(speedX > 1)
    {
      speedX -= 2.f;
    }

    if(speedX < -1)
    {
      speedX += 2.f;
    } 
  }
  speedometer.setString("Speed: " + std::to_string(realSpeed) + " km/h");
  //move the backgrounds left or right
  float treeSpeed = elapsed.asSeconds() * speedX / 5;
  float grassSpeed = elapsed.asSeconds() * speedX;
  float mountainSpeed = elapsed.asSeconds() * speedX /10;
  grass.move(grassSpeed,0);
  grassLeft.move(grassSpeed,0);
  grassRight.move(grassSpeed,0);
  tree.move(treeSpeed, 0);
  treeRight.move(treeSpeed, 0);
  treeLeft.move(treeSpeed, 0);
  mountain.move(mountainSpeed, 0);
  mountainRight.move(mountainSpeed, 0);
  mountainLeft.move(mountainSpeed, 0);
  //restart the timer
  playerDeltaTime.restart();
}

void Game::createBullet()
{
  //moving bullets
  const auto reload = bulletSpawnTimer.getElapsedTime();
  const auto bulletDeltaTime = bulletDeltaTimer.getElapsedTime();
  const auto reloadDelay = sf::seconds(0.5f);
  //sf::Time bullet = bulletTime.getElapsedTime();
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && reload > reloadDelay)
  {
    //create new sprite referencing shot. set start postiion and add it to the bulletlist
    auto pBullet = std::make_shared<Bullet>(Bullet(shoot, directionLeft, player.getPosition()));
    bullets.push_back(pBullet);
    bulletSpawnTimer.restart();
  }

  for (auto i = bullets.begin(); i != bullets.end(); i++)
  {
    (*i)->updateBullet(bulletDeltaTime.asSeconds());
  }

  bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const std::shared_ptr<Bullet>& o) { return !o->isBulletAlive(); }), bullets.end());
  bulletDeltaTimer.restart();
}

void Game::createExplosion()
{
  const auto explosionDeltaTime = explosionDeltaTimer.getElapsedTime();
  //create animation using 16 images sheet. all sprites 30x30px big
  boom.setSpriteSheet(explosion);
  for (unsigned j = 0; j < explosion.getSize().y; j+=30)
  {
    for (unsigned i = 0; i < explosion.getSize().x; i+=30)
    {
      boom.addFrame(sf::IntRect(i,j,30,30));
    }
  }
  //loop tru explosions vector and animate the pointers
  for (auto i = explosions.begin(); i != explosions.end(); i++)
  {
    (*i)->play(boom);
    (*i)->update(explosionDeltaTime);
    (*i)->move(speedX*explosionDeltaTime.asSeconds(),0);
  }
  //remove pointer from vector if isAlive returns false.
  explosions.erase(std::remove_if(explosions.begin(), explosions.end(),[this](const std::shared_ptr<AnimatedSprite>& o){ return !o->isAlive(); }), explosions.end());
  explosionDeltaTimer.restart();
}

void Game::createEnemies()
{
  //create random starting position
  int posx = rand() % 2350 + (-750);
  int posy = -20;
  const auto spawnSome = enemySpawnTimer.getElapsedTime();
  const auto timeSinceStart = enemyDeltaTime.getElapsedTime();
  timerDifficulty = sf::seconds(5.f);
  if (spawnSome > timerDifficulty)
  {
    auto pEnemy = std::make_shared<Enemy>(Enemy(bomb,enemyMoveSpeed,posx,posy));
    enemies.push_back(pEnemy);
    enemySpawnTimer.restart();
  }

  for (auto i = enemies.begin(); i != enemies.end(); i++)
  {
    (*i)->updateEnemy(speedX, timeSinceStart.asSeconds());
  }

  enemies.erase(std::remove_if(enemies.begin(), enemies.end(),[this](const std::shared_ptr<Enemy> o)
  {
    //delete enemy if isenemy alive returns true. create explosion @ same cordinates
    if (!(*o).isEnemyAlive())
    {    
      auto pExplosion = std::make_shared<AnimatedSprite>(AnimatedSprite(sf::seconds(0.05f),false,false));
      pExplosion->setPosition(o->getEnemyPosition());
      explosions.push_back(pExplosion);
      hitTheGround++;
      return true;
    }
    else
      return false;
  }
  ), enemies.end());

  enemyDeltaTime.restart();
}

void Game::collision()
{
  for (auto e = enemies.begin(); e != enemies.end();)
  {
    //if element is removed dont increment the iterator and skip the collison check for the next element!
    bool collides = false;
    for (auto i = bullets.begin(); i != bullets.end(); i++)
    {
      sf::FloatRect enemiez = (*e)->getEnemyGlobalBounds();
      sf::FloatRect bulletz = (*i)->getBulletGlobalBounds();
      //checks if bullet hits enemy. if true animate explosion and break out of loop
      if (bulletz.intersects(enemiez))
      {
        auto pExplosion = std::make_shared<AnimatedSprite>(AnimatedSprite(sf::seconds(0.05f),false,false));
        pExplosion->setPosition((*e)->getEnemyPosition());
        explosions.push_back(pExplosion);
        bullets.erase(i);
        e = enemies.erase(e);
        collides = true;
        break;
      }
    }
    //if no collision happens check next index
    if (!collides)
      e++;
  }
  for (auto i = enemies.begin(); i != enemies.end(); i++)
  {
    sf::FloatRect playerbox = player.getGlobalBounds();
    if (playerbox.intersects((*i)->getEnemyGlobalBounds()))
    {
      auto pExplosion = std::make_shared<AnimatedSprite>(AnimatedSprite(sf::seconds(0.05f),false,false));
      pExplosion->setPosition(player.getPosition());
      explosions.push_back(pExplosion);
      player.setPosition(400, 472);
      speedX = 0;
      lives--;
      enemies.erase(i);
      break;
    }
  }
  bombsExploded.setString("Bombs Exploded: " + std::to_string(hitTheGround));
  showEnemiesLeft.setString("Enemies left: " + std::to_string(enemies.size()));
  livesLeft.setString("Lives left: " + std::to_string(lives));
}

void Game::backgroundLoop()
{ 
  if (grass.getPosition().x > 400)
  {
    grassLeft.setPosition(-1200, 472);
    grass.setPosition(-400, 472);
    grassRight.setPosition(400, 472);
  }
  else if (grass.getPosition().x < -400)
  {
    grassLeft.setPosition(-400, 472);
    grass.setPosition(400, 472);
    grassRight.setPosition(1200, 472);
  }
  if (tree.getPosition().x > 400)
  {
    treeLeft.setPosition(-1200, 285);
    tree.setPosition(-400, 285);
    treeRight.setPosition(400, 285);
  }
  else if (tree.getPosition().x < -400)
  {
    treeLeft.setPosition(-400, 285);
    tree.setPosition(400, 285);
    treeRight.setPosition(1200, 285);
  }

  if (mountain.getPosition().x > 400)
  {
    mountainLeft.setPosition(-1200, 0);
    mountain.setPosition(-400, 0);
    mountainRight.setPosition(400, 0);
  }
  else if (mountain.getPosition().x < -400)
  {
    mountainLeft.setPosition(-400, 0);
    mountain.setPosition(400, 0);
    mountainRight.setPosition(1200, 0);
  }
}

bool Game::checkIfGameOver()
{
  if (lives == 0 || hitTheGround > 3)
  {
    timeLeft.restart();
    enemies.clear();
    bullets.clear();
    speedX = 0;
    return true;
  }
  else
  {
    playerMove();
    return false;
  }
}

bool Game::checkWinner()
{
  const auto timer = timeLeft.getElapsedTime();
  int countDown = levelTime.asSeconds() - timer.asSeconds();
  if (timer >= levelTime)
  {
    enemies.clear();
    bullets.clear();
    timeRemaining.setString("You saved the planet!");
    speedX = 0;
    return true;
  }
  else
  { 
    timeRemaining.setString("Time left : " + std::to_string(countDown) + " seconds");
    return false;
  }
}

void Game::render()
{
  //clear old draw new place and show
  window.clear();
  window.draw(mountain);
  window.draw(mountainRight);
  window.draw(mountainLeft);
  window.draw(tree);
  window.draw(treeRight);
  window.draw(treeLeft);
  window.draw(grass);
  window.draw(grassRight);
  window.draw(grassLeft);
  window.draw(showEnemiesLeft);
  window.draw(livesLeft);
  window.draw(bombsExploded);
  window.draw(timeRemaining);
  window.draw(showLevel);
  window.draw(speedometer);

  if(checkWinner())
  {
    gameOver.setString("You survived.");
    tryAgain.setString("Press ENTER to play the next level");
    window.draw(gameOver);
    window.draw(tryAgain);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
    {
      bullets.clear();
      enemies.clear();
      timeLeft.restart();
      hitTheGround = 0;
      levelTime = levelTime + sf::seconds(30.f);
      level++;
      timerDifficulty -= sf::seconds(0.5f);
      enemyMoveSpeed += 40.f;
      player.setPosition(400, 472);
      showLevel.setString("Level: " + std::to_string(level));
    }
  }
  else if (checkIfGameOver())
  {	
    tryAgain.setString("Press ENTER to try again\nPress ESCAPE to exit");
    gameOver.setString("Game Over");
    window.draw(gameOver);
    window.draw(tryAgain);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
    { 
      timeLeft.restart();
      lives = 3;
      level = 1;
      hitTheGround = 0;
      levelTime = sf::seconds(60.f);
      timerDifficulty = sf::seconds(5.f);
      enemyMoveSpeed = 40.f;
      player.setPosition(400, 472);
      showLevel.setString("Level: " + std::to_string(level));
    }
  }
  else
  {
    window.draw(player);
    //draw all from enemies vector and bullets
    for (auto e = enemies.begin(); e != enemies.end(); e++)
    {
      window.draw(*(*e));
    }
    for (auto b = bullets.begin(); b != bullets.end(); b++)
    {
      window.draw(*(*b));
    }
    for (auto x = explosions.begin(); x != explosions.end(); x++)
    {
      window.draw(*(*x));        
    }
  }
  window.display(); 
}

void Game::Run()
{
  while (window.isOpen())
  { 
    //create a view relative to speed
    sf::View gameview (sf::Vector2f(player.getPosition().x-speedX*0.7f,300.f),sf::Vector2f(800,600));
    window.setView(gameview);
    speedometer.setPosition(window.getView().getCenter()+sf::Vector2f(-390,-300));
    timeRemaining.setPosition(window.getView().getCenter()+sf::Vector2f(-120,-300));
    livesLeft.setPosition(window.getView().getCenter()+sf::Vector2f(240,-300));
    showEnemiesLeft.setPosition(window.getView().getCenter()+sf::Vector2f(-390,270));
    bombsExploded.setPosition(window.getView().getCenter()+sf::Vector2f(-120,270));
    showLevel.setPosition(window.getView().getCenter()+sf::Vector2f(280,270));
    backgroundLoop();
    render();
    createBullet();
    createEnemies();
    createExplosion();
    collision();
    processEvent();
  }
}

Game::Game() : window(sf::VideoMode(800, 600, 32), "Rotes Flugzeug")
{
  //game window with player and background etc automatically created when i create instance of flyinggame class
  level = 1;
  speedX = 0;
  lives = 3;
  hitTheGround = 0;
  levelTime = sf::seconds(60.f);
  enemyMoveSpeed = 40.f;
  window.setMouseCursorVisible(false);
  /*window.setFramerateLimit(3);*/
  window.setVerticalSyncEnabled(true);
  mountainpic.loadFromFile("pic/new_mountain.png");
  icon.loadFromFile("pic/icon.png");
  grasspic.loadFromFile("pic/new_grass.png");
  treepic.loadFromFile("pic/trees_new.png");
  shoot.loadFromFile("pic/bullet-s.png");
  bomb.loadFromFile("pic/bomb_new.png");
  plane.loadFromFile("pic/player_right.png");
  explosion.loadFromFile("pic/explosion_2.png");
  font.loadFromFile("roboto-black.ttf");
  player.setTexture(plane);
  tree.setTexture(treepic);
  treeRight.setTexture(treepic);
  treeLeft.setTexture(treepic);
  grass.setTexture(grasspic);
  grassLeft.setTexture(grasspic);
  grassRight.setTexture(grasspic);
  mountain.setTexture(mountainpic);
  mountainRight.setTexture(mountainpic);
  mountainLeft.setTexture(mountainpic);
  tree.setPosition(0, 285);
  treeRight.setPosition(800, 285);
  treeLeft.setPosition(-800, 285);
  grass.setPosition(0,472);
  grassRight.setPosition(800,472);
  grassLeft.setPosition(-800,472);
  mountain.setPosition(0, 0);
  mountainRight.setPosition(800, 0);
  mountainLeft.setPosition(-800, 0);
  player.setPosition(400, 472);
  player.setOrigin(32,16);
  player.setTextureRect(sf::IntRect(0, 0, 64, 32));
  tree.setTextureRect(sf::IntRect(0, 280, 800, 250));
  treeRight.setTextureRect(sf::IntRect(0, 280, 800, 250));
  treeLeft.setTextureRect(sf::IntRect(0, 280, 800, 250));
  grass.setTextureRect(sf::IntRect(0, 472, 800,200));
  grassRight.setTextureRect(sf::IntRect(0, 472, 800,200));
  grassLeft.setTextureRect(sf::IntRect(0, 472, 800,200));
  showEnemiesLeft.setFont(font);
  showEnemiesLeft.setCharacterSize(24);
  showEnemiesLeft.setColor(sf::Color::Red);
  livesLeft.setPosition(620, 10);
  livesLeft.setFont(font);
  livesLeft.setCharacterSize(24);
  livesLeft.setColor(sf::Color::Red);
  bombsExploded.setFont(font);
  bombsExploded.setCharacterSize(24);
  bombsExploded.setColor(sf::Color::Red);
  gameOver.setPosition(200, 200);
  gameOver.setFont(font);
  gameOver.setCharacterSize(64);
  gameOver.setColor(sf::Color::Red);
  timeRemaining.setFont(font);
  timeRemaining.setCharacterSize(24);
  timeRemaining.setColor(sf::Color::Red);
  showLevel.setFont(font);
  showLevel.setCharacterSize(24);
  showLevel.setColor(sf::Color::Red);
  showLevel.setString("Level: " + std::to_string(level));
  speedometer.setFont(font);
  speedometer.setCharacterSize(24);
  speedometer.setColor(sf::Color::Red);
  tryAgain.setFont(font);
  tryAgain.setCharacterSize(16);
  tryAgain.setColor(sf::Color::Red);
  tryAgain.setPosition(260, 280);
  window.setIcon(icon.getSize().x, icon.getSize().y,icon.getPixelsPtr());
};
