#pragma once

#include "game/LevelData.h"

class LevelManager {
public:
    static LevelData createTutorialLevel(); // 引导关卡

    // 以下三个关卡目前暂不实现 2026/07/08 @fish
    static LevelData createLevel01();
    static LevelData createLevel02();
    static LevelData createLevel03();
};


