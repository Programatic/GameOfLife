cmake_minimum_required (VERSION 3.0)
project (gameoflife)

set (CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets CONFIG REQUIRED)

set(SOURCE_DIRECTORY "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIRECTORY "${CMAKE_SOURCE_DIR}/include")

set(SOURCE_FILES
  "${SOURCE_DIRECTORY}/main.cpp"
  "${SOURCE_DIRECTORY}/gamewindow.cpp"
  "${SOURCE_DIRECTORY}/cell.cpp"
  )
set(HEADER_FILES
  "${INCLUDE_DIRECTORY}/gamewindow.hpp"
  "${INCLUDE_DIRECTORY}/cell.hpp"
  )

add_executable(gameoflife ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(gameoflife Qt5::Widgets)
