#include "Tower.h"
#include "Enemy.h"
#include "Bullet.h"
#include "GameController.h"
#include <QPixmap>
#include <QLineF>

Tower::Tower(TowerType type, QPointF pos, GameController *ctrl)
    : QGraphicsPixmapItem(nullptr),
    m_type(type),
    m_controller(ctrl),
    m_lastAttackTime(0)
{
    setPos(pos);
    initTowerAttr(type);
}

void Tower::initTowerAttr(TowerType type)
{
    switch (type)
    {
    case TowerType::ArrowTower:
        m_attackRange = 160;
        m_attackDamage = 25;
        m_attackInterval = 800;
        m_buildCost = 50;
        setPixmap(QPixmap(":/assets/images/towers/arrow.png"));
        break;
    case TowerType::MageTower:
        m_attackRange = 140;
        m_attackDamage = 60;
        m_attackInterval = 1200;
        m_buildCost = 100;
        setPixmap(QPixmap(":/assets/images/towers/mage.png"));
        break;
    case TowerType::CannonTower:
        m_attackRange = 180;
        m_attackDamage = 100;
        m_attackInterval = 1800;
        m_buildCost = 150;
        setPixmap(QPixmap(":/assets/images/towers/cannon.png"));
        break;
    case TowerType::IceTower:
        m_attackRange = 150;
        m_attackDamage = 15;
        m_attackInterval = 600;
        m_buildCost = 80;
        setPixmap(QPixmap(":/assets/images/towers/ice.png"));
        break;
    case TowerType::HolyTower:
        m_attackRange = 170;
        m_attackDamage = 40;
        m_attackInterval = 900;
        m_buildCost = 120;
        setPixmap(QPixmap(":/assets/images/towers/holy.png"));
        break;
    }
}

Bullet* Tower::updateAttack(const QList<Enemy *> &enemyList)
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    if (now - m_lastAttackTime < m_attackInterval)
        return nullptr;

    Enemy* target = findTarget(enemyList);
    if (!target)
        return nullptr;

    m_lastAttackTime = now;
    return createBullet(target);
}

Enemy* Tower::findTarget(const QList<Enemy *> &enemyList)
{
    Enemy* nearest = nullptr;
    qreal minDis = m_attackRange;

    for (Enemy* e : enemyList)
    {
        if (e->isDead())
            continue;
        QLineF disLine(pos(), e->pos());
        qreal dis = disLine.length();
        if (dis < minDis)
        {
            minDis = dis;
            nearest = e;
        }
    }
    return nearest;
}

Bullet* Tower::createBullet(Enemy *target)
{
    return new Bullet(this->pos(), target, m_attackDamage);
}

int Tower::getBuildCost() const
{
    return m_buildCost;
}

int Tower::getAttackRange() const
{
    return m_attackRange;
}

int Tower::getDamage() const
{
    return m_attackDamage;
}

int Tower::getAttackInterval() const
{
    return m_attackInterval;
}