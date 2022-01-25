#include "Game.h"

#include "Common.h"

Game::Game(const std::string& configFile)
{
	init(configFile);
}

void Game::init(const std::string& config)
{
	//Read in config file here
	//	use the premade PlayerConfig,Enemy Config, BulletConfig variables 

	//Makes sure to change the rand seed so it doesnt spawn the same as the last games
	srand(time(nullptr));

	//To be implemented if I need
	//m_backgroundTexture.loadFromFile("art/ezgif-2-76dec4f055.jpg");
	//m_backgroundSprite.setTexture(m_backgroundTexture);
	//m_backgroundSprite.setPosition(sf::Vector2f(0, 0));

	//Read in the config file 
	std::fstream input{config};

	if (!input.is_open())
	{
		std::cout << "Failed to open: " << config << '\n';
		exit(-1);
	}

	std::string identifier{};
	while (input >> identifier)
	{
		if (identifier == "Window")
		{
			unsigned int width{};
			input >> width;

			unsigned int height{};
			input >> height;

			int frameLimit{};
			input >> frameLimit;

			int fullScreen{};
			input >> fullScreen;
			if (fullScreen == 0)
			{
				m_window.create(sf::VideoMode(width, height), "Geometry Wars", sf::Style::Close);
				m_window.setFramerateLimit(frameLimit);
			}
			else if (fullScreen == 1)
			{
				auto fullscreenMode{ sf::VideoMode::getFullscreenModes() };
				//fullscreenMode[0] is the most compatible mode for fullscreen on this device
				m_window.create(fullscreenMode[0], "Geometry Wars", sf::Style::Fullscreen);
				m_window.setFramerateLimit(frameLimit);
			}
		}
		else if (identifier == "Font")
		{
			std::string filepath{};
			input >> filepath;
			if (!m_font.loadFromFile(filepath))
			{
				std::cerr << "Failed to load font. Filepath: " << filepath;
			}

			m_scoreText.setFont(m_font);
			
			int fontSize{};
			input >> fontSize;
			m_scoreText.setCharacterSize(fontSize);

			sf::Vector3i RGB{};
			input >> RGB.x;
			input >> RGB.y;
			input >> RGB.z;
			m_scoreText.setFillColor(sf::Color(RGB.x, RGB.y, RGB.z));
		}
		else if (identifier == "Player")
		{
			int shapeRadius{};
			input >> shapeRadius;
			m_playerConfig.SR = shapeRadius;

			int collisionRadius{};
			input >> collisionRadius;
			m_playerConfig.CR = collisionRadius;

			float speed{};
			input >> speed;
			m_playerConfig.S = speed;

			sf::Vector3i fillColor{};
			input >> fillColor.x;
			input >> fillColor.y;
			input >> fillColor.z;
			m_playerConfig.FR = fillColor.x;
			m_playerConfig.FG = fillColor.y;
			m_playerConfig.FB = fillColor.z;

			sf::Vector3i outlineColor{};
			input >> outlineColor.x;
			input >> outlineColor.y;
			input >> outlineColor.z;
			m_playerConfig.OR = outlineColor.x;
			m_playerConfig.OG = outlineColor.y;
			m_playerConfig.OB = outlineColor.z;

			int outlineThickness{};
			input >> outlineThickness;
			m_playerConfig.OT = outlineThickness;

			int vertices{};
			input >> vertices;
			m_playerConfig.V = vertices;
		}
		else if (identifier == "Enemy")
		{
			int shapeRadius{};
			input >> shapeRadius;
			m_enemyConfig.SR = shapeRadius;

			int collisionRadius{};
			input >> collisionRadius;
			m_enemyConfig.CR = collisionRadius;

			float minSpeed{};
			input >> minSpeed;
			m_enemyConfig.SMIN = minSpeed;

			float maxSpeed{};
			input >> maxSpeed;
			m_enemyConfig.SMAX = maxSpeed;

			sf::Vector3i outlineColor{};
			input >> outlineColor.x;
			input >> outlineColor.y;
			input >> outlineColor.z;
			m_enemyConfig.OR = outlineColor.x;
			m_enemyConfig.OG = outlineColor.y;
			m_enemyConfig.OB = outlineColor.z;

			int outlineThickness{};
			input >> outlineThickness;
			m_enemyConfig.OT = outlineThickness;

			int minVertices{};
			input >> minVertices;
			m_enemyConfig.VMIN = minVertices;

			int maxVertices{};
			input >> maxVertices;
			m_enemyConfig.VMAX = maxVertices;

			int smallLifespan{};
			input >> smallLifespan;
			m_enemyConfig.L = smallLifespan;

			int spawnInterval{};
			input >> spawnInterval;
			m_enemyConfig.SI = spawnInterval;
		}
		else if (identifier == "Bullet")
		{
			int shapeRadius{};
			input >> shapeRadius;
			m_bulletConfig.SR = shapeRadius;

			int collisionRadius{};
			input >> collisionRadius;
			m_bulletConfig.CR = collisionRadius;

			float speed{};
			input >> speed;
			m_bulletConfig.S = speed;

			sf::Vector3i fillColor{};
			input >> fillColor.x;
			input >> fillColor.y;
			input >> fillColor.z;
			m_bulletConfig.FR = fillColor.x;
			m_bulletConfig.FG = fillColor.y;
			m_bulletConfig.FB = fillColor.z;

			sf::Vector3i outlineColor{};
			input >> outlineColor.x;
			input >> outlineColor.y;
			input >> outlineColor.z;
			m_bulletConfig.OR = outlineColor.x;
			m_bulletConfig.OG = outlineColor.y;
			m_bulletConfig.OB = outlineColor.z;

			int outlineThickness{};
			input >> outlineThickness;
			m_bulletConfig.OT = outlineThickness;

			int shapeVertices{};
			input >> shapeVertices;
			m_bulletConfig.V = shapeVertices;

			int lifespan{};
			input >> lifespan;
			m_bulletConfig.L = lifespan;

			int specialBulletAmount{};
			input >> specialBulletAmount;
			m_bulletConfig.SB = specialBulletAmount;
		}
	}

	m_scoreText.setPosition(0, 0);
	m_scoreText.setString(std::to_string(m_score));

	spawnPlayer();
}

