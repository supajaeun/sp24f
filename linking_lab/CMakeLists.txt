# sp24f/linking_lab/CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(linking_lab)

# C++17 표준 사용
set(CMAKE_CXX_STANDARD 17)

# 디버깅을 위한 컴파일러 플래그 설정
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# LevelDB 라이브러리 경로 설정
add_subdirectory(leveldb)

# main.cpp 컴파일 및 링크
add_executable(leveldb_bench main.cpp)

# LevelDB 라이브러리 및 헤더 포함
target_link_libraries(leveldb_bench PRIVATE leveldb)
target_include_directories(leveldb_bench PRIVATE ${PROJECT_SOURCE_DIR}/leveldb/include)
