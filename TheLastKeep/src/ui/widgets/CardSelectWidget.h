/* 功能 每波结束后的卡牌选择界面
 * 负责显示三张卡
 * 玩家点一张后发信号
 * 不负责修改 buff，该部分由 CardManager 实现
 */

#pragma once

#include "card/Card.h"

#include <QVector>
#include <QWidget>

class CardChoiceButton;

class CardSelectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CardSelectWidget(QWidget *parent = nullptr);

    void setCards(const QVector<CardInfo> &cards);

signals:
    void cardSelected(CardType type);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<CardChoiceButton *> m_cardButtons;
    bool m_selectionLocked = false;
};