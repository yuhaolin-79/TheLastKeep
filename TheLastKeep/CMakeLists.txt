cmake_minimum_required(VERSION 3.19)
project(TheLastKeep LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(TheLastKeep
    WIN32 MACOSX_BUNDLE
    src/main.cpp

    src/common/GameTypes.h
    src/common/GameConstants.h
    src/common/Result.h

    src/ui/MainWindow.h
    src/ui/Mainwindow.cpp

    src/ui/pages/StartPage.h
    src/ui/pages/StartPage.cpp
    src/ui/pages/LevelSelectPage.h
    src/ui/pages/LevelSelectPage.cpp
    src/ui/pages/GamePage.h
    src/ui/pages/GamePage.cpp
    src/ui/pages/ResultPage.h
    src/ui/pages/ResultPage.cpp

    src/ui/widgets/HUDWidget.h
    src/ui/widgets/HUDWidget.cpp
    src/ui/widgets/CardSelectWidget.h
    src/ui/widgets/CardSelectWidget.cpp
    src/ui/widgets/PauseOverlay.h
    src/ui/widgets/PauseOverlay.cpp

    src/scene/GameScene.h
    src/scene/GameScene.cpp

    src/core/GameController.h
    src/core/GameController.cpp
    src/core/StateManager.h
    src/core/StateManager.cpp
    src/core/BattleSystem.h
    src/core/BattleSystem.cpp
    src/core/CollisionSystem.h
    src/core/CollisionSystem.cpp

    src/map/GameMap.h
    src/map/GameMap.cpp
    src/map/Tile.h

    src/level/LevelData.h
    src/level/LevelData.cpp
    src/level/LevelManager.h
    src/level/LevelManager.cpp

    src/entity/Enemy.h
    src/entity/Enemy.cpp
    src/entity/Tower.h
    src/entity/Tower.cpp
    src/entity/Bullet.h
    src/entity/Bullet.cpp
    src/entity/Castle.h
    src/entity/Castle.cpp

    src/card/Card.h
    src/card/Card.cpp
    src/card/CardManager.h
    src/card/CardManager.cpp

    src/data/ResourceManager.h
    src/data/ResourceManager.cpp
    src/data/ConfigManager.h
    src/data/ConfigManager.cpp
    src/data/SaveManager.h
    src/data/SaveManager.cpp
    src/data/SoundManager.h
    src/data/SoundManager.cpp


)

qt_add_resources(TheLastKeep "game_resources"
    PREFIX "/"
    BASE "resources"
    FILES
        resources/images/background.png
        resources/images/bullet_cannon.png
        resources/images/bullet_common.png
        resources/images/bullet_crossbow.png
        resources/images/bullet_flame.png
        resources/images/bullet_magic.png
        resources/images/bullet_shrine.png
        resources/images/card0.png
        resources/images/card1.png
        resources/images/card2.png
        resources/images/card3.png
        resources/images/coin.png
        resources/images/effect_cannon.png
        resources/images/effect_common.png
        resources/images/effect_crossbow.png
        resources/images/effect_flame.png
        resources/images/effect_magic.png
        resources/images/effect_shrine.png
        resources/images/enemy_boss_DemonWolf.png
        resources/images/enemy_goblin.png
        resources/images/enemy_skeletonSoldier.png
        resources/images/enemy_wildmonster.png
        resources/images/guidedmap.png
        resources/images/HUD.png
        resources/images/image.png
        resources/images/image-1.png
        resources/images/image-2.png
        resources/images/ProjectStructure.png
        resources/images/tower_cannon.png
        resources/images/tower_common.png
        resources/images/tower_crossbow.png
        resources/images/tower_flame.png
        resources/images/tower_magic.png
        resources/images/tower_shrine.png
        resources/images/tutorialLevelMap.png
        resources/images/ui/level_select_button.png
        resources/images/ui/quit_button.png
        resources/images/ui/start_button.png
)

target_include_directories(TheLastKeep
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(TheLastKeep
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS TheLastKeep
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET TheLastKeep
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
