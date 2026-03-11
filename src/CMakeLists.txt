add_library(core
        ${CMAKE_SOURCE_DIR}/src/core/Candle.cpp
        ${CMAKE_SOURCE_DIR}/src/core/TimeSeries.cpp
        ${CMAKE_SOURCE_DIR}/src/core/CsvLoader.cpp
        ${CMAKE_SOURCE_DIR}/src/core/Exceptions.cpp)

target_include_directories(core
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include)

add_library(indicators
        ${CMAKE_SOURCE_DIR}/src/indicators/MovingAverage.cpp
        ${CMAKE_SOURCE_DIR}/src/indicators/DoubleMA.cpp)

target_include_directories(indicators
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(indicators
        PUBLIC
        core)

add_library(analysis
        ${CMAKE_SOURCE_DIR}/src/analysis/SpikeDetector.cpp
        ${CMAKE_SOURCE_DIR}/src/analysis/Statistics.cpp)

target_include_directories(analysis
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(analysis
        PUBLIC
        core)