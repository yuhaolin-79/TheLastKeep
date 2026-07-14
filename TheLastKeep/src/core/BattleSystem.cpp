/* 战斗系统
 * 负责 生成敌人 更新敌人 更新塔 更新子弹 碰撞检测
 *     敌人到达城堡 敌人死亡奖励金币 波次是否结束
 *     战斗对象清理
 *      @chen 2026/07/10 version 1.0
 */
#include "core/BattleSystem.h"
#include "core/CollisionSystem.h"
#include "core/GameController.h"
#include "entity/Tower.h"
#include "entity/Enemy.h"
#include "entity/Bullet.h"
#include "entity/Castle.h"

#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPixmap>
#include <QTimer>

BattleSystem::BattleSystem(GameController* gameCtrl,
                           Castle* castle,
                           QGraphicsScene* scene,
                           QObject* parent)
    : QObject(parent),
    m_gameController(gameCtrl),
    m_mainCastle(castle),
    m_scene(scene)
{
}

void BattleSystem::frameUpdate(){
    updateAllTowers();
    updateAllEnemies();

    if (!m_gameController || !m_gameController->isRunning()) {
        cleanDeadEnemies();
        return;
    }

    updateAllBullets();
    runGlobalCollisionCheck();
    cleanDeadEnemies();
}

void BattleSystem::addTower(Tower* tower){
    if(!tower)
        return;

    m_towerContainer.append(tower);

    // 图形对象必须加入 scene 才能显示。这里集中处理，避免 GameController 同时管逻辑和显示。
    if(m_scene && !tower->scene()){
        m_scene->addItem(tower);
    }
}

void BattleSystem::spawnEnemy(Enemy* enemy)
{
    if(!enemy)
        return;

    m_enemyContainer.append(enemy);
    if(m_scene && !enemy->scene()){
        m_scene->addItem(enemy);
    }
}

void BattleSystem::spawnWaveEnemies(const QList<Enemy*> &waveEnemyGroup){
    for(Enemy* enemy:waveEnemyGroup){
        spawnEnemy(enemy);
    }
}

bool BattleSystem::isWaveAllClear() const{
    for(Enemy* enemy:m_enemyContainer){
        if(enemy && !enemy->isDead())
            return false;
    }
    return true;
}

int BattleSystem::activeEnemyCount() const
{
    int count = 0;
    for(Enemy* enemy : m_enemyContainer){
        if(enemy && !enemy->isDead()){
            ++count;
        }
    }
    return count;
}

void BattleSystem::clearAllBattleObjects(){
    while (!m_bulletContainer.isEmpty()) {
        Bullet* bullet = m_bulletContainer.takeLast();
        if(m_scene && bullet && bullet->scene() == m_scene) m_scene->removeItem(bullet);
        delete bullet;
    }

    while (!m_enemyContainer.isEmpty()) {
        Enemy* enemy = m_enemyContainer.takeLast();
        if(m_scene && enemy && enemy->scene() == m_scene) m_scene->removeItem(enemy);
        delete enemy;
    }

    while (!m_towerContainer.isEmpty()) {
        Tower* tower = m_towerContainer.takeLast();
        if(m_scene && tower && tower->scene() == m_scene) m_scene->removeItem(tower);
        delete tower;
    }
}

void BattleSystem::updateAllTowers(){
    for(Tower* tower:m_towerContainer){
        if(!tower) continue;
        Bullet* bullet = tower->updateAttack(m_enemyContainer);
        if(bullet){
            m_bulletContainer.append(bullet);
            if(m_scene && !bullet->scene()){
                m_scene->addItem(bullet);
            }
        }
    }
}

void BattleSystem::updateAllEnemies(){
    for(Enemy* enemy:m_enemyContainer){
        if(!enemy || enemy->isDead())
            continue;

        enemy->updateMove();

        // 敌人抵达城堡后立即扣除城堡血量，并把敌人标记为死亡等待统一清理。
        if(CollisionSystem::enemyReachCastle(enemy, m_mainCastle)){
            int damageToCastle=enemy->getCastleDamage();
            m_gameController->damageCastle(damageToCastle);
            enemy->setDead(true);
        }
    }
}

void BattleSystem::updateAllBullets(){
    for(int i = m_bulletContainer.size() - 1; i >= 0; --i){
        Bullet* bullet = m_bulletContainer[i];
        if(!bullet || !bullet->hasLiveTarget()){
            removeBulletAt(i);
            continue;
        }
        bullet->updateMove();
    }
}

void BattleSystem::runGlobalCollisionCheck(){
    for(int i=m_bulletContainer.size()-1;i>=0;i--){
        Bullet* bullet=m_bulletContainer[i];
        if(!bullet || !bullet->hasLiveTarget()){
            removeBulletAt(i);
            continue;
        }

        bool isHit=false;

        for(Enemy* enemy:m_enemyContainer){
            if(!enemy || enemy->isDead())
                continue;

            // collidesWithItem 依赖贴图包围盒；hitTarget 使用距离兜底，保证小贴图也能命中。
            if(CollisionSystem::isBulletHitEnemy(bullet,enemy) || bullet->hitTarget()){
                Tower* srcTower = bullet->getShootTower();
                int finalDamage = srcTower ? static_cast<int>(srcTower->getRealDamage()) : bullet->getDamage();

                enemy->takeDamage(finalDamage);
                if(srcTower){
                    showHitEffect(enemy->pos(), srcTower->effectResourcePath());
                }
                isHit=true;
                break;
            }
        }
        if(isHit){
            removeBulletAt(i);
        }
    }
}

void BattleSystem::cleanDeadEnemies(){
    for (int enemyIndex = m_enemyContainer.size() - 1; enemyIndex >= 0; --enemyIndex) {
        Enemy* enemy = m_enemyContainer[enemyIndex];
        if (enemy && !enemy->isDead()) {
            continue;
        }

        if (enemy) {
            for (int bulletIndex = m_bulletContainer.size() - 1; bulletIndex >= 0; --bulletIndex) {
                Bullet* bullet = m_bulletContainer[bulletIndex];
                if (bullet && bullet->targetEnemy() == enemy) {
                    removeBulletAt(bulletIndex);
                }
            }
        }

        m_enemyContainer.removeAt(enemyIndex);
        if(m_scene && enemy && enemy->scene() == m_scene) m_scene->removeItem(enemy);
        delete enemy;
    }
}

void BattleSystem::removeBulletAt(int index)
{
    if(index < 0 || index >= m_bulletContainer.size()){
        return;
    }

    Bullet* bullet = m_bulletContainer[index];
    if(m_scene && bullet && bullet->scene() == m_scene){
        m_scene->removeItem(bullet);
    }
    delete bullet;
    m_bulletContainer.removeAt(index);
}

void BattleSystem::showHitEffect(const QPointF& pos, const QString& effectPath)
{
    if(!m_scene || effectPath.isEmpty()){
        return;
    }

    QPixmap pixmap(effectPath);
    if(pixmap.isNull()){
        return;
    }

    QGraphicsPixmapItem* item = m_scene->addPixmap(
        pixmap.scaled(46, 46, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    item->setOffset(-item->pixmap().width() / 2.0, -item->pixmap().height() / 2.0);
    item->setPos(pos);
    item->setZValue(40);

    // The effect belongs to this battle session. Binding the callback to the
    // long-lived scene would let it survive cleanup after the item was deleted.
    QTimer::singleShot(160, this, [scene = m_scene, item]() {
        if(item->scene() == scene){
            scene->removeItem(item);
        }
        delete item;
    });
}