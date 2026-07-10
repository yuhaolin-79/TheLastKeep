/* 碰撞检测工具
 * 负责 判断子弹是否命中敌人
 *     判断敌人是否到达终点
 *     查找塔攻击范围内敌人
 * 可以考虑 QGraphicsItem::collidesWithItem()
 *  @chen 2026/07/10 version 1.0
 */
#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H
#include<QList>
#include<QPointF>

class Tower;
class Enemy;
class Bullet;
class Castle;

class CollisionSystem{
public:
    static bool isBulletHitEnemy(Bullet* bullet, Enemy* enemy);
    static bool enemyReachCastle(Enemy* enemy, Castle*  castle);
    static QList<Enemy*> getEnemiesInTowerRange(Tower* tower, const QList<Enemy*>& allEnemies);
};

#endif