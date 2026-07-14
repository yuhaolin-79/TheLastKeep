#pragma once

#include "level/LevelData.h"

class LevelManager {
public:
    static LevelData createLevel(int levelId);
    static LevelData createTutorialLevel();
    static LevelData createLevel01();
    static LevelData createLevel02();
    static LevelData createLevel03();
};