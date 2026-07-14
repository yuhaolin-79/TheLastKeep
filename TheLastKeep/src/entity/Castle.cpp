/* 要塞实体
 * 负责
 * 生命值
 * 扣血
 * 死亡判断
 * 显示要塞图片
 */

#include "entity/Castle.h"
#include "common/GameConstants.h"
#include "core/GameController.h"
#include <QColor>
#include <QPainter>
#include <QPen>
#include <QPixmap>

static QPixmap loadCastlePixmap()
{
    QPixmap pixmap(":/images/tower_shrine.png");
    if (pixmap.isNull()) {
        // 城堡是碰撞终点，缺图时也必须有可见面积，否则敌人到达判定不直观。
        QPixmap fallback(56, 56);
        fallback.fill(Qt::transparent);
        QPainter painter(&fallback);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setBrush(QColor(170, 80, 80));
        painter.setPen(QPen(QColor(40, 30, 30), 2));
        painter.drawRect(8, 18, 40, 30);
        painter.drawRect(14, 8, 8, 12);
        painter.drawRect(34, 8, 8, 12);
        return fallback;
    }
    return pixmap.scaled(58, 58, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

Castle::Castle(QPointF pos, GameController* ctrl)
    : QGraphicsPixmapItem(nullptr),
    m_controller(ctrl),
    m_maxHp(GameConstants::DefaultCastleMaxHp),
    m_hp(GameConstants::DefaultCastleMaxHp)
{
    setPos(pos);
    setPixmap(loadCastlePixmap());
    setOffset(-pixmap().width() / 2.0, -pixmap().height() / 2.0);
    setZValue(10);
}

void Castle::takeDamage(int dmg)
{
    if (dmg <= 0) return;
    m_hp -= dmg;
    if (m_hp < 0) m_hp = 0;
}

bool Castle::isDestroyed() const
{
    return m_hp <= 0;
}

int Castle::currentHp() const
{
    return m_hp;
}

int Castle::maxHp() const
{
    return m_maxHp;
}