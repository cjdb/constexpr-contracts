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

./test/${1} > "`pwd`/.${1}.out" 2>&1
if [[ ${?} -eq 0 ]]; then
	exit 1
fi

if [[ -z "${2}" ]]; then
	touch "`pwd`/.${1}.expected"
else
	echo "${2}" > "`pwd`/.${1}.expected"
fi

diff "`pwd`/.${1}.out" "`pwd`/.${1}.expected"
result=${?}
rm "`pwd`/.${1}.out" "`pwd`/.${1}.expected"
exit ${result}
