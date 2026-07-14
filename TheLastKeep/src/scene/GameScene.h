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

    void loadLevel(const LevelData &levelData);
    void loadTutorialLevel();
    QVector<QPointF> currentWayPoints() const;
    QVector<QVector<QPointF>> currentWayPointPaths() const;
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