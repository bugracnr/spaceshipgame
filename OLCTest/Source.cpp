#include"olcSimpleEngine.h"

/*
	INSTRUCTIONS: 

	Controls: W,A,S,D 

	Shoot: SPACE

	Game ends if the player collides with an enemy or an asteroid

*/



class Example : public olc::PixelGameEngine
{
	olc::Sprite* shipSprite;
	olc::Sprite* enemySprite;
	olc::Sprite* asteroidSprite;
	olc::Sprite* rBulletSprite;
	olc::Sprite* lBulletSprite;
	olc::Sprite* explosionSprite;
	olc::Sprite* astExpSprite; // asteroid explosion


	// ship
	double shipX = 50, shipY = 50;
	double shipSpeed = 125;

	int shipDimX = 23;
	int shipDimY = 25;



	// bullet
	double rBulletX, rBulletY;
	double bulletSpeed = 200;
	bool bBullet = 0;
	int bulletDimX = 5;
	int bulletDimY = 4;


	// enemy 
	double enemyX, enemyY;
	double enemySpeed = 100;

	int enemyDimX = 27;
	int enemyDimY = 25;

	bool bEnemy = 0;

	int enemyCount = 0;

	double enemyExplosionTime = 0;

	// asteroid 
	double asteroidX, asteroidY;
	double asteroidSpeed = 75;

	int asteroidDimX = 30;
	int asteroidDimY = 25;

	bool bAsteroid = 0;
	int asteroidCount = 0;
	double asteroidExplosionTime = 0;

	// explosion
	double explosionX, explosionY;
	bool bExplosion = 0;

	// explosion
	double astExplosionX, astExplosionY;
	bool bAstExplosion = 0;

