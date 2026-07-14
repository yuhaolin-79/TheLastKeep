/* 文件目录 src/common/GameType.h
 * 功能 存放整个项目通用的枚举、结构体、游戏状态 所有模块共享的基础头文件
 * 应该包含 GameStatus TileType TowerType EnemyType CardType GameStats
 *        BuffStats EnemySpawnData WaveData CardData
 * @fish 2026/07/10 VERSION 1.0
 */

#pragma once

// 游戏状态 即游戏目前处于什么阶段

enum class GameStatus {
    Menu,        //主菜单
    LevelSelect, //关卡选择
    Running,     //游戏运行中，QTimer运行
    Paused,      //暂停，QTimer停止
    CardSelection, //波次结束，等待玩家选择卡牌
    Win,         //胜利
    Lose,        //失败
};
