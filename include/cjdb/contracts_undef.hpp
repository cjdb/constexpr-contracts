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
#ifndef CJDB_CONTRACTS_UNDEF_HPP

#ifndef CJDB_CONTRACTS_HPP
#  error "This file must be included after \"cjdb/contracts.hpp\""
#endif // CJDB_CONTRACTS_HPP

#undef CJDB_CONTRACTS_HPP
#undef CJDB_IS_DEBUG
#undef CJDB_QUIET_CONTRACT
#undef CJDB_ALERTING_CONTRACT

#undef CJDB_QUIET_EXPECTS
#undef CJDB_ALERT_EXPECTS

#undef CJDB_QUIET_ENSURES
#undef CJDB_ALERT_ENSURES

#undef CJDB_QUIET_ASSERT
#undef CJDB_ALERT_ASSERT

#undef CJDB_EXPECTS_IMPL
#undef CJDB_ASSERT_IMPL
#undef CJDB_ENSURES_IMPL

#undef CJDB_CONTRACT_IMPL

#undef CJDB_TO_STRING
#undef CJDB_TO_STRING_IMPL

#undef CJDB_AXIOM

#endif // CJDB_CONTRACTS_UNDEF_HPP
