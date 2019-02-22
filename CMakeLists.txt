CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(bootcp)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_BUILD_TYPE Debug)

SET(SRC_DIR src)
SET(INC_DIR include)

SET(LIB_FILES ${SRC_DIR}/ssl.cpp
    ${SRC_DIR}/bootcp.cpp
    ${SRC_DIR}/client.cpp
    ${SRC_DIR}/server.cpp
    ${SRC_DIR}/msg.cpp
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

if (WITH_SSL)
    FIND_PACKAGE(OpenSSL REQUIRED)
    INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})
ENDIF

ADD_EXECUTABLE(main ${SRC_DIR}/main.cpp)

IF (WIN32)
    ADD_LIBRARY(bootcp ${LIB_FILES})
    if (WITH_SSL)
        TARGET_LINK_LIBRARIES(${OPENSSL_LIBRARIES})
    ENDIF
	TARGET_LINK_LIBRARIES(main ${LIB_LIST})
ELSE ()
    ADD_LIBRARY(bootcp SHARED ${LIB_FILES})
    if (WITH_SSL)
        TARGET_LINK_LIBRARIES(${OPENSSL_LIBRARIES})
    ENDIF
	TARGET_LINK_LIBRARIES(main bootcp pthread)
ENDIF ()
