#pragma once

#include <QGraphicsScene>

#include "game/GameMap.h"

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
