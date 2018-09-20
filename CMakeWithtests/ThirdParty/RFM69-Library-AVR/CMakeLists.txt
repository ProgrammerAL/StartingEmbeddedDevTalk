cmake_minimum_required(VERSION 3.11)

if(IsBuildingArduinoPro328)
	set(MMCU atmega328p)
	add_definitions(-Dmmcu=${MMCU})
	add_definitions(-DIsBuildingArduinoPro328=${IsBuildingArduinoPro328})
	add_definitions(-DARDUINO=10805)
	add_definitions(-DARDUINO_AVR_PRO)
	add_definitions(-DARDUINO_ARCH_AVR)
	add_definitions(-D__AVR_ATmega328P__=1)
	add_definitions(-DBAUD=9600)
	add_definitions(-DF_CPU=8000000L)
else()
	message(WARNING "Unknown board being compiled for. Will probably fail because we don't have some #defines setup.")
endif()

# Sets the compiler Needs to come before the project function
set(CMAKE_SYSTEM_NAME Generic)

# Project setup
project (RFM69-Library-AVR C)

# Library Files to be compiled
set(LibFiles 
	"RFM69.c"
	"spi.c"
)

include_directories(
  ${RFM69AVR_SOURCE_DIR}
)

add_library(${PROJECT_NAME} STATIC ${LibFiles})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)


