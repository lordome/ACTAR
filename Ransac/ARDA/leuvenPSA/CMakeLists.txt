cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(leuvenPSA)

file(GLOB PC_FILES "src/*.cpp")

add_library(leuvenPSA SHARED ${PC_FILES})
target_link_libraries(leuvenPSA ${ROOT_LIBRARIES} -lm -lstdc++)
install(TARGETS leuvenPSA DESTINATION bin/)
