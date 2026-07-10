/* 目录 src/common/GameConstants.h
 * 功能 存放固定参数，避免 magic number
 * @fish 2026/07/10 VERSION 1.0
 */

#pragma once

namespace GameConstants {

// 游戏窗口固定大小
constexpr int WindowWidth = 1280;
constexpr int WindowHeight = 720;

// 游戏主循环间隔 33ms 实际体验大致为 30FPS
constexpr int FrameIntervalMs = 33;

}