#ifndef LEVELB_H
#define LEVELB_H

#include "Scene.h"

class LevelB : public Scene {
public:
    static constexpr int LEVEL_WIDTH = 32,
                         LEVEL_HEIGHT = 8;
private:
    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
        142,  0,  0,  0, 0,  0,  0, 0,  0,  0,  0,  0,  0, 0,  0,  0, 0,  0,  0, 0,142,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,142,
        123,  0,  0,  0, 0,  0,  0, 0,  0,  0,  0,  0,  0, 0,  0,  0, 0,  0,  0, 0,121,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0, 15,
        123,  0,  0,  0, 0,  0,  0, 0,  0,  0,  0,  0,  0, 0,  0,  0, 0,  0,  0, 0,121,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0, 15,
        123,  0,  0,  0, 0,  0,  0, 0,  0,  0,  0,  0,  0, 0,  0,  0, 0,  0,  0, 0,121,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0, 15,
        123,  0,  0,  0, 0,  0,  0, 0,  0,  0,  0,  0,  0, 0,  0,  0, 0,  0,  0, 0,151,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0, 15,
        18, 110,110,111, 0,  0,  0, 0,  0,  0,  0,  0,  0, 0,  0,  0, 0,145,148, 0,  0,  0,  0,  0,142, 0,  0,  0,  0,  0,  0, 13,
        18,  11, 11,124, 0,  0,145,148,  0,  0,145,148, 0, 0,145,148, 0,  0,  0, 0,  0,  0,  0,  0,151, 0,145,113,113,113,113,146,
        18,  11, 11,124, 0,  0,  0, 0,  0,  0,  0,  0,  0, 0,  0,  0, 0,  0,  0, 0,145,161,148,  0,  0, 0,  0,  0,  0,  0,  0, 13
    };

public:
    static constexpr float TILE_DIMENSION       = 75.0f,
                        ACCELERATION_OF_GRAVITY = 981.0f,
                        END_GAME_THRESHOLD      = 800.0f;

    LevelB();
    LevelB(Vector2 origin, const char *bgHexCode);
    ~LevelB();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif