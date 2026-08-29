dnl ***************************************************************************
dnl @info   rpiserver ver.1.0
dnl @author Vladimir Roncevic <elektron.ronca@gmail.com>
dnl @brief  Process this file with autoconf to produce a configure script.
dnl ***************************************************************************
AC_INIT([rpiserver], [1.0.0], [elektron.ronca@gmail.com])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([subdir-objects foreign])
AM_SILENT_RULES([yes])

AC_PROG_CXX
AC_PROG_CC
AC_LANG([C++])

CXXFLAGS="-std=c++17 -Wall -Wextra -pedantic $CXXFLAGS"

AC_CHECK_LIB([pthread], [pthread_create], [
  PTHREAD_LIBS="-pthread"
  AC_SUBST([PTHREAD_LIBS])
], [
  AC_MSG_ERROR([POSIX threads library (pthread) is required but not found.])
])

PKG_CHECK_MODULES([NLOHMANN_JSON], [nlohmann_json], [
  AC_SUBST([NLOHMANN_JSON_CFLAGS])
  AC_SUBST([NLOHMANN_JSON_LIBS])
], [
  AC_CHECK_HEADER([nlohmann/json.hpp], [
    NLOHMANN_JSON_CFLAGS=""
    NLOHMANN_JSON_LIBS=""
    AC_SUBST([NLOHMANN_JSON_CFLAGS])
    AC_SUBST([NLOHMANN_JSON_LIBS])
  ], [
    AC_MSG_ERROR([nlohmann/json.hpp is required but not found. Please install nlohmann-json3-dev.])
  ])
])

GIT_REVISION=$(git -C .. rev-parse --short HEAD 2>/dev/null || echo "unknown")
RPI_SERVER_VERSION_WITH_REV="${PACKAGE_VERSION}-${GIT_REVISION}"
AC_MSG_NOTICE([RPI_SERVER_VERSION_WITH_REV is $RPI_SERVER_VERSION_WITH_REV])

AC_CONFIG_COMMANDS_PRE([
  if test -f src/rpi_server_config.h; then
    sed -i "s|^#define RPI_SERVER_VERSION \".*\"|#define RPI_SERVER_VERSION \"${RPI_SERVER_VERSION_WITH_REV}\"|" src/rpi_server_config.h
  else
    AC_MSG_ERROR([src/rpi_server_config.h does not exist. Ensure it is available.])
  fi
])

AC_CONFIG_FILES([
    Makefile
    src/Makefile
])
AC_OUTPUT
