/* 战斗系统
 * 负责 生成敌人 更新敌人 更新塔 更新子弹 碰撞检测
 *     敌人到达城堡 敌人死亡奖励金币 波次是否结束
 *     战斗对象清理
 *      @chen 2026/07/10 version 1.0
 */
#ifndef BATTLESYSTEM_H
#define BATTLESYSTEM_H
#include<QList>
#include<QObject>

class GameController;
class Tower;
class Enemy;
class Bullet;
class Castle;

class BattleSystem:public QObject{
    Q_OBJECT
public:
    //全局控制器，城堡实体
    explicit BattleSystem(GameController* gamectrl, Castle* castle, QObject* parent = nullptr);
    //每帧战斗总更新入口，主循环使用
    void frameUpdate();
    //外部接口：添加防御塔
    void addTower(Tower* tower);
    //外部接口：生成一波敌人加入战斗列表
    void spawnWaveEnemies(const QList<Enemy*>& waveEnemyGroup);
    //判断当前波次敌人是否全部阵亡/抵达城堡（波次清空）
    bool isWaveAllClear() const;
    //清空战斗实体
    void clearAllBattleObjects();

private:
    //分模块更新逻辑
    void updateAllTowers();
    void updateAllEnemies();
    void updateAllBullets();
    //全局碰撞检测总逻辑
    void runGlobalCollisionCheck();
    //实时清理死亡敌人
    void cleanDeadEnemies();
    //成员变量
    GameController* m_gameController;
    Castle* m_mainCastle;
    QList<Tower*> m_towerContainer;
    QList<Enemy*> m_enemyContainer;
    QList<Bullet*> m_bulletContainer;
};

#endif