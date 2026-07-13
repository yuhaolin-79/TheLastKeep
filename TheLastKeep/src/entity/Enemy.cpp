/* 敌人实体
 * 负责
 * 沿路径移动 保存血量 受到伤害
 * 死亡判断 到达城堡判断 奖励金币
 * 分数价值
 */

#include "entity/Enemy.h"
#include "core/GameController.h"
#include <QColor>
#include <QLineF>
#include <QPainter>
#include <QPen>
#include <QPixmap>

static QPixmap loadEnemyPixmap(EnemyType type)
{
    QString path;
    QColor fallbackColor;

    switch (type) {
    case EnemyType::NormalGoblin:
        path = ":/images/enemy_goblin.png";
        fallbackColor = QColor(90, 190, 110);
        break;
    case EnemyType::HeavyArmor:
        path = ":/images/enemy_skeletonSoldier.png";
        fallbackColor = QColor(120, 120, 140);
        break;
    case EnemyType::WolfRider:
        path = ":/images/enemy_wildmonster.png";
        fallbackColor = QColor(220, 160, 80);
        break;
    case EnemyType::Wizard:
        path = ":/images/enemy_skeletonSoldier.png";
        fallbackColor = QColor(130, 90, 210);
        break;
    case EnemyType::Boss:
        path = ":/images/enemy_boss_DemonWolf.png";
        fallbackColor = QColor(190, 70, 70);
        break;
    }

    QPixmap pixmap(path);
    if (pixmap.isNull()) {
        // 所有本地资源都应放在 /images 下；如果临时缺图，仍显示色块方便验证逻辑。
        QPixmap fallback(30, 30);
        fallback.fill(Qt::transparent);
        QPainter painter(&fallback);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setBrush(fallbackColor);
        painter.setPen(QPen(QColor(30, 30, 30), 2));
        painter.drawEllipse(2, 2, 26, 26);
        return fallback;
    }

    return pixmap.scaled(44, 44, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

Enemy::Enemy(const QVector<QPointF> &path, EnemyType type, GameController *ctrl)
    : QGraphicsPixmapItem(nullptr),
    m_path(path),
    m_currentTargetIdx(0),
    m_controller(ctrl),
    m_isDead(false)
{
    initEnemyAttr(type);
    if (!m_path.isEmpty())
    {
        setPos(m_path.first());
    }
}

void Enemy::initEnemyAttr(EnemyType type)
{
    switch (type)
    {
    case EnemyType::NormalGoblin:
        m_maxHp = 100;
        m_speed = 2;
        m_rewardGold = 10;
        m_castleDamage = 10;
        break;
    case EnemyType::HeavyArmor:
        m_maxHp = 260;
        m_speed = 1;
        m_rewardGold = 25;
        m_castleDamage = 25;
        break;
    case EnemyType::WolfRider:
        m_maxHp = 70;
        m_speed = 4;
        m_rewardGold = 15;
        m_castleDamage = 15;
        break;
    case EnemyType::Wizard:
        m_maxHp = 140;
        m_speed = 2;
        m_rewardGold = 20;
        m_castleDamage = 20;
        break;
    case EnemyType::Boss:
        m_maxHp = 900;
        m_speed = 1;
        m_rewardGold = 200;
        m_castleDamage = 100;
        break;
    }

    m_hp = m_maxHp;
    setPixmap(loadEnemyPixmap(type));
    setOffset(-pixmap().width() / 2.0, -pixmap().height() / 2.0);
    setZValue(20);
}

void Enemy::updateMove()
{
    if (isDead() || m_currentTargetIdx >= m_path.size())
        return;

    QPointF target = m_path[m_currentTargetIdx];
    QLineF line(pos(), target);
    qreal dist = line.length();

    if (dist < m_speed)
    {
        setPos(target);
        m_currentTargetIdx++;
    }
    else
    {
        QPointF dir = line.unitVector().p2() - line.unitVector().p1();
        setPos(pos() + dir * m_speed);
    }
}

void Enemy::takeDamage(int damage)
{
    if (damage <= 0 || isDead())
        return;

    m_hp -= damage;
    if (m_hp <= 0 && m_controller)
    {
        m_controller->addGold(m_rewardGold);
    }
}

bool Enemy::isDead() const
{
    return m_hp <= 0|| m_isDead;
}

bool Enemy::reachedCastle() const
{
    return m_currentTargetIdx >= m_path.size();
}

int Enemy::getReward() const
{
    return m_rewardGold;
}

int Enemy::getCastleDamage() const
{
    return m_castleDamage;
}

void Enemy::setDead(bool isDead)
{
    m_isDead = isDead;
}

bool Enemy::isHpZero() const
{
    return m_hp <= 0;
}