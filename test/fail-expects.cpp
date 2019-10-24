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
#include <cjdb/contracts.hpp>
#include <cstdio>
#include <cstdlib>

int main(int argc, char**)
{
	CJDB_EXPECTS(argc != 1);
	if (argc == 1) {
		std::fprintf(stderr, "This should never be printed.\n");
		std::exit(1);
	}

	constexpr auto return_code = 255;
	return return_code;
}
