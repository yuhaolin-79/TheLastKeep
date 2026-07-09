cmake_minimum_required(VERSION 3.19)
project(TheLastKeep LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(TheLastKeep
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/ui/Mainwindow.h
    src/ui/Mainwindow.cpp

    src/game/GameScene.h
    src/game/GameScene.cpp
    src/game/GameMap.h
    src/game/GameMap.cpp
    src/game/LevelData.h
    src/game/LevelData.cpp
    src/game/LevelManager.h
    src/game/LevelManager.cpp

    src/game/enemy.h
    src/game/enemy.cpp
    src/game/tower.h
    src/game/tower.cpp
    src/game/bullet.h
    src/game/bullet.cpp
    src/game/castle.h
    src/game/castle.cpp
    src/game/gamecontroller.h
    src/game/gamecontroller.cpp
    src/game/card.h
    src/game/card.cpp
    src/game/cardmanager.h
    src/game/cardmanager.cpp
)

qt_add_resources(TheLastKeep "game_resources"
    PREFIX "/"
    BASE "resources"
    FILES
        resources/images/background.png
        resources/images/tutorialLevelMap.png
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
