# 合并修复文档

## 概述

- **日期**: 2026-07-10
- **操作**: `origin/master` → `main` 分支合并
- **合并基线**: 
  - `main` (本地): `22bfd4e` 重构界面切换增加按钮贴图
  - `origin/master`: `36bfa96` 卡牌与碰撞系统，战斗系统
- **构建套件**: Desktop Qt 6.11.1 MSVC2022 64bit

---

## 冲突文件及处理

### 1. `src/core/GameController.h`
- **冲突原因**: HEAD 新增 QTimer/StateManager 头文件和私有成员；origin/master 新增 Card/CardManager 头文件和私有成员
- **处理方式**: 合并双方，同时保留两套系统
  - 头文件: `<QTimer>` + `"card/Card.h"` `"card/CardManager.h"`
  - 公开方法: 游戏生命周期接口 (HEAD) + 金币管理接口 + 卡牌 Buff 接口 (origin/master)
  - 私有成员: `QTimer *m_timer` / `StateManager` (HEAD) + `CardManager m_cardMgr` / 城堡血量 / 金币 (origin/master)
- **新增方法**: `damageCastle(int damage)` — BattleSystem 调用

### 2. `src/core/GameController.cpp`
- **冲突原因**: 同上，cpp 实现分别存在于两个分支的不同位置
- **处理方式**: 完整合并所有方法
  - HEAD: 构造 (`m_scene`, `m_timer`) + 游戏生命周期 (`startGame`, `pauseGame`, `resumeGame` 等) + `isRunning` / `status` / `updateFrame` / `startTimerSafely` / `stopTimerSafely`
  - origin/master: 构造 (`m_castleMaxHp`, `m_castleCurrentHp`, `m_gold`, `m_cardMgr`) + 金币管理 (`addGold`, `spendGold`, `canBuildTower`) + 卡牌接口 (`waveFinishShowCard`, `selectBuffCard`, `getGlobalBuff`, `gameRestartReset`)
- **新增实现**: `damageCastle(int)` — 扣减 `m_castleCurrentHp`，HP <= 0 时 emit `gameFinished(false, 0)`

### 3. `src/entity/Bullet.cpp`
- **冲突原因**: HEAD 中全部核心代码被注释；origin/master 有活跃实现且构造函数增加了 `Tower* shootTower` 参数
- **处理方式**: 采用 origin/master 的活跃版本，并补全取消注释 `updateMove()` 和 `hitTarget()`
- **修复问题**: 命中检测代码块 (`if (dist < 8)`) 原处于全局作用域（浮动代码），已归入 `updateMove()` 函数体内

### 4. `src/entity/Enemy.cpp`
- **冲突原因**: HEAD 中全部核心代码被注释；origin/master 有活跃实现
- **处理方式**: 采用 origin/master 版本，取消注释 `updateMove()`、`takeDamage()`、`reachedCastle()`
- **关键变更**: origin/master 版本新增 `m_isDead`（手动死亡标记）、`m_castleDamage`（撞城堡伤害）、`setDead()`、`isHpZero()`、`getCastleDamage()`

### 5. `src/entity/Tower.cpp`
- **冲突原因**: HEAD 中全部核心代码被注释；origin/master 有活跃实现
- **处理方式**: 采用 origin/master 版本，取消注释构造函数 `Tower()` 和 `initTowerAttr()`
- **关键变更**: origin/master 版本引入 `getRealAttackRange()` / `getRealDamage()` 方法，通过 `GameController::getGlobalBuff()` 获取 BuffState 倍率加成

### 6. `.qtcreator/CMakeLists.txt.user`
- **冲突原因**: 版本注释差异 (QtCreator 20.0.0 vs 19.0.2)
- **处理方式**: 保留 HEAD (20.0.0)，后续由 QtCreator 自动更新

---

## 合并后补充修复

以下问题是合并后代码不一致导致编译失败，已逐一修复：

### 编译错误修复

