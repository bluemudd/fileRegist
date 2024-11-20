# 최소 CMake 버전 설정
cmake_minimum_required(VERSION 3.10)

# 프로젝트 이름과 언어 설정
project(FileServer LANGUAGES C)


set(CMAKE_C_STANDARD 99)

# 헤더 파일 경로 설정 (필요한 경우)
include_directories(${CMAKE_SOURCE_DIR})

# 소스 파일 목록 설정
set(SOURCE_FILES
    file_upload.c
    mk_list.c
    del_file.c
    mk_dir.c
    server.c
)


set(EXECUTABLE_NAME server)


add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

