#include "Level0.h"

Level0::Level0()                                      : Scene { {0.0f}, nullptr   } {}
Level0::Level0(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

Level0::~Level0() { shutdown(); }

void Level0::initialise()
{
   mGameState.nextSceneID = -1; 

   mGameState.bgm = LoadMusicStream("assets/game/04 - Silent Forest.wav");
   SetMusicVolume(mGameState.bgm, 0.33f);

   mGameState.map = new Map(
      LEVEL_WIDTH, LEVEL_HEIGHT,
      (unsigned int *) mLevelData,
      "assets/game/grass.png",
      TILE_DIMENSION,
      9, 18,
      mOrigin
   );
}

void Level0::update(float deltaTime)
{
   UpdateMusicStream(mGameState.bgm);

   if (IsKeyPressed(KEY_ENTER)) {
       mGameState.nextSceneID = 1;
   }
}

void Level0::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));
   
   mGameState.map->render();

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
   delete mGameState.map;
   mGameState.map = nullptr; // Good practice

   UnloadMusicStream(mGameState.bgm);
   // UnloadSound(mGameState.jumpSound); 
}