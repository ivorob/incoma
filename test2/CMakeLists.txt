CMAKE_MINIMUM_REQUIRED (VERSION 3.4.3)
SET (PROJECT incoma-test2)
PROJECT (${PROJECT})

SET (CMAKE_AUTOMOC ON)
SET (CMAKE_AUTOUIC ON)
SET (CMAKE_CXX_STANDARD 11)

SET (SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
SET (INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
SET (UI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

SET (SOURCES
        ${SRC_DIR}/main.cpp
        ${SRC_DIR}/Logger.cpp
        ${SRC_DIR}/LogFileDumper.cpp
        ${SRC_DIR}/LoggerWidget.cpp
        ${SRC_DIR}/LoggerController.cpp
        ${SRC_DIR}/MainWindow.cpp
        ${SRC_DIR}/LoggerProvider.cpp
        ${SRC_DIR}/DataServer.cpp
        ${SRC_DIR}/DataClient.cpp
        ${SRC_DIR}/SafeThreadContainer.cpp
    )

SET (HEADERS
        ${INCLUDE_DIR}/Logger.h
        ${INCLUDE_DIR}/LogFileDumper.h
        ${INCLUDE_DIR}/LoggerWidget.h
        ${INCLUDE_DIR}/LoggerController.h
        ${INCLUDE_DIR}/MainWindow.h
        ${INCLUDE_DIR}/LoggerProvider.h
        ${INCLUDE_DIR}/DataServer.h
        ${INCLUDE_DIR}/DataClient.h
        ${INCLUDE_DIR}/SafeThreadContainer.h
    )

SET (UIS
        ${UI_DIR}/LoggerWidget.ui
        ${UI_DIR}/MainWindow.ui
    )

FIND_PACKAGE (Qt5 COMPONENTS Widgets Concurrent Network)

IF (APPLE)
    SET (CMAKE_OSX_ARCHITECTURES "x86_64")
ENDIF (APPLE)

IF (APPLE)
    ADD_EXECUTABLE (${PROJECT} MACOSX_BUNDLE ${SOURCES} ${HEADERS} ${UIS})
ELSE ()
    ADD_EXECUTABLE (${PROJECT} ${SOURCES} ${HEADERS} ${UIS})
ENDIF (APPLE)

TARGET_INCLUDE_DIRECTORIES (${PROJECT} PUBLIC ${INCLUDE_DIR})
TARGET_LINK_LIBRARIES (${PROJECT} Qt5::Widgets Qt5::Concurrent Qt5::Network Qt5::Core)

IF (APPLE)
ADD_CUSTOM_TARGET (run
        COMMAND open ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}.app
    )
ADD_DEPENDENCIES (run ${PROJECT})
ENDIF (APPLE)
