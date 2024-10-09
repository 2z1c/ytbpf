cmake_minimum_required(VERSION 3.10)
project(ytbpf_resources)

include(${CMAKE_SOURCE_DIR}/BuildBpfResource.cmake)

custom_build_bpf_skel_obj(helloworld unittests/helloworld.bpf.c unittests/helloworld.cpp)
custom_build_bpf_skel_obj(hello1 unittests/helloworld.bpf.c unittests/helloworld.cpp)



