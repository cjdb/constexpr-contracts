# find_package(absl CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)
# find_package(constexpr-contracts CONFIG REQUIRED)
# find_package(fmt CONFIG REQUIRED)
# find_package(range-v3 CONFIG REQUIRED)

if(${PROJECT_NAME}_ENABLE_CLANG_TIDY)
	find_package(ClangTidy REQUIRED)
endif()