void Game::setPaused(bool paused)
{
	m_paused = paused;
}

void Game::sMovement()
{
	Vec2 playerVelocity;
	if (m_player->cInput->left)
	{
		playerVelocity.x -= m_playerConfig.S;
	}
	if (m_player->cInput->right)
	{
		playerVelocity.x += m_playerConfig.S;
	}
	if (m_player->cInput->up)
	{
		playerVelocity.y -= m_playerConfig.S;
	}
	if (m_player->cInput->down)
	{
		playerVelocity.y += m_playerConfig.S;
	}

	for (auto e : m_entities.getEntities())
	{
		if (e->tag() == entityTags::player)
		{
			//Updates the position of the player
			m_player->cTransform->pos += playerVelocity;
			//Rotates the player
			e->cTransform->angle += 1.0f;
			e->cShape->circle.setRotation(e->cTransform->angle);
		}
		if (e->cTransform)
		{
			//Updates the position of the entities
			e->cTransform->pos += e->cTransform->velocity;
			//Rotates the player
			e->cTransform->angle += 1.0f;
			e->cShape->circle.setRotation(e->cTransform->angle);
		}
	}
}

void Game::sUserInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
			{
				m_running = false;
				break;
			}
			case sf::Event::KeyPressed:
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					m_running = false;
				}
				else if (event.key.code == sf::Keyboard::P)
				{
					m_paused ? setPaused(false) : setPaused(true);
				}
				//Player input
				else if (event.key.code == sf::Keyboard::A) { m_player->cInput->left = true; }
				else if (event.key.code == sf::Keyboard::D) { m_player->cInput->right = true; }
				else if (event.key.code == sf::Keyboard::W) { m_player->cInput->up = true; }
				else if (event.key.code == sf::Keyboard::S) { m_player->cInput->down = true; }

				break;
			}
			case sf::Event::KeyReleased:
			{
				if (event.key.code == sf::Keyboard::A) { m_player->cInput->left = false; }
				else if (event.key.code == sf::Keyboard::D) { m_player->cInput->right = false; }
				else if (event.key.code == sf::Keyboard::W) { m_player->cInput->up = false; }
				else if (event.key.code == sf::Keyboard::S) { m_player->cInput->down = false; }
				break;
			}
			case sf::Event::MouseButtonPressed:
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if (m_player->cInput->leftMouse == false)
					{
						m_player->cInput->leftMouse = true;
					}
				}
				if(event.mouseButton.button == sf::Mouse::Right)
				{
					if (m_player->cInput->rightMouse == false)
					{
						m_player->cInput->rightMouse = true;
					}
				}
				break;
			}
			case sf::Event::MouseButtonReleased:
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					m_player->cInput->leftMouse = false;
				}
				break;
			}

			default:
				break;
		}
	}
}

