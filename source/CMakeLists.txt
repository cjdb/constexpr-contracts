cxx_library(
   TARGET greeting
   FILENAMES "greeting.cpp"
)

cxx_binary(
   TARGET main1
   FILENAMES "main.cpp"
   LINK_TARGETS greeting
)

cxx_binary(
   TARGET main2
   FILENAMES "main.cpp" "greeting.cpp"
)
