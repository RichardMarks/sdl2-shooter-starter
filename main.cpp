#include "window.h"
#include "entity.h"

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

constexpr int PLAYER_WIDTH = 32;
constexpr int PLAYER_HEIGHT = 32;
constexpr double PLAYER_SPEED = 400;

constexpr int PLAYER_PROJECTILE_WIDTH = 8;
constexpr int PLAYER_PROJECTILE_HEIGHT = 8;
constexpr double PLAYER_PROJECTILE_SPEED = 500;

constexpr int NUM_ENEMIES = 25;

constexpr int ENEMY_WIDTH = 50;
constexpr int ENEMY_HEIGHT = 50;
constexpr double ENEMY_SPEED = 180;

int main()
{
  std::cout << "creating window" << std::endl;
  gamelib::Window window("Shooter", WIDTH, HEIGHT);

  std::cout << "setting up keymap" << std::endl;
  window.keymap.emplace("quit", SDL_SCANCODE_ESCAPE);
  window.keymap.emplace("up", SDL_SCANCODE_UP);
  window.keymap.emplace("down", SDL_SCANCODE_DOWN);
  window.keymap.emplace("left", SDL_SCANCODE_LEFT);
  window.keymap.emplace("right", SDL_SCANCODE_RIGHT);
  window.keymap.emplace("fire", SDL_SCANCODE_SPACE);

  auto setRandomPosition = [&](gamelib::Entity &entity)
  {
    entity.setWorldPositionX(window.getRandomInRangeInt(0, WIDTH));
    entity.setWorldPositionY(window.getRandomInRangeInt(0, HEIGHT));
  };

  auto setRandomVelocity = [&](gamelib::Entity &entity, double speed)
  {
    entity.setVelocityX(window.getRandomInRangeDouble(-1, 1) * speed);
    entity.setVelocityY(window.getRandomInRangeDouble(-1, 1) * speed);
  };

  std::cout << "creating entities.." << std::endl;
  std::vector<std::unique_ptr<gamelib::Entity>> enemies;
  std::vector<std::unique_ptr<gamelib::Entity>> projectiles;
  std::cout << "creating player entity" << std::endl;
  gamelib::Entity player(WIDTH * 0.5, HEIGHT * 0.5, (const char *[]){"Player", nullptr});

  std::cout << "creating enemy entities" << std::endl;

  for (int i = 0; i < NUM_ENEMIES; i++)
  {
    auto enemy = std::make_unique<gamelib::Entity>((const char *[]){"Enemy", nullptr});

    setRandomPosition(*(enemy.get()));
    setRandomVelocity(*(enemy.get()), ENEMY_SPEED);

    enemies.push_back(std::move(enemy));
  }

  float firingRate = 0.1f;
  float firingTime = 0.0f;

  auto fireWeaponAtTarget = [&](double weaponX, double weaponY, double targetX, double targetY)
  {
    double angleToTarget = atan2(targetY - weaponY, targetX - weaponX);
    double projectileVelocityX = cos(angleToTarget) * PLAYER_PROJECTILE_SPEED;
    double projectileVelocityY = sin(angleToTarget) * PLAYER_PROJECTILE_SPEED;

    auto projectile = std::make_unique<gamelib::Entity>(
        weaponX,
        weaponY,
        projectileVelocityX,
        projectileVelocityY,
        (const char *[]){"Projectile", "Player", nullptr});

    projectiles.push_back(std::move(projectile));
  };

  bool isMouseDown = false;

  auto handlePlayerWeaponFiring = [&](gamelib::Entity &entity, float deltaTime)
  {
    int mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    bool leftMouseButtonDown = mouseState & SDL_BUTTON(SDL_BUTTON_LEFT);

    bool canFire = false;

    if (leftMouseButtonDown && !isMouseDown)
    {
      isMouseDown = true;
      canFire = true;
    }
    else if (!leftMouseButtonDown && isMouseDown)
    {
      isMouseDown = false;
    }

    if (window.isKeyPressed("fire") || leftMouseButtonDown)
    {
      firingTime += deltaTime;
      if (firingTime >= firingRate)
      {
        firingTime -= firingRate;
        canFire = true;
      }
    }

    if (canFire)
    {
      fireWeaponAtTarget(entity.getWorldPositionX(), entity.getWorldPositionY(), mouseX, mouseY);
    }
  };

  auto handlePlayerMovement = [&](gamelib::Entity &entity, float deltaTime)
  {
    double moveX = 0;
    double moveY = 0;
    if (window.isKeyPressed("up"))
    {
      moveY = -1;
    }
    else if (window.isKeyPressed("down"))
    {
      moveY = 1;
    }
    if (window.isKeyPressed("left"))
    {
      moveX = -1;
    }
    else if (window.isKeyPressed("right"))
    {
      moveX = 1;
    }

    if (moveX != 0.0 || moveY != 0.0)
    {
      double magnitude = sqrt(moveX * moveX + moveY * moveY);
      if (magnitude != 0.0)
      {
        moveX /= magnitude;
        moveY /= magnitude;
      }
    }
    entity.setVelocityX(PLAYER_SPEED * moveX);
    entity.setVelocityY(PLAYER_SPEED * moveY);

    entity.applyVelocity(deltaTime);
  };

  auto updatePlayer = [&](gamelib::Entity &entity, float deltaTime)
  {
    handlePlayerMovement(entity, deltaTime);
    handlePlayerWeaponFiring(entity, deltaTime);
  };

  auto renderPlayer = [&](gamelib::Entity &entity)
  {
    SDL_Rect rect = {
        static_cast<int>(entity.getWorldPositionX() - (PLAYER_WIDTH * 0.5)),
        static_cast<int>(entity.getWorldPositionY() - (PLAYER_HEIGHT * 0.5)),
        PLAYER_WIDTH,
        PLAYER_HEIGHT,
    };
    SDL_SetRenderDrawColor(window.getRenderer().get(), 0, 255, 0, 255);
    SDL_RenderFillRect(window.getRenderer().get(), &rect);

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    SDL_SetRenderDrawColor(window.getRenderer().get(), 255, 255, 255, 255);
    rect.x = mouseX;
    rect.y = 0;
    rect.w = 1;
    rect.h = HEIGHT;
    SDL_RenderFillRect(window.getRenderer().get(), &rect);
    rect.x = 0;
    rect.y = mouseY;
    rect.w = WIDTH;
    rect.h = 1;
    SDL_RenderFillRect(window.getRenderer().get(), &rect);
  };

  auto updatePlayerProjectile = [&](gamelib::Entity &entity, float deltaTime)
  {
    entity.applyVelocity(deltaTime);
  };

  auto renderPlayerProjectile = [&](gamelib::Entity &entity)
  {
    SDL_Rect rect = {
        static_cast<int>(entity.getWorldPositionX() - (PLAYER_PROJECTILE_WIDTH * 0.5)),
        static_cast<int>(entity.getWorldPositionY() - (PLAYER_PROJECTILE_HEIGHT * 0.5)),
        PLAYER_PROJECTILE_WIDTH,
        PLAYER_PROJECTILE_HEIGHT};
    SDL_SetRenderDrawColor(window.getRenderer().get(), 0, 255, 255, 255);
    SDL_RenderFillRect(window.getRenderer().get(), &rect);
  };

  auto updateEnemy = [&](gamelib::Entity &entity, float deltaTime)
  {
    entity.applyVelocity(deltaTime);
    if (entity.getWorldPositionX() < 0 || entity.getWorldPositionX() > WIDTH)
    {
      entity.setVelocityX(entity.getVelocityX() * -1);
      entity.applyVelocity(deltaTime);
    }
    if (entity.getWorldPositionY() < 0 || entity.getWorldPositionY() > HEIGHT)
    {
      entity.setVelocityY(entity.getVelocityY() * -1);
      entity.applyVelocity(deltaTime);
    }
  };

  auto renderEnemy = [&](gamelib::Entity &entity)
  {
    SDL_Rect rect = {
        static_cast<int>(entity.getWorldPositionX() - (ENEMY_WIDTH * 0.5)),
        static_cast<int>(entity.getWorldPositionY() - (ENEMY_HEIGHT * 0.5)),
        ENEMY_WIDTH,
        ENEMY_HEIGHT,
    };
    SDL_SetRenderDrawColor(window.getRenderer().get(), 255, 0, 0, 255);
    SDL_RenderFillRect(window.getRenderer().get(), &rect);
  };

  auto isOffScreen = [&](std::unique_ptr<gamelib::Entity> &entity)
  {
    gamelib::Entity &entityRef = *(entity.get());
    auto x = entityRef.getWorldPositionX();
    auto y = entityRef.getWorldPositionY();
    return x < 0 || x > WIDTH || y < 0 || y > HEIGHT;
  };

  auto isDead = [&](std::unique_ptr<gamelib::Entity> &entity)
  {
    gamelib::Entity &entityRef = *(entity.get());
    return entityRef.hasTag("DEAD");
  };

  while (window.isOpen())
  {
    window.processEvents();

    float deltaTime = window.resetClock();

    // update here

    if (window.isKeyPressed("quit"))
    {
      window.close();
    }

    updatePlayer(player, deltaTime);

    for (auto &entity : projectiles)
    {
      gamelib::Entity &projectileEntity = *(entity.get());

      updatePlayerProjectile(projectileEntity, deltaTime);

      SDL_Rect projectileRect = {
          static_cast<int>(projectileEntity.getWorldPositionX() - (PLAYER_PROJECTILE_WIDTH * 0.5)),
          static_cast<int>(projectileEntity.getWorldPositionY() - (PLAYER_PROJECTILE_WIDTH * 0.5)),
          PLAYER_PROJECTILE_WIDTH,
          PLAYER_PROJECTILE_HEIGHT};

      SDL_Rect intersectionRect;

      for (auto &entityB : enemies)
      {
        gamelib::Entity &enemyEntity = *(entityB.get());
        SDL_Rect enemyRect = {
            static_cast<int>(enemyEntity.getWorldPositionX() - (ENEMY_WIDTH * 0.5)),
            static_cast<int>(enemyEntity.getWorldPositionY() - (ENEMY_WIDTH * 0.5)),
            ENEMY_WIDTH,
            ENEMY_HEIGHT};

        if (SDL_IntersectRect(&projectileRect, &enemyRect, &intersectionRect))
        {
          // mark the enemy to be erased (or maybe reduce its health/shield percentage..)
          enemyEntity.setTag("DEAD");

          // erase the projectile (move the projectile way off screen and it will be deleted)
          projectileEntity.setWorldPositionX(-9999);
        }
      }
    }

    // remove dead enemies
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), isDead), enemies.end());

    // remove projectiles that are off screen
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), isOffScreen), projectiles.end());

    for (auto &entity : enemies)
    {
      updateEnemy(*(entity.get()), deltaTime);
    }

    window.prepareRender();

    // draw here

    for (auto &entity : enemies)
    {
      renderEnemy(*(entity.get()));
    }

    for (auto &entity : projectiles)
    {
      renderPlayerProjectile(*(entity.get()));
    }

    renderPlayer(player);

    window.presentRender();
  }

  return 0;
}
