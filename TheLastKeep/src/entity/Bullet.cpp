/* 子弹实体
 * 负责
 * 追踪目标 移动 命中判断 造成伤害
 * 标记是否删除
 *
 */

#include "entity/Bullet.h"
#include "entity/Enemy.h"
#include <QPixmap>
#include <QLineF>

// Bullet::Bullet(QPointF startPos, Enemy *target, int damage)
//     : QGraphicsPixmapItem(nullptr),
//     m_targetEnemy(target),
//     m_damage(damage),
//     m_speed(6)
// {
//     setPos(startPos);
//     setPixmap(QPixmap(":/assets/images/bullets/arrow.png"));
// }

// void Bullet::updateMove()
// {
//     // 目标死亡直接失效
//     if (!m_targetEnemy || m_targetEnemy->isDead())
//         return;

//     QPointF targetPos = m_targetEnemy->pos();
//     QLineF line(pos(), targetPos);
//     qreal dist = line.length();

//     // 距离足够近，命中敌人
//     if (dist < 8)
//     {
//         m_targetEnemy->takeDamage(m_damage);
//         return;
//     }

//     // 向敌人移动
//     QPointF dir = line.unitVector().p2() - line.unitVector().p1();
//     setPos(pos() + dir * m_speed);
// }

// bool Bullet::hitTarget() const
// {
//     if (!m_targetEnemy || m_targetEnemy->isDead())
//         return true;
//     QLineF line(pos(), m_targetEnemy->pos());
//     return line.length() < 8;
// }

// int Bullet::getDamage() const
// {
//     return m_damage;
// }