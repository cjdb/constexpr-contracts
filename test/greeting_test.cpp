#include <project_template/greeting.hpp>
#include <catch2/catch.hpp>
#include <string_view>

TEST_CASE("greeting returns expected values")
{
	CHECK(project_template::greet("C++") == "Hello, C++");
	CHECK(project_template::greet("project_template") == "Hello, project_template");
}
