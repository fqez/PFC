cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
include (FindPkgConfig)

set(GUI_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/gui)

find_package(Qt4 REQUIRED)
find_package (OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

set(QT_USE_QTOPENGL TRUE)

include(${QT_USE_FILE})

# Qwt include directory
set(QWT_INCLUDE_DIR /usr/include/qwt-qt4)
if(NOT QWT_INCLUDE_DIR)
  message(FATAL_ERROR "QWT_INCLUDE_DIR should be set to a path like ")
endif()
include_directories(${QWT_INCLUDE_DIR})

# Qwt libraries
set(QWT_LIBRARY /usr/lib/libqwt-qt4.so.5)
if(NOT QWT_LIBRARY)
  message(FATAL_ERROR "QWT_LIBRARY should be set to a filepath like ")
endif()

set(qwt_libraries ${QWT_LIBRARY})

find_package(OpenCV REQUIRED)

SET(qt_SOURCES main.cpp 
			   control/threadcontrol.cpp gui/threadgui.cpp gui/gui.cpp shared.cpp gui/glWidget.cpp 
)

SET(qt_HEADERS
			   control/threadcontrol.h gui/threadgui.h gui/gui.h shared.h gui/glWidget.h gui/LCDNumber.h
)

#SET(qt_FORMS gui/gui.ui)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

QT4_WRAP_CPP(qt_HEADERS_MOC ${qt_HEADERS})
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

MESSAGE(${QT_INCLUDES})

SET(DEPS_DIR /home/fran/test/JdeRobot/Deps)
SET(INTERFACES_CPP /home/fran/test/JdeRobot/src/stable/interfaces/cpp)
SET(JDE_LIBS /home/fran/test/JdeRobot/src/stable/libs)

include(${DEPS_DIR}/ice/CMakeLists.txt)
include(${DEPS_DIR}/pthread/CMakeLists.txt)
include_directories(
	${INTERFACES_CPP}
	${JDE_LIBS}
)

add_executable(nao_control 
	control/control.cpp
	shared.cpp
	gui/glWidget.cpp
	main.cpp
	raked_search.cpp	
	${qt_SOURCES} 
	${qt_FORMS} 
	${qt_HEADERS_MOC})

target_link_libraries(nao_control 
	${QT_LIBRARIES}
	GL 
	GLU 
	glut
	${qwt_libraries}
	${OpenCV_LIBS}
	${ZeroCIce_LIBRARIES}
	${INTERFACES_CPP}/jderobot/libJderobotInterfaces.so
	${JDE_LIBS}/parallelIce/libparallelIce.so
	${JDE_LIBS}/jderobotutil/libjderobotutil.so
	${CMAKE_THREAD_LIBS_INIT}
)
