/* 功能 游戏画布
 * 负责 显示地图 显示 QGraphicsItem
 *     处理鼠标点击 把点击位置发给 GameController
 *     拖拽建塔时显示放置辅助层
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
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSimpleTextItem>
#include <QMimeData>
#include <QPen>
#include <QPixmap>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent){
    setupScene();
}

void GameScene::setupScene()
{
    clear();
    m_placementHintItems.clear();

    setSceneRect(0,
                 0,
                 GameConstants::WindowWidth,
                 GameConstants::WindowHeight);

    drawBackground();
}

void GameScene::clearSceneSafely()
{
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
    LevelData tutorialLevel = LevelManager::createTutorialLevel();

    m_map.loadLevel(tutorialLevel);

    clear();
    m_placementHintItems.clear();

    setSceneRect(
        0,
        0,
        m_map.cols() * m_map.tileSize(),
        m_map.rows() * m_map.tileSize()
        );

    m_map.drawBackground(this);

    // 默认不显示任何建塔辅助层；只有拖拽塔进入地图时才显示网格和区域颜色。

    update();
}
QVector<QPointF> GameScene::currentWayPoints() const
{
    return m_map.getWayPoints();
}

QPointF GameScene::tileCenter(int row, int col) const
{
    return m_map.gridToSceneCenter(row, col);
}

QPoint GameScene::gridForScenePos(const QPointF& scenePos) const
{
    return m_map.sceneToGrid(scenePos);
}

bool GameScene::canBuildAtGrid(const QPoint& gridPos) const
{
    return m_map.isBuildable(gridPos.x(), gridPos.y());
}

bool GameScene::canBuildAtScenePos(const QPointF& scenePos) const
{
    return canBuildAtGrid(gridForScenePos(scenePos));
}

void GameScene::showPlacementHints()
{
    hidePlacementHints();
    drawPlacementGrid();
}

void GameScene::hidePlacementHints()
{
    for(QGraphicsItem* item : m_placementHintItems){
        removeItem(item);
        delete item;
    }
    m_placementHintItems.clear();
}

void GameScene::drawPlacementGrid()
{
    QPen gridPen(QColor(255, 255, 255, 82));
    gridPen.setWidth(1);

    for(int row = 0; row < m_map.rows(); ++row){
        for(int col = 0; col < m_map.cols(); ++col){
            QColor fillColor(20, 20, 20, 30);
            QPen cellPen = gridPen;

            if(m_map.isBuildable(row, col)){
                fillColor = QColor(65, 210, 120, 118);
                cellPen = QPen(QColor(255, 230, 130, 210), 2);
            } else if(m_map.isRoad(row, col)){
                fillColor = QColor(210, 80, 70, 72);
            }

            QGraphicsRectItem* cell = addRect(
                col * m_map.tileSize(),
                row * m_map.tileSize(),
                m_map.tileSize(),
                m_map.tileSize(),
                cellPen,
                QBrush(fillColor));
            cell->setZValue(60);
            m_placementHintItems.append(cell);
        }
    }
}

bool GameScene::isTowerDrag(const QGraphicsSceneDragDropEvent *event) const
{
    return event && event->mimeData()->hasFormat("application/x-thelastkeep-tower");
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        emit sceneRightClicked(event->scenePos());
    }

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
void GameScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if(isTowerDrag(event)){
        showPlacementHints();
        event->acceptProposedAction();
        return;
    }
    QGraphicsScene::dragEnterEvent(event);
}

void GameScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if(isTowerDrag(event)){
        event->acceptProposedAction();
        return;
    }
    QGraphicsScene::dragMoveEvent(event);
}

void GameScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    hidePlacementHints();
    QGraphicsScene::dragLeaveEvent(event);
}

void GameScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if(isTowerDrag(event)){
        bool ok = false;
        int towerType = QString::fromUtf8(event->mimeData()->data("application/x-thelastkeep-tower")).toInt(&ok);
        if(ok){
            emit sceneTowerDropped(event->scenePos(), towerType);
        }
        hidePlacementHints();
        event->acceptProposedAction();
        return;
    }
    QGraphicsScene::dropEvent(event);
}