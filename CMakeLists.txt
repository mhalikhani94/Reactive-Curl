cmake_minimum_required(VERSION 3.15)

project(ReactiveCurl LANGUAGES CXX C)
add_definitions(-DCURL_STATICLIB)
add_definitions(-DOPENSSL_USE_STATIC_LIBS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O1")

MESSAGE( STATUS "CURL_INCLUDE_DIRS: " ${CURL_INCLUDE_DIRS})
MESSAGE( STATUS "OPENSSL_INCLUDE_DIR: " ${OPENSSL_INCLUDE_DIR})
MESSAGE( STATUS "ZLIB_INCLUDE_DIR: " ${ZLIB_INCLUDE_DIR})
MESSAGE( STATUS "JSONCPP_INCLUDE_DIR: " ${JSONCPP_INCLUDE_DIR})
MESSAGE( STATUS "YAMLCPP_INCLUDE_DIR: " ${YAMLCPP_INCLUDE_DIR})
MESSAGE( STATUS "CURL_LIBRARIES: " ${CURL_LIBRARIES})
MESSAGE( STATUS "ZLIB_LIBRARIES: " ${ZLIB_LIBRARIES})
MESSAGE( STATUS "OPENSSL_SSL_LIBRARIES: " ${OPENSSL_SSL_LIBRARIES})
MESSAGE( STATUS "OPENSSL_CRYPTO_LIBRARIES: " ${OPENSSL_CRYPTO_LIBRARIES})
MESSAGE( STATUS "YAMLCPP_LIBRARIES: " ${YAMLCPP_LIBRARIES})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O1 -Wnonnull")

set(HTTP_CLIENT_FILES
        src/http-client/curl-handler.hpp
        src/http-client/exceptions.hpp
        src/http-client/http-client.hpp
        src/http-client/http-request.hpp
        src/http-client/http-request.cpp
        src/http-client/http-response.hpp
        src/http-client/options.hpp
        src/http-client/request-builder.hpp
        src/http-client/request-builder.cpp
)

add_executable(ReactiveCurl src/reactive-curl.cpp src/types.hpp ${HTTP_CLIENT_FILES})

include_directories("include")
target_include_directories(ReactiveCurl PUBLIC
${CURL_INCLUDE_DIRS}
${OPENSSL_INCLUDE_DIR}
${ZLIB_INCLUDE_DIR}
${JSONCPP_INCLUDE_DIR}
${YAMLCPP_INCLUDE_DIR}
)
if(WIN32)
target_link_libraries(ReactiveCurl
${CURL_LIBRARIES}
${OPENSSL_SSL_LIBRARIES} 
${OPENSSL_CRYPTO_LIBRARIES}
${ZLIB_LIBRARIES}
${YAMLCPP_LIBRARIES}
wldap32 ws2_32 Crypt32.lib Wldap32 Normaliz
)
else()
target_link_libraries(ReactiveCurl
${CURL_LIBRARIES}
${ZLIB_LIBRARIES}
${OPENSSL_SSL_LIBRARIES}
${OPENSSL_CRYPTO_LIBRARIES}
${YAMLCPP_LIBRARIES}
dl pthread
)
endif(WIN32)

install(TARGETS ReactiveCurl DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/")