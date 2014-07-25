cmake_minimum_required(VERSION 2.6)
project (Typing_of_the_Stars)
set (Typing_of_the_Stars_VERSION_MAJOR 0)
set (Typing_of_the_Stars_VERSION_MINOR 2)

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set (TOTS_SOURCE_DIR src)

if(WIN32)
  set(CMAKE_CXX_FLAGS "/EHsc")
else(WIN32)
  set(CMAKE_CXX_FLAGS "-g -Wall")
endif(WIN32)

if(WIN32)
  find_program(ZIP_EXECUTABLE 7z PATHS "$ENV{ProgramFiles}/7-Zip") 
    if(ZIP_EXECUTABLE)
      file(TO_NATIVE_PATH "${ZIP_EXECUTABLE}" ZIP_EXECUTABLE)
    else(ZIP_EXECUTABLE)
      message(FATAL_ERROR "Could not find 7zip executable. Please install 7zip.")
    endif(ZIP_EXECUTABLE)
  set(VS_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vs_libs")
  file(TO_NATIVE_PATH "${VS_LIB_DIR}" VS_LIB_DIR_NATIVE)
endif(WIN32)

if(WIN32)
  set(SDL2_VERSION 2.0.3)
  set(SDL2_ARCHIVE "SDL2-devel-${SDL2_VERSION}-VC.zip")
  set(SDL2_ARCHIVE_PATH "${VS_LIB_DIR}/${SDL2_ARCHIVE}")

  if(NOT EXISTS "${VS_LIB_DIR}/${SDL2_ARCHIVE}")
    message("Downloading '${SDL2_ARCHIVE}' to '${VS_LIB_DIR_NATIVE}'...")
    file(DOWNLOAD
         "https://www.libsdl.org/release/SDL2-devel-${SDL2_VERSION}-VC.zip"
         "${SDL2_ARCHIVE_PATH}"
         SHOW_PROGRESS)
  endif()
  if(NOT EXISTS "${VS_LIB_DIR}/SDL2-${SDL2_VERSION}")
    message("Extracting '${SDL2_ARCHIVE}'...")
    file(TO_NATIVE_PATH "${SDL2_ARCHIVE_PATH}" SDL2_ARCHIVE_PATH_NATIVE)
    execute_process(COMMAND "${ZIP_EXECUTABLE}" x -tzip "-o${VS_LIB_DIR_NATIVE}" "${SDL2_ARCHIVE_PATH_NATIVE}")
  endif()
endif(WIN32)
find_package(SDL2 REQUIRED)

find_package(GLEW REQUIRED)

find_package(OpenGL REQUIRED)

if(WIN32)
  set(BOOST_VERSION 1_55_0)
  set(BOOST_ARCHIVE "boost_${BOOST_VERSION}.zip")
  set(BOOST_ARCHIVE_PATH "${VS_LIB_DIR}/${BOOST_ARCHIVE}")
  if(NOT EXISTS "${BOOST_ARCHIVE_PATH}")
    message("Downloading '${BOOST_ARCHIVE}' to '${VS_LIB_DIR_NATIVE}'...")
    file(DOWNLOAD
         "http://softlayer-dal.dl.sourceforge.net/project/boost/boost/1.55.0/${BOOST_ARCHIVE}"
         "${BOOST_ARCHIVE_PATH}"
         SHOW_PROGRESS)
  endif()
  set(BOOST_ROOT "${VS_LIB_DIR}/boost_${BOOST_VERSION}")
  if(NOT EXISTS "${BOOST_ROOT}")
    message("Extracting '${BOOST_ARCHIVE}'...")
    file(TO_NATIVE_PATH "${BOOST_ARCHIVE_PATH}" BOOST_ARCHIVE_PATH_NATIVE)
    execute_process(COMMAND "${ZIP_EXECUTABLE}" x -tzip "-o${VS_LIB_DIR_NATIVE}" "${BOOST_ARCHIVE_PATH_NATIVE}")
  endif()
  if(NOT EXISTS "${BOOST_ROOT}/b2.exe")
    execute_process(COMMAND "bootstrap.bat"
                    WORKING_DIRECTORY "${BOOST_ROOT}")
  endif()
  if(NOT EXISTS "${BOOST_ROOT}/lib")
    execute_process(COMMAND "b2.exe" install "--prefix=${BOOST_ROOT}"
                    WORKING_DIRECTORY "${BOOST_ROOT}")
  endif()
endif(WIN32)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS exception filesystem system)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${GLEW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${SDL2_INCLUDE_DIR})
#link_directories(${Boost_LIBRARY_DIRS} ${CMAKE_SOURCE_DIR}/vs_libs/glew-1.10.0/lib/Release/Win32 ${CMAKE_SOURCE_DIR}/vs_libs/SDL2-2.0.3/lib/x86)

add_subdirectory("${TOTS_SOURCE_DIR}")
