#include"olcSimpleEngine.h"

/*
	INSTRUCTIONS:

	Controls: W,A,S,D

	Shoot: SPACE

	Game ends if the player collides with an enemy or an asteroid

*/



class SpaceGame : public olc::PixelGameEngine
{
	olc::Sprite* shipSprite;
	olc::Sprite* boxSprite;
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

	// box
	double boxX, boxY;
	bool bBox = false;
	bool nuclear = false;
	int boxDimX = 30;
	int boxDimY = 20;
	int boxCount = 0;
	int nuclearCount = 0;


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


	// gameplay
	bool gameOver = false;
	double totalTime = 0;
	bool gameStarted = false;
	bool gamePaused = false;

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

		if (gameStarted)
		{
			if (gameOver) // game over screen
			{
				DrawString(50, 50, "GAME OVER");

				DrawString(50, 75, "ASTEROIDS HIT: " + std::to_string(asteroidCount));

				DrawString(50, 100, "ENEMIES HIT: " + std::to_string(enemyCount));

				DrawString(50, 125, "BOXES COLLECTED: " + std::to_string(boxCount));

				DrawString(50, 150, "NUCLEAR BLASTS PERFORMED: " + std::to_string(nuclearCount));

				DrawString(50, 175, "YOU HAVE SURVIVED " + std::to_string(totalTime) + " SECONDS");

				DrawString(50, 200, "TOTAL SCORE: " + std::to_string(asteroidCount * 3 + enemyCount * 5
					+ boxCount * 3 + nuclearCount * 4 + int(totalTime)));

				DrawString(50, 250, "PRESS E TO EXIT");

				// Throws exception when ship crashes 
				// delete shipSprite;
				// Throws exception when ship crashes 


				if (GetKey(olc::E).bPressed)
					return false;
				else
					return true;
			}
			else // gameplay
			{
				if (gamePaused)
				{
					DrawString(50, 50, "GAME PAUSED");

					DrawString(50, 75, "PRESS ENTER TO CONTINUE");

					DrawString(50, 100, "ASTEROIDS HIT: " + std::to_string(asteroidCount));

					DrawString(50, 125, "ENEMIES HIT: " + std::to_string(enemyCount));

					DrawString(50, 150, "BOXES COLLECTED: " + std::to_string(boxCount));

					DrawString(50, 175, "NUCLEAR BLASTS PERFORMED: " + std::to_string(nuclearCount));

					DrawString(50, 200, "TOTAL SCORE: " + std::to_string(asteroidCount * 3 + enemyCount * 5
						+ boxCount * 3 + nuclearCount * 4 + int(totalTime)));

					if (GetKey(olc::ENTER).bPressed)
						gamePaused = false;

					return true;
				}
				else
				{
					totalTime += fElapsedTime;

					// if there exists a box
					if (bBox) {

						DrawSprite(boxX, boxY, boxSprite, 1);


						if (GetMouse(0).bPressed) {
							if (GetMouseX() >= boxX && GetMouseX() <= boxX + boxDimX &&
								GetMouseY() >= boxY && GetMouseY() <= boxY + boxDimY)
							{
								boxCount++;
								nuclear = true;
								bBox = false;
								delete boxSprite;
							}
						}

						// hit the box to collect
						if (//top right hit
							shipX + shipDimX > boxX &&
							shipX + shipDimX < boxX + boxDimX &&
							shipY > boxY &&
							shipY < boxY + boxDimY
							||
							// bottom right hit
							shipX + shipDimX > boxX &&
							shipX + shipDimX < boxX + boxDimX &&
							shipY + shipDimY > boxY &&
							shipY + shipDimY < boxY + boxDimY)
						{
							boxCount++;
							nuclear = true;
							bBox = false;
							delete boxSprite;
						}

					}

					if (nuclear) {

						DrawString(80, 10, "Nuclear weapon is activated");
						DrawString(75, 30, "Press 'shift' to kill'em all");

						if (GetKey(olc::SHIFT).bPressed) {
							nuclearCount++;
							if (bAsteroid) {
								bAsteroid = false;
								delete asteroidSprite;
								bAstExplosion = true;
								asteroidExplosionTime = totalTime;
								astExplosionX = asteroidX;
								astExplosionY = asteroidY;

								astExpSprite = new olc::Sprite("Sprites/exp2.png");
								DrawSprite(astExplosionX, astExplosionY, astExpSprite, 1);
							}

							if (bEnemy) {
								bEnemy = false;
								delete enemySprite;
								bExplosion = true;
								enemyExplosionTime = totalTime;
								explosionX = enemyX;
								explosionY = enemyY;

								explosionSprite = new olc::Sprite("Sprites/explosion.png");
								DrawSprite(explosionX, explosionY, explosionSprite, 1);
							}
							nuclear = false;
						}
					}

					// if there exists an asteroid
					if (!bAsteroid)
					{
						bAsteroid = true;
						asteroidY = rand() % (400 - asteroidDimY);
						asteroidX = 400;
						asteroidSprite = new olc::Sprite("Sprites/asteroid.png");
						DrawSprite(asteroidX, asteroidY, asteroidSprite, 1);
					}

					if (bAsteroid)
					{
						// collision 
						if (// top right hit
							shipX + shipDimX > asteroidX &&
							shipX + shipDimX < asteroidX + asteroidDimX &&
							shipY > asteroidY &&
							shipY < asteroidY + asteroidDimY
							||
							// bottom right hit
							shipX + shipDimX > asteroidX &&
							shipX + shipDimX < asteroidX + asteroidDimX &&
							shipY + shipDimY > asteroidY &&
							shipY + shipDimY < asteroidY + asteroidDimY)
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
							if (
								// top right hit
								rBulletX + bulletDimX > asteroidX &&
								rBulletX + bulletDimX < asteroidX + asteroidDimX &&
								rBulletY > asteroidY &&
								rBulletY < asteroidY + asteroidDimY
								||
								// bottom right hit
								rBulletX + bulletDimX > asteroidX &&
								rBulletX + bulletDimX < asteroidX + asteroidDimX &&
								rBulletY + bulletDimY> asteroidY &&
								rBulletY + bulletDimY < asteroidY + asteroidDimY
								)
							{
								bBullet = 0;
								bAsteroid = 0;

								asteroidExplosionTime = totalTime;

								asteroidCount++;

								delete asteroidSprite;
								delete rBulletSprite;


								// draw explosion
								bAstExplosion = true;

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
							bAstExplosion = false;
						}
						else
							DrawSprite(astExplosionX, astExplosionY, astExpSprite, 1);
					}


					if (!bEnemy)
					{
						bEnemy = true;
						enemyY = rand() % (400 - enemyDimY);
						enemyX = 400;
						enemySprite = new olc::Sprite("Sprites/enemy.png");
						DrawSprite(enemyX, enemyY, enemySprite, 1);
					}

					if (bEnemy)
					{
						enemyX -= fElapsedTime * enemySpeed;

						// crash
						if (// top right hit
							shipX + shipDimX > enemyX &&
							shipX + shipDimX < enemyX + enemyDimX &&
							shipY > enemyY &&
							shipY < enemyY + enemyDimY
							||
							// bottom right hit
							shipX + shipDimX > enemyX &&
							shipX + shipDimX < enemyX + enemyDimX &&
							shipY + shipDimY > enemyY &&
							shipY + shipDimY < enemyY + enemyDimY)
						{
							gameOver = true;
						}


						DrawSprite(enemyX, enemyY, enemySprite, 1);



						if (enemyX <= -2 * enemyDimX)
						{
							delete enemySprite;
							bEnemy = false;
						}

						if (bBullet)
							// bullet hits
							if (// top right hit
								rBulletX + bulletDimX > enemyX &&
								rBulletX + bulletDimX < enemyX + enemyDimX &&
								rBulletY > enemyY &&
								rBulletY < enemyY + enemyDimY
								||
								// bottom right hit
								rBulletX + bulletDimX > enemyX &&
								rBulletX + bulletDimX < enemyX + enemyDimX &&
								rBulletY + bulletDimY > enemyY &&
								rBulletY + bulletDimY < enemyY + enemyDimY)
							{
								bBullet = 0;
								bEnemy = 0;

								enemyCount++;
								enemyExplosionTime = totalTime;

								if (enemyCount % 5 == 0) {
									bBox = true;
									boxX = rand() % (400 - boxDimX);
									boxY = rand() % (400 - boxDimY);
									boxSprite = new olc::Sprite("Sprites/box.png");
								}

								delete enemySprite;
								delete rBulletSprite;

								bExplosion = 1;

								explosionX = enemyX;
								explosionY = enemyY;

								explosionSprite = new olc::Sprite("Sprites/explosion.png");
								DrawSprite(explosionX, explosionY, explosionSprite, 1);
							}
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


					// The following keeps the ship on the screen
					if (shipX <= 0)
						shipX = 0;


					if (shipX >= 400 - shipDimX)
						shipX = 400 - shipDimX;

					if (shipY <= 0)
						shipY = 0;

					if (shipY >= 400 - shipDimY)
						shipY = 400 - shipDimY;


					// draw ship
					DrawSprite(shipX, shipY, shipSprite, 1);

					if (GetKey(olc::P).bPressed)
					{
						gamePaused = true;
					}

					return true;
				}
			}
		}
		else
		{
			DrawString(150, 100, "SPACESHIP GAME");

			DrawString(125, 125, "PRESS ENTER TO START");

			DrawString(100, 225, "GAMEPLAY");
			DrawString(100, 250, "-USE W-A-S-D TO CONTROL THE SHIP");
			DrawString(100, 275, "-PRESS SPACE TO SHOOT");
			DrawString(100, 300, "-AVOID COLLISIONS");


			if (GetKey(olc::ENTER).bPressed)
				gameStarted = true;

			return true;
		}
	}
};


int main()
{
	SpaceGame game;
	if (game.Construct(400, 400, 1, 1))
		game.Start();
	return 0;
}
