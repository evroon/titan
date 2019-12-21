# Install script for directory: C:/Users/Daniel/Desktop/OpenGL/Libraries/bullet3-2.87/examples/ThirdPartyLibs/clsocket

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/BULLET_PHYSICS")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/Daniel/Dropbox/Programming/Titan/Titan Designer/Bullet/lib/Debug/clsocket_Debug.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/Daniel/Dropbox/Programming/Titan/Titan Designer/Bullet/lib/Release/clsocket.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/Daniel/Dropbox/Programming/Titan/Titan Designer/Bullet/lib/MinSizeRel/clsocket_MinsizeRel.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/Daniel/Dropbox/Programming/Titan/Titan Designer/Bullet/lib/RelWithDebInfo/clsocket_RelWithDebugInfo.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "C:/Users/Daniel/Desktop/OpenGL/Libraries/bullet3-2.87/examples/ThirdPartyLibs/clsocket/src/ActiveSocket.h"
    "C:/Users/Daniel/Desktop/OpenGL/Libraries/bullet3-2.87/examples/ThirdPartyLibs/clsocket/src/Host.h"
    "C:/Users/Daniel/Desktop/OpenGL/Libraries/bullet3-2.87/examples/ThirdPartyLibs/clsocket/src/PassiveSocket.h"
    "C:/Users/Daniel/Desktop/OpenGL/Libraries/bullet3-2.87/examples/ThirdPartyLibs/clsocket/src/SimpleSocket.h"
    "C:/Users/Daniel/Desktop/OpenGL/Libraries/bullet3-2.87/examples/ThirdPartyLibs/clsocket/src/StatTimer.h"
    )
endif()

