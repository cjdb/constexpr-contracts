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
