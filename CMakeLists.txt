cmake_minimum_required(VERSION 2.8)

set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_C_COMPILER "arm-vita-eabi-gcc")
set(CMAKE_CXX_COMPILER "arm-vita-eabi-g++")

project(ime_test)
set(TITLE_ID "IMET00001")
set(TITLE_NAME "IME test")

set(CMAKE_C_FLAGS "-Wl,-q -Wall -std=gnu11")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g3") # added to gcc flags when MAKE_BUILD_TYPE=Debug
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -fno-rtti -fno-exceptions")

include_directories(
)

link_directories(
	${CMAKE_CURRENT_BINARY_DIR}
)

if (NOT ${RELEASE})
	add_definitions(-DENABLE_LOGGING)
endif()

add_executable(${PROJECT_NAME}.elf
	src/main.c
)

# this is a mess lol
target_link_libraries(${PROJECT_NAME}.elf
	SceLibKernel_stub
	vita2d
	SceDisplay_stub
	SceGxm_stub
	SceSysmodule_stub
	SceAppUtil_stub
	SceCommonDialog_stub
	SceIme_stub
	png
	jpeg
	z
	m
	c
	SceAppMgr_stub
	SceProcessmgr_stub
)

add_custom_target(${PROJECT_NAME}.velf ALL
	COMMAND vita-elf-create ${PROJECT_NAME}.elf ${PROJECT_NAME}.velf
)

add_custom_target(eboot.bin ALL
	COMMAND vita-make-fself ${PROJECT_NAME}.velf eboot.bin
)

add_custom_target(${PROJECT_NAME}.vpk ALL
	COMMAND vita-mksfoex -s TITLE_ID=${TITLE_ID} "${TITLE_NAME}" param.sfo
	COMMAND vita-pack-vpk -s param.sfo -b eboot.bin ${PROJECT_NAME}.vpk
)

add_custom_target(vpksend
	COMMAND curl -T ${PROJECT_NAME}.vpk ftp://$(PSVITAIP):1337/ux0:/
	DEPENDS ${PROJECT_NAME}.vpk
)

add_custom_target(send
	COMMAND curl -T eboot.bin ftp://$(PSVITAIP):1337/ux0:/app/${TITLE_ID}/
	DEPENDS eboot.bin
)

add_dependencies(${PROJECT_NAME}.velf ${PROJECT_NAME}.elf)
add_dependencies(eboot.bin ${PROJECT_NAME}.velf)
add_dependencies(${PROJECT_NAME}.vpk eboot.bin)
