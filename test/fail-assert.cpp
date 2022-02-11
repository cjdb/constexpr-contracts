//
//  Copyright Google LLC
//  Copyright Christopher Di Bella
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include "cjdb/contracts.hpp"

enum class test_kind { equal_to, not_equal_to, less, less_equal, greater_equal, greater };

namespace assert {
	void f(int argc) noexcept
	{
		switch (TEST) {
		case test_kind::equal_to:
			CJDB_ASSERT(argc == 0);
			break;
		case test_kind::not_equal_to:
			CJDB_ASSERT(argc != 1);
			break;
		case test_kind::less:
			CJDB_ASSERT(argc < 0);
			break;
		case test_kind::less_equal:
			CJDB_ASSERT(argc <= 0);
			break;
		case test_kind::greater_equal:
			CJDB_ASSERT(argc >= 4);
			break;
		case test_kind::greater:
			CJDB_ASSERT(argc > 4);
			break;
		}
	}
} // namespace assert

void g(int argc) noexcept
{
	assert::f(argc);
}

int main(int argc, char**)
{
	g(argc);
	return 1;
}
