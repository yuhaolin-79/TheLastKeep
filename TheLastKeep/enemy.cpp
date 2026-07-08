#include "Enemy.h"
#include "GameController.h"
#include <QPixmap>
#include <QLineF>

Enemy::Enemy(const QVector<QPointF> &path, EnemyType type, GameController *ctrl)
    : QGraphicsPixmapItem(nullptr),
    m_path(path),
    m_currentTargetIdx(0),
    m_controller(ctrl)
{
    initEnemyAttr(type);
    // 生成在路径第一个点
    if (!m_path.isEmpty())
    {
        setPos(m_path.first());
    }
}

void Enemy::initEnemyAttr(EnemyType type)
{
    switch (type)
    {
    case EnemyType::NormalGoblin:
        m_maxHp = 100;
        m_speed = 2;
        m_rewardGold = 10;
        setPixmap(QPixmap(":/assets/images/enemies/goblin.png"));
        break;
    case EnemyType::HeavyArmor:
        m_maxHp = 300;
        m_speed = 1;
        m_rewardGold = 25;
        setPixmap(QPixmap(":/assets/images/enemies/heavy.png"));
        break;
    case EnemyType::WolfRider:
        m_maxHp = 60;
        m_speed = 4;
        m_rewardGold = 15;
        setPixmap(QPixmap(":/assets/images/enemies/wolf.png"));
        break;
    case EnemyType::Wizard:
        m_maxHp = 120;
        m_speed = 1.5;
        m_rewardGold = 20;
        setPixmap(QPixmap(":/assets/images/enemies/wizard.png"));
        break;
    case EnemyType::Boss:
        m_maxHp = 1000;
        m_speed = 0.8;
        m_rewardGold = 200;
        setPixmap(QPixmap(":/assets/images/enemies/boss.png"));
        break;
    }
    m_hp = m_maxHp;
}

void Enemy::updateMove()
{
    // 死亡或路径走完不移动
    if (isDead() || m_currentTargetIdx >= m_path.size())
        return;

    QPointF target = m_path[m_currentTargetIdx];
    QLineF line(pos(), target);
    qreal dist = line.length();

    if (dist < m_speed)
    {
        setPos(target);
        m_currentTargetIdx++;
    }
    else
    {
        QPointF dir = line.unitVector().p2() - line.unitVector().p1();
        setPos(pos() + dir * m_speed);
    }
}

void Enemy::takeDamage(int damage)
{
    m_hp -= damage;
    // 敌人死亡发放金币
    if (m_hp <= 0 && m_controller)
    {
        m_controller->addGold(m_rewardGold);
    }
}

bool Enemy::isDead() const
{
    return m_hp <= 0;
}

bool Enemy::reachedCastle() const
{
    return m_currentTargetIdx >= m_path.size();
}

int Enemy::getReward() const
{
    return m_rewardGold;
}