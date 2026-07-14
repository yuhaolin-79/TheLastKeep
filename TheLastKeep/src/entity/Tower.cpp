/* 防御塔实体
 * 负责
 * 攻击范围 攻击间隔 攻击力 寻找目标
 * 生成子弹 应用 Buff
 *
 */

#include "entity/Tower.h"
#include "entity/Enemy.h"
#include "entity/Bullet.h"
#include "core/GameController.h"
#include <QColor>
#include <QLineF>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include "card/Card.h"

static QString towerPixmapPath(TowerType type)
{
    switch (type) {
    case TowerType::ArrowTower:
        return ":/images/tower_crossbow.png";
    case TowerType::MageTower:
        return ":/images/tower_magic.png";
    case TowerType::CannonTower:
        return ":/images/tower_cannon.png";
    case TowerType::IceTower:
        return ":/images/tower_flame.png";
    case TowerType::HolyTower:
        return ":/images/tower_shrine.png";
    }
    return ":/images/tower_common.png";
}

static QColor towerFallbackColor(TowerType type)
{
    switch (type) {
    case TowerType::ArrowTower: return QColor(80, 140, 220);
    case TowerType::MageTower: return QColor(140, 90, 220);
    case TowerType::CannonTower: return QColor(180, 110, 60);
    case TowerType::IceTower: return QColor(210, 90, 80);
    case TowerType::HolyTower: return QColor(230, 210, 110);
    }
    return QColor(180, 180, 180);
}

static QString towerDisplayName(TowerType type)
{
    switch (type) {
    case TowerType::ArrowTower: return "弩塔";
    case TowerType::MageTower: return "法塔";
    case TowerType::CannonTower: return "炮塔";
    case TowerType::IceTower: return "火塔";
    case TowerType::HolyTower: return "圣塔";
    }
    return "防御塔";
}

static QString towerEffectText(TowerType type)
{
    switch (type) {
    case TowerType::ArrowTower: return "稳定单体输出，攻速较快";
    case TowerType::MageTower: return "高伤害魔法弹，适合打厚血敌人";
    case TowerType::CannonTower: return "重型炮击，单发伤害最高";
    case TowerType::IceTower: return "快速火焰弹，压制低血量敌人";
    case TowerType::HolyTower: return "圣坛能量弹，射程和伤害均衡";
    }
    return "基础攻击";
}
static QPixmap loadTowerPixmap(TowerType type)
{
    QPixmap pixmap(towerPixmapPath(type));
    if (pixmap.isNull()) {
        // 防御塔缺图时使用方形标记，便于区分敌人的圆形 fallback。
        QPixmap fallback(42, 42);
        fallback.fill(Qt::transparent);
        QPainter painter(&fallback);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setBrush(towerFallbackColor(type));
        painter.setPen(QPen(QColor(30, 30, 30), 2));
        painter.drawRoundedRect(3, 3, 36, 36, 6, 6);
        return fallback;
    }

    return pixmap.scaled(58, 58, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

Tower::Tower(TowerType type, QPointF pos, GameController *ctrl)
    : QGraphicsPixmapItem(nullptr),
    m_type(type),
    m_controller(ctrl),
    m_lastAttackTime(0)
{
    setPos(pos);
    initTowerAttr(type);
}

void Tower::initTowerAttr(TowerType type)
{
    switch (type)
    {
    case TowerType::ArrowTower:
        m_attackRange = 170;
        m_attackDamage = 20;
        m_attackInterval = 760;
        m_buildCost = 50;
        break;
    case TowerType::MageTower:
        m_attackRange = 150;
        m_attackDamage = 40;
        m_attackInterval = 1180;
        m_buildCost = 100;
        break;
    case TowerType::CannonTower:
        m_attackRange = 185;
        m_attackDamage = 50;
        m_attackInterval = 1700;
        m_buildCost = 150;
        break;
    case TowerType::IceTower:
        m_attackRange = 155;
        m_attackDamage = 25;
        m_attackInterval = 620;
        m_buildCost = 80;
        break;
    case TowerType::HolyTower:
        m_attackRange = 175;
        m_attackDamage = 40;
        m_attackInterval = 900;
        m_buildCost = 120;
        break;
    }

    setPixmap(loadTowerPixmap(type));
    setOffset(-pixmap().width() / 2.0, -pixmap().height() / 2.0);
    setZValue(15);
}

Bullet* Tower::updateAttack(const QList<Enemy *> &enemyList)
{
    qint64 now=QDateTime::currentMSecsSinceEpoch();
    if (now-m_lastAttackTime<m_attackInterval)
        return nullptr;

    Enemy* target=findTarget(enemyList);
    if (!target)
        return nullptr;

    m_lastAttackTime=now;
    return createBullet(target);
}

Enemy* Tower::findTarget(const QList<Enemy *> &enemyList)
{
    Enemy* nearest=nullptr;
    float realRange=getRealAttackRange();
    qreal minDis=realRange;

    for (Enemy* e:enemyList)
    {
        if (!e || e->isDead())
            continue;
        QLineF disLine(pos(), e->pos());
        qreal dis=disLine.length();
        if (dis<minDis)
        {
            minDis=dis;
            nearest=e;
        }
    }
    return nearest;
}

Bullet* Tower::createBullet(Enemy *target)
{
    float finalDmg = getRealDamage();
    return new Bullet(this->pos(), target, static_cast<int>(finalDmg), this);
}

int Tower::getBuildCost() const
{
    return m_buildCost;
}

int Tower::getAttackRange() const
{
    return m_attackRange;
}

int Tower::getDamage() const
{
    return m_attackDamage;
}

int Tower::getAttackInterval() const
{
    return m_attackInterval;
}

TowerType Tower::towerType() const
{
    return m_type;
}

QString Tower::bulletResourcePath() const
{
    switch (m_type) {
    case TowerType::ArrowTower: return ":/images/bullet_crossbow.png";
    case TowerType::MageTower: return ":/images/bullet_magic.png";
    case TowerType::CannonTower: return ":/images/bullet_cannon.png";
    case TowerType::IceTower: return ":/images/bullet_flame.png";
    case TowerType::HolyTower: return ":/images/bullet_shrine.png";
    }
    return ":/images/bullet_common.png";
}

QString Tower::effectResourcePath() const
{
    switch (m_type) {
    case TowerType::ArrowTower: return ":/images/effect_crossbow.png";
    case TowerType::MageTower: return ":/images/effect_magic.png";
    case TowerType::CannonTower: return ":/images/effect_cannon.png";
    case TowerType::IceTower: return ":/images/effect_flame.png";
    case TowerType::HolyTower: return ":/images/effect_shrine.png";
    }
    return ":/images/effect_common.png";
}

float Tower::getRealAttackRange() const{
    BuffState buff=m_controller->getGlobalBuff();
    return m_attackRange*buff.attackRangeRate;
}

float Tower::getRealDamage() const{
    BuffState buff=m_controller->getGlobalBuff();
    return m_attackDamage*buff.damageRate;
}