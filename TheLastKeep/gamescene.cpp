#include "GameScene.h"

#include <QBrush>
#include <QColor>
#include <QDebug>
#include <QFile>
#include <QFont>
#include <QPen>
#include <QGraphicsSimpleTextItem>
#include <QPixmap>
#include <QGraphicsPixmapItem>
// 新增按钮相关头文件
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QFontMetrics>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent){
    setSceneRect(0, 0, 1280, 720);

    QPixmap background(":/res/image.png");

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

    //创建开始游戏、退出游戏按钮
    QGraphicsProxyWidget* btnStart = createBtn("开始游戏", 980, 240, 200, 60);
    QGraphicsProxyWidget* btnQuit  = createBtn("退出游戏", 980, 330, 200, 60);

    // 绑定按钮点击信号到自定义信号，传给主窗口处理
    QPushButton* startBtn = qobject_cast<QPushButton*>(btnStart->widget());
    QPushButton* quitBtn  = qobject_cast<QPushButton*>(btnQuit->widget());
    connect(startBtn, &QPushButton::clicked, this, &GameScene::sigStartGame);
    connect(quitBtn,  &QPushButton::clicked, this, &GameScene::sigQuitGame);
}

// 封装：统一创建游戏按钮，美化样式
QGraphicsProxyWidget* GameScene::createBtn(const QString& text, int x, int y, int w, int h)
{
    QPushButton* btn = new QPushButton(text);
    // 按钮尺寸、字体、古风样式适配城堡背景
    btn->setFixedSize(w, h);
    btn->setFont(QFont("Microsoft YaHei", 16, QFont::Bold));
    btn->setStyleSheet(R"(
        QPushButton{
            background-color: #4A5A70;
            color: #F2E9DB;
            border: 2px solid #8A9CB3;
            border-radius: 8px;
        }
        QPushButton:hover{
            background-color: #60738C;
        }
        QPushButton:pressed{
            background-color: #39485A;
        }
    )");
    // 放入场景代理控件
    QGraphicsProxyWidget* proxy = this->addWidget(btn);
    proxy->setPos(x, y);
    proxy->setZValue(10); // 按钮层级高于背景，不会被遮挡
    return proxy;
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