| # | 文件 | 问题描述 | 修复内容 |
|---|------|---------|---------|
| 1 | `Bullet.cpp:35-42` | 浮动代码在全局作用域（`if (dist < 8)` 不在函数内） | 取消注释 `updateMove()`，将代码归入函数体 |
| 2 | `Tower.cpp:17-67` | 构造函数和 `initTowerAttr()` 被注释 | 取消注释，恢复塔的创建和属性初始化 |
| 3 | `Castle.cpp` + `Castle.h` | Castle 类完全空实现，无 HP 成员 | 新增 `m_hp`/`m_maxHp`，实现 `takeDamage()`/`isDestroyed()`/`currentHp()`/`maxHp()` |
| 4 | `Enemy.cpp` | `updateMove()`/`takeDamage()`/`reachedCastle()` 被注释 | 取消注释，恢复敌人移动和受伤逻辑 |
| 5 | `GameController.h/cpp` | 缺少 `damageCastle(int)` | 新增方法：扣减城堡血量，HP<=0 发出失败信号 |
| 6 | `BattleSystem.cpp` | 缺少 `updateAllTowers()` 实现 + include 路径错误 + 未调用 `cleanDeadEnemies()` | 补充塔攻击更新逻辑，修正 `#include` 路径为 `core/BattleSystem.h`，在 `frameUpdate()` 中调用清理方法 |
| 7 | `CardManager.h` | `extern` 声明在 `#include <QVector>` 之前 | 调整声明顺序 |
| 8 | `CMakeLists.txt` | `MainWindow.cpp` 大小写与实际文件名 `Mainwindow.cpp` 不一致 | 修正为 `Mainwindow.cpp` |

### 逻辑修复

| # | 文件 | 问题描述 | 修复内容 |
|---|------|---------|---------|
| 1 | `BattleSystem.cpp` | `Enemy::takeDamage()` 已发放金币，`runGlobalCollisionCheck()` 又重复发放（双倍金币 bug） | 移除 `runGlobalCollisionCheck()` 中的重复金币奖励 |

---

## 架构变更说明

合并后 GameController 成为两套系统的交汇点：

```
GameController
├── QTimer 主循环 (HEAD)
│   ├── startGame() / pauseGame() / resumeGame() / stopGame()
│   ├── pauseForPageHidden() / resumeForPageShown()
│   ├── updateFrame() → 每帧入口
│   └── StateManager → 状态机
│
├── 金币 & 城堡系统 (origin/master)
│   ├── addGold() / spendGold() / canBuildTower()
│   ├── damageCastle() → 城堡受伤 → 失败判断
│   └── m_castleCurrentHp / m_castleMaxHp / m_gold
│
└── 卡牌 Buff 系统 (origin/master)
    ├── waveFinishShowCard() / selectBuffCard() / getGlobalBuff()
    ├── gameRestartReset()
    └── CardManager m_cardMgr
```

### 数据流

```
BattleSystem::frameUpdate()
  ├── updateAllTowers()    → Tower::updateAttack()  → Bullet 生成
  ├── updateAllEnemies()   → Enemy::updateMove()    → 路径移动
  ├── updateAllBullets()   → Bullet::updateMove()   → 子弹飞行 & 命中
  ├── runGlobalCollisionCheck() → 子弹-敌人碰撞
  │     └── Enemy::takeDamage() → HP<=0 → addGold()
  └── cleanDeadEnemies()   → 释放死亡敌人

敌人到达城堡:
  CollisionSystem::enemyReachCastle()
    → GameController::damageCastle()
    → m_castleCurrentHp -= damage
    → HP<=0 → emit gameFinished(false, 0)
```

### Buff 倍率链

```
CardManager → BuffState(damageRate, buildCostRate, goldRewardRate, attackRangeRate)
  ├── Tower::getRealDamage()      → m_attackDamage × buff.damageRate
  ├── Tower::getRealAttackRange() → m_attackRange × buff.attackRangeRate
  ├── GameController::addGold()   → baseGold × buff.goldRewardRate
  └── GameController::spendGold() → cost × buff.buildCostRate
```

---

## 涉及文件清单

### 冲突解决 (merge 时)
- `src/core/GameController.h`
- `src/core/GameController.cpp`
- `src/entity/Bullet.cpp`
- `src/entity/Enemy.cpp`
- `src/entity/Tower.cpp`
- `.qtcreator/CMakeLists.txt.user`

### 补充修复 (merge 后)
- `src/core/BattleSystem.cpp` — 补全缺失方法 + 修复 include + 移除双倍金币
- `src/entity/Castle.h` — 新增 HP 成员
- `src/entity/Castle.cpp` — 实现全部方法
- `src/card/CardManager.h` — 修复 extern 声明顺序
- `CMakeLists.txt` — 修正文件名大小写