void Game::sLifeSpan()
{
	for (auto e : m_entities.getEntities())
	{
		if (!e->cLifespan) { continue; }

		if (e->cLifespan->remaining > 0)
		{
			--e->cLifespan->remaining;
		}
		if (e->isActive() && e->cLifespan->remaining > 0)
		{
			float alphaMultiplier{ static_cast<float>(e->cLifespan->remaining) / static_cast<float>(e->cLifespan->total) };

			auto fillColor{ e->cShape->circle.getFillColor() };
			sf::Color newFillColor{ fillColor.r,fillColor.g,fillColor.b, static_cast<sf::Uint8>( 255 * alphaMultiplier) };
			e->cShape->circle.setFillColor(newFillColor);

			auto outlineColor{ e->cShape->circle.getOutlineColor() };
			sf::Color newOutlineColor{ outlineColor.r,outlineColor.g,outlineColor.b, static_cast<sf::Uint8>(255 * alphaMultiplier) };
			e->cShape->circle.setOutlineColor(newOutlineColor);
		}
		else if (e->cLifespan->remaining <= 0)
		{
			e->destroy();
		}
	}
}

void Game::sRender()
{
	m_window.clear();

	//m_window.draw(m_backgroundSprite);

	for (auto e : m_entities.getEntities())
	{
		e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
		m_window.draw(e->cShape->circle);
	}
	m_window.draw(m_scoreText);
	
	m_window.display();
}

void Game::sSpawner()
{
	if ((m_currentFrame - m_lastEnemySpawnTime) >= m_enemyConfig.SI)
	{
		spawnEnemy();
	}
	//Players bullet
	if (m_player->cInput->leftMouse == true)
	{
		Vec2 mousePos{ static_cast<double>(sf::Mouse::getPosition(m_window).x), static_cast<double>(sf::Mouse::getPosition(m_window).y) };
		spawnBullet(m_player, mousePos);
		m_player->cInput->leftMouse = false;
	}
	if (m_player->cInput->rightMouse == true)
	{
		spawnSpecialWeapon(m_player);
		m_player->cInput->rightMouse = false;
	}

}

