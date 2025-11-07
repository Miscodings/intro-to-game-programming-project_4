#include "LevelB.h"

LevelB::LevelB()                                      : Scene { {0.0f}, nullptr   } {}
LevelB::LevelB(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

LevelB::~LevelB() {
    for (auto& enemy : mGameState.enemies) delete enemy;
    delete mGameState.xochitl;
    delete mGameState.goal;
    delete mGameState.map;
    shutdown(); 
}

void LevelB::initialise()
{
   mGameState.nextSceneID = -1; 
   
   mGameState.bgm = LoadMusicStream("assets/game/music_level2.wav");
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
      {IDLE_RIGHT,  {  0,  1, 2, 3 }},
      {IDLE_LEFT, { 8, 9, 10, 11 }},
      {RIGHT,  {  16, 17, 18, 19, 20, 21, 22, 23 }},
      {LEFT, { 24, 25, 26, 27, 28, 29, 30, 31 }},
      {JUMP_RIGHT, { 32, 33, 34, 35, 36 }},
      {JUMP_LEFT, { 40, 41, 42, 43, 44 }},
   };

   float sizeRatio  = 48.0f / 64.0f;
   
   std::map<Direction, std::vector<int>> beeAnimationAtlas = {
      {LEFT,  { 0, 1, 2, 3 }},
      {RIGHT,  { 0, 1, 2, 3 }},
      {JUMP_RIGHT,  { 0, 1, 2, 3 }},
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

   std::vector<Vector2> beePositions = {
      {mOrigin.x - 525.0f, mOrigin.y + 95.0f},
      {mOrigin.x - 225.0f, mOrigin.y + 95.0f},
      {mOrigin.x + 525.0f, mOrigin.y + 95.0f},
   };

   for (auto& pos : beePositions) {
      Entity* bee = new Entity(
         pos,
         {250.0f * 0.5f, 250.0f * 0.5f}, 
         "assets/game/bee_fly.png",
         ATLAS,
         {1, 4},
         beeAnimationAtlas,
         NPC
      );
      bee->setDirection(RIGHT);
      bee->setAIType(VERTICAL_FLYER);
      bee->setAIState(WALKING); // or IDLE, doesn’t matter much
      bee->setAcceleration({0.0f, 0.0f});
      bee->setColliderDimensions({
         bee->getScale().x / 4.5f,
         bee->getScale().y / 4.0f
      });
      mGameState.enemies.push_back(bee);
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

void LevelB::update(float deltaTime)
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

   for (auto& bee : mGameState.enemies) {
      bee->update(deltaTime, mGameState.xochitl, mGameState.map, nullptr, 0);
      if (mGameState.xochitl->checkCollision(bee)) {
         mGameState.xochitl->setPosition({mOrigin.x - 1050.0f, mOrigin.y - 10.0f});
         PlaySound(mGameState.hurtSound);
      }
   }

   mGameState.goal->updateAnimationOnly(deltaTime);

   Vector2 currentPlayerPosition = { mGameState.xochitl->getPosition().x, mOrigin.y };

   if (mGameState.xochitl->getPosition().y > 800.0f)  {
      mGameState.xochitl->setPosition({mOrigin.x - 1050.0f, mOrigin.y - 10.0f});
      PlaySound(mGameState.hurtSound);
   }

   if (mGameState.xochitl->checkCollision(mGameState.goal))  {
      mGameState.nextSceneID = 3;
      PlaySound(mGameState.goalSound);
   }

   panCamera(&mGameState.camera, &currentPlayerPosition);
}

void LevelB::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));
   BeginMode2D(mGameState.camera);

   mGameState.map->render();
   mGameState.xochitl->render();

   for (auto& bee : mGameState.enemies) {
      bee->render();
      bee->displayCollider();
   }

   mGameState.goal->render();

   mGameState.xochitl->displayCollider();

   EndMode2D();
}

void LevelB::shutdown()
{
   UnloadMusicStream(mGameState.bgm);
   UnloadSound(mGameState.jumpSound);
   mGameState.enemies.clear();
}