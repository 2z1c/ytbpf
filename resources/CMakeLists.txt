cmake_minimum_required(VERSION 3.10)

project(ytbpf_resources)
# build libbpf static library
set(LIBBPF_DIR ${CMAKE_CURRENT_LIST_DIR}/libbpf)
file (GLOB LIBBPF_SOURCES ${LIBBPF_DIR}/src/*.[hc])
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libbpf/libbpf.a
    DEPENDS ${LIBBPF_SOURCES}
    COMMENT "build static libbpf libary"
    # COMMENT "make -C ${LIBBPF_DIR}/src BUILD_STATIC_ONLY=1 OBJDIR=${CMAKE_CURRENT_BINARY_DIR}/libbpf DESTDIR=${CMAKE_CURRENT_BINARY_DIR} INCLUDEDIR= LIBDIR= UAPIDIR= -j22 install"
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/libbpf
    COMMAND make -C ${LIBBPF_DIR}/src BUILD_STATIC_ONLY=1 OBJDIR=${CMAKE_CURRENT_BINARY_DIR}/libbpf DESTDIR=${CMAKE_CURRENT_BINARY_DIR}/libbpf INCLUDEDIR= LIBDIR= UAPIDIR= -j22 
)
add_custom_target(
    libbpf_obj 
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/libbpf/libbpf.a
)

# build bpftool
set(BPFTOOL_DIR ${CMAKE_CURRENT_LIST_DIR}/bpftool)
file(GLOB_RECURSE BPFTOOL_SOURCES ${BPFTOOL_DIR}/*.[ch])
set(BPFTOOL ${CMAKE_CURRENT_BINARY_DIR}/bpftool/bootstrap/bpftool)

add_custom_command(
    OUTPUT ${BPFTOOL}
    DEPENDS ${BPFTOOL_SOURCES}
    COMMENT "build bpftool tools"
    COMMAND echo "make -C ${BPFTOOL_DIR}/src ARCH= CROSS_COMPILE=  OUTPUT=${CMAKE_CURRENT_BINARY_DIR}/bpftool bootstrap -j22"
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/bpftool
    COMMAND make -C ${BPFTOOL_DIR}/src ARCH= CROSS_COMPILE=  OUTPUT=${CMAKE_CURRENT_BINARY_DIR}/bpftool/ bootstrap -j22
)

add_custom_target(
    bpftool_obj 
    DEPENDS ${BPFTOOL}
)

# build 
set(TESTLINK_SRC unittests/testlink.c)

function(add_custom_bpf_exectuable target_name src_files)
    add_executable(${target_name} ${src_files})
    add_dependencies(${target_name} libbpf_obj)
    target_link_libraries(${target_name} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/libbpf/libbpf.a)
endfunction()

function(custom_build_bpf_skel_obj target_name _bpf_src_ _app_srcs_)
    # string(REPLACE ".bpf.c" ".skel.h"  "${_bpf_src_}")
    get_filename_component(bpf_file_src_name "${_bpf_src_}" NAME)

    string(REPLACE ".bpf.c" ".skel.h" bpf_file_skel_name ${bpf_file_src_name})
    string(REPLACE ".bpf.c" ".bpf.o" bpf_file_obj_name ${bpf_file_src_name})
    set(bpf_virt_obj_target "${target_name}.virt.bpf")

    set(TARGET_OBJS_DIR "${CMAKE_CURRENT_BINARY_DIR}/objs.${target_name}")
    set(BPFCFLAGS "-g" "-O2" "-Wall")
    set(BPF_SKEL_INCLUDES "-I${CMAKE_CURRENT_LIST_DIR}/include/arm64" "-I${LIBBPF_DIR}/include/uapi" "-I${CMAKE_CURRENT_LIST_DIR}" "-I${CMAKE_CURRENT_LIST_DIR}/include")

    set(GXX_COMPILE_FLAGS "-Wall -O2 -std=c++17")
    # message("BPF_SRC: ${_bpf_src_} ${bpf_file_src_name} ${bpf_file_skel_name} ${bpf_file_obj_name}")

    add_custom_command(
        OUTPUT ${TARGET_OBJS_DIR} ${TARGET_OBJS_DIR}/${bpf_file_skel_name} ${TARGET_OBJS_DIR}/${bpf_file_obj_name}
        DEPENDS ${_bpf_src_} bpftool_obj libbpf_obj
        COMMAND mkdir -p ${TARGET_OBJS_DIR}
        # bpf.o
        # COMMAND echo "clang-14 ${BPFCFLAGS} -target bpf -D__TARGET_ARCH_arm64 ${BPF_SKEL_INCLUDES} -c ${CMAKE_CURRENT_LIST_DIR}/${_bpf_src_} -o ${TARGET_OBJS_DIR}/${bpf_file_obj_name}"
        COMMAND clang-14 -g -O2 -Wall -target bpf -D__TARGET_ARCH_arm64 ${BPF_SKEL_INCLUDES} -c ${CMAKE_CURRENT_LIST_DIR}/${_bpf_src_} -o ${TARGET_OBJS_DIR}/${bpf_file_obj_name}
        # skel.h
        COMMAND ${BPFTOOL} gen skeleton ${TARGET_OBJS_DIR}/${bpf_file_obj_name} > ${TARGET_OBJS_DIR}/${bpf_file_skel_name}
    )

    add_custom_target(
        ${bpf_virt_obj_target}
        DEPENDS ${TARGET_OBJS_DIR} ${TARGET_OBJS_DIR}/${bpf_file_skel_name} ${TARGET_OBJS_DIR}/${bpf_file_obj_name}  ${_app_srcs_}
    )

    add_executable(${target_name}  ${_app_srcs_})
    add_dependencies(${target_name} ${bpf_virt_obj_target})
    set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    target_compile_options(${target_name} PRIVATE -g -O2 -Wall -Wmissing-field-initializers -Werror)
    target_link_options(${target_name} PRIVATE "-static")
    target_link_libraries(${target_name} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/libbpf/libbpf.a -lelf -lz)
    target_include_directories(${target_name} PRIVATE ${TARGET_OBJS_DIR}/ )

endfunction()





# add_custom_bpf_exectuable(tlink1 ${TESTLINK_SRC})
# add_custom_bpf_exectuable(tlink2 ${TESTLINK_SRC})

custom_build_bpf_skel_obj(helloworld unittests/helloworld.bpf.c unittests/helloworld.cpp)
custom_build_bpf_skel_obj(hello1 unittests/helloworld.bpf.c unittests/helloworld.cpp)



