#pragma once

#include <QGraphicsScene>

class GameScene : public QGraphicsScene{
    Q_OBJECT

public:
    explicit GameScene(QObject *parent = nullptr);

private:
    void buildDemoMap();
};
