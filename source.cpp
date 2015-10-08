#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>

#include "Bullet.h"

//TODO-s
//organize code
//animate explosions
//BUG bullets change direction if player turns

class FlyingGame
{
private:
  sf::RenderWindow window;
  sf::Texture treepic;
  sf::Texture grasspic;
  sf::Texture mountainpic;
  sf::Texture plane;
  sf::Texture plane_left;
  sf::Texture enemy;
  sf::Texture shoot;
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
  sf::Sprite bullet;
  std::vector<sf::Sprite> enemies;
  std::vector<std::shared_ptr<Bullet>> bullets;
  sf::FloatRect enemiesbox;
  sf::Sprite oneEnemy;
  sf::Clock clock;
  sf::Clock enemySpeed;
  sf::Clock bulletSpeedClock;
  sf::Clock time_last_shot;
  sf::Clock enemySpawnTimer;
  sf::Clock timeLeft;
  sf::Clock bulletTime;
  float posX;
  float treeSpeed;
  float mountainSpeed;
  float grassSpeed;
  float speedX;
  float enemyMoveSpeed;
  int lives;
  int enemiesLeft;
  int hitTheGround;
  int enemiesKilled;
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
  bool directionLeft;


  void render()
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
    if (checkIfGameOver())
    {	
      tryAgain.setString("Press ENTER to try again\nPress ESCAPE to exit the game");
      gameOver.setString("Game Over");
      window.draw(gameOver);
      window.draw(tryAgain);
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
      { 
        bullets.clear();
        enemies.clear();
        timeLeft.restart();
        enemiesLeft = 0;
        lives = 3;
        level = 1;
        hitTheGround = 0;
        levelTime = sf::seconds(30.f);
        timerDifficulty = sf::seconds(4.f);
        enemyMoveSpeed = 40.f;
        player.setPosition(400, 472);
        showLevel.setString("Level: " + int2Str(level));
      }
    }
    else if(checkWinner())
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
        enemiesLeft = 0;
        hitTheGround = 0;
        levelTime = levelTime + sf::seconds(30.f);
        level++;
        timerDifficulty = timerDifficulty - sf::seconds(0.1f);
        enemyMoveSpeed = enemyMoveSpeed + 10.f;
        player.setPosition(400, 472);
        showLevel.setString("Level: " + int2Str(level));
      }
    }
    else
    {
      window.draw(player);
      //draw all from enemies vector and bullets
      for (auto e = enemies.begin(); e != enemies.end(); e++)
      {
        window.draw(*e);
      }
      for (auto b = bullets.begin(); b != bullets.end(); b++)
      {
        window.draw(*(*b));
      }
    }
    window.display();
  }

  void processEvent()
  {
    sf::Event event;
    //events handler bad and slow. just using for closing the window
    while (window.pollEvent(event))
      //  switch(event.type)
        //{
          // case sf::Event::KeyPressed : PlayerInput(event.key.code, true);
            // //    //  break;
              // //    //  case sf::Event::KeyReleased : PlayerInput(event.key.code, false);
                // //      break;
                  if (event.type == sf::Event::Closed)
                    window.close();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
      window.close();
    //}
  }

  void playerMove()
  {
    //timer
    sf::Time elapsed = clock.getElapsedTime();
    float maxSpeed = 500.f;
    int realSpeed = abs(speedX);
    //vector to get players position on the map
    sf::Vector2f spot = player.getPosition();
    std::string speedmeter = int2Str(abs(speedX));
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
            speedX -= 5.f;
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
            speedX += 3.f;
          }
          player.move(0, -speedX * elapsed.asSeconds());

       }
      }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
      //cant move any lower
      if (player.getPosition().y < 472)
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
      //change sprite to face left. player smooth move.
      player.setTexture(plane_left);    
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
      player.setTexture(plane);
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
    //move the backgrounds left or right
    treeSpeed = elapsed.asSeconds() * speedX / 1.5;
    grassSpeed = elapsed.asSeconds() * speedX;
    mountainSpeed = elapsed.asSeconds() * speedX / 3.5;
    grass.move(grassSpeed, 0);
    grassRight.move(grassSpeed, 0);
    grassLeft.move(grassSpeed, 0);
    tree.move(treeSpeed, 0);
    treeRight.move(treeSpeed, 0);
    treeLeft.move(treeSpeed, 0);
    mountain.move(mountainSpeed, 0);
    mountainRight.move(mountainSpeed, 0);
    mountainLeft.move(mountainSpeed, 0);
    //restart the timer
    clock.restart();
    //for tests
    ////int  X = speedX;
    //int  Y = player.getPosition().y;//mountain.getPosition().x;window.getView().getCenter().x;
    //printf_s("%d ", Y);
    speedometer.setString("Speed: " + int2Str(realSpeed) + " km/h");
  }

  void enemyMove()
  {
    sf::Time enemyElapsed = enemySpeed.getElapsedTime();
    sf::FloatRect playerbox = player.getGlobalBounds();
    enemyMoveSpeed = 40.f;
    for (auto i = enemies.begin(); i != enemies.end();)
    {
      enemiesbox = i->getGlobalBounds();

      if (playerbox.intersects(enemiesbox))
      {
        player.setPosition(400, 472);
        speedX = 0;
        lives = lives - 1;
      }

      sf::Vector2f asd = i->getPosition();
      if (asd.y >= 470)//funktsioniert nicht :? DOCH
      {
        //i->setPosition(asd.x, 480);
        hitTheGround++;
        enemiesLeft--;
        i->getPosition().y;
        enemies.erase(i);
        break;
      }
      else if (i->getPosition().x > 1600)
      {
        i->setPosition(-800, asd.y);
      }
      else if (i->getPosition().x < -800)
      {
        i->setPosition(1600, asd.y);
      }
      else
      {
        //enemy move x at the same speed the player does for circlemap effect. enemy y can change freely
        i->move(speedX*enemyElapsed.asSeconds(), enemyMoveSpeed * enemyElapsed.asSeconds());
        i++;
      }
    }
    livesLeft.setString("Lives left: " + int2Str(lives));
    bombsExploded.setString("Bombs Exploded: " + int2Str(hitTheGround));
    enemySpeed.restart();
  }

  void createBullet()
  {
    //moving bullets
    sf::Time reload = time_last_shot.getElapsedTime();
    sf::Time bulletSpeed = bulletSpeedClock.getElapsedTime();
    //sf::Time bullet = bulletTime.getElapsedTime();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
      //create new sprite reference shot. set start postiion and add it to the bulelt list
      if (reload > sf::seconds(0.8f))
      {
        std::shared_ptr<Bullet> pBullet(new Bullet(shoot, directionLeft, 1000, 500, player.getPosition()));      
        bullets.push_back(pBullet);
        time_last_shot.restart();
      }
    }

    for (auto i = bullets.begin(); i != bullets.end(); i++)
    {
      (*i)->Update(bulletSpeed.asSeconds());
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const std::shared_ptr<Bullet>& o) { return !o->IsAlive(); }), bullets.end());
    bulletSpeedClock.restart();
  }

  void collision()
  {
    for (auto e = enemies.begin(); e != enemies.end();)
    {
      //if element is removed dont increment the iterator and skip the collison check for the next element!
      bool collides = false;
      for (auto i = bullets.begin(); i != bullets.end(); i++)
      {
        sf::FloatRect enemiez = e->getGlobalBounds();
        sf::FloatRect bulletz = (*i)->getGlobalBounds();

        if (bulletz.intersects(enemiez))
        {
          enemiesLeft = enemiesLeft - 1;
          enemiesKilled++;
          bullets.erase(i);
          //assing iterator to return value of erase, because once you erase an elelemnt your pervious iterator is invalidated.
          e = enemies.erase(e);
          collides = true;
          break;
        }
      }
      if (!collides)
      {
        e++;
      }
    }
    showEnemiesLeft.setString("Enemies left: " + int2Str(enemiesLeft));
  }


  void placeEnemiesAtStart()
  {	
    float posx = rand() % 2350 + (-750);
    float posy = rand() % 50 + (-50);
    oneEnemy.setTexture(enemy);
    oneEnemy.setPosition(posx, posy);
  }

  void backgroundLoop()
  { 
    if (grass.getPosition().x > 400)
    {
      grassLeft.setPosition(-1200, 475);
      grass.setPosition(-400, 475);
      grassRight.setPosition(400, 475);
    }
    else if (grass.getPosition().x < -400)
    {
      grassLeft.setPosition(-400, 475);
      grass.setPosition(400, 475);
      grassRight.setPosition(1200, 475);
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

  static inline std::string int2Str(float x)
    //converts int to string with help of sstream namespace
  {
    std::stringstream type;
    type << x;
    return type.str();
  }

  bool checkIfGameOver()
  {
    if (lives == 0 || hitTheGround > 3)
    {
      enemies.clear();
      speedX = 0;
      enemiesLeft = 0;
      return true;
    }
    else
    {
      playerMove();
      createEnemies();
      return false;
    }
  }
  bool checkWinner()
  {
    sf::Time timer = timeLeft.getElapsedTime();
    int countDown = levelTime.asSeconds() - timer.asSeconds();
    if (timer >= levelTime)
    {
      enemies.clear();
      enemiesLeft = 0;
      timeRemaining.setString("You saved the planet!");
      speedX = 0;
      return true;
    }
    else
    { 
      playerMove();
      createEnemies();
      timeRemaining.setString("Time left : " + int2Str(countDown) + " seconds");
      return false;
    }

  }
  void createEnemies()
  {
    sf::Time spawnSome = enemySpawnTimer.getElapsedTime();
    timerDifficulty = sf::seconds(4.f);
    if (spawnSome > timerDifficulty)
    {
      enemiesLeft++;
      placeEnemiesAtStart();
      enemies.push_back(oneEnemy);
      enemySpawnTimer.restart();
    }
  }
public:
  void Run()
  {
    while (window.isOpen())
    {     
      processEvent();
      //create a view relative to speed
       sf::View gameview (sf::Vector2f(player.getPosition().x-speedX*0.7,300),sf::Vector2f(800,600));
       window.setView(gameview);
      speedometer.setPosition(window.getView().getCenter()+sf::Vector2f(-390,-300));
      timeRemaining.setPosition(window.getView().getCenter()+sf::Vector2f(-120,-300));
      livesLeft.setPosition(window.getView().getCenter()+sf::Vector2f(240,-300));
      showEnemiesLeft.setPosition(window.getView().getCenter()+sf::Vector2f(-390,270));
      bombsExploded.setPosition(window.getView().getCenter()+sf::Vector2f(-120,270));
      showLevel.setPosition(window.getView().getCenter()+sf::Vector2f(280,270));
     /* playerMove();*/
      enemyMove();
      createBullet();
      collision();
      backgroundLoop();
      //createEnemies();
       render();
    }
  }

  FlyingGame() : window(sf::VideoMode(800, 600, 32), "Rotes Flugzeug")
  {
    //game window with player and background etc automaticly created when i create instance of flyinggame class
    level = 1;
    speedX = 0;
    lives = 3;
    hitTheGround = 0;
    enemiesLeft = 0;
    enemiesKilled = 0;
    levelTime = sf::seconds(30.f);
    window.setMouseCursorVisible(false);
    //window.setFramerateLimit(3);
    window.setVerticalSyncEnabled(true);
    mountainpic.loadFromFile("pic/mountain.png");
    grasspic.loadFromFile("pic/new_grass.png");
    treepic.loadFromFile("pic/trees.png");
    shoot.loadFromFile("pic/bullet-s.png");
    enemy.loadFromFile("pic/bomb_new.png");
    plane.loadFromFile("pic/player_right.png");
    plane_left.loadFromFile("pic/player_left.png");
    font.loadFromFile("roboto-black.ttf");
    plane.setSmooth(true);
    plane_left.setSmooth(true);
    shoot.setSmooth(true);
    enemy.setSmooth(true);
    player.setTexture(plane);
    tree.setTexture(treepic);
    treeRight.setTexture(treepic);
    treeLeft.setTexture(treepic);
    grassRight.setTexture(grasspic);
    grass.setTexture(grasspic);
    grassLeft.setTexture(grasspic);
    mountain.setTexture(mountainpic);
    mountainRight.setTexture(mountainpic);
    mountainLeft.setTexture(mountainpic);
    tree.setPosition(0, 285);
    treeRight.setPosition(800, 285);
    treeLeft.setPosition(-800, 285);
    grass.setPosition(0, 475);
    grassRight.setPosition(800, 475);
    grassLeft.setPosition(-800, 475);
    mountain.setPosition(0, 0);
    mountainRight.setPosition(800, 0);
    mountainLeft.setPosition(-800, 0);
    player.setPosition(400, 472);
    player.setOrigin(32,16);
    player.setTextureRect(sf::IntRect(0, 0, 64, 32));
    tree.setTextureRect(sf::IntRect(0, 280, 800, 250));
    treeRight.setTextureRect(sf::IntRect(0, 280, 800, 250));
    treeLeft.setTextureRect(sf::IntRect(0, 280, 800, 250));
    mountain.setTextureRect(sf::IntRect(0, 0, 800, 500));
    mountainRight.setTextureRect(sf::IntRect(0, 0, 800, 500));
    mountainLeft.setTextureRect(sf::IntRect(0, 0, 800, 500));
    grass.setTextureRect(sf::IntRect(0, 470, 800, 200));
    grassRight.setTextureRect(sf::IntRect(0, 470, 800, 200));
    grassLeft.setTextureRect(sf::IntRect(0, 470, 800, 200));
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
    showLevel.setString("Level: " + int2Str(level));
    speedometer.setFont(font);
    speedometer.setCharacterSize(24);
    speedometer.setColor(sf::Color::Red);
    tryAgain.setFont(font);
    tryAgain.setCharacterSize(24);
    tryAgain.setColor(sf::Color::Red);
    tryAgain.setPosition(220, 280);
  }
};


int main()
{
  srand(time(0));
  FlyingGame game;
  game.Run();
  return EXIT_SUCCESS;
}
