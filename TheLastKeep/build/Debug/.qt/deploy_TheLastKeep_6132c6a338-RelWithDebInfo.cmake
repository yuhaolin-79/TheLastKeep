include("E:/TheLastKeep/TheLastKeep/build/Debug/.qt/QtDeploySupport-RelWithDebInfo.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/TheLastKeep-plugins-RelWithDebInfo.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "E:/TheLastKeep/TheLastKeep/build/Debug/RelWithDebInfo/TheLastKeep.exe"
    GENERATE_QT_CONF
)
