/*
 * 卡牌数据和工具函数
 * 封装
 * 卡牌类型 卡牌名称 卡牌描述 图标路径
 * @Chen 2026/07/10 version 1.0
 */
#include "Card.h"
#include <QVector>

// 静态函数：创建4种卡牌基础模板，绑定卡面图片、标题、描述文本
QVector<CardInfo> createAllCardTemplateStatic(){
    QVector<CardInfo> templates;

    //伤害提升卡模板
    CardInfo damageCard;
    damageCard.type = CardType::DamageUp;
    damageCard.titleText = "强攻符文";
    damageCard.descText = "所有防御塔伤害永久提升15%";
    damageCard.buffValue = 0.15f;
    damageCard.cardBackgroundPath = ":/images/tower_buff.png";
    damageCard.cardIconPath = ":/images/tower_buff.png";
    templates.append(damageCard);

    //建造降价卡模板
    CardInfo costCard;
    costCard.type = CardType::BuildCostDown;
    costCard.titleText = "建材折扣";
    costCard.descText = "建造防御塔价格永久降低10%";
    costCard.buffValue = 0.90f;
    costCard.cardBackgroundPath = ":/images/coin_buff.png";
    costCard.cardIconPath = ":/images/coin_buff.png";
    templates.append(costCard);

    //金币加成卡模板
    CardInfo goldCard;
    goldCard.type = CardType::GoldRewardUp;
    goldCard.titleText = "淘金术";
    goldCard.descText = "击杀敌人获得金币永久提升20%";
    goldCard.buffValue = 0.20f;
    goldCard.cardBackgroundPath = ":/images/coin_buff.png";
    goldCard.cardIconPath = ":/images/coin_buff.png";
    templates.append(goldCard);

    //范围扩大卡模板
    CardInfo rangeCard;
    rangeCard.type = CardType::RangeUp;
    rangeCard.titleText = "望远透镜";
    rangeCard.descText = "所有防御塔攻击范围永久扩大10%";
    rangeCard.buffValue = 0.10f;
    rangeCard.cardBackgroundPath = ":/images/random_buff.png";
    rangeCard.cardIconPath = ":/images/random_buff.png";
    templates.append(rangeCard);

    return templates;
}