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

class Bullet : public QGraphicsPixmapItem
{
public:
    Bullet(QPointF startPos, Enemy* target, int damage);

    void updateMove();
    bool hitTarget() const;
    int getDamage() const;

private:
    Enemy* m_targetEnemy;
    int m_damage;
    int m_speed;
};

#endif // BULLET_H
