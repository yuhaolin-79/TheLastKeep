/* 功能 游戏总控制器，核心调度类
 * 负责 Qtimer 主循环 开始游戏 暂停游戏
 *     恢复游戏 停止游戏 清空游戏 处理鼠标点击
 *     建塔请求 卡牌选择 波次推进 胜负判断
 *     更新 HUD
 */


#pragma once

#include <QObject>
#include <QTimer>

#include "card/Card.h"
#include "card/CardManager.h"

#include "common/GameTypes.h"
#include "core/StateManager.h"

class GameScene;

// GameController：游戏总控制器
// @fish 2026/07/10 VERSSION 1.0
// 第一阶段职责：
// 1. 管理 QTimer
// 2. 管理游戏状态 Running / Paused / Menu
// 3. 加载关卡
// 4. 页面隐藏时暂停，页面显示时恢复
//
// MainWindow 不应该直接控制 QTimer。
// GameScene 不应该直接控制 QTimer。

class GameController : public QObject {
    Q_OBJECT

public:
    explicit GameController(GameScene *scene, QObject *parent = nullptr);
    ~GameController();

    // 加载关卡 VERSION 1.0 只支持 levelID=0 的 引导关卡
    bool loadLevel(int levelID);

    // 游戏生命周期接口 非常重要的部分
    void startGame();
    void pauseGame();
    void resumeGame();
    void stopGame();
    void clearGame();

    // QStackedWidget 接口
    void pauseForPageHidden();
    void resumeForPageShown();

    bool isRunning() const;
    GameStatus status() const;

    // 金币管理接口
    void addGold(int num);
    void spendGold(int num);
    bool canBuildTower(int cost) const;

signals:
    void statusChanged(GameStatus status);

    void gameFinished(bool win, int score);

public slots:
    void updateFrame();

    //卡牌buff系统接口
    QVector<CardInfo>waveFinishShowCard();
    void selectBuffCard(CardType type);
    BuffState getGlobalBuff()const;
    void gameRestartReset();

private:
    void startTimerSafely();
    void stopTimerSafely();

private:
    GameScene *m_scene = nullptr;

    QTimer *m_timer = nullptr;
    StateManager m_stateManager;

    int m_currentLevelId = -1;
    bool m_loaded = false;

    // 区分玩家主动暂停和页面隐藏导致暂停
    bool m_pausedByPageHidden = false;

    int m_castleMaxHp;    // 城堡最大血量
    int m_castleCurrentHp;// 城堡当前血量
    int m_gold;    // 当前金币
    CardManager m_cardMgr;
};
