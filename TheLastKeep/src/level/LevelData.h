#pragma once

#include "common/GameConstants.h"

#include <QPoint>
#include <QString>
#include <QVector>

struct InitialTowerPlacement {
    int row = 0;
    int col = 0;
    int towerType = 0;
};

struct LevelData {
    QString levelname;
    QString backgroundPath;

    int rows = 22;
    int cols = 40;
    int tileSize = 32;
    int initialGold = GameConstants::DefaultInitialGold;

    QVector<QString> mapData;
    QVector<QPoint> wayPoints;
    QVector<QVector<QPoint>> wayPointPaths;
    QVector<InitialTowerPlacement> initialTowers;
};