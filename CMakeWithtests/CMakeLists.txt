cmake_minimum_required(VERSION 3.11)

option(BuildUnitTestExecutables "Build executables for unit tests" ON)
option(BuildExecutable "Builds the projects in the Devices folder" OFF)

#Note: It's important to set the system to Generic to allow cross-compile
set(CMAKE_SYSTEM_NAME Generic)

if(IsBuildingArduinoPro328)
	set(IsBuildingEmbeddedApp ON)
	set(MMCU atmega328p)
	add_definitions(-Dmmcu=${MMCU})
	add_definitions(-DIsBuildingArduinoPro328=${IsBuildingArduinoPro328})
	add_definitions(-DARDUINO=10805)
	add_definitions(-DARDUINO_AVR_PRO)
	add_definitions(-DARDUINO_ARCH_AVR)
	add_definitions(-D__AVR_ATmega328P__=1)
	add_definitions(-DBAUD=9600)
	add_definitions(-DF_CPU=8000000L)	
elseif(BuildUnitTestExecutable)
	set(IsBuildingEmbeddedApp OFF)
else()
	message(ERROR "Unknown board being compiled for. Will probably fail because we don't have some #defines setup.")
endif()

set(AVR_OBJCOPY "${AVR_TOOLCHAIN_PATH}/avr-objcopy.exe")
set(AVR_SIZE	"${AVR_TOOLCHAIN_PATH}/avr-size.exe")
set(AVR_DUMP	"${AVR_TOOLCHAIN_PATH}/avr-objdump.exe")
set(AVRDUDE		"C:/Program Files (x86)/Arduino/hardware/tools/avr/bin/avrdude.exe") #TODO: Figure out how to use this without needing arduino installed

set(PROGRAMMER "usbtiny")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_SYSTEM_NAME Generic)
project(cmake-avr-demo CXX)

set(ELF_FILE "${PROJECT_NAME}.elf")
set(HEX_FILE "${PROJECT_NAME}.hex")
set(EEP_FILE "${PROJECT_NAME}.eep")

# Files to be compiled
set(AppDeviceSrcFiles 
	"App/AppDevice/src/main.cpp"
)

set(AppLogicSrcFiles 
	"App/AppLogic/src/AppRunner.cpp"
	"App/AppLogic/src/AppSettings.cpp"
)

set(DeviceUtilsSrcFiles 
	"DeviceUtils/src/TransceiverUtils.cpp"
	"DeviceUtils/src/TransceiverWrapper.cpp"
	"DeviceUtils/src/AvrTimeUtilFunctions.cpp"
)

set(EntitiesSrcFiles 
	"Entities/src/ReceivedData.cpp"
	)

set(MessageHandlingSrcFiles 
	"MessageHandling/src/BinarySerializableMessageBase.cpp"
	"MessageHandling/src/BinarySerializableMessageUtils.cpp"
	"MessageHandling/src/Messages/Device/DeviceBroadcastMessage.cpp"
)

set(Rfm69AvrWrapperSrcFiles 
	"Rfm69AvrWrapper/src/Rfm69AvrController.cpp"
)

set(UtilitiesSrcFiles 
	"Utilities/src/IntByteConversionUtilities.cpp"
	"Utilities/src/UnsafeByteArraySlice.cpp"
)

# Just include the root. This way everything can #include from the root without needing "../" all over the place
# Ex: #include <MessageHandling/include/MessageType.h>
include_directories(
	"./"
)

if(IsBuildingEmbeddedApp)
	add_executable(${PROJECT_NAME} 
		${AppDeviceSrcFiles} 
		${AppLogicSrcFiles} 
		${DeviceUtilsSrcFiles} 
		${EntitiesSrcFiles} 
		${MessageHandlingSrcFiles}
		${Rfm69AvrWrapperSrcFiles}
		${UtilitiesSrcFiles}
		${RFM69LibraryAVRSrcFiles}
		)

	#Since this is a C project, things were all funky when the individual files are added to this build. For now, just let it have its own CMake project
	#TODO: Revisit compiling here instead of its own project later on
	add_subdirectory("ThirdParty/RFM69-Library-AVR" "ThirdParty/RFM69-Library-AVR/bin")

	target_link_libraries(${PROJECT_NAME} PUBLIC RFM69-Library-AVR)

	function(add_additional_clean_file FILENAME)
		get_directory_property(EXTRA_CLEAN_FILES ADDITIONAL_MAKE_CLEAN_FILES)
		set_directory_properties(
			PROPERTIES
			ADDITIONAL_MAKE_CLEAN_FILES "${EXTRA_CLEAN_FILES};${FILENAME}"
		)

		set_source_files_properties(${FILENAME} PROPERTIES GENERATED TRUE)
	endfunction(add_additional_clean_file)

	add_additional_clean_file(${ELF_FILE})
	add_additional_clean_file(${HEX_FILE})
	add_additional_clean_file(${EEP_FILE})

	set_target_properties(
		${PROJECT_NAME}
		PROPERTIES
		OUTPUT_NAME ${ELF_FILE}
		COMPILE_FLAGS "-mmcu=${MMCU} -I. -gdwarf-2 -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wall -Wno-unknown-pragmas -Wundef -std=gnu++17 -Wl,--gc-section -B \"C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.2.209/gcc/dev/atmega328p\""
		LINK_FLAGS "-mmcu=${MMCU} -I. -gdwarf-2 -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wall -Wno-unknown-pragmas -Wundef -std=gnu++17 -Wundef -MD -MP -MF .dep/main.elf.d --output ${ELF_FILE} -Wl,-lc,--entry=main"
	)

	add_custom_command(
		TARGET ${PROJECT_NAME}
		POST_BUILD
		COMMAND ${AVR_OBJCOPY} -O ihex -R .eeprom ${ELF_FILE} ${HEX_FILE}
		COMMAND ${AVR_OBJCOPY} -O ihex -j .eeprom --set-section-flags=.eeprom="alloc,load" --no-change-warnings --change-section-lma .eeprom=0 ${ELF_FILE} ${EEP_FILE}
		COMMAND ${AVR_SIZE} ${ELF_FILE} --mcu=${MMCU} --format=avr
	)

	add_custom_target("UploadAppToDevice"
		${AVRDUDE} -C"${AvrDudeConfigFile}" -v -p${MMCU} -carduino -P${DeploymentPort} -b57600 -D -U"flash:w:${HEX_FILE}:i"
		DEPENDS ${PROJECT_NAME}
		COMMENT "Uploading")

elseif(BuildUnitTestExecutable)
	add_subdirectory("ThirdParty/GoogleTest" "ThirdParty/GoogleTest/bin")
	
	#Note: file extension seems to be needed for this to work on Windows. Haven't tested it on Linux with no extension, but it probably works. I think.
	set(TestExecutable "${PROJECT_NAME}Tests.exe")
	set(MessageHandlingTestFiles 
		"MessageHandling/test/Messages/Device/DeviceBroadcastMessageTests.cpp"
		"MessageHandling/test/BinarySerializableMessageUtilsTests.cpp"
	)

	set(UtilitiesTestFiles 
		"Utilities/test/IntByteConversionUtilitiesTests.cpp"
	)

	enable_testing()
	add_executable(${TestExecutable} 
		${MessageHandlingTestFiles} 
		${MessageHandlingSrcFiles}
		${UtilitiesTestFiles} 
		${UtilitiesSrcFiles}	
		${EntitiesSrcFiles}
	)
	
	target_link_libraries(${TestExecutable} gtest_main)
	add_test(NAME "${PROJECT_NAME}Tests" COMMAND "${TestExecutable}")
endif(IsBuildingEmbeddedApp)
