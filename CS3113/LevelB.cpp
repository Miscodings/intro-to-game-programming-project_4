#include "LevelB.h"

// ... (constructors are fine) ...
LevelB::LevelB()                                      : Scene { {0.0f}, nullptr   } {}
LevelB::LevelB(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}
LevelB::~LevelB() { shutdown(); }


// ... (initialise is fine) ...
void LevelB::initialise()
{
   // Add this line to prevent scene from immediately switching
   mGameState.nextSceneID = -1; 
   
   mGameState.bgm = LoadMusicStream("assets/game/04 - Silent Forest.wav");
   SetMusicVolume(mGameState.bgm, 0.33f);
   // ... rest of initialise is fine
   // ...
}

void LevelB::update(float deltaTime)
{
   UpdateMusicStream(mGameState.bgm);

   mGameState.xochitl->update(
      deltaTime,
      nullptr,
      mGameState.map,
      nullptr,
      0
   );

   Vector2 currentPlayerPosition = { mGameState.xochitl->getPosition().x, mOrigin.y };

   // --- FIXED: This function was not defined anywhere, causing an error ---
   // panCamera(&mGameState.camera, &currentPlayerPosition);
   
   // A basic camera follow implementation:
   mGameState.camera.target = mGameState.xochitl->getPosition();
}

// ... (render and shutdown are fine) ...
// In LevelA.cpp (and LevelB.cpp)
void LevelB::render()
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

void LevelB::shutdown()
{
   delete mGameState.xochitl;
   delete mGameState.map;
   mGameState.xochitl = nullptr;
   mGameState.map = nullptr;
   UnloadMusicStream(mGameState.bgm);
   UnloadSound(mGameState.jumpSound);
}