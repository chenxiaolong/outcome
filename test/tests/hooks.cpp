/* Unit testing for outcomes
(C) 2013-2017 Niall Douglas <http://www.nedproductions.biz/> (149 commits)


Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License in the accompanying file
Licence.txt or at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


Distributed under the Boost Software License, Version 1.0.
    (See accompanying file Licence.txt or copy at
          http://www.boost.org/LICENSE_1_0.txt)
*/

#define _CRT_SECURE_NO_WARNINGS

#include "../../include/outcome/outcome.hpp"
#include "../quickcpplib/include/boost/test/unit_test.hpp"

//! [extended_error_coding1]
namespace extended_error_coding
{
  using OUTCOME_V2_NAMESPACE::in_place_type;
  // Use static storage to convey extended error info from result construction to outcome conversion
  static char extended_error_info[256];

  // Use the error_code type as the ADL bridge for the hooks by creating a type here
  struct error_code : public std::error_code
  {
    using std::error_code::error_code;
    error_code() = default;
    error_code(std::error_code ec)  // NOLINT
    : std::error_code(ec)
    {
    }
  };
  // Localise result to using the local error_code so this namespace gets looked up for the hooks
  template <class R> using result = OUTCOME_V2_NAMESPACE::result<R, error_code>;
  // Specialise the result construction hook for our localised result
  template <class T> constexpr inline void hook_result_construction(OUTCOME_V2_NAMESPACE::in_place_type_t<T>, result<int> *res) noexcept
  {
    // Write the value in the result into the static storage
    snprintf(extended_error_info, sizeof(extended_error_info), "%d", res->assume_value());
  }
  template <class T> constexpr inline void hook_result_construction(OUTCOME_V2_NAMESPACE::in_place_type_t<T>, result<std::string> *res) noexcept
  {
    // Write the value in the result into the static storage
    snprintf(extended_error_info, sizeof(extended_error_info), "%s", res->assume_value().c_str());
  }
}  // namespace extended_error_coding
//! [extended_error_coding1]

BOOST_AUTO_TEST_CASE(works / result / hooks, "Tests that you can hook result's construction")
{
  using namespace OUTCOME_V2_NAMESPACE;
  extended_error_coding::result<int> a(5);
  BOOST_CHECK(!strcmp(extended_error_coding::extended_error_info, "5"));
  extended_error_coding::result<std::string> b("niall");
  BOOST_CHECK(!strcmp(extended_error_coding::extended_error_info, "niall"));
}

//! [extended_error_coding2]
namespace extended_error_coding
{
  // Localise outcome to using the local error_code so this namespace gets looked up for the hooks
  template <class R> using outcome = OUTCOME_V2_NAMESPACE::outcome<R, error_code, std::string>;
  namespace detail
  {
    // Use inheritance to gain access to state
    template <class R> struct outcome_payload_poker : public outcome<R>
    {
      void _poke_payload(typename extended_error_coding::outcome<R>::payload_exception_type &&p)
      {
        this->_ptr = std::move(p);
        this->_state._status |= OUTCOME_V2_NAMESPACE::detail::status_have_payload;
      }
    };
  }  // namespace detail

  // Specialise the outcome copy and move conversion hook for our localised result
  template <class T, class R> constexpr inline void hook_outcome_copy_construction(OUTCOME_V2_NAMESPACE::in_place_type_t<const result<T> &>, outcome<R> *res) noexcept
  {
    // when copy constructing from a result<T>, place extended_error_coding::extended_error_info into the payload
    static_cast<detail::outcome_payload_poker<R> *>(res)->_poke_payload(extended_error_info);
  }
  template <class T, class R> constexpr inline void hook_outcome_move_construction(OUTCOME_V2_NAMESPACE::in_place_type_t<result<T> &&>, outcome<R> *res) noexcept
  {
    // when move constructing from a result<T>, place extended_error_coding::extended_error_info into the payload
    static_cast<detail::outcome_payload_poker<R> *>(res)->_poke_payload(extended_error_info);
  }
}  // namespace extended_error_coding
//! [extended_error_coding2]

BOOST_AUTO_TEST_CASE(works / outcome / hooks, "Tests that you can hook outcome's conversion from a result")
{
  using namespace extended_error_coding;
  outcome<int> a(result<int>(5));
  BOOST_CHECK(a.payload() == "5");
  outcome<std::string> b(result<std::string>("niall"));
  BOOST_CHECK(b.payload() == "niall");

  // Make sure hook does not fire for any other kind of outcome copy or move, only when converting from our custom result only
  outcome<int> c(5);
  outcome<long> d(c);  // can't be the same type as source, else copy elision takes place and no ADL hook calling
  BOOST_CHECK(!d.has_payload());
  outcome<int> e(OUTCOME_V2_NAMESPACE::result<int>(5));
  BOOST_CHECK(!e.has_payload());
}