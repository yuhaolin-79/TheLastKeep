#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>

class GameController : public QObject
{
    Q_OBJECT
public:
    explicit GameController(QObject *parent = nullptr);

    // 城堡相关接口（给Castle调用）
    void damageCastle(int damage);
    int castleHp() const;

    // 金币系统接口（塔/敌人调用）
    void addGold(int num);
    void spendGold(int num);
    bool canBuildTower(int cost) const;
    int getGold() const;

private:
    int m_castleMaxHp;    // 城堡最大血量
    int m_castleCurrentHp;// 城堡当前血量
    int m_gold;           // 当前金币
};

#endif // GAMECONTROLLER_H