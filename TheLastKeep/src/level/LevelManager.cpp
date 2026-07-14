#include "level/LevelManager.h"

#include "common/GameConstants.h"

namespace {
void fillRect(LevelData &level, int top, int left, int bottom, int right, QChar marker)
{
    for (int row = top; row <= bottom; ++row) {
        if (row < 0 || row >= level.rows) {
            continue;
        }

        for (int col = left; col <= right; ++col) {
            if (col < 0 || col >= level.cols) {
                continue;
            }

            level.mapData[row][col] = marker;
        }
    }
}

LevelData createEmptyLevel(const QString &name, const QString &backgroundPath)
{
    LevelData level;
    level.levelname = name;
    level.backgroundPath = backgroundPath;
    level.rows = 22;
    level.cols = 40;
    level.tileSize = 32;
    level.initialGold = GameConstants::DefaultInitialGold;
    level.mapData = QVector<QString>(level.rows, QString(level.cols, '.'));
    return level;
}
}

LevelData LevelManager::createLevel(int levelId)
{
    switch (levelId) {
    case 1:
        return createLevel01();
    case 2:
        return createLevel02();
    case 3:
        return createLevel03();
    case 0:
    default:
        return createTutorialLevel();
    }
}

// 格子类型定义在 GameMap.h

LevelData LevelManager::createTutorialLevel()
{
    LevelData level = createEmptyLevel("Tutorial Level", ":/images/tutorialLevelMap.png");

    fillRect(level, 4, 16, 5, 18, 'B');
    fillRect(level, 4, 21, 5, 23, 'B');
    fillRect(level, 4, 24, 5, 26, 'B');
    fillRect(level, 5, 13, 6, 15, 'B');

    fillRect(level, 9, 10, 10, 12, 'B');
    fillRect(level, 9, 16, 10, 18, 'B');
    fillRect(level, 9, 22, 10, 24, 'B');
    fillRect(level, 9, 25, 10, 27, 'B');

    fillRect(level, 13, 7, 14, 9, 'B');
    fillRect(level, 14, 13, 15, 15, 'B');
    fillRect(level, 14, 19, 15, 21, 'B');
    fillRect(level, 13, 22, 14, 24, 'B');

    fillRect(level, 16, 16, 17, 18, 'B');
    fillRect(level, 16, 24, 17, 26, 'B');

    fillRect(level, 18, 5, 18, 8, 'R');
    fillRect(level, 14, 7, 18, 7, 'R');
    fillRect(level, 14, 7, 14, 11, 'R');
    fillRect(level, 11, 11, 14, 11, 'R');
    fillRect(level, 11, 11, 11, 20, 'R');
    fillRect(level, 7, 20, 11, 20, 'R');
    fillRect(level, 7, 20, 7, 28, 'R');
    fillRect(level, 6, 28, 6, 28, 'R');
    fillRect(level, 5, 29, 5, 29, 'R');
    fillRect(level, 4, 30, 4, 30, 'R');
    fillRect(level, 18, 5, 18, 5, 'S');
    fillRect(level, 3, 31, 3, 31, 'E');

    level.wayPoints = {
        QPoint(18, 5), QPoint(18, 7), QPoint(14, 7), QPoint(14, 11),
        QPoint(11, 11), QPoint(11, 20), QPoint(7, 20), QPoint(7, 28),
        QPoint(6, 28), QPoint(5, 29), QPoint(4, 30), QPoint(3, 31)
    };

    return level;
}

LevelData LevelManager::createLevel01()
{
    LevelData level = createEmptyLevel("Level 01", ":/images/level01Map.png");

    fillRect(level, 5, 11, 6, 13, 'B');
    fillRect(level, 5, 24, 6, 26, 'B');
    fillRect(level, 9, 6, 10, 8, 'B');
    fillRect(level, 9, 16, 10, 18, 'B');
    fillRect(level, 9, 28, 10, 30, 'B');
    fillRect(level, 13, 10, 14, 12, 'B');
    fillRect(level, 13, 22, 14, 24, 'B');
    fillRect(level, 16, 16, 17, 18, 'B');
    fillRect(level, 16, 30, 17, 32, 'B');

    fillRect(level, 17, 3, 17, 8, 'R');
    fillRect(level, 12, 8, 17, 8, 'R');
    fillRect(level, 12, 8, 12, 15, 'R');
    fillRect(level, 7, 15, 12, 15, 'R');
    fillRect(level, 7, 15, 7, 23, 'R');
    fillRect(level, 4, 23, 7, 23, 'R');
    fillRect(level, 4, 23, 4, 31, 'R');
    fillRect(level, 3, 31, 3, 33, 'R');
    fillRect(level, 17, 3, 17, 3, 'S');
    fillRect(level, 3, 34, 3, 34, 'E');

    level.wayPoints = {
        QPoint(17, 3),
        QPoint(17, 8),
        QPoint(12, 8),
        QPoint(12, 15),
        QPoint(7, 15),
        QPoint(7, 23),
        QPoint(4, 23),
        QPoint(4, 31),
        QPoint(3, 33),
        QPoint(3, 34)
    };

    level.initialTowers = {
        InitialTowerPlacement{10, 7, 0},
        InitialTowerPlacement{6, 25, 0}
    };

    return level;
}

