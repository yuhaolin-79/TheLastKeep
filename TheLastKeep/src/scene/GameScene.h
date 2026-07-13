/* 功能 游戏画布
 * 负责 显示地图 显示 QGraphicsItem
 *     处理鼠标点击 把点击位置发给 GameController
 *     拖拽建塔时显示放置辅助层
 *     清理场景
 */

#pragma once

#include <QGraphicsScene>
#include <QList>

#include "map/GameMap.h"

class QGraphicsItem;
class QGraphicsSceneDragDropEvent;
class QGraphicsSceneMouseEvent;

class GameScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit GameScene(QObject *parent = nullptr);

    void setupScene();
    void clearSceneSafely();

    void loadTutorialLevel();
    QVector<QPointF> currentWayPoints() const;
    QPointF tileCenter(int row, int col) const;
    QPoint gridForScenePos(const QPointF& scenePos) const;
    bool canBuildAtGrid(const QPoint& gridPos) const;
    bool canBuildAtScenePos(const QPointF& scenePos) const;

public slots:
    void showPlacementHints();
    void hidePlacementHints();

signals:
    void sceneLeftClicked(const QPointF &scenePos);
    void sceneRightClicked(const QPointF &scenePos);
    void sceneTowerDropped(const QPointF &scenePos, int towerTypeValue);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

private:
    void drawBackground();
    void drawPlacementGrid();
    bool isTowerDrag(const QGraphicsSceneDragDropEvent *event) const;

private:
    GameMap m_map;
    QList<QGraphicsItem*> m_placementHintItems;
};