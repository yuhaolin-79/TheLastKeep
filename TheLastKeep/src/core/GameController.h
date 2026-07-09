/* 功能 游戏总控制器，核心调度类
 * 负责 Qtimer 主循环 开始游戏 暂停游戏
 *     恢复游戏 停止游戏 清空游戏 处理鼠标点击
 *     建塔请求 卡牌选择 波次推进 胜负判断
 *     更新 HUD
 */


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