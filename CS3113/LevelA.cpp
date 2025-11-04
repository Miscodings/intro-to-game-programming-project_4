#include "LevelA.h"

LevelA::LevelA()                                      : Scene { {0.0f}, nullptr   } {}
LevelA::LevelA(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

LevelA::~LevelA() { shutdown(); }

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
      "assets/game/tileset.png",   // texture filepath
      TILE_DIMENSION,              // tile size
      4, 1,                        // texture cols & rows
      mOrigin                      // in-game origin
   );

   /*
      ----------- PROTAGONIST -----------
   */
   std::map<Direction, std::vector<int>> xochitlAnimationAtlas = {
      {DOWN,  {  0,  1,  2,  3,  4,  5,  6,  7 }},
      {LEFT,  {  8,  9, 10, 11, 12, 13, 14, 15 }},
      {UP,    { 24, 25, 26, 27, 28, 29, 30, 31 }},
      {RIGHT, { 40, 41, 42, 43, 44, 45, 46, 47 }},
   };

   float sizeRatio  = 48.0f / 64.0f;

   // Assets from @see https://sscary.itch.io/the-adventurer-female
   mGameState.xochitl = new Entity(
      {mOrigin.x - 300.0f, mOrigin.y - 200.0f}, // position
      {250.0f * sizeRatio, 250.0f},             // scale
      "assets/game/walk.png",                   // texture file address
      ATLAS,                                    // single image or atlas?
      { 6, 8 },                                 // atlas dimensions
      xochitlAnimationAtlas,                    // actual atlas
      PLAYER                                    // entity type
   );

   mGameState.xochitl->setJumpingPower(550.0f);
   mGameState.xochitl->setColliderDimensions({
      mGameState.xochitl->getScale().x / 3.5f,
      mGameState.xochitl->getScale().y / 3.0f
   });
   mGameState.xochitl->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});

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

   mGameState.xochitl->update(
      deltaTime,      // delta time / fixed timestep
      nullptr,        // player
      mGameState.map, // map
      nullptr,        // collidable entities
      0               // col. entity count
   );

   Vector2 currentPlayerPosition = { mGameState.xochitl->getPosition().x, mOrigin.y };

   if (mGameState.xochitl->getPosition().y > 800.0f) mGameState.nextSceneID = 1;

   panCamera(&mGameState.camera, &currentPlayerPosition);
}

// In LevelA.cpp (and LevelB.cpp)
void LevelA::render()
{
    // This is a screen-space operation, it can be inside or outside the camera block
    ClearBackground(ColorFromHex(mBGColourHexCode));

    // --- Start the camera view ---
    BeginMode2D(mGameState.camera);

        // --- Draw all game world objects here ---
        mGameState.xochitl->render();
        mGameState.map->render();

    // --- End the camera view ---
    EndMode2D();
    
    // If you wanted to draw UI on top (like a score), you would do it here,
    // outside the Mode2D block.
}

void LevelA::shutdown()
{
   delete mGameState.xochitl;
   delete mGameState.map;

   UnloadMusicStream(mGameState.bgm);
   UnloadSound(mGameState.jumpSound);
}