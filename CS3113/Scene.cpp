#include "Scene.h"

// Default constructor
Scene::Scene() : mOrigin{ 0.0f, 0.0f }
{
    mGameState.xochitl      = nullptr;
    mGameState.map          = nullptr;
    mGameState.enemy        = nullptr;
    mGameState.goal         = nullptr;
    mGameState.nextSceneID  = -1;
    mGameState.camera       = { 0 };
}

// Parameterized constructor
Scene::Scene(Vector2 origin, const char *bgHexCode) : mOrigin{origin}, mBGColourHexCode{bgHexCode} 
{
    mGameState.xochitl      = nullptr;
    mGameState.map          = nullptr;
    mGameState.enemy        = nullptr;
    mGameState.goal         = nullptr;
    mGameState.nextSceneID  = -1;
    mGameState.camera       = { 0 };
}