void Game::sCollision()
{
	for (auto player : m_entities.getEntities(entityTags::player))
	{
		for (auto enemy : m_entities.getEntities(entityTags::enemy))
		{
			//if there's a collision between player and enemy
			//Destroy the player,//Destroy the enemy,//Respawn the player

			Vec2 diff{ enemy->cTransform->pos.x - player->cTransform->pos.x , enemy->cTransform->pos.y - player->cTransform->pos.y };

			double collisionRadiusSQ{ (player->cCollision->radius + enemy->cCollision->radius) * (player->cCollision->radius + enemy->cCollision->radius) };
			double distSQ{ (diff.x * diff.x) + (diff.y * diff.y) };

			if (distSQ < collisionRadiusSQ)
			{
				//makes sure the player is alive and doesnt spawn 2 players
				if (player->isActive())
				{
					m_score = 0;
					m_scoreText.setString(std::to_string(m_score));

					enemy->destroy();
					player->destroy();
					spawnPlayer();
				}
			}
		}

		for (auto enemy : m_entities.getEntities(entityTags::smallEnemy))
		{
			//if there's a collision between player and enemy
			//Destroy the player,//Destroy the enemy,//Respawn the player
		
			Vec2 diff{ enemy->cTransform->pos.x - player->cTransform->pos.x , enemy->cTransform->pos.y - player->cTransform->pos.y };
		
			double collisionRadiusSQ{ (player->cCollision->radius + enemy->cCollision->radius) * (player->cCollision->radius + enemy->cCollision->radius) };
			double distSQ{ (diff.x * diff.x) + (diff.y * diff.y) };
		
			if (distSQ < collisionRadiusSQ)
			{
				//makes sure the player is alive so it doesnt spawn 2 players
				if (player->isActive())
				{
					m_score /= 2;
					m_scoreText.setString(std::to_string(m_score));

					player->destroy();
					enemy->destroy();
					spawnPlayer();
				}
			}
		}
	}

	for (auto bullet : m_entities.getEntities(entityTags::bullet))
	{
		for (auto enemy : m_entities.getEntities(entityTags::enemy))
		{
			//if there's a collision between player and enemy
			//Destroy the player,//Destroy the enemy,//Respawn the player

			Vec2 diff{ enemy->cTransform->pos.x - bullet->cTransform->pos.x , enemy->cTransform->pos.y - bullet->cTransform->pos.y };

			double collisionRadiusSQ{ (bullet->cCollision->radius + enemy->cCollision->radius) * (bullet->cCollision->radius + enemy->cCollision->radius) };
			double distSQ{ (diff.x * diff.x) + (diff.y * diff.y) };

			if (distSQ < collisionRadiusSQ)
			{
				//Updates the score
				m_score += enemy->cScore->score;
				m_scoreText.setString(std::to_string(m_score));

				spawnSmallEnemies(enemy);
				bullet->destroy();
				enemy->destroy();

				break;
			}
		}

		for (auto enemy : m_entities.getEntities(entityTags::smallEnemy))
		{
			//Updates the score

			Vec2 diff{ enemy->cTransform->pos.x - bullet->cTransform->pos.x , enemy->cTransform->pos.y - bullet->cTransform->pos.y };

			double collisionRadiusSQ{ (bullet->cCollision->radius + enemy->cCollision->radius) * (bullet->cCollision->radius + enemy->cCollision->radius) };
			double distSQ{ (diff.x * diff.x) + (diff.y * diff.y) };

			if (distSQ < collisionRadiusSQ)
			{
				m_score += enemy->cScore->score;
				m_scoreText.setString(std::to_string(m_score));

				bullet->destroy();
				enemy->destroy();

				break;
			}
		}
	}

	//General Collision ie walls && ground && ceiling for player

	for (auto e : m_entities.getEntities(entityTags::player))
	{
		//Checks to see if player collided with walls
		if (e->cTransform->pos.x + m_playerConfig.CR > m_window.getSize().x)
		{
			e->cTransform->pos.x -= m_playerConfig.S;
		}
		else if (e->cTransform->pos.x - m_playerConfig.CR < 0)
		{
			e->cTransform->pos.x += m_playerConfig.S;
		}

		if (e->cTransform->pos.y + m_playerConfig.CR > m_window.getSize().y)
		{
			e->cTransform->pos.y -= m_playerConfig.S;
		}
		else if (e->cTransform->pos.y - m_playerConfig.CR < 0)
		{
			e->cTransform->pos.y += m_playerConfig.S;
		}
	}

	//General Collision ie walls && ground && ceiling for entities
	for (auto e : m_entities.getEntities())
	{
		if (e->tag() == entityTags::bullet || e->tag() == entityTags::enemy || e->tag() == entityTags::smallEnemy)
		{
			if (e->cTransform->pos.x + e->cCollision->radius > m_window.getSize().x)
			{
				e->cTransform->velocity.x *= -1;
			}
			else if (e->cTransform->pos.x - e->cCollision->radius < 0)
			{
				e->cTransform->velocity.x *= -1;
			}
			if (e->cTransform->pos.y + e->cCollision->radius > m_window.getSize().y)
			{
				e->cTransform->velocity.y *= -1;
			}
			else if (e->cTransform->pos.y - e->cCollision->radius < 0)
			{
				e->cTransform->velocity.y *= -1;
			}
		}
	}
}

