#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

//TODO-s
//bulletsmove away from player loction. direction - and +. away at a steady speed. bullets keep their speed and direction until hit something or disappear.
//erase bullets if too long. add livesLef timer etc. rewrite spagethicode. add easy medium hard gamemodes. highscore maybe? startscreen?
//fix flickering when moving up and down at high speed (opengl solves this)
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
	sf::Texture explosion;
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
	std::vector<sf::Sprite> bullets;
	sf::FloatRect enemiesbox;
	sf::Sprite oneEnemy;
	sf::Clock clock;
	sf::Clock enemySpeed;
	sf::Clock bulletSpeedClock;
	sf::Clock time_last_shot;
	sf::Clock bulletTime;
	sf::Clock enemySpawnTimer;
	sf::Clock timeLeft;
	float posX;
	float treeSpeed;
	float mountainSpeed;
	float grassSpeed;
	float speedX;
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
	bool direction_left;

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
			tryAgain.setString("Press SPACE to try again\nPress ESCAPE to exit the game");
			gameOver.setString("Game Over");
			window.draw(gameOver);
			window.draw(tryAgain);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{ 
				enemies.clear();
				timeLeft.restart();
				enemiesLeft = 0;
				lives = 3;
				hitTheGround = 0;
				
			}
		}
		else if (checkWinner())
		{
			gameOver.setString("You Survived");
			window.draw(gameOver);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				enemies.clear();
				timeLeft.restart();
				enemiesLeft = 0;
				lives = 3;
				hitTheGround = 0;	
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
				window.draw(*b);
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
			// //    //case sf::Event::KeyPressed : PlayerInput(event.key.code, true);
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
		float playerSpeed = 700.f;
		float maxSpeed = 450.f;
		//accelerator havent used yet
		float accelerator = 10.f;
		//vector to get players position on the map
		sf::Vector2f spot = player.getPosition();
    speedometer.setString("Speed: " + int2Str(abs(speedX)) + "km/h");
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			//cant move out of the map
			if (player.getPosition().y <= 40)
			{
				player.setPosition(spot.x, 40);
			}
			else
			{
				//if moving left. meaning speed is < 0
				if (speedX < 0)
				{
					if (speedX < -maxSpeed)
					{
						speedX = -maxSpeed;
					}
					else
					{
						speedX -= 3.f;
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
			if (player.getPosition().y >= 450)
			{
				player.setPosition(spot.x, 450);
			}
			else
			{
				if (speedX < 0)
				{
					if (speedX < -maxSpeed)
					{
						speedX = -maxSpeed;
					}
					else
					{
						speedX -= 3.f;
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
						speedX += 3.f;
					}
					player.move(0, speedX * elapsed.asSeconds());
				}
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			/*direction_left = true;*/
			//change sprite to face left. player smooth move.
			player.setTexture(plane_left);
			//starting speed if i turn around
			player.move(playerSpeed*elapsed.asSeconds(), 0);
			//if player reaches 700,y stop
			if (spot.x >= 720)
			{
				player.setPosition(720, spot.y);
			}

			if (speedX > maxSpeed)
			{
				speedX = maxSpeed;
			}
			else
			{
				speedX += 5.f;
			}
		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			/*direction_left = false;*/
			player.setTexture(plane);
			player.move(-playerSpeed*elapsed.asSeconds(), 0);

			if (spot.x <= 20)
			{
				player.setPosition(20, spot.y);
			}

			if (speedX < -maxSpeed)
			{
				speedX = -maxSpeed;
			}
			else
			{
				speedX -= 5.f;
			}
		}
		//move the backgrounds left or right
		treeSpeed = elapsed.asSeconds() * speedX / 2;
		grassSpeed = elapsed.asSeconds() * speedX;
		mountainSpeed = elapsed.asSeconds() * speedX / 4;
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
		//int  X = speedX;
		//int  Y = tree.getPosition().y;
		//printf_s("%d ", X);
	}

	void enemyMove()
	{
		sf::Time enemyElapsed = enemySpeed.getElapsedTime();
		sf::FloatRect playerbox = player.getGlobalBounds();

		for (auto i = enemies.begin(); i != enemies.end();)
		{
			enemiesbox = i->getGlobalBounds();

			if (playerbox.intersects(enemiesbox))
			{
				player.setPosition(400, 460);
				speedX = 0;
				lives = lives - 1;
			}

			sf::Vector2f asd = i->getPosition();
			if (asd.y >= 480)//funktsioniert nicht :? DOCH
			{
				i->setPosition(asd.x, 480);
				hitTheGround++;
				enemiesLeft--;
				enemies.erase(i);
				break;
			}
			else if (i->getPosition().x > 1595)
			{
				i->setPosition(-795, asd.y);
			}
			else if (i->getPosition().x < -795)
			{
				i->setPosition(1595, asd.y);
			}
			else
			{
				//enemy move x at the same speed the player does for circlemap effect. enemy y can change freely
				i->move(speedX*enemyElapsed.asSeconds(), 20 * enemyElapsed.asSeconds());
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
		/*sf::Time bulletEraser = bulletTime.getElapsedTime();*/
		sf::Time reload = time_last_shot.getElapsedTime();
		sf::Time bulletSpeed = bulletSpeedClock.getElapsedTime();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			//create new sprite reference shot. set start postiion and add it to the bulelt list
			if (reload > sf::seconds(0.2))
			{
				sf::Sprite *nshot = new sf::Sprite;
				nshot->setPosition(player.getPosition().x + 20, player.getPosition().y + 20);
				nshot->setTexture(shoot);
				bullets.push_back(*nshot);
				time_last_shot.restart();
        if(player.getPosition().x > 350)
        {
          direction_left = true;
        }
        else
        {
          direction_left = false;
        }
			}
		}

		for (auto i = bullets.begin(); i != bullets.end();)
		{
			//according to direction fly left or right
			if (direction_left)
			{
				i->move(-1000 * bulletSpeed.asSeconds(), 0);
			}
			else
			{
				i->move(1000 * bulletSpeed.asSeconds(), 0);
			}
			//if bullet flies of the screen or is older then 0.3sec then erase from vector
			if (i->getPosition().x <= 0 || i->getPosition().x >= 800)
			{
				bullets.erase(i);
				break;
			}
			else
			{
				i++;
			}
		}
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
				sf::FloatRect bulletz = i->getGlobalBounds();

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
		showEnemiesLeft.setString("Enemies left:" + int2Str(enemiesLeft));
	}

	void placeEnemiesAtStart()
	{	
			float posx = rand() % 1550 + (-750);
			float posy = rand() % 50 + (-50);
			oneEnemy.setTexture(enemy);
			oneEnemy.setPosition(posx, posy);
	}

	void backgroundLoop()
	{
		if (grass.getPosition().x >= 795)
		{
			grassLeft.setPosition(-800, 475);
			grass.setPosition(0, 475);
			grassRight.setPosition(800, 475);
		}
		else if (tree.getPosition().x >= 795)
		{
			treeLeft.setPosition(-800, 285);
			tree.setPosition(0, 285);
			treeRight.setPosition(800, 285);
		}

		else if (mountain.getPosition().x >= 795)
		{
			mountainLeft.setPosition(-795, 0);
			mountain.setPosition(0, 0);
			mountainRight.setPosition(795, 0);
		}
		else if (grass.getPosition().x <= -795)
		{
			grassLeft.setPosition(-800, 475);
			grass.setPosition(0, 475);
			grassRight.setPosition(800, 475);
		}
		else if (tree.getPosition().x <= -795)
		{
			treeLeft.setPosition(-800, 285);
			tree.setPosition(0, 285);
			treeRight.setPosition(800, 285);
		}
		else if (mountain.getPosition().x <= -795)
		{
			mountainLeft.setPosition(-795, 0);
			mountain.setPosition(0, 0);
			mountainRight.setPosition(795, 0);
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
			speedX = 0;
			return true;
		}
		else
		{
			return false;
		}
	}
	bool checkWinner()
	{
		sf::Time timer = timeLeft.getElapsedTime();
		float countDown = timer.asSeconds();
		if (countDown >= 60.f)
		{
			timeRemaining.setString("You saved the planet!");
			speedX = 0;
			return true;
		}
		else
		{ 
      timeRemaining.setString("Time Survived: " + int2Str(countDown));
			return false;
		}

	}
  void createEnemies()
	{
		sf::Time spawnSome = enemySpawnTimer.getElapsedTime();
		sf::Time timerDifficulty = sf::seconds(4);
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
			playerMove();
			enemyMove();
			createBullet();
			collision();
			backgroundLoop();
			createEnemies();
			render();
		}
	}

	FlyingGame() : window(sf::VideoMode(800, 600, 32), "FlyingGame")
	{
		//game window with player and background etc automaticly created when i create instance of flyinggame class
		level = 1;
		speedX = 0;
		lives = 3;
		hitTheGround = 0;
		enemiesLeft = 0;
		enemiesKilled = 0;
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
		explosion.loadFromFile("pic/explosion.png");
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
		player.setPosition(400, 460);
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
		showEnemiesLeft.setPosition(10,540);
		showEnemiesLeft.setFont(font);
		showEnemiesLeft.setCharacterSize(24);
		showEnemiesLeft.setColor(sf::Color::Red);
		livesLeft.setPosition(620, 10);
		livesLeft.setFont(font);
		livesLeft.setCharacterSize(24);
		livesLeft.setColor(sf::Color::Red);
		bombsExploded.setPosition(280, 540);
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
		timeRemaining.setPosition(280, 10);
		showLevel.setFont(font);
		showLevel.setCharacterSize(24);
		showLevel.setColor(sf::Color::Red);
		showLevel.setPosition(620, 540);
		showLevel.setString("Level: " + int2Str(level));
    speedometer.setFont(font);
    speedometer.setPosition(10,10);
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
