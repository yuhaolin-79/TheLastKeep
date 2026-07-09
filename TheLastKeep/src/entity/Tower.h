/* 防御塔实体
 * 负责
 * 攻击范围 攻击间隔 攻击力 寻找目标
 * 生成子弹 应用 Buff
 *
 */



#ifndef TOWER_H
#define TOWER_H

#include <QGraphicsPixmapItem>
#include <QList>
#include <QDateTime>

class Enemy;
class Bullet;
class GameController;

enum class TowerType
{
    ArrowTower,
    MageTower,
    CannonTower,
    IceTower,
    HolyTower
};

class Tower : public QGraphicsPixmapItem
{
public:
    Tower(TowerType type, QPointF pos, GameController* ctrl);
    ~Tower() = default;

    // 每帧执行攻击逻辑，生成子弹返回
    Bullet* updateAttack(const QList<Enemy*>& enemyList);
    // 查找范围内最近敌人
    Enemy* findTarget(const QList<Enemy*>& enemyList);
    // 创建子弹指向目标敌人
    Bullet* createBullet(Enemy* target);

    int getBuildCost() const;
    int getAttackRange() const;
    int getDamage() const;
    int getAttackInterval() const;

private:
    void initTowerAttr(TowerType type);

private:
    TowerType m_type;
    int m_attackRange;
    int m_attackDamage;
    int m_attackInterval;
    int m_buildCost;
    qint64 m_lastAttackTime;
    GameController* m_controller;
};

#endif // TOWER_H