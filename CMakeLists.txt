cmake_minimum_required(VERSION 2.8.11 )
project ( CoRoSensorUI )

add_definitions(-std=gnu++11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#############
### FILES ###
#############

set ( SOURCES
	src/main.cpp
	src/mainwindow.cpp
	src/connection.cpp
	src/communicator.cpp
	src/graphics.cpp
	src/log.cpp
)

set ( MOC_HEADERS
	src/mainwindow.h
	src/communicator.h
	src/circular_buffer.h
	src/finger_data.h
)

set ( UIS
	src/mainwindow.ui
)

set ( RESOURCES
	images/images.qrc
)

###################
### DPENDENCIES ###
###################

find_package( FFTW REQUIRED )
include_directories( ${FFTW_INCLUDES} )

find_package( MathGL REQUIRED )
include_directories( ${MathGL_INCLUDE_DIRS} )

# Next lines needed for building all Qt projects
find_package( Qt5Widgets REQUIRED )
include_directories(${Qt5Widgets_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})
# Add compiler flags for building executables (-fPIE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

#qt5_add_resources( ${RESOURCES} ) 
#qt5_generate_moc( ${SOURCES} )
#qt5_wrap_cpp( CoRoSensorUI_HEADERS_MOC ${SOURCES} )
#qt5_wrap_ui( CoRoSensorUI_FORMS_HEADERS ${UIS} )

# find_package( QtSerialPort REQUIRED )
# include_directories( ${QtSerialPort_INCLUDE_DIR} )


#############
### BUILD ###
#############

add_executable( CoRoSensorUI
	${SOURCES}
	${MOC_SRCS}
	${RES_SOURCES}
	${UI_HEADERS}
)
qt5_use_modules( CoRoSensorUI SerialPort )
target_link_libraries( CoRoSensorUI
	Qt5::Widgets
	${FFTW_LIBRARIES}
	${MathGL_LIBRARIES}
)