	bool gameOver = 0;
	double totalTime = 0;

public:
	bool OnUserCreate() //override
	{
		shipSprite = new olc::Sprite("Sprites/spaceship.png");		

		return true;
	}
	bool OnUserUpdate(float fElapsedTime) //override
	{
		//Drawing the background
		Clear(olc::DARK_GREEN);
		SetPixelMode(olc::Pixel::ALPHA);
		//Drawing the background


		if (gameOver) // game over screen
		{
			DrawString(50, 50, "GAME OVER");

			DrawString(50, 100, "ASTEROIDS HIT: " + std::to_string(asteroidCount));

			DrawString(50, 150, "ENEMIES HIT: " + std::to_string(enemyCount));

			DrawString(50, 200, "YOU HAVE SURVIVED " + std::to_string(totalTime) + " SECONDS");

			DrawString(50, 250, "PRESS E TO EXIT");

			if (GetKey(olc::E).bPressed)
				return false;
			else
				return true;
		}
		else // gameplay
		{
			totalTime += fElapsedTime;


			// The following keeps the ship on the screen
			if (shipX <= 0)
				shipX = 0;


			if (shipX >= 400 - shipDimX)
				shipX = 400 - shipDimX;

			if (shipY <= 0)
				shipY = 0;

			if (shipY >= 400 - shipDimY)
				shipY = 400 - shipDimY;


			// if there exists an asteroid
			if (bAsteroid)
			{
				// collision 
				if (shipY > asteroidY - shipDimY + 1 &&
					shipY < asteroidY + asteroidDimY + shipDimY - 1 &&
					shipX > asteroidX - shipDimX + 1 &&
					shipX < asteroidX + asteroidDimX - shipDimX)
				{
					gameOver = 1;
				}

				// position update
				asteroidX -= fElapsedTime * asteroidSpeed;
				
				// Draw asteroid
				DrawSprite(asteroidX, asteroidY, asteroidSprite, 1);


				// If the asteroid is out of screen
				if (asteroidX <= -2 * asteroidDimX)
				{
					delete asteroidSprite;
					bAsteroid = 0;
				}



				if (bBullet)
					// bullet hits the asteroid
					if (rBulletY > asteroidY - bulletDimY + 1 &&
						rBulletY < asteroidY + asteroidDimX + bulletDimY - 1 &&
						rBulletX > asteroidX - bulletDimX + 1 &&
						rBulletX < asteroidX + asteroidDimX - bulletDimX)
					{
						bBullet = 0;
						bAsteroid = 0;

						asteroidExplosionTime = totalTime;

						asteroidCount++;

						delete asteroidSprite;
						delete rBulletSprite;


						// draw explosion
						bAstExplosion = 1;

						astExplosionX = asteroidX;
						astExplosionY = asteroidY;

						astExpSprite = new olc::Sprite("Sprites/exp2.png");
						DrawSprite(astExplosionX, astExplosionY, astExpSprite, 1);
					}

			}

			if (bAstExplosion)
			{
				if (totalTime - asteroidExplosionTime > 0.5)
				{
					delete astExpSprite;
					bAstExplosion = 0;
				}
				else
					DrawSprite(astExplosionX, astExplosionY, astExpSprite, 1);
			}

			if (bEnemy)
			{
				enemyX -= fElapsedTime * enemySpeed;

				// crash
				if (shipY > enemyY - shipDimY + 1 &&
					shipY < enemyY + enemyDimY + shipDimY - 1 &&
					shipX > enemyX - shipDimX + 1 &&
					shipX < enemyX + enemyDimX - shipDimX)
				{
					gameOver = 1;
				}

				DrawSprite(enemyX, enemyY, enemySprite, 1);


				if (enemyX < double(rand() % 400) && bAsteroid == 0)
				{
					bAsteroid = 1;					
					asteroidY = rand() % (400 - asteroidDimY);
					asteroidX = 400;
					asteroidSprite = new olc::Sprite("Sprites/asteroid.png");
					DrawSprite(asteroidX, asteroidY, asteroidSprite, 1);
				}


				if (enemyX <= -2 * enemyDimX)
				{
					delete enemySprite;
					bEnemy = 0;
				}

				if (bBullet)
					// bullet hits
					if (rBulletY > enemyY - bulletDimY + 1 &&
						rBulletY < enemyY + enemyDimY + bulletDimY - 1 &&
						rBulletX > enemyX - bulletDimX + 1 &&
						rBulletX < enemyX + enemyDimX - bulletDimX)
					{
						bBullet = 0;
						bEnemy = 0;

						enemyCount++;
						enemyExplosionTime = totalTime;


						delete enemySprite;
						delete rBulletSprite;

						bExplosion = 1;

						explosionX = enemyX;
						explosionY = enemyY;

						explosionSprite = new olc::Sprite("Sprites/explosion.png");
						DrawSprite(explosionX, explosionY, explosionSprite, 1);
					}
			}


			if (!bEnemy)
			{
				bEnemy = 1;
				enemyY = rand() % (400 - enemyDimY);
				enemyX = 400;
				enemySprite = new olc::Sprite("Sprites/enemy.png");
				DrawSprite(enemyX, enemyY, enemySprite, 1);
			}



			if (bExplosion)
			{
				if (totalTime - enemyExplosionTime > 0.5)
				{
					delete explosionSprite;
					bExplosion = 0;
				}
				else
					DrawSprite(explosionX, explosionY, explosionSprite, 2);
			}

			DrawSprite(shipX, shipY, shipSprite, 1);


			if (bBullet)
			{
				rBulletX += fElapsedTime * bulletSpeed;
				DrawSprite(rBulletX, rBulletY, rBulletSprite, 2);

				if (rBulletX > 400)
				{
					bBullet = 0;
					delete rBulletSprite;
				}
			}


			// Ship movement
			if (GetKey(olc::W).bHeld) // up
			{
				shipY -= shipSpeed * fElapsedTime;
			}
			if (GetKey(olc::A).bHeld) // right
			{
				shipX -= shipSpeed * fElapsedTime;
			}
			if (GetKey(olc::S).bHeld) // down
			{
				shipY += shipSpeed * fElapsedTime;
			}
			if (GetKey(olc::D).bHeld) // left
			{
				shipX += shipSpeed * fElapsedTime;
			}


			if (!bBullet)
			{
				// ship can fire
				if (GetKey(olc::SPACE).bPressed)
				{
					bBullet = 1;
					rBulletX = shipX + 10;
					rBulletY = shipY + 10;
					rBulletSprite = new olc::Sprite("Sprites/rbullet.png");
					DrawSprite(rBulletX, rBulletY, rBulletSprite, 2);
				}
			}

			return true;
		}
	}
};

int main()
{
	Example demo;
	if (demo.Construct(400, 400, 2, 2))
		demo.Start();
	return 0;
}
