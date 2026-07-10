/* 战斗系统
 * 负责 生成敌人 更新敌人 更新塔 更新子弹 碰撞检测
 *     敌人到达城堡 敌人死亡奖励金币 波次是否结束
 *     战斗对象清理
 *      @chen 2026/07/10 version 1.0
 */
#include"BattleSystem.h"
#include"CollisionSystem.h"
#include"core/GameController.h"
#include"entity/Tower.h"
#include"entity/Enemy.h"
#include"entity/Bullet.h"
#include"entity/Castle.h"

BattleSystem::BattleSystem(GameController* gameCtrl, Castle* castle, QObject* parent)
    : m_gameController(gameCtrl), m_mainCastle(castle), QObject(parent){
}
void BattleSystem::frameUpdate(){
    updateAllTowers();
    updateAllEnemies();
    updateAllBullets();
    runGlobalCollisionCheck();

    if(isWaveAllClear()){
        m_gameController->waveFinishShowCard();
    }
}

void BattleSystem::addTower(Tower* tower){
    if(tower!=nullptr)
        m_towerContainer.append(tower);
}

void BattleSystem::spawnWaveEnemies(const QList<Enemy*> &waveEnemyGroup){
    for(Enemy* enemy:waveEnemyGroup){
        if(enemy!=nullptr)
            m_enemyContainer.append(enemy);
    }
}

bool BattleSystem::isWaveAllClear() const{
    for(Enemy* enemy:m_enemyContainer){
        if(!enemy->isDead())
            return false;
    }
    return true;
}

void BattleSystem::clearAllBattleObjects(){
    //释放塔内存
    for (Tower* t:m_towerContainer) delete t;
    m_towerContainer.clear();
    //释放敌人内存
    for (Enemy* e:m_enemyContainer) delete e;
    m_enemyContainer.clear();
    //释放子弹内存
    for (Bullet* b:m_bulletContainer) delete b;
    m_bulletContainer.clear();
}

void BattleSystem::updateAllEnemies(){
    for(Enemy* enemy:m_enemyContainer){
        if(enemy->isDead())
            continue;
        //敌人沿路径移动
        enemy->updateMove();
        //碰撞检测
        if(CollisionSystem::enemyReachCastle(enemy, m_mainCastle)){
            int damageToCastle=enemy->getCastleDamage();
            m_gameController->damageCastle(damageToCastle);
            enemy->setDead(true);
        }
    }
}

void BattleSystem::updateAllBullets(){
    for(Bullet* bullet:m_bulletContainer){
        bullet->updateMove();
    }
}

void BattleSystem::runGlobalCollisionCheck(){
    for(int i=m_bulletContainer.size()-1;i>=0;i--){
        Bullet* bullet=m_bulletContainer[i];
        bool isHit=false;

        for(Enemy* enemy:m_enemyContainer){
            if(enemy->isDead())
                continue;
            if(CollisionSystem::isBulletHitEnemy(bullet,enemy)){
                // 从子弹拿到发射塔，实时读取当前全局Buff真实伤害
                Tower* srcTower = bullet->getShootTower();
                float realDamageFloat = srcTower->getRealDamage();
                int finalDamage = static_cast<int>(realDamageFloat);

                enemy->takeDamage(finalDamage);
                isHit=true;
                // 仅血量耗尽击杀时奖励金币，抵达城堡标记死亡不奖励
                if(enemy->isHpZero())
                {
                    int baseGold=enemy->getReward();
                    m_gameController->addGold(baseGold);
                }
                break;
            }
        }
        if(isHit){
            delete bullet;
            m_bulletContainer.removeAt(i);
        }
    }
}

void BattleSystem::cleanDeadEnemies(){
    QList<Enemy*> aliveEnemies;
    for (Enemy* e : m_enemyContainer){
        // isDead() 同时覆盖两种情况：
        // 1. 敌人血量 ≤ 0（被塔子弹击杀）
        // 2. 敌人抵达城堡执行 setDead(true)
        if (e->isDead()){
            delete e; // 释放死亡敌人内存
        }
        else{
            aliveEnemies.append(e); // 存活敌人保留
        }
    }
    // 更新容器，只保留活着的敌人
    m_enemyContainer.swap(aliveEnemies);
}