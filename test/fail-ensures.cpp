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

namespace ensures {
	int magic([[maybe_unused]] int argc) noexcept
	{
		switch (TEST) {
		case test_kind::equal_to:
			return CJDB_ENSURES(argc == 0), argc * 2;
			break;
		case test_kind::not_equal_to:
			return CJDB_ENSURES(argc != 1), argc * 2;
			break;
		case test_kind::less:
			return CJDB_ENSURES(argc < 0), argc * 2;
			break;
		case test_kind::less_equal:
			return CJDB_ENSURES(argc <= 0), argc * 2;
			break;
		case test_kind::greater_equal:
			return CJDB_ENSURES(argc >= 4), argc * 2;
			break;
		case test_kind::greater:
			return CJDB_ENSURES(argc > 4), argc * 2;
			break;
		}
	}
} // namespace ensures

int magic2(int x) noexcept
{
	return ensures::magic(x);
}

int main(int const argc, char**)
{
	(void)magic2(argc);
#if not defined(CJDB_CONTRACTS_DISABLED)
	return 255;
#endif
}
