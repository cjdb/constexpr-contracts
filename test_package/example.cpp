//
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
#include <iostream>
#include "cjdb/contracts.hpp"

constexpr int check_constexpr_contract(int x) noexcept
{
	CJDB_EXPECTS(x == 0);

	++x;
	CJDB_ASSERT(x == 1);

	--x;
	return CJDB_ENSURES(x, x == 0);
}

int main() {
	[[maybe_unused]] constexpr auto x = check_constexpr_contract(0);

	auto y = 0;
	return check_constexpr_contract(y);
}
