/* 要塞实体
 * 负责
 * 生命值
 * 扣血
 * 死亡判断
 * 显示要塞图片
 */

#include "entity/Castle.h"
#include "core/GameController.h"
#include <QPixmap>

Castle::Castle(QPointF pos, GameController *ctrl)
    : QGraphicsPixmapItem(nullptr), m_controller(ctrl)
{
    setPos(pos);
    setPixmap(QPixmap(":/assets/images/castle.png"));
}

void Castle::takeDamage(int dmg)
{
    if (m_controller != nullptr)
    {
        m_controller->damageCastle(dmg);
    }
}

bool Castle::isDestroyed() const
{
    if (!m_controller)
        return true;
    return m_controller->castleHp() <= 0;
}