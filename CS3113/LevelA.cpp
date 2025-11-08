#include "LevelA.h"

LevelA::LevelA()                                      : Scene { {0.0f}, nullptr   } {}
LevelA::LevelA(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

LevelA::~LevelA() {
   for (auto& enemy : mGameState.enemies) delete enemy;
   mGameState.enemies.clear();
   delete mGameState.xochitl;
   delete mGameState.goal;
   delete mGameState.map;
   shutdown(); 
}

void LevelA::initialise()
{
   for (auto& enemy : mGameState.enemies) delete enemy;
   mGameState.enemies.clear();

   mGameState.nextSceneID = -1; 
   
   mGameState.bgm = LoadMusicStream("assets/game/music_level1.wav");
   SetMusicVolume(mGameState.bgm, 0.33f);
   PlayMusicStream(mGameState.bgm);

   mGameState.jumpSound = LoadSound("assets/game/step.ogg");
   mGameState.hurtSound = LoadSound("assets/game/hurt.ogg");
   mGameState.goalSound = LoadSound("assets/game/goal.ogg");

   /*
      ----------- MAP -----------
   */
   mGameState.map = new Map(
      LEVEL_WIDTH, LEVEL_HEIGHT,   // map grid cols & rows
      (unsigned int *) mLevelData, // grid data
      "assets/game/grass.png",     // texture filepath
      TILE_DIMENSION,              // tile size
      9, 18,                       // texture cols & rows
      mOrigin                      // in-game origin
   );

   /*
      ----------- PROTAGONIST -----------
   */
   std::map<Direction, std::vector<int>> xochitlAnimationAtlas = {
      {IDLE_RIGHT,  { 0,  1, 2, 3 }},
      {IDLE_LEFT, { 8, 9, 10, 11 }},
      {RIGHT,  {  16, 17, 18, 19, 20, 21, 22, 23 }},
      {LEFT, { 24, 25, 26, 27, 28, 29, 30, 31 }},
      {JUMP_RIGHT, { 32, 33, 34, 35, 36 }},
      {JUMP_LEFT, { 40, 41, 42, 43, 44 }},
   };

   float sizeRatio  = 48.0f / 64.0f;
   
   std::map<Direction, std::vector<int>> boarAnimationAtlas = {
      {LEFT,  { 0, 1, 2, 3, 4, 5 }},
      {RIGHT, { 6, 7, 8, 9, 10, 11 }},
   };
   
   std::map<Direction, std::vector<int>> goalAnimationAtlas = {
      {RIGHT,  { 0, 1, 2, 3, 4,
                5, 6, 7, 8, 9,
                10, 11, 12, 13, 14,
                15, 16, 17, 18, 19,
                20, 21, 22, 23, 24,
                25, 26, 27, 28, 29,
                30, 31, 32, 33, 34,
                35, 36, 37, 38, 39 }},
   };

   // Assets from @see https://sscary.itch.io/the-adventurer-female
   mGameState.xochitl = new Entity(
      {mOrigin.x - 1050.0f, mOrigin.y - 10.0f}, // position
      {250.0f * 0.8f, 250.0f * 0.6f},           // scale
      "assets/game/sprites.png",                // texture file address
      ATLAS,                                    // single image or atlas?
      { 6, 8 },                                 // atlas dimensions
      xochitlAnimationAtlas,                    // actual atlas
      PLAYER                                    // entity type
   );

   std::vector<Vector2> enemyPositions = {
      {mOrigin.x - 425.0f, mOrigin.y - 10.0f}
   };

   for (auto& pos : enemyPositions) {
      Entity* boar = new Entity(
         pos,
         {250.0f * 0.5f, 250.0f * 0.35f},
         "assets/game/boar_walk.png",
         ATLAS,
         {2, 6},
         boarAnimationAtlas,
         NPC
      );
      boar->setAIType(WANDERER);
      boar->setAIState(WALKING);
      boar->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
      boar->setColliderDimensions({
         boar->getScale().x / 4.5f,
         boar->getScale().y / 1.3f
      });

      mGameState.enemies.push_back(boar);
   }

   mGameState.goal = new Entity(
      {mOrigin.x + 1000.0f, mOrigin.y + 100.0f}, // fixed position
      {250.0f * 0.35f, 250.0f * 0.4f},                        // scale
      "assets/game/campfire.png",               // texture file
      ATLAS,                                    // atlas type
      {8, 5},                                   // atlas dimensions
      goalAnimationAtlas,                        // animation frames
      NONE                                       // entity type (not NPC/PLAYER)
   );

   mGameState.xochitl->setJumpingPower(550.0f);
   mGameState.xochitl->setColliderDimensions({
      mGameState.xochitl->getScale().x / 4.5f,
      mGameState.xochitl->getScale().y / 1.5f
   });
   mGameState.xochitl->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});

   mGameState.goal->resetMovement();
   mGameState.goal->setAIState(IDLE);
   mGameState.goal->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
   mGameState.goal->setDirection(RIGHT);
   /*
      ----------- CAMERA -----------
   */
   mGameState.camera = { 0 };                                    // zero initialize
   mGameState.camera.target = mGameState.xochitl->getPosition(); // camera follows player
   mGameState.camera.offset = mOrigin;                           // camera offset to center of screen
   mGameState.camera.rotation = 0.0f;                            // no rotation
   mGameState.camera.zoom = 1.0f;                                // default zoom
}

