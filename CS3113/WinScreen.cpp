#include "WinScreen.h"

WinScreen::WinScreen()                                      : Scene { {0.0f}, nullptr   } {}
WinScreen::WinScreen(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

WinScreen::~WinScreen() { shutdown(); }

void WinScreen::initialise()
{
   mGameState.nextSceneID = -1; 

   mGameState.bgm = LoadMusicStream("assets/game/music_level1.wav");
   mBackgroundTexture = LoadTexture("assets/game/sky.png");
   PlayMusicStream(mGameState.bgm);

   mGameState.map = new Map(
      LEVEL_WIDTH, LEVEL_HEIGHT,
      (unsigned int *) mLevelData,
      "assets/game/grass.png",
      TILE_DIMENSION,
      9, 18,
      mOrigin
   );
}

void WinScreen::update(float deltaTime)
{
   UpdateMusicStream(mGameState.bgm);

   if (IsKeyPressed(KEY_ENTER)) {
       mGameState.nextSceneID = 0;
   }
}

void WinScreen::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));
   float scaleX = (float)GetScreenWidth() / mBackgroundTexture.width;
   float scaleY = (float)GetScreenHeight() / mBackgroundTexture.height;

   DrawTexturePro(
      mBackgroundTexture,
      { 0, 0, (float)mBackgroundTexture.width, (float)mBackgroundTexture.height },
      { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
      { 0, 0 },
      0.0f,
      WHITE
   );
   
   mGameState.map->render();

   const char* titleText = "You've Won!";
   int titleFontSize = 60;
   int titleTextWidth = MeasureText(titleText, titleFontSize);
   DrawText(titleText, (GetScreenWidth() - titleTextWidth) / 2, GetScreenHeight() / 4, titleFontSize, WHITE);

   const char* promptText = "Press ENTER to go back to the Main Menu.";
   int promptFontSize = 30;
   int promptTextWidth = MeasureText(promptText, promptFontSize);
   DrawText(promptText, (GetScreenWidth() - promptTextWidth) / 2, GetScreenHeight() / 2, promptFontSize, LIGHTGRAY);
}

void WinScreen::shutdown()
{
   delete mGameState.map;
   mGameState.map = nullptr;
   UnloadTexture(mBackgroundTexture);

   UnloadMusicStream(mGameState.bgm);
}