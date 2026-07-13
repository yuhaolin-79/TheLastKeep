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

    // 游戏窗口大小 1280*720。
    // 教程关卡使用 40列 * 22行 * 32px，让逻辑网格更细，路径更容易贴合背景图。
    int rows = 22;
    int cols = 40;
    int tileSize = 32;

    QVector<QString> mapData; // 地图字符数据
    QVector<QPoint> wayPoints; // 路径点，使用 row col 定位
};
