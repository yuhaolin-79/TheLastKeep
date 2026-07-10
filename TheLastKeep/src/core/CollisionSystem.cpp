/* 碰撞检测工具
 * 负责 判断子弹是否命中敌人
 *     判断敌人是否到达终点
 *     查找塔攻击范围内敌人
 * 可以考虑 QGraphicsItem::collidesWithItem()
 *  @chen 2026/07/10 version 1.0
 */

#include"CollisionSystem.h"
#include"entity/Tower.h"
#include"entity/Enemy.h"
#include"entity/Bullet.h"
#include"entity/Castle.h"
#include<QLineF>

bool CollisionSystem::isBulletHitEnemy(Bullet* bullet, Enemy* enemy){
    if(!bullet||!enemy)
        return false;
    return bullet->collidesWithItem(enemy);
}

bool CollisionSystem::enemyReachCastle(Enemy* enemy, Castle* castle){
    if(!enemy||!castle)
        return false;
    QLineF distanceLine(enemy->pos(), castle->pos());
    return distanceLine.length()<20.0;
}

QList<Enemy*> CollisionSystem::getEnemiesInTowerRange(Tower* tower, const QList<Enemy *> &allEnemies){
    QList<Enemy*> targetlist;
    if(!tower)
        return targetlist;
    //真实射程
    float realAttackRange=tower->getRealAttackRange();
    QPointF towerPos=tower->pos();

    for(Enemy* enemy:allEnemies){
        if(!enemy||enemy->isDead())
            continue;
        QLineF disLine(towerPos, enemy->pos());
        if(disLine.length()<realAttackRange){
            targetlist.append(enemy);
        }
    }
    return targetlist;
}