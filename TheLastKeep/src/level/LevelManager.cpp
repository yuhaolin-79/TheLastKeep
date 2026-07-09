/* 关卡管理器
 * 负责 根据levelID 返回关卡数据
 * 创建各个关卡
 * 后续可以改成从 JSON 加载
 */



#include "level/LevelManager.h"

LevelData LevelManager::createTutorialLevel() {
    LevelData level;

    level.levelname = "Tutorial Level";
    level.backgroundPath = ":/images/tutorialLevelMap.png";

    level.rows = 18;
    level.cols = 32;
    level.tileSize = 40;

    // E Empty 不可建塔，不可行走
    // R Road 道路，敌人行走区域
    // B Buildable 可建塔区域
    // S Start 敌人入口
    // E End 城堡终点
    level.mapData = {
        "............................EEEE",
        "........................RRRREEEE",
        "........................R.......",
        ".....BBB....BBB....BBB..R.......",
        ".....BBB....BBB....BBB..R.......",
        "........................R.......",
        "..............RRRRRRRRRRR.......",
        "..............R.................",
        "....BBB.......R....BBB..........",
        "....BBB.......R....BBB..........",
        "..............R.................",
        "........RRRRRRR.................",
        "........R.......................",
        "........R....BBB....BBB.........",
        "........R....BBB....BBB.........",
        "SSSSRRRRR.......................",
        "................................",
        "................................"
    };
    level.wayPoints = {
        QPoint(15, 0),   // 入口
        QPoint(15, 8),
        QPoint(11, 8),
        QPoint(11, 14),
        QPoint(6, 14),
        QPoint(6, 24),
        QPoint(1, 24),
        QPoint(1, 31)    // 城堡终点
    };

    return level;
}

LevelData LevelManager::createLevel01() {
    LevelData level;
    level.levelname = "Level 01";
    return level;
}

LevelData LevelManager::createLevel02() {
    LevelData level;
    level.levelname = "Level 02";
    return level;
}

LevelData LevelManager::createLevel03() {
    LevelData level;
    level.levelname = "Level 03";
    return level;
}