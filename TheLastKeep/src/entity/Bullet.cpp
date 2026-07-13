/* 子弹实体
 * 负责
 * 追踪目标 移动 命中判断 造成伤害
 * 标记是否删除
 *
 */

#include "entity/Bullet.h"
#include "entity/Enemy.h"
#include "entity/Tower.h"
#include <QColor>
#include <QLineF>
#include <QPainter>
#include <QPixmap>

static QPixmap loadBulletPixmap(Tower* tower)
{
    QString path = tower ? tower->bulletResourcePath() : QString(":/images/bullet_common.png");
    QPixmap pixmap(path);
    if (pixmap.isNull()) {
        // 资源缺失时给出一个明显的小圆点，避免逻辑正常但画面完全看不见。
        QPixmap fallback(14, 14);
        fallback.fill(Qt::transparent);
        QPainter painter(&fallback);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setBrush(QColor(255, 230, 90));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(1, 1, 12, 12);
        return fallback;
    }
    return pixmap.scaled(22, 22, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

Bullet::Bullet(QPointF startPos, Enemy *target, int damage, Tower* shootTower)
    : QGraphicsPixmapItem(nullptr),
    m_targetEnemy(target),
    m_damage(damage),
    m_speed(7),
    m_shootTower(shootTower)
{
    setPos(startPos);
    setPixmap(loadBulletPixmap(shootTower));
    setOffset(-pixmap().width() / 2.0, -pixmap().height() / 2.0);
    setZValue(30);
}

void Bullet::updateMove()
{
    if (!hasLiveTarget())
        return;

    QPointF targetPos = m_targetEnemy->pos();
    QLineF line(pos(), targetPos);
    qreal dist = line.length();

    // 命中后的扣血由 BattleSystem::runGlobalCollisionCheck 统一处理，
    // 这里仅停止移动，避免同一颗子弹在两个地方重复造成伤害。
    if (dist < 8)
        return;

    QPointF dir = line.unitVector().p2() - line.unitVector().p1();
    setPos(pos() + dir * m_speed);
}

bool Bullet::hitTarget() const
{
    if (!hasLiveTarget())
        return false;
    QLineF line(pos(), m_targetEnemy->pos());
    return line.length() < 10;
}

bool Bullet::hasLiveTarget() const
{
    return m_targetEnemy && !m_targetEnemy->isDead();
}

int Bullet::getDamage() const
{
    return m_damage;
}

Tower* Bullet::getShootTower() const
{
    return m_shootTower;
}