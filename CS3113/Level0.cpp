#include "Level0.h"

Level0::Level0()                                      : Scene { {0.0f}, nullptr   } {}
Level0::Level0(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

Level0::~Level0() { shutdown(); }

void Level0::initialise()
{
   // Set to -1 to prevent an immediate scene change
   mGameState.nextSceneID = -1; 

   mGameState.bgm = LoadMusicStream("assets/game/04 - Silent Forest.wav");
   SetMusicVolume(mGameState.bgm, 0.33f);

   mGameState.map = new Map(
      LEVEL_WIDTH, LEVEL_HEIGHT,
      (unsigned int *) mLevelData,
      "assets/game/tileset.png",
      TILE_DIMENSION,
      4, 1,
      mOrigin
   );
}

void Level0::update(float deltaTime)
{
   UpdateMusicStream(mGameState.bgm);

   // When the user presses enter, switch to Level A (scene index 1)
   if (IsKeyPressed(KEY_ENTER)) {
       mGameState.nextSceneID = 1;
   }
}

void Level0::render()
{
   // 1. Clear the background (this is a screen-space operation)
   ClearBackground(ColorFromHex(mBGColourHexCode));
   
   // 2. Render the map (this is a world-space object, but without a camera it's drawn from the top-left)
   mGameState.map->render();

   // 3. Draw the text (this is a screen-space operation)
   const char* titleText = "Rise of Xochitl";
   int titleFontSize = 60;
   int titleTextWidth = MeasureText(titleText, titleFontSize);
   DrawText(titleText, (GetScreenWidth() - titleTextWidth) / 2, GetScreenHeight() / 4, titleFontSize, WHITE);

   const char* promptText = "Press ENTER to Start";
   int promptFontSize = 30;
   int promptTextWidth = MeasureText(promptText, promptFontSize);
   DrawText(promptText, (GetScreenWidth() - promptTextWidth) / 2, GetScreenHeight() / 2, promptFontSize, LIGHTGRAY);
}

void Level0::shutdown()
{
   // --- FIXED: Added delete to prevent memory leak ---
   delete mGameState.map;
   mGameState.map = nullptr; // Good practice

   UnloadMusicStream(mGameState.bgm);
   
   // --- FIXED: Removed this line because jumpSound was never loaded in this scene ---
   // UnloadSound(mGameState.jumpSound); 
}