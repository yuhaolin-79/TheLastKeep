#ifndef CASTLE_H
#define CASTLE_H

// 图形图元父类头文件
#include <QGraphicsPixmapItem>
#include <QPointF>

// 前置声明控制器
class GameController;

class Castle : public QGraphicsPixmapItem
{
public:

    explicit Castle(QPointF pos, GameController* ctrl);

    // 成员函数声明
    void takeDamage(int dmg);
    bool isDestroyed() const;

private:
    // 私有成员变量
    GameController* m_controller;
};

#endif // CASTLE_H
