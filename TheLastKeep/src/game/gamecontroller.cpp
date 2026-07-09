#include "game/GameController.h"

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