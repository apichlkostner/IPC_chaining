cmake_minimum_required(VERSION 3.5)

# add the executable
add_executable(sender sender.cpp)

find_package(Poco REQUIRED Net
                  REQUIRED Foundation)
target_link_libraries(sender PUBLIC Poco::Net
                             PUBLIC Poco::Foundation)

target_include_directories(sender PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

# add the install targets
install(TARGETS sender DESTINATION bin)
