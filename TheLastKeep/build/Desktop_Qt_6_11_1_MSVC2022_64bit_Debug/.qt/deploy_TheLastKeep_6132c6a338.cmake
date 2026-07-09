include("E:/TheLastKeep/TheLastKeep/build/Desktop_Qt_6_11_1_MSVC2022_64bit_Debug/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/TheLastKeep-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "E:/TheLastKeep/TheLastKeep/build/Desktop_Qt_6_11_1_MSVC2022_64bit_Debug/TheLastKeep.exe"
    GENERATE_QT_CONF
)
