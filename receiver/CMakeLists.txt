cmake_minimum_required(VERSION 3.5)

# add the executable
add_executable(receiver receiver.cpp)

find_package(Poco REQUIRED Net
                  REQUIRED Foundation)
target_link_libraries(receiver PUBLIC Poco::Net
                               PUBLIC Poco::Foundation)


target_include_directories(receiver PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

# add the install targets
install(TARGETS receiver DESTINATION bin)
