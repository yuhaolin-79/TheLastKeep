/* 功能 地图系统
 * 负责
 * 地图格子
 * 地图绘制数据
 * scene 坐标 row / col 坐标 转换
 * 判断能否建塔
 * 路径点
 * 入口 城堡位置
 */

#pragma once

#include "level/LevelData.h"

#include <QGraphicsScene>
#include <QPoint>
#include <QPointF>
#include <QVector>
#include <QString>

// 格子类型
enum class TileType {
    Empty, // 普通区域，不允许建塔
    Road, // 道路，敌人从此过
    Buildable, // 可建塔区域
    Start, // 敌人出发区域
    End // 终点，城堡
};

class GameMap {
public:
    GameMap();

    void loadLevel(const LevelData& levelData);

    void drawBackground(QGraphicsScene* scene);
    void drawGrid(QGraphicsScene* scene);
    void drawDebugTiles(QGraphicsScene* scene);
    void drawWayPoints(QGraphicsScene* scene);
    void drawBuildableHints(QGraphicsScene* scene);

    QPoint sceneToGrid(const QPointF& scenePos) const;
    QPointF gridToSceneCenter(int row, int col) const;

    bool isValidGrid(int row, int col) const;
    bool isBuildable(int row, int col) const;
    bool isRoad(int row, int col) const;
    bool isStart(int row, int col) const;
    bool isEnd(int row, int col) const;

    QVector<QPointF> getWayPoints() const;
    QVector<QVector<QPointF>> getWayPointPaths() const;

    int rows() const;
    int cols() const;
    int tileSize() const;
    QString levelName() const;

private:
    TileType getTileType(int row, int col) const;

private:
    LevelData m_levelData;
};