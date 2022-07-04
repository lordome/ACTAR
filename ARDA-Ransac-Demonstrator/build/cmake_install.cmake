# Install script for directory: /home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/arda" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/arda")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/arda"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/arda")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/arda" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/arda")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/arda"
         OLD_RPATH "/home/lorenzo/root/lib:/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build:/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/leuvenPSA:/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/libMFM:/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/pClasses:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/arda")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libARDAlib.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libARDAlib.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libARDAlib.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/libARDAlib.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libARDAlib.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libARDAlib.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libARDAlib.so"
         OLD_RPATH "/home/lorenzo/root/lib:/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/libMFM:/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/pClasses:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libARDAlib.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/precalibrator" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/precalibrator")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/precalibrator"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/precalibrator")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/precalibrator" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/precalibrator")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/precalibrator"
         OLD_RPATH "/home/lorenzo/root/lib:/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build:/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/leuvenPSA:/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/libMFM:/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/pClasses:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/precalibrator")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/convert2txt" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/convert2txt")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/convert2txt"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/convert2txt")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/convert2txt" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/convert2txt")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/convert2txt"
         OLD_RPATH "/home/lorenzo/root/lib:/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build:/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/leuvenPSA:/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/libMFM:/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/pClasses:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/convert2txt")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/macro" TYPE DIRECTORY FILES "/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/KaitoMacro/")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/monitorScripts" TYPE DIRECTORY FILES "/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/monitorScripts/")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/scripts" TYPE DIRECTORY FILES "/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/scripts/")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/macro" TYPE FILE FILES
    "/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/inc/cLookupTable.h"
    "/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/inc/cRawEvent.h"
    "/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/inc/cHit.h"
    "/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/inc/cRawSignal.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES
    "/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/GecoTable.dat"
    "/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/PSATable.dat"
    "/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/preprocessorTable"
    "/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/rootlogon.C"
    "/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/loadARDA.sh"
    "/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/libdict_rdict.pcm"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/libMFM/cmake_install.cmake")
  include("/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/pClasses/cmake_install.cmake")
  include("/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/leuvenPSA/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
