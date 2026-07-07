#include "game/GameScene.h"

#include <QBrush>
#include <QColor>
#include <QDebug>
#include <QFile>
#include <QFont>
#include <QPen>
#include <QGraphicsSimpleTextItem>
#include <QPixmap>
#include <QGraphicsPixmapItem>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent){
    setSceneRect(0, 0, 1280, 720);

    QPixmap background(":/images/background.png");

    // qDebug() << "file exists:" << QFile::exists(path);

    // if(background.isNull()){
    //     addSimpleText("Background image load failed!!!");
    //     return;
    // }

    QPixmap scaledBackground = background.scaled(
        1280,
        720,
        Qt::IgnoreAspectRatio,
        Qt::FastTransformation
        );

    QGraphicsPixmapItem *backgroundItem = addPixmap(scaledBackground);
    backgroundItem->setPos(0, 0);
    backgroundItem->setZValue(-100);

    //buildDemoMap();
}

void GameScene::buildDemoMap(){
    auto *title = addSimpleText("The Last Keep");
    title->setBrush(QColor("#F2E9DB"));
    title->setFont(QFont("Microsoft YaHei", 28, QFont::Bold));
    title->setPos(40, 30);

    QPen pathPen(QColor("#D2B48C"));
    pathPen.setWidth(28);
    pathPen.setCapStyle(Qt::RoundCap);

    addLine(80, 560, 360, 560, pathPen);
    addLine(360, 560, 360, 330, pathPen);
    addLine(360, 330, 760, 330, pathPen);
    addLine(760, 330, 760, 500, pathPen);
    addLine(760, 500, 1120, 500, pathPen);

    QPen towerPen(QColor("#F2E9D8"));
    towerPen.setWidth(3);

    addRect(260, 410, 72, 72, towerPen, QBrush(QColor("#7B8FA1")));
    addRect(520, 210, 72, 72, towerPen, QBrush(QColor("#7B8FA1")));
    addRect(900, 380, 72, 72, towerPen, QBrush(QColor("#7B8FA1")));

    addEllipse(92, 542, 36, 36, QPen(QColor("#222222")), QBrush(QColor("#B84A4A")));
}