include(CMakeFindDependencyMacro)

# Same syntax as find_package
find_dependency(MYDEP REQUIRED)

# Add the targets file
include("${CMAKE_CURRENT_LIST_DIR}/contracts_consolation-targets.cmake")
