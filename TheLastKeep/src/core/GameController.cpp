/* 功能 游戏总控制器，核心调度类
 * 负责 Qtimer 主循环 开始游戏 暂停游戏
 *     恢复游戏 停止游戏 清空游戏 处理鼠标点击
 *     建塔请求 卡牌选择 波次推进 胜负判断
 *     更新 HUD
 */

#include "core/GameController.h"

#include "scene/GameScene.h"
#include "common/GameConstants.h"
#include "core/BattleSystem.h"
#include "entity/Castle.h"
#include "entity/Tower.h"

#include <QDebug>
#include <QList>

GameController::GameController(GameScene *scene, QObject *parent)
    : QObject(parent),
    m_scene(scene),
    m_timer(new QTimer(this)),
    m_castleMaxHp(1000),
    m_castleCurrentHp(1000),
    m_gold(200),
    m_cardMgr(this)
{
    // QTimer 每触发一次 timeout 就调用 updateFrame()。
    // updateFrame 是当前游戏主循环入口，所有战斗系统和波次调度都从这里推进一帧。
    connect(m_timer, &QTimer::timeout, this, &GameController::updateFrame);

    // GameScene 只负责告诉控制器“玩家点了哪里”，是否能建塔、是否扣金币由控制器统一判断。
    if(m_scene){
        connect(m_scene, &GameScene::sceneTowerDropped,
                this, &GameController::handleTowerDropped);
    }
}

GameController::~GameController(){
    stopTimerSafely();
    clearBattleObjects();
}

