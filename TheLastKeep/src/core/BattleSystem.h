/* 战斗系统
 * 负责 生成敌人 更新敌人 更新塔 更新子弹 碰撞检测
 *     敌人到达城堡 敌人死亡奖励金币 波次是否结束
 *     战斗对象清理
 *      @chen 2026/07/10 version 1.0
 */
#ifndef BATTLESYSTEM_H
#define BATTLESYSTEM_H
#include <QList>
#include <QObject>

class GameController;
class QGraphicsScene;
class Tower;
class Enemy;
class Bullet;
class Castle;

class BattleSystem:public QObject{
    Q_OBJECT
public:
    // 全局控制器、城堡实体和场景指针都由 GameController 创建后传入。
    // BattleSystem 负责把战斗对象加入 scene，并在清理时从 scene 移除。
    explicit BattleSystem(GameController* gamectrl,
                          Castle* castle,
                          QGraphicsScene* scene,
                          QObject* parent = nullptr);
    // 每帧战斗总更新入口，主循环使用。
    void frameUpdate();
    // 外部接口：添加防御塔。
    void addTower(Tower* tower);
    // 外部接口：添加单个敌人，供波次系统按时间间隔刷怪。
    void spawnEnemy(Enemy* enemy);
    // 外部接口：生成一波敌人加入战斗列表。
    void spawnWaveEnemies(const QList<Enemy*>& waveEnemyGroup);
    // 判断当前波次敌人是否全部阵亡/抵达城堡（波次清空）。
    bool isWaveAllClear() const;
    int activeEnemyCount() const;
    // 清空战斗实体。
    void clearAllBattleObjects();

private:
    // 分模块更新逻辑。
    void updateAllTowers();
    void updateAllEnemies();
    void updateAllBullets();
    // 全局碰撞检测总逻辑。
    void runGlobalCollisionCheck();
    // 实时清理死亡敌人。
    void cleanDeadEnemies();
    void removeBulletAt(int index);
    void showHitEffect(const QPointF& pos, const QString& effectPath);

private:
    GameController* m_gameController = nullptr;
    Castle* m_mainCastle = nullptr;
    QGraphicsScene* m_scene = nullptr;
    QList<Tower*> m_towerContainer;
    QList<Enemy*> m_enemyContainer;
    QList<Bullet*> m_bulletContainer;
};

#endif