cmake_minimum_required(VERSION 3.10)

project(ytbpf_resources)

set(LIBBPF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libbpf)

file (GLOB LIBBPF_SOURCES ${LIBBPF_DIR}/src/*.[hc])
message("SOURCES: ${LIBBPF_SOURCES}")

# build libbpf static library
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libbpf/libbpf.a
    DEPENDS ${LIBBPF_SOURCES}
    COMMENT "build static libbpf libary"
    COMMENT "make -C ${LIBBPF_DIR}/src BUILD_STATIC_ONLY=1 OBJDIR=${CMAKE_CURRENT_BINARY_DIR}/libbpf DESTDIR=${CMAKE_CURRENT_BINARY_DIR} INCLUDEDIR= LIBDIR= UAPIDIR= -j22 install"
    COMMAND make -C ${LIBBPF_DIR}/src BUILD_STATIC_ONLY=1 OBJDIR=${CMAKE_CURRENT_BINARY_DIR}/libbpf DESTDIR=${CMAKE_CURRENT_BINARY_DIR} INCLUDEDIR= LIBDIR= UAPIDIR= -j22 
    COMMAND make -C ${LIBBPF_DIR}/src BUILD_STATIC_ONLY=1 OBJDIR=${CMAKE_CURRENT_BINARY_DIR}/libbpf DESTDIR=${CMAKE_CURRENT_BINARY_DIR} INCLUDEDIR= LIBDIR= UAPIDIR= -j22 install
)
add_custom_target(libbpf DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/libbpf/libbpf.a)


set(TESTLINK_SRC unittests/testlink.c)

function(add_custom_bpf_exectuable target_name src_files)
    add_executable(${target_name} ${src_files})
    add_dependencies(${target_name} libbpf)
    target_link_libraries(${target_name} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/libbpf/libbpf.a)
endfunction()


add_custom_bpf_exectuable(tlink1 ${TESTLINK_SRC})
add_custom_bpf_exectuable(tlink2 ${TESTLINK_SRC})
