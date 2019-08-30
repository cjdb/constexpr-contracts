#!/bin/bash
#
#  Copyright Christopher Di Bella
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

if [[ -z "${2}" ]]; then
	is_debug=0
else
	is_debug=1
	echo "${2}" > "`pwd`/.${1}.expected"
fi

./test/${1} > "`pwd`/.${1}.out" 2>&1
if [[ ${?} -eq 0 && is_debug -eq 1 ]]; then
	echo "./test/${1} unexpectedly passes"
	exit 1
fi

diff -N "`pwd`/.${1}.out" "`pwd`/.${1}.expected"
result=${?}
rm "`pwd`/.${1}.out" "`pwd`/.${1}.expected"
exit ${result}
