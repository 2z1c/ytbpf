cmake_minimum_required(VERSION 3.10)
project(ytbpf)

include(${CMAKE_SOURCE_DIR}/resources/BuildBpfResource.cmake)


custom_build_bpf_skel_obj(
  helloworld 
  resources/unittests/helloworld.bpf.c 
  resources/unittests/helloworld.cpp
)
custom_build_bpf_skel_obj(
  hello1 
  resources/unittests/helloworld.bpf.c 
  resources/unittests/helloworld.cpp
)