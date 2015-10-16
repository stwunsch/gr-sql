find_package(PkgConfig)
pkg_check_modules(PC_MYSQL QUIET libmysqlclient)

find_path(
    MYSQL_INCLUDE_DIRS
    NAMES mysql.h
    HINTS ${PC_MYSQL_INCLUDEDIR}
          ${PC_MYSQL_INCLUDE_DIRS}
)

if(APPLE)
    find_library(
        MYSQL_LIBRARIES
        NAMES libmysql.dylib
        HINTS ${PC_LIBXML_LIBDIR}
              ${PC_LIBXML_LIBRARY_DIRS}
    )
else(APPLE)
    find_library(
        MYSQL_LIBRARIES
        NAMES libmysqlclient.so
        HINTS ${PC_LIBXML_LIBDIR}
              ${PC_LIBXML_LIBRARY_DIRS}
    )
endif(APPLE)

#message(WARNING "MYSQL INCLUDE " ${MYSQL_INCLUDE_DIRS})
#message(WARNING "MYSQL LIBRARY " ${MYSQL_LIBRARIES})

if(MYSQL_LIBRARIES AND MYSQL_INCLUDE_DIRS)
    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(MYSQL DEFAULT_MSG MYSQL_LIBRARIES MYSQL_INCLUDE_DIRS)
    mark_as_advanced(MYSQL_LIBRARIES MYSQL_INCLUDE_DIRS)
else()
    message(FATAL_ERROR "Mysql client library is required, but was not found.")
endif()