LevelData LevelManager::createLevel02()
{
    LevelData level = createEmptyLevel("Level 02", ":/images/level02Map.png");

    fillRect(level, 3, 15, 4, 17, 'B');
    fillRect(level, 3, 22, 4, 24, 'B');
    fillRect(level, 6, 3, 7, 5, 'B');
    fillRect(level, 6, 13, 7, 15, 'B');
    fillRect(level, 6, 25, 7, 27, 'B');
    fillRect(level, 6, 35, 7, 37, 'B');
    fillRect(level, 12, 11, 13, 13, 'B');
    fillRect(level, 12, 27, 13, 29, 'B');
    fillRect(level, 16, 15, 17, 17, 'B');
    fillRect(level, 16, 23, 17, 25, 'B');

    fillRect(level, 2, 5, 4, 5, 'R');
    fillRect(level, 4, 5, 4, 10, 'R');
    fillRect(level, 4, 10, 10, 10, 'R');
    fillRect(level, 10, 10, 10, 20, 'R');

    fillRect(level, 2, 34, 4, 34, 'R');
    fillRect(level, 4, 29, 4, 34, 'R');
    fillRect(level, 4, 29, 10, 29, 'R');
    fillRect(level, 10, 20, 10, 29, 'R');

    fillRect(level, 10, 20, 20, 20, 'R');
    fillRect(level, 2, 5, 2, 5, 'S');
    fillRect(level, 2, 34, 2, 34, 'S');
    fillRect(level, 20, 20, 20, 20, 'E');

    const QVector<QPoint> leftPath = {
        QPoint(2, 5), QPoint(4, 5), QPoint(4, 10),
        QPoint(10, 10), QPoint(10, 20), QPoint(20, 20)
    };
    const QVector<QPoint> rightPath = {
        QPoint(2, 34), QPoint(4, 34), QPoint(4, 29),
        QPoint(10, 29), QPoint(10, 20), QPoint(20, 20)
    };

    level.wayPoints = leftPath;
    level.wayPointPaths = {leftPath, rightPath};

    return level;
}

LevelData LevelManager::createLevel03()
{
    LevelData level = createEmptyLevel("Level 03", ":/images/level03Map.png");

    fillRect(level, 1, 7, 2, 9, 'B');
    fillRect(level, 1, 14, 2, 16, 'B');
    fillRect(level, 1, 24, 2, 26, 'B');
    fillRect(level, 1, 31, 2, 33, 'B');
    fillRect(level, 6, 5, 7, 7, 'B');
    fillRect(level, 6, 12, 7, 14, 'B');
    fillRect(level, 6, 25, 7, 27, 'B');
    fillRect(level, 6, 31, 7, 33, 'B');
    fillRect(level, 11, 5, 12, 7, 'B');
    fillRect(level, 11, 12, 12, 14, 'B');
    fillRect(level, 16, 7, 17, 9, 'B');
    fillRect(level, 16, 14, 17, 16, 'B');
    fillRect(level, 16, 25, 17, 27, 'B');
    fillRect(level, 16, 31, 17, 33, 'B');

    const QVector<int> spawnRows = {4, 9, 14, 19};
    for (int row : spawnRows) {
        fillRect(level, row, 0, row, 20, 'R');
        fillRect(level, row, 0, row, 0, 'S');
    }
    fillRect(level, 4, 20, 19, 20, 'R');
    fillRect(level, 11, 20, 11, 35, 'R');
    fillRect(level, 11, 35, 11, 35, 'E');

    for (int row : spawnRows) {
        level.wayPointPaths.append({
            QPoint(row, 0),
            QPoint(row, 20),
            QPoint(11, 20),
            QPoint(11, 35)
        });
    }
    level.wayPoints = level.wayPointPaths.first();

    return level;
}