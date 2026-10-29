cmake_minimum_required(VERSION 3.16)

project(smartHome VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Mqtt)
find_package(Qt6 REQUIRED COMPONENTS Core)

qt_policy(SET QTP0001 NEW)


qt_add_executable(appsmartHome
    main.cpp
    mqttmanager.h
    mqttmanager.cpp
)

qt_add_qml_module(appsmartHome
    URI smartHome
    VERSION 1.0
    QML_FILES
        Main.qml
        MainScreen.qml
        DeviceCard.qml
        DeviceDetailScreen.qml
        SensorCard.qml
        OutputCard.qml
    SOURCES 
        systemcontroller.h 
        systemcontroller.cpp
        device.h 
        device.cpp
        fridge.h 
        fridge.cpp
        devicemanager.h
        devicemanager.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appsmartHome PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appsmartHome
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appsmartHome
    PRIVATE Qt6::Quick
)
target_link_libraries(appsmartHome PRIVATE Qt6::Core)
target_link_libraries(appsmartHome PRIVATE Qt6::Mqtt)
target_link_libraries(appsmartHome PRIVATE Qt6::Core)

include(GNUInstallDirs)
install(TARGETS appsmartHome
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
