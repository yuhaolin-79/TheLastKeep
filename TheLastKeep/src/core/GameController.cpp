/* 功能 游戏总控制器，核心调度类
 * 负责 Qtimer 主循环 开始游戏 暂停游戏
 *     恢复游戏 停止游戏 清空游戏 处理鼠标点击
 *     建塔请求 卡牌选择 波次推进 胜负判断
 *     更新 HUD
 */

#include "core/GameController.h"

#include "scene/GameScene.h"
#include "common/GameConstants.h"

#include <QDebug>

GameController::GameController(GameScene *scene, QObject *parent)
    : QObject(parent),
    m_scene(scene),
    m_timer(new QTimer(this))
{
    // Qtimer 每触发一次 timeout 就调用 updateFrame()
    connect(m_timer, &QTimer::timeout, this, &GameController::updateFrame);
}

GameController::~GameController(){
    stopTimerSafely();
}

bool GameController::loadLevel(int levelId){
    if(!m_scene){
        qWarning() << "GameController::loadLevel failed: scene is null";
        return false;
    }

    stopTimerSafely();

    m_currentLevelId = levelId;

    // 当前阶段 VERSION 1.0 ，先默认加载 tutorial
    // 后面需要接 LevelManager::loadLevel(levelId)
    m_scene->loadTutorialLevel();

    m_loaded = true;
    m_pausedByPageHidden = false;

    // 加载完还没开始跑，先设置为 Paused
    m_stateManager.setStatus(GameStatus::Paused);
    emit statusChanged(GameStatus::Paused);

    qDebug() << "Level loaded:" << levelId;

    return true;
}

void GameController::startGame() {
    if(!m_loaded) {
        qWarning() << "GameController::startGame failed: level not loaded";
        return;
    }

    m_stateManager.setStatus(GameStatus::Running);
    m_pausedByPageHidden = false;

    startTimerSafely();

    emit statusChanged(GameStatus::Running);

    qDebug() << "Game started";
}

void GameController::pauseGame(){
    if(!m_stateManager.isRunning()){
        return;
    }

    stopTimerSafely();

    m_stateManager.setStatus((GameStatus::Paused));
    m_pausedByPageHidden = false;

    emit statusChanged(GameStatus::Paused);

    qDebug() << "Game paused by user";
}

void GameController::resumeGame(){
    if(!m_loaded){
        return;
    }

    if(!m_stateManager.isPaused()){
        return;
    }

    m_stateManager.setStatus(GameStatus::Running);
    m_pausedByPageHidden = false;

    startTimerSafely();

    emit statusChanged(GameStatus::Running);

    qDebug() << "Game resumed by user";
}

void GameController::pauseForPageHidden(){
    // 只有游戏正在运行，页面隐藏才需要暂停
    // 如果本来是Paused 不用设置m_pausedByPageHidden

    if(!m_stateManager.isRunning()){
        return;
    }

    stopTimerSafely();

    m_stateManager.setStatus(GameStatus::Paused);
    m_pausedByPageHidden = true;

    emit statusChanged(GameStatus::Paused);

    qDebug() << "Game paused by GamePageHidden";
}

void GameController::resumeForPageShown(){
    // 只有因为页面隐藏而暂停，才自动恢复

    if(!m_pausedByPageHidden){
        return;
    }

    if(!m_loaded){
        return;
    }

    m_pausedByPageHidden = false;
    m_stateManager.setStatus(GameStatus::Running);

    startTimerSafely();

    emit statusChanged(GameStatus::Running);

    qDebug() << "Game resumed by GamePageShown";
}

void GameController::stopGame(){
    stopTimerSafely();

    m_stateManager.setStatus(GameStatus::Menu);
    m_pausedByPageHidden = false;

    emit statusChanged(GameStatus::Menu);

    qDebug() << "Game stopped";
}

void GameController::clearGame(){
    stopTimerSafely();

    if(m_scene){
        m_scene->clearSceneSafely();
    }

    m_loaded = false;
    m_currentLevelId = -1;
    m_pausedByPageHidden = false;

    m_stateManager.setStatus(GameStatus::Menu);
    emit statusChanged(GameStatus::Menu);

    qDebug() << "Game cleared";
}

bool GameController::isRunning() const
{
    return m_stateManager.isRunning();
}

GameStatus GameController::status() const
{
    return m_stateManager.status();
}

void GameController::updateFrame()
{
    // 防御性判断：即使 timeout 误触发，非 Running 状态也不更新游戏
    if (!m_stateManager.canUpdateGame()) {
        return;
    }

    // VERSION 1.0 先只打印，证明 QTimer 正在运行
    // 后面改成：
    // m_battleSystem->update(GameConstants::FrameIntervalMs);
    qDebug() << "Game tick";

    // 后面胜负判断也放这里
}

void GameController::startTimerSafely()
{
    if (!m_timer) {
        return;
    }

    if (!m_timer->isActive()) {
        m_timer->start(GameConstants::FrameIntervalMs);
    }
}

void GameController::stopTimerSafely()
{
    if (!m_timer) {
        return;
    }

    if (m_timer->isActive()) {
        m_timer->stop();
    }
}


