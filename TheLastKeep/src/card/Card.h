/* 卡牌数据和工具函数
 * 封装
 * 卡牌类型 卡牌名称 卡牌描述 图标路径
 * @Chen 2026/07/10 version 1.0
 */
#ifndef CARD_H
#define CARD_H

#include <QString>

// 卡牌增益类型，和设计文档一一对应
enum class CardType{
    DamageUp,        // 塔伤害提升
    BuildCostDown,   // 建造塔降价
    GoldRewardUp,    // 击杀金币加成
    RangeUp          // 塔攻击范围扩大
};

// 单张卡牌完整信息结构体：包含卡面图、标题、描述、增益数值、图标
struct CardInfo{
    CardType type;
    // 卡牌顶部标题文本
    QString titleText;
    // 卡牌正文描述文本（卡面内说明文字）
    QString descText;
    // 增益数值
    float buffValue;
    // 卡牌背景卡面完整资源路径
    QString cardBackgroundPath;
    // 卡牌左上角小图标路径（伤害/金币/范围/折扣图标）
    QString cardIconPath;
};

// 全局Buff倍率状态，所有实体通过GameController读取
struct BuffState{
    float damageRate = 1.0f;        // 塔伤害倍率，初始1倍
    float buildCostRate = 1.0f;     // 建造价格倍率，小于1降价
    float goldRewardRate = 1.0f;    // 敌人击杀金币倍率
    float attackRangeRate = 1.0f;   // 塔攻击范围倍率
};

// 工具函数：选中卡牌后更新BuffState倍率
inline void applyCardBuff(BuffState& buff, CardType type){
    switch (type){
    case CardType::DamageUp:
        buff.damageRate += 0.15f;
        break;
    case CardType::BuildCostDown:
        buff.buildCostRate *= 0.90f;
        break;
    case CardType::GoldRewardUp:
        buff.goldRewardRate += 0.20f;
        break;
    case CardType::RangeUp:
        buff.attackRangeRate += 0.10f;
        break;
    default:
        break;
    }
}

#endif // CARD_H