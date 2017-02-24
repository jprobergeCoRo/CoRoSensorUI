# - Try to find QtSerialPort (an add-on for the Qt5 library, providing a 
# single interface for both hardware and virtual serial ports.) 
# Once done this will define 
# 
#  QtSerialPort_FOUND - system has QtSerialPort 
#  QtSerialPort_INCLUDE_DIR - the QtSerialPort include directory 
#  QtSerialPort_LIBRARIES - the libraries needed to use QtSerialPort 
# 
# Copyright (c) 2012, Oleg Linkin, <MaledictusDeMagog@gmail.com> 
# 
# Redistribution and use is allowed according to the terms of the BSD license. 
# For details see the accompanying COPYING-CMAKE-SCRIPTS file. 
 
IF (QtSerialPort_INCLUDE_DIR AND QtSerialPort_LIBRARIES)
	# in cache already
	SET (QtSerialPort_FOUND TRUE) 
ELSE (QtSerialPort_INCLUDE_DIR AND QtSerialPort_LIBRARIES)
	FIND_LIBRARY (QtSerialPort_LIBRARIES
					NAMES
					SerialPort
					PATH_SUFFIXES
					qt5
					)
	FIND_PATH (QtSerialPort_INCLUDE_DIR QtSerialPort/qserialport.h
				PATHS
				${QtSerialPort_DIR}/include
				ENV PATH
				)
ENDIF ()
 
IF (QtSerialPort_LIBRARIES AND QtSerialPort_INCLUDE_DIR) 
	      SET (QtSerialPort_FOUND 1)
ENDIF ()
 
IF (QtSerialPort_FOUND)
	      MESSAGE (STATU "Found the QtSerialPort libraries at: ${QtSerialPort_LIBRARIES}")
	      MESSAGE (STATU "Found QtSerialPort headers at: ${QtSerialPort_INCLUDE_DIR}")
ELSE (QtSerialPort_FOUND)
	      IF (QtSerialPort_FIND_REQUIRED)
	              MESSAGE (FATAL_ERRO "Could NOT find required QtSerialPort library, aborting")
	      ELSE (QtSerialPort_FIND_REQUIRED)
	              MESSAGE (STATU "Could NOT find QtSerialPort")
	      ENDIF ()
ENDIF ()