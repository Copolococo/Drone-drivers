cmake_minimum_required(VERSION 3.10)

project(Drone)

add_executable(Drone main.cpp driver.cpp driver.h)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Drone PRIVATE Threads::Threads)

target_include_directories(Drone PUBLIC
	"${PROJECT_BINARY_DIR}"
)
