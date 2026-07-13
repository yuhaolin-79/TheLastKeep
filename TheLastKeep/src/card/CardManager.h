/* 卡牌管理器
 * 负责
 * 随机生成三张卡
 * 保证不重复
 * 应用卡牌效果
 * 保存全局 Buff
 * 重置 Buff
 * @Chen 2026/07/10 version 1.0
 */
#ifndef CARDMANAGER_H
#define CARDMANAGER_H

#include <QObject>
#include <QVector>
#include "Card.h"

extern QVector<CardInfo> createAllCardTemplateStatic();

class CardManager : public QObject{
    Q_OBJECT
public:
    explicit CardManager(QObject *parent = nullptr);

    // 波次结束调用：随机生成3张不重复类型卡牌，返回给UI弹窗渲染卡面+文字
    QVector<CardInfo> randomGenThreeCards();

    // 玩家选中卡牌，叠加本局全局Buff
    void selectOneCard(CardType cardType);

    // 对外获取当前所有Buff倍率，GameController/Tower读取
    BuffState getCurrentBuff() const;

    // 新游戏开局重置所有Buff倍率为初始1.0
    void resetAllBuff();

private:
    // 初始化全部4张卡牌模板数据（内置卡面路径、标题、描述文本）
    QVector<CardInfo> createAllCardTemplate() const;

private:
    BuffState m_globalBuff;              // 本局持续生效的全局Buff
    QVector<CardInfo> m_tempShowCards;   // 当前波次弹窗的3张卡牌缓存
};

#endif // CARDMANAGER_H