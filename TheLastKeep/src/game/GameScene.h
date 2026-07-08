#pragma once

#include <QGraphicsScene>

#include "game/GameMap.h"

class GameScene : public QGraphicsScene{
    Q_OBJECT

public:
    explicit GameScene(QObject *parent = nullptr);

private:
    void buildDemoMap();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    void loadTutorialLevel();

private:
    GameMap m_map;
};
