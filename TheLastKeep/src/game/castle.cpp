#include "game/Castle.h"
#include "game/GameController.h"
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