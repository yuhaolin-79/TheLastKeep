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

    level.rows = 22;
    level.cols = 40;
    level.tileSize = 32;

    // E Empty 不可建塔，不可行走
    // R Road 道路，敌人行走区域
    // B Buildable 可建塔区域
    // S Start 敌人入口
    // E End 城堡终点
    level.mapData = {
        "............................BBBB....EEEE",
        "............................BBBBRRRREEEE",
        "............................BBBBR.......",
        "......................BBBBBBBBBBR.......",
        "......................BBBBBBBBBBR.......",
        "..................BBBBBBBBBBBBBBR.......",
        "..................BBRRRRRRRRRRRR.......",
        "..................BBR...BBBBBBBB........",
        "..........BBBBBBBBBBR...BBBBBBBB........",
        "..........BBBBBBBBBBR...BBBBBBBB........",
        "......BBBBBBBBBBBBBBR...................",
        "......BBRRRRRRRRRRRRR...................",
        "......BBR..BBBBBBBB.....................",
        "......BBR..BBBBBBBB....BBBBBBBB.........",
        "......BBR..BBBBBBBB....BBBBBBBB.........",
        "......BBR..BBBBBBBB....BBBBBBBB.........",
        "SSSSRRRRR..BBBBBBBB.....................",
        "BBBBBBBBB...............................",
        "BBBBBBBBB...............................",
        "........................................",
        "........................................",
        "........................................"
    };
    level.wayPoints = {
        QPoint(16, 0),   // 入口：左下角道路
        QPoint(16, 8),
        QPoint(11, 8),
        QPoint(11, 20),
        QPoint(6, 20),
        QPoint(6, 32),
        QPoint(1, 32),
        QPoint(1, 39)    // 城堡终点：右上角要塞
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