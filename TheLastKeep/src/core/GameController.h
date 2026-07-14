/* 功能 游戏总控制器，核心调度类
 * 负责 Qtimer 主循环 开始游戏 暂停游戏
 *     恢复游戏 停止游戏 清空游戏 处理鼠标点击
 *     建塔请求 卡牌选择 波次推进 胜负判断
 *     更新 HUD
 */

#pragma once

#include <QObject>
#include <QTimer>
#include <QVector>
#include <QPointF>
#include <QPoint>

#include "card/Card.h"
#include "card/CardManager.h"

#include "common/GameTypes.h"
#include "core/StateManager.h"
#include "entity/Enemy.h"
#include "entity/Tower.h"
#include "level/LevelData.h"

class BattleSystem;
class Castle;
class GameScene;

// GameController：游戏总控制器
// @fish 2026/07/10 VERSSION 1.0
// 当前职责：
// 1. 管理 QTimer 和游戏状态
// 2. 加载关卡
// 3. 管理页面隐藏/显示时的暂停恢复
// 4. 持有 Castle + BattleSystem
// 5. 暂时承担轻量波次刷怪，后续可拆分为 WaveManager
class GameController : public QObject {
    Q_OBJECT

public:
    explicit GameController(GameScene *scene, QObject *parent = nullptr);
    ~GameController();

    bool loadLevel(int levelID);

    void startGame();
    void pauseGame();
    void resumeGame();
    void stopGame();
    void clearGame();

    void pauseForPageHidden();
    void resumeForPageShown();

    bool isRunning() const;
    GameStatus status() const;

    void addGold(int num);
    void spendGold(int num);
    bool canBuildTower(int cost) const;

    void damageCastle(int damage);
    void handleSceneLeftClicked(const QPointF& scenePos);
    void setSelectedTowerType(TowerType type);
    void handleTowerDropped(const QPointF& scenePos, int towerTypeValue);

signals:
    void statusChanged(GameStatus status);
    void statsChanged(int gold, int hp, int maxHp, int wave, int totalWave);
    void cardChoicesReady(const QVector<CardInfo> &cards);
    void gameFinished(bool win, int score);

public slots:
    void updateFrame();

    QVector<CardInfo>waveFinishShowCard();
    void selectBuffCard(CardType type);
    BuffState getGlobalBuff()const;
    void gameRestartReset();

private:
    void startTimerSafely();
    void stopTimerSafely();
    void setupBattleForLevel(const LevelData &levelData);
    void addInitialTower(const InitialTowerPlacement &placement);
    void clearBattleObjects();
    void startNextWave();
    void updateWaveSpawn();
    QVector<EnemyType> createWaveTypes(int wave) const;
    void emitStatsChanged();
    bool tryBuildTowerAt(const QPointF& scenePos);

private:
    GameScene *m_scene = nullptr;

    QTimer *m_timer = nullptr;
    StateManager m_stateManager;

    int m_currentLevelId = -1;
    bool m_loaded = false;
    bool m_pausedByPageHidden = false;

    int m_castleMaxHp;
    int m_castleCurrentHp;
    int m_gold;
    CardManager m_cardMgr;

    Castle* m_castle = nullptr;
    BattleSystem* m_battleSystem = nullptr;

    QVector<QVector<QPointF>> m_currentPaths;
    QVector<QPoint> m_builtTowerGrids;
    QVector<EnemyType> m_pendingEnemies;
    int m_totalWaves = 3;
    int m_currentWave = 0;
    int m_spawnIndex = 0;
    int m_spawnElapsedMs = 0;
    int m_spawnIntervalMs = 850;
    bool m_waveSpawning = false;
    bool m_waitingForCardSelection = false;
    bool m_updatingFrame = false;
    bool m_finishEmitted = false;
    TowerType m_selectedTowerType = TowerType::ArrowTower;
};