/* 敌人实体
 * 负责
 * 沿路径移动 保存血量 受到伤害
 * 死亡判断 到达城堡判断 奖励金币
 * 分数价值
 */


#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPixmapItem>
#include <QVector>
#include <QPointF>

// 前置声明
class GameController;
class Bullet;

// 敌人类型枚举
enum class EnemyType
{
    NormalGoblin,
    HeavyArmor,
    WolfRider,
    Wizard,
    Boss
};

class Enemy : public QGraphicsPixmapItem
{
public:
    Enemy(const QVector<QPointF>& path, EnemyType type, GameController* ctrl);

    // 每帧移动更新
    void updateMove();
    // 受到伤害
    void takeDamage(int damage);
    // 判断是否死亡
    bool isDead() const;
    // 是否走到城堡终点
    bool reachedCastle() const;
    // 获取击杀奖励金币
    int getReward() const;

private:
    // 根据类型初始化血量、速度、奖励、贴图
    void initEnemyAttr(EnemyType type);

private:
    QVector<QPointF> m_path;        // 移动路径点
    int m_currentTargetIdx;         // 当前目标路径下标
    int m_hp;
    int m_maxHp;
    int m_speed;
    int m_rewardGold;
    GameController* m_controller;
};

#endif // ENEMY_H