cmake_minimum_required(VERSION 3.8)

project(volbx LANGUAGES CXX VERSION 2.0.0 DESCRIPTION "Graphical tool for data manipulation")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# find Qt packages
find_package(Qt5 COMPONENTS Widgets Core Gui Network Xml Test REQUIRED)
add_definitions(${QT_DEFINITIONS})

# QWT
if(DEFINED ENV{QWT_ROOT})
    message(STATUS "QWT_ROOT environment variable defined, using QWT_ROOT paths.")
    set(QWT_INCLUDE_PATH $ENV{QWT_ROOT}/include)
    set(QWT_BINARY_PATH $ENV{QWT_ROOT}/lib)

    # copy QWT dlls for easy launch
    file(GLOB_RECURSE QWT_DLLS "$ENV{QWT_ROOT}/lib/*.dll" )
    file(COPY ${QWT_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY ${QWT_DLLS} DESTINATION ${CMAKE_BINARY_DIR}/Tests)
else()
    configure_file(CMakeLists.txt.qwt qwt/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/qwt)
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/qwt)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/qwt/install)
    file(COPY ${CMAKE_BINARY_DIR}/qwt/qwt/src/qwt-build/lib DESTINATION ${CMAKE_BINARY_DIR}/qwt/install)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/qwt/install/include)
    file(GLOB QWT_HEADERS "${CMAKE_BINARY_DIR}/qwt/qwt/src/qwt/src/*.h" )
    file(COPY ${QWT_HEADERS} DESTINATION ${CMAKE_BINARY_DIR}/qwt/install/include)
    set(QWT_INCLUDE_PATH ${CMAKE_BINARY_DIR}/qwt/install/include)
    set(QWT_BINARY_PATH ${CMAKE_BINARY_DIR}/qwt/install/lib)
endif()

message(STATUS "Using QWT_INCLUDE_PATH ${QWT_INCLUDE_PATH}")
message(STATUS "Using QWT_BINARY_PATH ${QWT_BINARY_PATH}")

set(THIRD_PARTY_LIBS zlib quazip wble qwtble)
foreach(file_i ${THIRD_PARTY_LIBS})
    configure_file(CMakeLists.txt.${file_i} ${file_i}/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} .
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${file_i})
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${file_i} )
endforeach(file_i)

# copy dlls for easy launch
file(GLOB_RECURSE DLLS "${CMAKE_BINARY_DIR}/*.dll" )
file(COPY ${DLLS} DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${DLLS} DESTINATION ${CMAKE_BINARY_DIR}/Tests)

# workaround for not working passing "CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<dir>" in ExternalProject_Add
set(workaround ON CACHE BOOL "" FORCE)

include_directories(${QWT_INCLUDE_PATH})
include_directories(${CMAKE_BINARY_DIR}/zlib/install/include)
include_directories(${CMAKE_BINARY_DIR}/quazip/install/include)
include_directories(${CMAKE_BINARY_DIR}/wble/install/include)
include_directories(${CMAKE_BINARY_DIR}/qwtble/install/include)

LINK_DIRECTORIES(${QWT_BINARY_PATH})
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/zlib/install/bin)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/quazip/install/lib)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/wble/install/lib)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/qwtble/install/lib)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(GUI)
include_directories(Import)
include_directories(Shared)
include_directories(Common)
include_directories(Datasets)
include_directories(ModelsAndViews)
include_directories(DataProvider)

add_subdirectory(Shared)
add_subdirectory(Common)
add_subdirectory(Datasets)
add_subdirectory(ModelsAndViews)
add_subdirectory(DataProvider)

set(${PROJECT_NAME}_SOURCES
    Volbx/main.cpp
    Volbx/Volbx.ico
    Volbx/VolbxProperties.h
    Volbx/VolbxProperties.rc
    GUI/About.cpp
    GUI/About.h
    GUI/About.ui
    GUI/CheckUpdatesDialog.cpp
    GUI/CheckUpdatesDialog.h
    GUI/CheckUpdatesDialog.ui
    GUI/DockTitleBar.cpp
    GUI/DockTitleBar.h
    GUI/DockTitleBar.ui
    GUI/DockWidget.cpp
    GUI/DockWidget.h
    GUI/Export.cpp
    GUI/Export.h
    GUI/Export.ui
    GUI/FiltersDock.cpp
    GUI/FiltersDock.h
    GUI/FiltersDock.ui
    GUI/MainTab.cpp
    GUI/MainTab.h
    GUI/MainTabWidget.cpp
    GUI/MainTabWidget.h
    GUI/PlotDockWidget.cpp
    GUI/PlotDockWidget.h
    GUI/SaveDatasetAs.cpp
    GUI/SaveDatasetAs.h
    GUI/SaveDatasetAs.ui
    GUI/TabBar.cpp
    GUI/TabBar.h
    GUI/ViewDockWidget.cpp
    GUI/ViewDockWidget.h
    GUI/VolbxMain.cpp
    GUI/VolbxMain.h
    GUI/VolbxMain.ui
    Import/ColumnsPreview.cpp
    Import/ColumnsPreview.h
    Import/DatasetDefinitionVisualization.cpp
    Import/DatasetDefinitionVisualization.h
    Import/DatasetDefinitionVisualization.ui
    Import/DatasetImportTab.cpp
    Import/DatasetImportTab.h
    Import/DatasetsListBrowser.cpp
    Import/DatasetsListBrowser.h
    Import/DatasetsListBrowser.ui
    Import/ImportData.cpp
    Import/ImportData.h
    Import/ImportData.ui
    Import/ImportTab.cpp
    Import/ImportTab.h
    Import/SpreadsheetsImportTab.cpp
    Import/SpreadsheetsImportTab.h
    Import/SpreadsheetsImportTab.ui
    Import/ColumnsPreview.cpp
    Import/ColumnsPreview.h
    Import/DatasetDefinitionVisualization.cpp
    Import/DatasetDefinitionVisualization.h
    Import/DatasetDefinitionVisualization.ui
    Import/DatasetImportTab.cpp
    Import/DatasetImportTab.h
    Import/DatasetsListBrowser.cpp
    Import/DatasetsListBrowser.h
    Import/DatasetsListBrowser.ui
    Import/ImportData.cpp
    Import/ImportData.h
    Import/ImportData.ui
    Import/ImportTab.cpp
    Import/ImportTab.h
    Import/SpreadsheetsImportTab.cpp
    Import/SpreadsheetsImportTab.h
    Import/SpreadsheetsImportTab.ui
   )

# remove background console window.
if(WIN32)
set(GUI_TYPE WIN32)
endif(WIN32)
if (APPLE)
set(GUI_TYPE MACOSX_BUNDLE)
endif (APPLE)

qt5_add_resources(RESOURCES Resources/Resources.qrc)
add_custom_target(generated_qrc DEPENDS ${RESOURCES})

add_executable(${PROJECT_NAME} ${GUI_TYPE} ${${PROJECT_NAME}_SOURCES} ${RESOURCES} Volbx/VolbxProperties.rc)
add_dependencies(${PROJECT_NAME} generated_qrc)

target_link_libraries(${PROJECT_NAME} shared common datasets modelsAndViews dataProvider wble qwtble Qt5::Core Qt5::Network Qt5::Widgets Qt5::Gui Qt5::Xml)
target_link_libraries(${PROJECT_NAME} optimized qwt)
target_link_libraries(${PROJECT_NAME} debug qwtd)
target_link_libraries(${PROJECT_NAME} optimized quazip5)
target_link_libraries(${PROJECT_NAME} debug quazip5d)

add_subdirectory(Update)

enable_testing()
add_subdirectory(Tests)
