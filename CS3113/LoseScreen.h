#ifndef LOSESCREEN_H
#define LOSESCREEN_H

#include "Scene.h"

class LoseScreen : public Scene {
public:
    static constexpr int LEVEL_WIDTH = 14,
                         LEVEL_HEIGHT = 8;
private:
    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
        11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11
    };

public:
    static constexpr float TILE_DIMENSION = 75.0f;
    Texture2D mBackgroundTexture;

    LoseScreen();
    LoseScreen(Vector2 origin, const char *bgHexCode);
    ~LoseScreen();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif