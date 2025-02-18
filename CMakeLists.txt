 cmake_minimum_required(VERSION 3.5)

project(Qplugs VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(./include/)

set(QT6_ROOT D:/DevelopmentKits/Qt/6.8.2/msvc2022_64)



if (WIN32)
    set(Qt6_DIR ${QT6_ROOT}/lib/cmake/Qt6)
    find_package(Qt6 REQUIRED COMPONENTS Widgets Xml WebEngineWidgets)
    find_package(OpenCV CONFIG REQUIRED)
    include_directories(${OpenCV_INCLUDE_DIRS})
    message(STATUS ${OpenCV_INCLUDE_DIRS})
else ()
    find_package(Qt6 REQUIRED COMPONENTS Widgets Xml WebEngineWidgets)
    find_package(OpenCV REQUIRED)
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()




FILE(GLOB INCLUDE_FILES "include/*.h")
FILE(GLOB MODULES_FILES "src/*.cpp")
set(CMAKE_AUTOUIC_SEARCH_PATHS "interface-file")
FILE(GLOB UI_FILES "interface-file/*.ui")

qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
    main.cpp ${INCLUDE_FILES} ${MODULES_FILES} ${UI_FILES}
    resource.qrc
)

# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD   #复制动态库
#                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#                     ${OPENCV_DLLS} $<TARGET_FILE_DIR:${PROJECT_NAME}>)
message(STATUS "lib ${OpenCV_LIBS}")
target_link_libraries(${PROJECT_NAME}  PRIVATE Qt6::Widgets Qt6::Xml Qt6::WebEngineWidgets ${OpenCV_LIBS})


## dll
#file(GLOB OpenCV_DLLS ${OpenCV_INSTALL_PATH}/bin/opencv*.dll)
##copy dll file
#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#     ${OpenCV_DLLS} $<TARGET_FILE_DIR:${PROJECT_NAME}>)
#qt dll
find_program(WINDEPLOYQT NAMES windeployqt.exe HINTS "${QT6_ROOT}/bin")
if(MSVC)
    #copy dll file
     add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
             COMMAND ${WINDEPLOYQT} $<TARGET_FILE_DIR:${PROJECT_NAME}>
             COMMENT "Deploying Qt DLLs using windeployqt"
     )
endif ()
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${PROJECT_NAME})

set_target_properties(${PROJECT_NAME}  PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
