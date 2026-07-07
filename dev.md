# 最后的要塞(The Last Keep)开发日志

# 2026/07/07

- 建立Qt CMake工程项目和目录结构
- 写出`MainWindow + GameScene + QGraphicsView`的基本框架，运行后显示游戏场景

创建完成后的项目结构

```
TheLastKeep/
├── CMakeLists.txt
├── main.cpp
├── mainwindow.h
├── mainwindow.cpp
└── mainwindow.ui
```

为了适合于工程开发，对项目结构做一定修改

```
TheLastKeep/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── ui/
│   │   ├── MainWindow.h
│   │   └── MainWindow.cpp
│   └── game/
│       ├── GameScene.h
│       └── GameScene.cpp
├── resources/
│   ├── resources.qrc
│   ├── images/
│   └── sounds/
```

可以看到场景是可以正常显示的
![场景图1](image.png)

我们换成游戏主界面背景图
![游戏主界面](image-1.png)
