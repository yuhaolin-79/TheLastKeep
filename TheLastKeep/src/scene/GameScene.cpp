/* 功能 游戏画布
 * 负责 显示地图 显示 QGraphicsItem
 *     处理鼠标点击 把点击位置发给 GameController
 *     清理场景
 */

#include "scene/GameScene.h"
#include "level/LevelManager.h"
#include "common/GameConstants.h"

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
    setupScene();
}

void GameScene::setupScene()
{
    clear();

    setSceneRect(0,
                 0,
                 GameConstants::WindowWidth,
                 GameConstants::WindowHeight);

    drawBackground();
}

void GameScene::clearSceneSafely()
{
    // 注意：
    // 调用这个函数之前，GameController 必须已经 stopTimerSafely()
    setupScene();
}

void GameScene::drawBackground()
{
    QPixmap background(":/images/background.png");

    if (background.isNull()) {
        qWarning() << "Background image load failed: :/images/background.png";
        setBackgroundBrush(QBrush(QColor("#1E1E1E")));
        return;
    }

    QPixmap scaledBackground = background.scaled(
        GameConstants::WindowWidth,
        GameConstants::WindowHeight,
        Qt::IgnoreAspectRatio,
        Qt::FastTransformation
        );

    QGraphicsPixmapItem *backgroundItem = addPixmap(scaledBackground);
    backgroundItem->setPos(0, 0);
    backgroundItem->setZValue(-100);
}

void GameScene::loadTutorialLevel()
{
    // 当前保留你已有的教程关卡加载方式
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

    // 调试阶段建议保留
    m_map.drawDebugTiles(this);
    m_map.drawGrid(this);
    m_map.drawWayPoints(this);

    update();
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit sceneLeftClicked(event->scenePos());
    } else if (event->button() == Qt::RightButton) {
        emit sceneRightClicked(event->scenePos());
    }

    // 当前阶段保留调试输出，方便验证地图坐标
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
