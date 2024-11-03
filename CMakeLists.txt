cmake_minimum_required(VERSION 3.5)

project(hand_cmake VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(./include/)

find_package(Qt6 REQUIRED COMPONENTS Widgets Xml WebEngineWidgets)


#msvc
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_library(OPENCV INTERFACE IMPORTED)
    set_target_properties(OPENCV PROPERTIES
                INTERFACE_LINK_LIBRARIES "opencv_world481")
    include_directories(${OPENCV_DIR}/include)  #头文件
    link_directories(${OPENCV_DIR}/x64/vc17/lib)  #lib位置
    file(GLOB OPENCV_DLLS ${OPENCV_DIR}/x64/vc17/bin/*.dll)
endif()



find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

FILE(GLOB INCLUDE_FILES "include/*.h")
FILE(GLOB MODULES_FILES "src/*.cpp")
set(CMAKE_AUTOUIC_SEARCH_PATHS "interface-file")
FILE(GLOB UI_FILES "interface-file/*.ui")

qt_add_executable(hand_cmake
    MANUAL_FINALIZATION
    ${INCLUDE_FILES} ${MODULES_FILES} ${UI_FILES}
    resource.qrc
)

# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD   #复制动态库
#                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#                     ${OPENCV_DLLS} $<TARGET_FILE_DIR:${PROJECT_NAME}>)
message(STATUS "lib ${OpenCV_LIBS}")
target_link_libraries(hand_cmake PRIVATE Qt6::Widgets Qt6::Xml Qt6::WebEngineWidgets ${OpenCV_LIBS})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.hand_cmake)

set_target_properties(hand_cmake PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS hand_cmake
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(hand_cmake)
endif()
