CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(bootcp)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_BUILD_TYPE Debug)

SET(SRC_DIR src)
SET(INC_DIR include)

SET(LIB_FILES ${SRC_DIR}/tcpconn.cpp
    ${SRC_DIR}/sslconn.cpp
    ${SRC_DIR}/simplemsg.cpp
    ${SRC_DIR}/httpparser.cpp
    ${SRC_DIR}/httpmsgid.cpp
    ${SRC_DIR}/httpmsg.cpp
    ${SRC_DIR}/httprequest.cpp
    ${SRC_DIR}/httpresponse.cpp
    ${SRC_DIR}/httpclient.cpp
    ${SRC_DIR}/url.cpp
    ${SRC_DIR}/httpserver.cpp)

INCLUDE_DIRECTORIES(${SRC_DIR} ${INC_DIR})

SET(WITH_SSL 1)

SET(LIB_LIST bootcp)

if (WITH_SSL)
    FIND_PACKAGE(OpenSSL REQUIRED)
    INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})
    LIST(APPEND LIB_LIST ${OPENSSL_LIBRARIES})
ENDIF()

ADD_EXECUTABLE(main ${SRC_DIR}/main.cpp)

IF (WIN32)
    ADD_LIBRARY(bootcp ${LIB_FILES})
	TARGET_LINK_LIBRARIES(main ${LIB_LIST})
ELSE ()
    ADD_LIBRARY(bootcp ${LIB_FILES})
    LIST(APPEND LIB_LIST pthread)
	TARGET_LINK_LIBRARIES(main ${LIB_LIST})
ENDIF ()