void Game::spawnPlayer()
{
	//Create every entity by calling EntityManager.addEntity(tag) because Entity constructor is private
	//This returns a std::shared_ptr<Entity>
	auto playerEntity = m_entities.addEntity(entityTags::player);

	Vec2 middleWindow{ m_window.getSize().x * 0.5, m_window.getSize().y * 0.5 };
	
	playerEntity->cTransform = std::make_shared<CTransform>(middleWindow, Vec2(0, 0), 0.0f);

	//Creates the entities shape
	playerEntity->cShape = std::make_shared<CShape>(m_playerConfig.SR, m_playerConfig.V,sf::Color(m_playerConfig.FR, m_playerConfig.FG,
			m_playerConfig.FB), sf::Color(m_playerConfig.OR,m_playerConfig.OG, m_playerConfig.OB),m_playerConfig.OT);

	//Give the entity an collision component
	playerEntity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);

	//Add an input component to take the player's input
	playerEntity->cInput = std::make_shared<CInput>();

	//This goes slightly against the EntityManagers paradigm, but we use the player so much, it's worth it
	m_player = playerEntity;
}

void Game::spawnEnemy()
{
	auto e = m_entities.addEntity(entityTags::enemy);
	
	int minPosX{ m_enemyConfig.CR };
	int maxPosX{ static_cast<int>(m_window.getSize().x - m_enemyConfig.CR) };
	
	int minPosY{ m_enemyConfig.CR };
	int maxPosY{ static_cast<int>(m_window.getSize().y - m_enemyConfig.CR) };
	
	Vec2 randPos
	{ 
		static_cast<double>(minPosX + (rand() % (maxPosX - minPosX + 1))),
		static_cast<double>(minPosY + (rand() % (maxPosY - minPosY + 1)))
	};
	
	//Randomize the speed of the enemy and checks if (0) true->redo
	int randSpeed
	{ 
		static_cast<int>(m_enemyConfig.SMIN + rand() % static_cast<int>((m_enemyConfig.SMAX - m_enemyConfig.SMIN + 1)))
	};
	
	//Makes sure the velocity is not 0
	while (randSpeed == 0)
	{
		randSpeed = m_enemyConfig.SMIN + rand() % static_cast<int>((m_enemyConfig.SMAX - m_enemyConfig.SMIN + 1));
	}
	
	//Gives the entity the random position on the screen 
	e->cTransform = std::make_shared<CTransform>(randPos, Vec2(randSpeed, randSpeed), 0.0f);
	
	//Give the enemy a score of 100
	e->cScore = std::make_shared<CScore>(100);
	
	int vertices{ m_enemyConfig.VMIN + (rand() % (m_enemyConfig.VMAX - m_enemyConfig.VMIN + 1)) };
	
	sf::Color fillColor
	{
		static_cast<sf::Uint8>(rand() % (1 + 255)),
		static_cast<sf::Uint8>(rand() % (1 + 255)),
		static_cast<sf::Uint8>(rand() % (1 + 255)) 
	};
	
	sf::Color outlineColor
	{ 
		static_cast<sf::Uint8>(m_enemyConfig.OR),
		static_cast<sf::Uint8>(m_enemyConfig.OG),
		static_cast<sf::Uint8>(m_enemyConfig.OB)
	};
	
	int outlineThickness{ m_enemyConfig.OT };
	
	e->cShape = std::make_shared<CShape>(m_enemyConfig.SR, vertices, fillColor,outlineColor, outlineThickness);
	
	e->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);
	
	//Record when the most recent enemy was spawned
	m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> parent)
{

	//Spawn a number of small enemies equal to the number of vertices of the original
	size_t vertices{ parent->cShape->circle.getPointCount() };

	Vec2 ParentPos{ parent->cTransform->pos.x ,parent->cTransform->pos.y };
	Vec2 normalizedParentPos{ Vec2::normalize(ParentPos) };

	//Set each enemy to the same color as the original, half the size
	sf::Color parentFill{ parent->cShape->circle.getFillColor() };
	sf::Color parentOutline{ parent->cShape->circle.getOutlineColor() };
	float parentTickness{ parent->cShape->circle.getOutlineThickness() };

	float smallEnemyRadius{ parent->cShape->circle.getRadius() * 0.5f };
	float smallEnemyCollisionRadius{ parent->cCollision->radius * 0.5f };

	float angle{ 0 };

	std::cout << ParentPos;
	std::cout << normalizedParentPos;

	for (size_t i{ 0 }; i < vertices; ++i)
	{
		auto e = m_entities.addEntity(entityTags::smallEnemy);

		//Small enemies are worth double points of the original enemy
		e->cScore = std::make_shared<CScore>(parent->cScore->score * 2);

		e->cShape = std::make_shared<CShape>(smallEnemyRadius, vertices, parentFill, parentOutline, parentTickness);

		e->cCollision = std::make_shared<CCollision>(smallEnemyCollisionRadius);

		e->cLifespan = std::make_shared<CLifeSpan>(m_enemyConfig.L);

		//Calculate the velocity

		double radians{ angle * Math::PI / 180.0 };

		Vec2 velocity
		{
			std::cos(radians) * normalizedParentPos.x + std::sin(radians) * normalizedParentPos.y,
			std::sin(radians) * normalizedParentPos.x - std::cos(radians) * normalizedParentPos.y,
		};

		//Get the lenght of the vector 
		float L{ sqrtf(velocity.x * velocity.x + velocity.y * velocity.y) };

		//Normalize the vector to get an unit vector
		Vec2 normalizedVelocity{ velocity.x / L,velocity.y / L };
		
		//Scales the normalized vertor by the parents velocity
		Vec2 newVelocity{ normalizedVelocity.x * parent->cTransform->velocity.x, normalizedVelocity.y * parent->cTransform->velocity.y };
		
		e->cTransform = std::make_shared<CTransform>(ParentPos, newVelocity, 0);

		angle += 360 / vertices;
	}

}

