/* 功能 游戏画布
 * 负责 显示地图 显示 QGraphicsItem
 *     处理鼠标点击 把点击位置发给 GameController
 *     清理场景
 */

#include "scene/GameScene.h"
#include "level/LevelManager.h"

#include <QBrush>
#include <QColor>
#include <QDebug>
#include <QFile>
#include <QFont>
#include <QPen>
#include <QGraphicsSimpleTextItem>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QPushButton>
#include <QGraphicsProxyWidget>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent){
    setSceneRect(0, 0, 1280, 720);

    QPixmap background(":/images/background.png");

    QPixmap scaledBackground = background.scaled(
        1280,
        720,
        Qt::IgnoreAspectRatio,
        Qt::FastTransformation
        );

    QGraphicsPixmapItem *backgroundItem = addPixmap(scaledBackground);
    backgroundItem->setPos(0, 0);
    backgroundItem->setZValue(-100);

    // 创建开始游戏、退出游戏按钮
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
    QGraphicsProxyWidget* proxy = this->addWidget(btn);
    proxy->setPos(x, y);
    proxy->setZValue(10);
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

void GameScene::loadTutorialLevel() {
    LevelData tutorialLevel = LevelManager::createTutorialLevel();

    m_map.loadLevel(tutorialLevel);

    clear();

    setSceneRect(
        0,
        0,
        m_map.cols() * m_map.tileSize(),
        m_map.rows() * m_map.tileSize()
        );

    m_map.drawBackground(this);

    // 调试阶段建议都打开
    m_map.drawDebugTiles(this);
    m_map.drawGrid(this);
    m_map.drawWayPoints(this);

    // 强制刷新场景显示
    update();
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QPoint gridPos = m_map.sceneToGrid(event->scenePos());

    int row = gridPos.x();
    int col = gridPos.y();

    qDebug() << "Clicked scene pos:" << event->scenePos()
             << "grid row:" << row
             << "col:" << col;

    if (m_map.isBuildable(row, col)) {
        qDebug() << "Buildable tile.";
    } else if (m_map.isRoad(row, col)) {
        qDebug() << "Road tile.";
    } else if (m_map.isEnd(row, col)) {
        qDebug() << "Castle end tile.";
    } else {
        qDebug() << "Empty tile.";
    }

    QGraphicsScene::mousePressEvent(event);
}
