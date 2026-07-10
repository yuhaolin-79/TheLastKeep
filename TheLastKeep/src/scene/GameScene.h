/* 功能 游戏画布
 * 负责 显示地图 显示 QGraphicsItem
 *     处理鼠标点击 把点击位置发给 GameController
 *     清理场景
 */

#pragma once

#include <QGraphicsScene>

#include "map/GameMap.h"

class QGraphicsSceneMouseEvent;

// GameScene：游戏画布
// 职责：
// 1. 显示地图
// 2. 显示 QGraphicsItem
// 3. 接收鼠标点击并转发
// 4. 清理场景
//
// 不负责：
// 1. 开始游戏按钮
// 2. 退出游戏按钮
// 3. 页面切换
// 4. QTimer
class GameScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit GameScene(QObject *parent = nullptr);

    void setupScene();
    void clearSceneSafely();

    void loadTutorialLevel();

signals:
    void sceneLeftClicked(const QPointF &scenePos);
    void sceneRightClicked(const QPointF &scenePos);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    void drawBackground();

private:
    GameMap m_map;
};