//Spawns a bullet from a given entity to a target location
void Game::spawnBullet(std::shared_ptr<Entity> shooter, const Vec2& mousePos)
{
	auto e = m_entities.addEntity(entityTags::bullet);

	e->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);

	e->cLifespan = std::make_shared<CLifeSpan>(m_bulletConfig.L);

	e->cShape = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.V,
		sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB), 
		sf::Color(m_bulletConfig.OR, m_bulletConfig.OG,
		m_bulletConfig.OB), m_bulletConfig.OT);


	Vec2 difference{ mousePos.x - shooter->cTransform->pos.x, mousePos.y - shooter->cTransform->pos.y };

	difference.normalize();

	Vec2 velocity{m_bulletConfig.S * difference.x, m_bulletConfig.S * difference.y};

	e->cTransform = std::make_shared<CTransform>(shooter->cTransform->pos, velocity, 0);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	
	float angle{ 0 };

	for (int i{ 0 }; i < m_bulletConfig.SB; ++i)
	{
		auto e = m_entities.addEntity(entityTags::bullet);

		e->cShape = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.V,
			sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
			sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);

		e->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);

		e->cLifespan = std::make_shared<CLifeSpan>(m_bulletConfig.L);


		Vec2 normalizedPos{ Vec2::normalize(m_player->cTransform->pos) };

		//Calculate the velocity

		double radians{ angle * Math::PI / 180.0 };

		Vec2 velocity
		{
			std::cos(radians) * normalizedPos.x + std::sin(radians) * normalizedPos.y,
			std::sin(radians) * normalizedPos.x - std::cos(radians) * normalizedPos.y,
		};

		//Get the lenght of the vector 
		double L{ velocity.lenght() };

		//Normalize the vector to get an unit vector
		Vec2 normalizedVelocity{ velocity.x / L,velocity.y / L };

		//Scales the normalized vertor by the parents velocity
		Vec2 newVelocity{ normalizedVelocity.x * m_bulletConfig.S, normalizedVelocity.y * m_bulletConfig.S };

		e->cTransform = std::make_shared<CTransform>(m_player->cTransform->pos, newVelocity, 0);

		angle += 360 / m_bulletConfig.SB;
	}
}

void Game::run()
{
	while (m_running)
	{
		m_entities.update();

		if (!m_paused)
		{
			sLifeSpan();
			sSpawner();
			sMovement();
			sCollision();
		}
		sUserInput();
		sRender();

		//Increment the current frame
		++m_currentFrame;
	}
}