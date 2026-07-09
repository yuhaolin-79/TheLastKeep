/* 功能 游戏画布
 * 负责 显示地图 显示 QGraphicsItem
 *     处理鼠标点击 把点击位置发给 GameController
 *     清理场景
 */

#pragma once

#include <QGraphicsScene>

#include "map/GameMap.h"

class QPushButton;
class QGraphicsProxyWidget;

class GameScene : public QGraphicsScene{
    Q_OBJECT

public:
    explicit GameScene(QObject *parent = nullptr);

    // 加载教程关卡（由 MainWindow 在"开始游戏"时调用）
    void loadTutorialLevel();

signals:
    void sigStartGame();
    void sigQuitGame();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    void buildDemoMap();
    QGraphicsProxyWidget* createBtn(const QString& text, int x, int y, int w, int h);

private:
    GameMap m_map;
};