void LevelA::update(float deltaTime)
{
   UpdateMusicStream(mGameState.bgm);
   if (fabs(mGameState.xochitl->getVelocity().x) < 0.1f) {
      if (mGameState.xochitl->getDirection() == RIGHT)
         mGameState.xochitl->setDirection(IDLE_RIGHT);
      else if (mGameState.xochitl->getDirection() == LEFT)
         mGameState.xochitl->setDirection(IDLE_LEFT);
   } else {
      if (mGameState.xochitl->getVelocity().x > 0)
         mGameState.xochitl->setDirection(RIGHT);
      else
         mGameState.xochitl->setDirection(LEFT);
   }
   
   mGameState.xochitl->update(deltaTime, nullptr, mGameState.map, nullptr, 0);

   for (auto& enemy : mGameState.enemies) {
      enemy->update(deltaTime, mGameState.xochitl, mGameState.map, nullptr, 0);
      if (mGameState.xochitl->checkCollision(enemy)) {
         mGameState.xochitl->setPosition({mOrigin.x - 1050.0f, mOrigin.y - 10.0f});
         PlaySound(mGameState.hurtSound);
         GameState::lives--;
      }
   }

   mGameState.goal->updateAnimationOnly(deltaTime);

   Vector2 currentPlayerPosition = { mGameState.xochitl->getPosition().x, mOrigin.y };

   if (mGameState.xochitl->getPosition().y > 800.0f) {
      mGameState.xochitl->setPosition({mOrigin.x - 1050.0f, mOrigin.y - 10.0f});
      PlaySound(mGameState.hurtSound);
      GameState::lives--;
   }

   if (mGameState.xochitl->checkCollision(mGameState.goal)) {
      mGameState.nextSceneID = 2;
      PlaySound(mGameState.goalSound);
   }

   if (GameState::lives <= 0) {
      mGameState.nextSceneID = 5;
   }

   panCamera(&mGameState.camera, &currentPlayerPosition);
}

void LevelA::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));
   BeginMode2D(mGameState.camera);

   mGameState.map->render();
   mGameState.xochitl->render();
   for (auto& enemy : mGameState.enemies) {
      enemy->render();
   }
   mGameState.goal->render();

   EndMode2D();
   DrawText(TextFormat("Lives: %d", GameState::lives), 10, 10, 20, WHITE);
}

void LevelA::shutdown()
{
   UnloadMusicStream(mGameState.bgm);
   UnloadSound(mGameState.jumpSound);
   UnloadSound(mGameState.hurtSound);
   UnloadSound(mGameState.goalSound);
}