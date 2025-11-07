#include "CS3113/LevelA.h"
#include "CS3113/LevelB.h"
#include "CS3113/LevelC.h"
#include "CS3113/Level0.h"

// Global Constants
constexpr int SCREEN_WIDTH     = 1000,
              SCREEN_HEIGHT    = 600,
              FPS              = 120;
constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus   = RUNNING;
float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f;

Scene *gCurrentScene = nullptr;
std::vector<Scene*> gLevels;

Level0 *gLevel0 = nullptr;
LevelA *gLevelA = nullptr;
LevelB *gLevelB = nullptr;
LevelC *gLevelC = nullptr;

// Function Declarations
void switchToScene(Scene *scene);
void initialise();
void processInput();
void update();
void render();
void shutdown();

void switchToScene(Scene *scene)
{   
    if (scene) {
        if (gCurrentScene) gCurrentScene->shutdown();   
        gCurrentScene = scene;
        gCurrentScene->initialise();
    }

}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Scenes");
    InitAudioDevice();

    // --- Create scene objects ---
    gLevel0 = new Level0(ORIGIN, "#76b6ff"); // Blue color for menu
    gLevelA = new LevelA(ORIGIN, "#87a7ebff");
    gLevelB = new LevelB(ORIGIN, "#011627");
    gLevelC = new LevelC(ORIGIN, "#774709");

    gLevels.push_back(gLevel0);
    gLevels.push_back(gLevelA);
    gLevels.push_back(gLevelB);
    gLevels.push_back(gLevelC);

    switchToScene(gLevels[0]);

    SetTargetFPS(FPS);
}

void processInput() 
{
    if (!gCurrentScene) return;

    if (gCurrentScene->getState().xochitl) 
    {
        gCurrentScene->getState().xochitl->resetMovement();

        if      (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) gCurrentScene->getState().xochitl->moveLeft();
        else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) gCurrentScene->getState().xochitl->moveRight();

        if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_SPACE)) && gCurrentScene->getState().xochitl->isCollidingBottom())
        {
            gCurrentScene->getState().xochitl->jump();
            PlaySound(gCurrentScene->getState().jumpSound);
        }
    }

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() 
{
    if (!gCurrentScene) return;

    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    gTimeAccumulator += deltaTime;

    while (gTimeAccumulator >= FIXED_TIMESTEP)
    {
        gCurrentScene->update(FIXED_TIMESTEP);
        gTimeAccumulator -= FIXED_TIMESTEP;
    }
}

void render()
{
    BeginDrawing();
    if (gCurrentScene) {
        gCurrentScene->render();
    }
    EndDrawing();
}

void shutdown() 
{
    // A single loop is the safest way to delete scenes
    for (auto scene : gLevels) {
        delete scene;
    }
    gLevels.clear();

    CloseAudioDevice();
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();

        if (gCurrentScene && gCurrentScene->getState().nextSceneID != -1)
        {
            int id = gCurrentScene->getState().nextSceneID;
            if (id >= 0 && id < gLevels.size()) {
                switchToScene(gLevels[id]);
            }
        }

        render();
    }

    shutdown();

    return 0;
}