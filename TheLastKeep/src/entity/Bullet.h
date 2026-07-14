/* 子弹实体
 * 负责
 * 追踪目标 移动 命中判断 造成伤害
 * 标记是否删除
 *
 */



#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>
#include <QPointF>

class Enemy;
class Tower;

class Bullet : public QGraphicsPixmapItem
{
public:
    Bullet(QPointF startPos, Enemy* target, int damage,Tower* shootTower);

    void updateMove();
    bool hitTarget() const;
    bool hasLiveTarget() const;
    int getDamage() const;
    Tower* getShootTower()const;
    Enemy* targetEnemy() const;

private:
    Enemy* m_targetEnemy;
    int m_damage;
    int m_speed;
    Tower* m_shootTower;
};

#endif // BULLET_H