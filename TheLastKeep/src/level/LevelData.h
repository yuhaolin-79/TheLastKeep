/* 关卡纯数据
 *
 */

#pragma once

#include <QString>
#include <QVector>
#include <QPoint>

struct LevelData {
    QString levelname; //关卡名称
    QString backgroundPath; //背景图路径

    //游戏窗口大小 1280*720 设置为 32列18行 每格40*40，但考虑实际情况，有很多并不会被使用
    int rows = 18;
    int cols = 32;
    int tileSize = 40;

    QVector<QString> mapData; // 地图字符数据
    QVector<QPoint> wayPoints; // 路径点，使用 row col 定位
};
