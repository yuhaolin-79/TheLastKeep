/* 关卡管理器
 * 负责 根据levelID 返回关卡数据
 * 创建各个关卡
 * 后续可以改成从 JSON 加载
 */


#pragma once

#include "level/LevelData.h"

class LevelManager {
public:
    static LevelData createTutorialLevel(); // 引导关卡

    // 以下三个关卡目前暂不实现 2026/07/08 @fish
    static LevelData createLevel01();
    static LevelData createLevel02();
    static LevelData createLevel03();
};


