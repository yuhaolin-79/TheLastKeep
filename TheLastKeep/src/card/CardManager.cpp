/* 卡牌管理器
 * 负责
 * 随机生成三张卡
 * 保证不重复
 * 应用卡牌效果
 * 保存全局 Buff
 * 重置 Buff
 * @Chen 2026/07/10 version 1.0
 */
#include "CardManager.h"
#include <QRandomGenerator>

CardManager::CardManager(QObject *parent)
    : QObject(parent){
    resetAllBuff();
}

QVector<CardInfo> CardManager::createAllCardTemplate() const{
    return createAllCardTemplateStatic();
}

// 随机生成3张不重复卡牌
QVector<CardInfo> CardManager::randomGenThreeCards(){
    QVector<CardInfo> allTemplates = createAllCardTemplate();
    m_tempShowCards.clear();

    while (m_tempShowCards.size() < 3){
        int randIndex = QRandomGenerator::global()->bounded(allTemplates.size());
        CardInfo pick = allTemplates[randIndex];

        bool isRepeat = false;
        const QVector<CardInfo>& tempRef = m_tempShowCards;
        for (const auto& card : m_tempShowCards){
            if (card.type == pick.type){
                isRepeat = true;
                break;
            }
        }
        if (!isRepeat){
            m_tempShowCards.append(pick);
        }
    }
    return m_tempShowCards;
}

// 选中卡牌，更新全局Buff倍率
void CardManager::selectOneCard(CardType cardType){
    applyCardBuff(m_globalBuff, cardType);
    m_tempShowCards.clear();
}

// 获取当前全局Buff倍率
BuffState CardManager::getCurrentBuff() const{
    return m_globalBuff;
}

// 重置所有Buff至初始状态
void CardManager::resetAllBuff(){
    m_globalBuff.damageRate = 1.0f;
    m_globalBuff.buildCostRate = 1.0f;
    m_globalBuff.goldRewardRate = 1.0f;
    m_globalBuff.attackRangeRate = 1.0f;
    m_tempShowCards.clear();
}