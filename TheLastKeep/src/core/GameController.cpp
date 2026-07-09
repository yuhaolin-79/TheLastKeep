/* 功能 游戏总控制器，核心调度类
 * 负责 Qtimer 主循环 开始游戏 暂停游戏
 *     恢复游戏 停止游戏 清空游戏 处理鼠标点击
 *     建塔请求 卡牌选择 波次推进 胜负判断
 *     更新 HUD
 */

#include "core/GameController.h"

GameController::GameController(QObject *parent)
    : QObject(parent),
    m_castleMaxHp(1000),   // 城堡初始总血量1000
    m_castleCurrentHp(1000),
    m_gold(200)            // 开局金币200
{

}

// 城堡受到伤害扣血
void GameController::damageCastle(int damage)
{
    m_castleCurrentHp -= damage;
    // 血量最低限制为0，不会出现负数
    if (m_castleCurrentHp < 0)
        m_castleCurrentHp = 0;
}

// 获取城堡当前剩余血量
int GameController::castleHp() const
{
    return m_castleCurrentHp;
}

// 增加金币（击杀敌人触发）
void GameController::addGold(int num)
{
    if (num > 0)
        m_gold += num;
}

// 消耗金币（建造塔触发）
void GameController::spendGold(int num)
{
    if (num > 0 && m_gold >= num)
        m_gold -= num;
}

// 判断金币是否足够建造塔
bool GameController::canBuildTower(int cost) const
{
    return m_gold >= cost;
}

// 获取当前金币
int GameController::getGold() const
{
    return m_gold;
}