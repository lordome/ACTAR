# CMakeLists.txt for ARDA project. It creates a library with dictionary and a main program
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(ARDA)

set(CMAKE_CXX_STANDARD 14)
# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS RIO Net)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc ${CMAKE_CURRENT_SOURCE_DIR}/libMFM ${CMAKE_CURRENT_SOURCE_DIR}/pClasses/inc ${CMAKE_CURRENT_SOURCE_DIR}/leuvenPSA/inc)

add_subdirectory(libMFM)
add_subdirectory(pClasses)
add_subdirectory(leuvenPSA)

file(GLOB MY_SRC_FILES "src/c*")
file(GLOB MAIN_SRC     "src/main.cc")
file(GLOB PRECAL_SRC   "src/precalibrator.cc")
file(GLOB CONV_SRC     "src/convert2txt.cc")

# ROOT_GENERATE_DICTIONARY(G__cHit cHit.h LINKDEF LinkDef.h)
# ROOT_GENERATE_DICTIONARY(G__cRawEvent cRawEvent.h LINKDEF LinkDef.h)
# ROOT_GENERATE_DICTIONARY(G__cLookupTable cLookupTable.h LINKDEF LinkDef.h)
# add_library(ARDAobj SHARED G__cHit.cxx G__cRawEvent.cxx G__cLookupTable.cxx)
# target_link_libraries(ARDAobj ${ROOT_LIBRARIES})

option(INCRAWSIG "Include raw signal into the output TTree" OFF)
if(INCRAWSIG)
    add_definitions(-DINCRAWSIG)
endif(INCRAWSIG)

ROOT_GENERATE_DICTIONARY(G__dict cHit.h cRawEvent.h cLookupTable.h cRawSignal.h cPhysicalHit.h cPhysicalEvent.h cFittedLine.h cFittedEvent.h cVertex.h LINKDEF LinkDef.h)
add_library(ARDAlib SHARED G__dict.cxx ${MY_SRC_FILES} libMFM pClasses leuvenPSA)
target_link_libraries(ARDAlib MFM pClasses ${ROOT_LIBRARIES})

add_executable(arda ${MAIN_SRC})
target_link_libraries(arda ARDAlib ${ROOT_LIBRARIES} leuvenPSA stdc++fs)

add_executable(precalibrator ${PRECAL_SRC})
target_link_libraries(precalibrator ARDAlib ${ROOT_LIBRARIES} leuvenPSA stdc++fs)

add_executable(convert2txt ${CONV_SRC})
target_link_libraries(convert2txt ARDAlib ${ROOT_LIBRARIES} leuvenPSA stdc++fs)


install(TARGETS arda ARDAlib         DESTINATION bin/)
install(TARGETS precalibrator        DESTINATION bin/)
install(TARGETS convert2txt          DESTINATION bin/)

install(DIRECTORY KaitoMacro/        DESTINATION bin/macro)
install(DIRECTORY monitorScripts/    DESTINATION bin/monitorScripts)
install(DIRECTORY scripts/    DESTINATION bin/scripts)
install(FILES inc/cLookupTable.h inc/cRawEvent.h inc/cHit.h inc/cRawSignal.h DESTINATION bin/macro)
install(FILES GecoTable.dat PSATable.dat preprocessorTable rootlogon.C loadARDA.sh ${CMAKE_BINARY_DIR}/libdict_rdict.pcm DESTINATION bin/)
