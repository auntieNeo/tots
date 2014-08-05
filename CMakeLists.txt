cmake_minimum_required(VERSION 2.6)
project (Typing_of_the_Stars)
set (Typing_of_the_Stars_VERSION_MAJOR 0)
set (Typing_of_the_Stars_VERSION_MINOR 2)

enable_testing()

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(WIN32)
  set(CMAKE_CXX_FLAGS "/EHsc")
  add_definitions(-DWIN32)
else(WIN32)
  set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")
  add_definitions(-DLINUX)
endif(WIN32)

if(WIN32)
  # FIXME: this won't find 64bit 7z if cmake is 32bit
  find_program(ZIP_EXECUTABLE 7z PATHS "$ENV{ProgramFiles}/7-Zip" "$ENV{ProgramFiles(X86)}/7-Zip") 
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
  set(SDL2_DIR "${VS_LIB_DIR}/SDL2-${SDL2_VERSION}")
  if(NOT EXISTS "${SDL2_DIR}")
    message("Extracting '${SDL2_ARCHIVE}'...")
    file(TO_NATIVE_PATH "${SDL2_ARCHIVE_PATH}" SDL2_ARCHIVE_PATH_NATIVE)
    execute_process(COMMAND "${ZIP_EXECUTABLE}" x -tzip "-o${VS_LIB_DIR_NATIVE}" "${SDL2_ARCHIVE_PATH_NATIVE}")
  endif()
  set(CMAKE_INCLUDE_PATH "${SDL2_DIR}/include;${CMAKE_INCLUDE_PATH}")
  set(CMAKE_LIBRARY_PATH "${SDL2_DIR}/lib/x86;${CMAKE_LIBRARY_PATH}")
# FIXME: need to distinguish 32 bit and 64 bit builds
#  set(CMAKE_LIBRARY_PATH "${SDL2_DIR}/lib/x64;${CMAKE_LIBRARY_PATH}")
endif(WIN32)
find_package(SDL2 REQUIRED)

if(WIN32)
  set(GLEW_VERSION 1.10.0)
  set(GLEW_ARCHIVE "glew-${GLEW_VERSION}-win32.zip")
  set(GLEW_ARCHIVE_PATH "${VS_LIB_DIR}/${GLEW_ARCHIVE}")
  if(NOT EXISTS "${GLEW_ARCHIVE_PATH}")
    message("Downloading '${GLEW_ARCHIVE}' to '${VS_LIB_DIR_NATIVE}'...")
    file(DOWNLOAD
         "http://iweb.dl.sourceforge.net/project/glew/glew/${GLEW_VERSION}/glew-${GLEW_VERSION}-win32.zip"
         "${GLEW_ARCHIVE_PATH}"
         SHOW_PROGRESS)
  endif()
  set(GLEW_DIR "${VS_LIB_DIR}/glew-${GLEW_VERSION}")
  if(NOT EXISTS "${GLEW_DIR}")
    message("Extracting '${GLEW_ARCHIVE}'...")
    file(TO_NATIVE_PATH "${GLEW_ARCHIVE_PATH}" GLEW_ARCHIVE_PATH_NATIVE)
    execute_process(COMMAND "${ZIP_EXECUTABLE}" x -tzip "-o${VS_LIB_DIR_NATIVE}" "${GLEW_ARCHIVE_PATH_NATIVE}")
  endif()
  set(CMAKE_INCLUDE_PATH "${GLEW_DIR}/include;${CMAKE_INCLUDE_PATH}")
  set(CMAKE_LIBRARY_PATH "${GLEW_DIR}/lib/Release/Win32;${CMAKE_LIBRARY_PATH}")
# FIXME: need to distinguish 32 bit and 64 bit builds
#  set(CMAKE_LIBRARY_PATH "${GLEW_DIR}/lib/Release/x64;${CMAKE_LIBRARY_PATH}")
endif(WIN32)
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
  set(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_MULTITHREADED ON)
endif(WIN32)
find_package(Boost REQUIRED COMPONENTS exception filesystem system unit_test_framework)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${GLEW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${SDL2_INCLUDE_DIR})
#link_directories(${Boost_LIBRARY_DIRS} ${CMAKE_SOURCE_DIR}/vs_libs/glew-1.10.0/lib/Release/Win32 ${CMAKE_SOURCE_DIR}/vs_libs/SDL2-2.0.3/lib/x86)

add_custom_target(documentation
    plantuml -v -o ./src/documentation/images '"./src/**.(c|cpp|dox|h)"'
    COMMAND doxygen)

add_subdirectory("./src")