bool GameController::loadLevel(int levelId){
    if(!m_scene){
        qWarning() << "GameController::loadLevel failed: scene is null";
        return false;
    }

    stopTimerSafely();
    clearBattleObjects();

    m_currentLevelId = levelId;
    m_scene->loadTutorialLevel();

    setupTutorialBattlePrototype();

    m_loaded = true;
    m_pausedByPageHidden = false;

    m_stateManager.setStatus(GameStatus::Paused);
    emit statusChanged(GameStatus::Paused);
    emitStatsChanged();

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
    emitStatsChanged();

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

void GameController::addGold(int num)
{
    if (num > 0){
        BuffState buff = m_cardMgr.getCurrentBuff();
        int finalGold = static_cast<int>(num*buff.goldRewardRate);
        m_gold+=finalGold;
        emitStatsChanged();
        qDebug() << "Gold +" << finalGold << "current:" << m_gold;
    }
}

void GameController::spendGold(int num)
{
    if (num > 0){
        BuffState buff = m_cardMgr.getCurrentBuff();
        int finalCost = static_cast<int>(num*buff.buildCostRate);
        if(m_gold >= finalCost){
            m_gold -= finalCost;
            emitStatsChanged();
        }
    }
}

bool GameController::canBuildTower(int cost) const
{
    BuffState buff = m_cardMgr.getCurrentBuff();
    int finalCost = static_cast<int>(cost * buff.buildCostRate);
    return m_gold >= finalCost;
}

void GameController::damageCastle(int damage)
{
    if (damage <= 0) return;
    m_castleCurrentHp -= damage;
    if (m_castleCurrentHp < 0) m_castleCurrentHp = 0;

    if(m_castle){
        m_castle->takeDamage(damage);
    }

    emitStatsChanged();
    qDebug() << "Castle damage:" << damage << "HP:" << m_castleCurrentHp << "/" << m_castleMaxHp;

    if (m_castleCurrentHp <= 0) {
        stopTimerSafely();
        m_stateManager.setStatus(GameStatus::Lose);
        emit statusChanged(GameStatus::Lose);
        emit gameFinished(false, 0);
    }
}

void GameController::pauseForPageHidden(){
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
    clearBattleObjects();

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
    if (!m_stateManager.canUpdateGame()) {
        return;
    }

    updateWaveSpawn();

    if(m_battleSystem){
        m_battleSystem->frameUpdate();
    }

    // 当前波已刷完且场上敌人清空，进入波间状态。
    if(m_battleSystem
        && !m_waveSpawning
        && !m_waitingForNextWave
        && m_spawnIndex >= m_pendingEnemies.size()
        && m_battleSystem->isWaveAllClear()){

        QVector<CardInfo> cards = waveFinishShowCard();
        qDebug() << "Wave" << m_currentWave << "clear, generated card choices:" << cards.size();

        if(m_currentWave >= m_totalWaves){
            stopTimerSafely();
            m_stateManager.setStatus(GameStatus::Win);
            emit statusChanged(GameStatus::Win);
            emit gameFinished(true, m_gold + m_castleCurrentHp);
            return;
        }

        m_waitingForNextWave = true;
        m_betweenWaveElapsedMs = 0;
    }
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

void GameController::setupTutorialBattlePrototype()
{
    if(!m_scene){
        return;
    }

    m_currentPath = m_scene->currentWayPoints();
    if(m_currentPath.isEmpty()){
        qWarning() << "Tutorial battle setup failed: empty path";
        return;
    }

    m_castleCurrentHp = m_castleMaxHp;
    m_gold = 260;
    m_currentWave = 0;
    m_spawnIndex = 0;
    m_spawnElapsedMs = 0;
    m_waveSpawning = false;
    m_waitingForNextWave = false;
    m_betweenWaveElapsedMs = 0;
    m_cardMgr.resetAllBuff();

    // 城堡放在路径最后一个点。Castle 内部用 offset 把贴图中心对齐到该坐标。
    m_castle = new Castle(m_currentPath.last(), this);
    m_scene->addItem(m_castle);

    m_battleSystem = new BattleSystem(this, m_castle, m_scene, this);

    // 防御塔不再预放置。玩家需要点击可建塔区域，用金币购买并放置默认箭塔。
    // 后续可以在 HUD 或塔盘中选择不同塔型，这里先完成购买和占位闭环。
    m_builtTowerGrids.clear();

    startNextWave();
}

void GameController::clearBattleObjects()
{
    if(m_battleSystem){
        m_battleSystem->clearAllBattleObjects();
        delete m_battleSystem;
        m_battleSystem = nullptr;
    }

    if(m_scene && m_castle && m_castle->scene() == m_scene){
        m_scene->removeItem(m_castle);
    }
    delete m_castle;
    m_castle = nullptr;

    m_currentPath.clear();
    m_pendingEnemies.clear();
    m_builtTowerGrids.clear();
}

void GameController::startNextWave()
{
    if(m_currentWave >= m_totalWaves){
        return;
    }

    ++m_currentWave;
    m_pendingEnemies = createWaveTypes(m_currentWave);
    m_spawnIndex = 0;
    m_spawnElapsedMs = m_spawnIntervalMs;
    m_waveSpawning = true;
    m_waitingForNextWave = false;
    m_betweenWaveElapsedMs = 0;

    emitStatsChanged();
    qDebug() << "Wave started:" << m_currentWave << "enemy count:" << m_pendingEnemies.size();
}

void GameController::updateWaveSpawn()
{
    if(m_waitingForNextWave){
        m_betweenWaveElapsedMs += GameConstants::FrameIntervalMs;
        if(m_betweenWaveElapsedMs >= m_betweenWaveDelayMs){
            startNextWave();
        }
        return;
    }

    if(!m_waveSpawning || !m_battleSystem || m_currentPath.isEmpty()){
        return;
    }

    m_spawnElapsedMs += GameConstants::FrameIntervalMs;
    if(m_spawnElapsedMs < m_spawnIntervalMs){
        return;
    }

    m_spawnElapsedMs = 0;

    if(m_spawnIndex >= m_pendingEnemies.size()){
        m_waveSpawning = false;
        return;
    }

    Enemy* enemy = new Enemy(m_currentPath, m_pendingEnemies[m_spawnIndex], this);
    enemy->setPos(m_currentPath.first() - QPointF(48.0, 0));
    m_battleSystem->spawnEnemy(enemy);
    ++m_spawnIndex;

    if(m_spawnIndex >= m_pendingEnemies.size()){
        m_waveSpawning = false;
    }
}

QVector<EnemyType> GameController::createWaveTypes(int wave) const
{
    if(wave <= 1){
        return {
            EnemyType::NormalGoblin,
            EnemyType::NormalGoblin,
            EnemyType::WolfRider,
            EnemyType::NormalGoblin,
            EnemyType::HeavyArmor
        };
    }

    if(wave == 2){
        return {
            EnemyType::NormalGoblin,
            EnemyType::WolfRider,
            EnemyType::HeavyArmor,
            EnemyType::Wizard,
            EnemyType::WolfRider,
            EnemyType::HeavyArmor
        };
    }

    return {
        EnemyType::WolfRider,
        EnemyType::HeavyArmor,
        EnemyType::Wizard,
        EnemyType::NormalGoblin,
        EnemyType::HeavyArmor,
        EnemyType::Boss
    };
}

void GameController::setSelectedTowerType(TowerType type)
{
    m_selectedTowerType = type;
    qDebug() << "Selected tower type changed" << static_cast<int>(type);
}
void GameController::handleTowerDropped(const QPointF& scenePos, int towerTypeValue)
{
    if(towerTypeValue < static_cast<int>(TowerType::ArrowTower)
        || towerTypeValue > static_cast<int>(TowerType::HolyTower)){
        qDebug() << "Build failed: invalid dropped tower type" << towerTypeValue;
        return;
    }

    m_selectedTowerType = static_cast<TowerType>(towerTypeValue);
    tryBuildTowerAt(scenePos);
}
void GameController::handleSceneLeftClicked(const QPointF& scenePos)
{
    if(!m_loaded || !m_battleSystem || !m_scene){
        return;
    }

    if(!m_stateManager.isRunning()){
        qDebug() << "Build ignored: game is not running";
        return;
    }

    tryBuildTowerAt(scenePos);
}

bool GameController::tryBuildTowerAt(const QPointF& scenePos)
{
    QPoint gridPos = m_scene->gridForScenePos(scenePos);

    if(!m_scene->canBuildAtGrid(gridPos)){
        qDebug() << "Build failed: tile is not buildable" << gridPos;
        return false;
    }

    if(m_builtTowerGrids.contains(gridPos)){
        qDebug() << "Build failed: tile already has tower" << gridPos;
        return false;
    }

    Tower* tower = new Tower(m_selectedTowerType,
                             m_scene->tileCenter(gridPos.x(), gridPos.y()),
                             this);

    int cost = tower->getBuildCost();
    if(!canBuildTower(cost)){
        qDebug() << "Build failed: not enough gold. cost:" << cost << "gold:" << m_gold;
        delete tower;
        return false;
    }

    spendGold(cost);
    m_battleSystem->addTower(tower);
    m_builtTowerGrids.append(gridPos);

    qDebug() << "Built tower at" << gridPos << "type:" << static_cast<int>(m_selectedTowerType) << "cost:" << cost;
    return true;
}
void GameController::emitStatsChanged()
{
    emit statsChanged(m_gold, m_castleCurrentHp, m_castleMaxHp, m_currentWave, m_totalWaves);
}

QVector<CardInfo> GameController::waveFinishShowCard(){
    return m_cardMgr.randomGenThreeCards();
}

void GameController::selectBuffCard(CardType type){
    m_cardMgr.selectOneCard(type);
}

BuffState GameController::getGlobalBuff()const{
    return m_cardMgr.getCurrentBuff();
}

void GameController::gameRestartReset(){
    m_castleCurrentHp = m_castleMaxHp;
    m_gold = 260;
    m_cardMgr.resetAllBuff();
    emitStatsChanged();
}