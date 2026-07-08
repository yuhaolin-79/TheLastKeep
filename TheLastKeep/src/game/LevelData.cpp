#include "game/GameMap.h"

#include <QBrush>
#include <QColor>
#include <QPen>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsSimpleTextItem>

GameMap::GameMap() {
}

void GameMap::loadLevel(const LevelData& levelData) {
    m_levelData = levelData;
}

void GameMap::drawBackground(QGraphicsScene* scene) {
    QPixmap background(m_levelData.backgroundPath);

    if (background.isNull()) {
        scene->addSimpleText("Background image load failed: " + m_levelData.backgroundPath);
        return;
    }

    QPixmap scaledBackground = background.scaled(
        m_levelData.cols * m_levelData.tileSize,
        m_levelData.rows * m_levelData.tileSize,
        Qt::IgnoreAspectRatio,
        Qt::FastTransformation
        );

    QGraphicsPixmapItem* backgroundItem = scene->addPixmap(scaledBackground);
    backgroundItem->setPos(0, 0);
    backgroundItem->setZValue(-100);
}

void GameMap::drawGrid(QGraphicsScene* scene) {
    QPen gridPen(QColor(255, 255, 255, 60));
    gridPen.setWidth(1);

    for (int row = 0; row < m_levelData.rows; ++row) {
        for (int col = 0; col < m_levelData.cols; ++col) {
            scene->addRect(
                col * m_levelData.tileSize,
                row * m_levelData.tileSize,
                m_levelData.tileSize,
                m_levelData.tileSize,
                gridPen,
                Qt::NoBrush
                );
        }
    }
}

void GameMap::drawDebugTiles(QGraphicsScene* scene) {
    for (int row = 0; row < m_levelData.rows; ++row) {
        for (int col = 0; col < m_levelData.cols; ++col) {
            TileType type = getTileType(row, col);

            QColor color;
            bool needDraw = true;

            switch (type) {
            case TileType::Road:
                color = QColor(180, 120, 60, 90);
                break;
            case TileType::Buildable:
                color = QColor(60, 180, 90, 100);
                break;
            case TileType::Start:
                color = QColor(0, 255, 255, 140);
                break;
            case TileType::End:
                color = QColor(255, 60, 60, 140);
                break;
            default:
                needDraw = false;
                break;
            }

            if (needDraw) {
                scene->addRect(
                    col * m_levelData.tileSize,
                    row * m_levelData.tileSize,
                    m_levelData.tileSize,
                    m_levelData.tileSize,
                    QPen(Qt::NoPen),
                    QBrush(color)
                    );
            }
        }
    }
}

void GameMap::drawWayPoints(QGraphicsScene* scene) {
    QPen pen(QColor(255, 220, 0));
    pen.setWidth(3);

    QBrush brush(QColor(255, 220, 0, 180));

    QVector<QPointF> points = getWayPoints();

    for (int i = 0; i < points.size(); ++i) {
        QPointF p = points[i];

        scene->addEllipse(
            p.x() - 6,
            p.y() - 6,
            12,
            12,
            pen,
            brush
            );

        if (i + 1 < points.size()) {
            scene->addLine(p.x(), p.y(), points[i + 1].x(), points[i + 1].y(), pen);
        }

        QGraphicsSimpleTextItem* text = scene->addSimpleText(QString::number(i));
        text->setBrush(QBrush(Qt::white));
        text->setPos(p.x() + 5, p.y() + 5);
        text->setZValue(50);
    }
}

QPoint GameMap::sceneToGrid(const QPointF& scenePos) const {
    int col = static_cast<int>(scenePos.x()) / m_levelData.tileSize;
    int row = static_cast<int>(scenePos.y()) / m_levelData.tileSize;

    return QPoint(row, col);
}

QPointF GameMap::gridToSceneCenter(int row, int col) const {
    return QPointF(
        col * m_levelData.tileSize + m_levelData.tileSize / 2.0,
        row * m_levelData.tileSize + m_levelData.tileSize / 2.0
        );
}

bool GameMap::isValidGrid(int row, int col) const {
    return row >= 0
           && row < m_levelData.rows
           && col >= 0
           && col < m_levelData.cols;
}

bool GameMap::isBuildable(int row, int col) const {
    return getTileType(row, col) == TileType::Buildable;
}

bool GameMap::isRoad(int row, int col) const {
    TileType type = getTileType(row, col);

    return type == TileType::Road
           || type == TileType::Start
           || type == TileType::End;
}

bool GameMap::isStart(int row, int col) const {
    return getTileType(row, col) == TileType::Start;
}

bool GameMap::isEnd(int row, int col) const {
    return getTileType(row, col) == TileType::End;
}

QVector<QPointF> GameMap::getWayPoints() const {
    QVector<QPointF> result;

    for (const QPoint& point : m_levelData.wayPoints) {
        int row = point.x();
        int col = point.y();

        result.push_back(gridToSceneCenter(row, col));
    }

    return result;
}

int GameMap::rows() const {
    return m_levelData.rows;
}

int GameMap::cols() const {
    return m_levelData.cols;
}

int GameMap::tileSize() const {
    return m_levelData.tileSize;
}

QString GameMap::levelName() const {
    return m_levelData.levelname;
}

TileType GameMap::getTileType(int row, int col) const {
    if (!isValidGrid(row, col)) {
        return TileType::Empty;
    }

    if (row >= m_levelData.mapData.size()) {
        return TileType::Empty;
    }

    if (col >= m_levelData.mapData[row].size()) {
        return TileType::Empty;
    }

    QChar ch = m_levelData.mapData[row][col];

    if (ch == 'R') return TileType::Road;
    if (ch == 'B') return TileType::Buildable;
    if (ch == 'S') return TileType::Start;
    if (ch == 'E') return TileType::End;

    return TileType::Empty;
}