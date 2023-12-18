cmake_minimum_required(VERSION 3.10)

project(RingworldStage2 VERSION 1.0)


#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_EXE_LINKER_FLAGS "${GCC_COVERAGE_LINK_FLAGS}")


find_package(SDL2 REQUIRED SDL2)
find_package(SDL2_image REQUIRED SDL2_image)
#find_package(SDL2_mixer REQUIRED SDL2_mixer)
#find_package(udev REQUIRED udev)

option(DEBUG_SHOWVRAM "Displays the contents of VDP1/2 vram" OFF)
option(SOFTWARE_RASTER "Use software raster instead of OpenGL" ON)
option(LITTLE_ENDIAN "Modifies reading of Saturn Big Endian files to swap for x86" ON)

file(GLOB_RECURSE SRCS src/*.c include/*.c thirdparty/*.c)

add_executable(RingworldStage2 ${SRCS})

#set_property(TARGET RingworldStage2 PROPERTY POSITION_INDEPENDENT_CODE FALSE)

target_include_directories(RingworldStage2 PUBLIC "include/")
target_include_directories(RingworldStage2 PUBLIC "thirdparty/")
target_include_directories(RingworldStage2 PUBLIC "thirdparty/gl3w")

target_link_libraries(RingworldStage2 PRIVATE SDL2 SDL2_image m)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

if(DEBUG_SHOWVRAM)
set_target_properties(RingworldStage2 PROPERTIES COMPILE_DEFINITIONS "SHOW_VRAM=1")
endif()

if(SOFTWARE_RASTER)
set_target_properties(RingworldStage2 PROPERTIES COMPILE_DEFINITIONS "SOFTWARE_RASTER=1")
endif()

if(LITTLE_ENDIAN)
set_target_properties(RingworldStage2 PROPERTIES COMPILE_DEFINITIONS "SSV_SWAP_ENDIAN=1")
endif()



