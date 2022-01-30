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
#include "cjdb/contracts.hpp"

namespace assert {
	void f(int const argc) noexcept
	{
		CJDB_ASSERT(argc > 1);
	}
} // namespace assert

void g(int const argc) noexcept
{
	assert::f(argc);
}

int main(int argc, char**)
{
	g(argc);
	constexpr auto return_code = 255;
	return argc == 1 ? 0 : return_code;
}
