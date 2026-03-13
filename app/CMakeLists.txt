option(OURPROJECT_ENABLE_QT "Build Qt GUI app when available" ON)

set(OURPROJECT_HAVE_QT OFF)
set(OURPROJECT_QT_LIBS "")

if (OURPROJECT_ENABLE_QT)
    find_package(Qt6 COMPONENTS Widgets Charts QUIET)
    if (Qt6_FOUND)
        set(OURPROJECT_HAVE_QT ON)
        set(OURPROJECT_QT_LIBS Qt6::Widgets Qt6::Charts)
    else()
        find_package(Qt5 COMPONENTS Widgets Charts QUIET)
        if (Qt5_FOUND)
            set(OURPROJECT_HAVE_QT ON)
            set(OURPROJECT_QT_LIBS Qt5::Widgets Qt5::Charts)
        endif()
    endif()
endif()

if (OURPROJECT_HAVE_QT)
    set(CMAKE_AUTOMOC ON)

    add_library(app_support
            ${CMAKE_SOURCE_DIR}/src/app/MainWindow.cpp
            ${CMAKE_SOURCE_DIR}/src/app/CandleChartView.cpp
            ${CMAKE_SOURCE_DIR}/src/app/AnalysisController.cpp
            ${CMAKE_SOURCE_DIR}/include/app/MainWindow.hpp
            ${CMAKE_SOURCE_DIR}/include/app/CandleChartView.hpp
            ${CMAKE_SOURCE_DIR}/include/app/AnalysisController.hpp)

    target_include_directories(app_support
            PUBLIC
            ${CMAKE_SOURCE_DIR}/include)

    target_link_libraries(app_support
            PUBLIC
            core
            indicators
            analysis
            ${OURPROJECT_QT_LIBS})

    add_executable(OurProject
            main.cpp)

    target_link_libraries(OurProject
            PRIVATE
            app_support)

    set_target_properties(OurProject
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
else()
    message(WARNING "Qt (Widgets+Charts) not found: building CLI app instead.")

    add_library(app_support
            ${CMAKE_SOURCE_DIR}/src/app/ChartRenderer.cpp)

    target_include_directories(app_support
            PUBLIC
            ${CMAKE_SOURCE_DIR}/include)

    target_link_libraries(app_support
            PUBLIC
            core
            indicators
            analysis)

    add_executable(OurProject
            main_cli.cpp)

    target_link_libraries(OurProject
            PRIVATE
            app_support)

    set_target_properties(OurProject
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()
