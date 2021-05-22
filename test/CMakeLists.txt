set(${PROJECT_NAME}_TEST_FRAMEWORK "Catch2::Catch2" CACHE STRING "")
set(${PROJECT_NAME}_NEEDS_TEST_MAIN ON CACHE BOOL "")

cxx_library(
   TARGET test_main
   FILENAMES "test_main.cpp"
   LINK_TARGETS Catch2::Catch2
)

cxx_test(
   TARGET test_greeting
   FILENAMES "greeting_test.cpp"
   LINK_TARGETS greeting
)
