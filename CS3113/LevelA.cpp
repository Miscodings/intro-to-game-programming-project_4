#include "LevelA.h"

LevelA::LevelA()                                      : Scene { {0.0f}, nullptr   } {}
LevelA::LevelA(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

LevelA::~LevelA() {
   delete mGameState.enemy;
   delete mGameState.xochitl;
   delete mGameState.map;
   shutdown(); 
}

void LevelA::initialise()
{
   mGameState.nextSceneID = -1; 
   
   mGameState.bgm = LoadMusicStream("assets/game/04 - Silent Forest.wav");
   SetMusicVolume(mGameState.bgm, 0.33f);
   // PlayMusicStream(mGameState.bgm);

   mGameState.jumpSound = LoadSound("assets/game/Dirt Jump.wav");

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
      {RIGHT,  {  0,  1, 2, 3, 4, 5, 6, 7 }},
      {LEFT, { 8, 9, 10, 11, 12, 13, 14, 15 }},
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
      {250.0f * 0.8f, 250.0f * 0.6f},             // scale
      "assets/game/walk.png",                   // texture file address
      ATLAS,                                    // single image or atlas?
      { 2, 8 },                                 // atlas dimensions
      xochitlAnimationAtlas,                    // actual atlas
      PLAYER                                    // entity type
   );

   mGameState.enemy = new Entity(
      {mOrigin.x - 450.0f, mOrigin.y - 300.0f}, // position
      {250.0f * 0.5f, 250.0f * 0.35f},             // scale
      "assets/game/boar_walk.png",              // texture file address
      ATLAS,                                    // single image or atlas?
      { 2, 6 },                                 // atlas dimensions
      boarAnimationAtlas,                       // actual atlas
      NPC                                       // entity type
   );

   mGameState.goal = new Entity(
      {mOrigin.x + 1000.0f, mOrigin.y + 80.0f}, // fixed position
      {250.0f * 0.4f, 250.0f * 0.6f},                        // scale
      "assets/game/campfire.png",               // texture file
      ATLAS,                                    // atlas type
      {8, 5},                                   // atlas dimensions
      goalAnimationAtlas,                        // animation frames
      NONE                                       // entity type (not NPC/PLAYER)
   );

   mGameState.xochitl->setJumpingPower(550.0f);
   mGameState.xochitl->setColliderDimensions({
      mGameState.xochitl->getScale().x / 4.5f,
      mGameState.xochitl->getScale().y / 4.0f
   });
   mGameState.xochitl->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});

   mGameState.enemy->setAIType(WANDERER);
   mGameState.enemy->setAIState(WALKING);
   mGameState.enemy->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
   mGameState.enemy->setColliderDimensions({
      mGameState.enemy->getScale().x / 4.5f,
      mGameState.enemy->getScale().y / 1.3f
   });

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

   mGameState.xochitl->update(deltaTime, nullptr, mGameState.map, nullptr, 0);
   mGameState.enemy->update(deltaTime, mGameState.xochitl, mGameState.map, nullptr, 0);
   mGameState.goal->updateAnimationOnly(deltaTime);

   Vector2 currentPlayerPosition = { mGameState.xochitl->getPosition().x, mOrigin.y };

   if (mGameState.xochitl->checkCollision(mGameState.enemy)) {
      mGameState.xochitl->setPosition({mOrigin.x - 1050.0f, mOrigin.y - 10.0f});
   }

   if (mGameState.xochitl->getPosition().y > 800.0f)  {
      mGameState.xochitl->setPosition({mOrigin.x - 1050.0f, mOrigin.y - 10.0f});
   }

   if (mGameState.xochitl->checkCollision(mGameState.goal)) mGameState.nextSceneID = 2;

   panCamera(&mGameState.camera, &currentPlayerPosition);
}

void LevelA::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));
   BeginMode2D(mGameState.camera);

   mGameState.map->render();
   mGameState.xochitl->render();
   mGameState.enemy->render();
   mGameState.goal->render();

   mGameState.xochitl->displayCollider();
   mGameState.enemy->displayCollider();

   EndMode2D();
}

void LevelA::shutdown()
{
   UnloadMusicStream(mGameState.bgm);
   UnloadSound(mGameState.jumpSound);
}