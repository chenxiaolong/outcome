/* value_storage.hpp
The world's most simple C++ monad
(C) 2015 Niall Douglas http://www.nedprod.com/
File Created: July 2015


Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#ifndef BOOST_OUTCOME_VALUE_STORAGE_IMPL
#error Please do not include this directly, include value_storage.hpp
#endif

template <class _value_type, class _error_type, class _exception_type, bool use_single_byte = enable_single_byte_value_storage<_value_type>::value> class BOOST_OUTCOME_VALUE_STORAGE_IMPL
{
  // Define stand in types for when these are void. As they are private, they
  // are disabled for SFINAE and any attempt to use them yields a useful error message.
  struct no_value_type
  {
  };
  struct no_error_type
  {
  };
  struct no_exception_type
  {
  };
  struct empty_type
  {
  };
  template <class U, class V> using devoid = typename std::conditional<!std::is_void<U>::value, U, V>::type;
  friend inline std::ostream &operator<<(std::ostream &s, const no_value_type &) { return s << "void"; }
public:
  static constexpr bool has_value_type = !std::is_void<_value_type>::value;
  static constexpr bool has_error_type = !std::is_void<_error_type>::value;
  static constexpr bool has_exception_type = !std::is_void<_exception_type>::value;
  static constexpr bool is_referenceable = true;
  static constexpr bool is_trivially_destructible = !BOOST_OUTCOME_VALUE_STORAGE_NON_TRIVIAL_DESTRUCTOR;
  typedef devoid<_value_type, no_value_type> value_type;
  typedef devoid<_error_type, no_error_type> error_type;
  typedef devoid<_exception_type, no_exception_type> exception_type;
  struct storage_type
  {
    enum storage_type_t : unsigned char
    {
      empty,
      value,
      error,
      exception
    };
  };
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4624)
#endif
  union {
    empty_type _empty;
    value_type _value_raw;
    value_type value;
    error_type error;          // Often 16 bytes surprisingly
    exception_type exception;  // Typically 8 bytes
  };
#ifdef _MSC_VER
#pragma warning(pop)
#endif
  unsigned char type;

  // Is this storage copy and move constructible?
  static constexpr bool is_copy_constructible = (!has_value_type || std::is_copy_constructible<value_type>::value) && (!has_error_type || std::is_copy_constructible<error_type>::value) && (!has_exception_type || std::is_copy_constructible<exception_type>::value);
  static constexpr bool is_move_constructible = (!has_value_type || std::is_move_constructible<value_type>::value) && (!has_error_type || std::is_move_constructible<error_type>::value) && (!has_exception_type || std::is_move_constructible<exception_type>::value);
  static constexpr bool is_copy_assignable = (!has_value_type || std::is_copy_assignable<value_type>::value) && (!has_error_type || std::is_copy_assignable<error_type>::value) && (!has_exception_type || std::is_copy_assignable<exception_type>::value);
  static constexpr bool is_move_assignable = (!has_value_type || std::is_move_assignable<value_type>::value) && (!has_error_type || std::is_move_assignable<error_type>::value) && (!has_exception_type || std::is_move_assignable<exception_type>::value);

  static constexpr bool is_nothrow_copy_constructible = (!has_value_type || std::is_nothrow_copy_constructible<value_type>::value) && (!has_error_type || std::is_nothrow_copy_constructible<error_type>::value) && (!has_exception_type || std::is_nothrow_copy_constructible<exception_type>::value);
  static constexpr bool is_nothrow_move_constructible = (!has_value_type || std::is_nothrow_move_constructible<value_type>::value) && (!has_error_type || std::is_nothrow_move_constructible<error_type>::value) && (!has_exception_type || std::is_nothrow_move_constructible<exception_type>::value);
  static constexpr bool is_nothrow_copy_assignable = (!has_value_type || std::is_nothrow_copy_assignable<value_type>::value) && (!has_error_type || std::is_nothrow_copy_assignable<error_type>::value) && (!has_exception_type || std::is_nothrow_copy_assignable<exception_type>::value);
  static constexpr bool is_nothrow_move_assignable = (!has_value_type || std::is_nothrow_move_assignable<value_type>::value) && (!has_error_type || std::is_nothrow_move_assignable<error_type>::value) && (!has_exception_type || std::is_nothrow_move_assignable<exception_type>::value);
  static constexpr bool is_nothrow_destructible = std::is_nothrow_destructible<value_type>::value && std::is_nothrow_destructible<exception_type>::value && std::is_nothrow_destructible<error_type>::value;

  constexpr BOOST_OUTCOME_VALUE_STORAGE_IMPL() noexcept : _empty(empty_type()), type(storage_type::empty) {}
  constexpr BOOST_OUTCOME_VALUE_STORAGE_IMPL(empty_t) noexcept : _empty(empty_type()), type(storage_type::empty) {}
  constexpr BOOST_OUTCOME_VALUE_STORAGE_IMPL(value_t) noexcept(std::is_nothrow_default_constructible<value_type>::value)
      : value(value_type())
      , type(storage_type::value)
  {
  }
  constexpr BOOST_OUTCOME_VALUE_STORAGE_IMPL(error_t) noexcept(std::is_nothrow_default_constructible<error_type>::value)
      : error(error_type())
      , type(storage_type::error)
  {
  }
  constexpr BOOST_OUTCOME_VALUE_STORAGE_IMPL(exception_t) noexcept(std::is_nothrow_default_constructible<exception_type>::value)
      : exception(exception_type())
      , type(storage_type::exception)
  {
  }
  constexpr BOOST_OUTCOME_VALUE_STORAGE_IMPL(const value_type &v) noexcept(std::is_nothrow_copy_constructible<value_type>::value)
      : value(v)
      , type(storage_type::value)
  {
  }
  constexpr BOOST_OUTCOME_VALUE_STORAGE_IMPL(const error_type &v) noexcept(std::is_nothrow_copy_constructible<error_type>::value)
      : error(v)
      , type(storage_type::error)
  {
  }
  constexpr BOOST_OUTCOME_VALUE_STORAGE_IMPL(const exception_type &v) noexcept(std::is_nothrow_copy_constructible<exception_type>::value)
      : exception(v)
      , type(storage_type::exception)
  {
  }
  constexpr BOOST_OUTCOME_VALUE_STORAGE_IMPL(value_type &&v) noexcept(std::is_nothrow_move_constructible<value_type>::value)
      : value(std::move(v))
      , type(storage_type::value)
  {
  }
  constexpr BOOST_OUTCOME_VALUE_STORAGE_IMPL(error_type &&v) noexcept(std::is_nothrow_move_constructible<error_type>::value)
      : error(std::move(v))
      , type(storage_type::error)
  {
  }
  constexpr BOOST_OUTCOME_VALUE_STORAGE_IMPL(exception_type &&v) noexcept(std::is_nothrow_move_constructible<exception_type>::value)
      : exception(std::move(v))
      , type(storage_type::exception)
  {
  }
  struct emplace_t
  {
  };
  template <class... Args>
  constexpr explicit BOOST_OUTCOME_VALUE_STORAGE_IMPL(emplace_t, Args &&... args)
#if !defined(_MSC_VER) || _MSC_VER > 190022816
  noexcept(std::is_nothrow_constructible<value_type, Args...>::value)
#endif
      : value(std::forward<Args>(args)...)
      , type(storage_type::value)
  {
  }
  // If it has a non trivial destructor, we actively need to copy and move as well as destroy
#if BOOST_OUTCOME_VALUE_STORAGE_NON_TRIVIAL_DESTRUCTOR
  BOOST_OUTCOME_VALUE_STORAGE_IMPL(const BOOST_OUTCOME_VALUE_STORAGE_IMPL &o) noexcept(is_nothrow_copy_constructible) : _empty(empty_type()), type(storage_type::empty)
  {
    switch(o.type)
    {
    case storage_type::empty:
      break;
    case storage_type::value:
      new(&_value_raw) value_type(o._value_raw);  // use _value_raw to work around byte packing
      break;
    case storage_type::error:
      new(&error) error_type(o.error);
      break;
    case storage_type::exception:
      new(&exception) exception_type(o.exception);
      break;
    }
    type = o.type;
  }
  BOOST_OUTCOME_VALUE_STORAGE_IMPL &operator=(const BOOST_OUTCOME_VALUE_STORAGE_IMPL &o) noexcept(is_nothrow_copy_assignable)
  {
    clear();
    switch(o.type)
    {
    case storage_type::empty:
      break;
    case storage_type::value:
      value = o.value;
      break;
    case storage_type::error:
      error = o.error;
      break;
    case storage_type::exception:
      exception = o.exception;
      break;
    }
    type = o.type;
    return *this;
  }
  BOOST_OUTCOME_VALUE_STORAGE_IMPL(BOOST_OUTCOME_VALUE_STORAGE_IMPL &&o) noexcept(is_nothrow_move_constructible) : _empty(empty_type()), type(storage_type::empty)
  {
    switch(o.type)
    {
    case storage_type::empty:
      break;
    case storage_type::value:
      new(&_value_raw) value_type(std::move(o._value_raw));  // use _value_raw to work around byte packing
      break;
    case storage_type::error:
      new(&error) error_type(std::move(o.error));
      break;
    case storage_type::exception:
      new(&exception) exception_type(std::move(o.exception));
      break;
    }
    type = o.type;
  }
  BOOST_OUTCOME_VALUE_STORAGE_IMPL &operator=(BOOST_OUTCOME_VALUE_STORAGE_IMPL &&o) noexcept(is_nothrow_move_assignable)
  {
    clear();
    switch(o.type)
    {
    case storage_type::empty:
      break;
    case storage_type::value:
      value = std::move(o.value);
      break;
    case storage_type::error:
      error = std::move(o.error);
      break;
    case storage_type::exception:
      exception = std::move(o.exception);
      break;
    }
    type = o.type;
    return *this;
  }
  ~BOOST_OUTCOME_VALUE_STORAGE_IMPL()
#if defined(__c2__) || (!defined(_MSC_VER) || _MSC_FULL_VER != 191024728 /* VS2017 RC1*/)
  noexcept(is_nothrow_destructible)
#endif
  {
    clear();
  }
#endif
  BOOST_OUTCOME_CONSTEXPR void clear() noexcept(is_nothrow_destructible)
  {
    switch(type)
    {
    case storage_type::empty:
      break;
    case storage_type::value:
      value.~value_type();
      type = storage_type::empty;
      break;
    case storage_type::error:
      error.~error_type();
      type = storage_type::empty;
      break;
    case storage_type::exception:
      exception.~exception_type();
      type = storage_type::empty;
      break;
    }
  }
};

#ifndef BOOST_OUTCOME_VALUE_STORAGE_IMPL_PACKED_INITIALISER
#if defined(__BYTE_ORDER__)
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define BOOST_OUTCOME_VALUE_STORAGE_IMPL_PACKED_INITIALISER(t, v) ((((unsigned char) (v)) & 0x3f) | ((((t) &0x3)) << 6))
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#error Unsure if endianness affects bits in a byte, you need to test and send a pull request on github
#endif
#endif
#ifndef BOOST_OUTCOME_VALUE_STORAGE_IMPL_PACKED_INITIALISER
#ifdef _MSC_VER
#define BOOST_OUTCOME_VALUE_STORAGE_IMPL_PACKED_INITIALISER(t, v) ((((unsigned char) (v)) & 0x3f) | ((((t) &0x3)) << 6))
#else
#error Could not figure out the endianness of this platform
#endif
#endif
#endif
template <class _value_type> class BOOST_OUTCOME_VALUE_STORAGE_IMPL<_value_type, void, void, true>
{
  static_assert(std::is_integral<_value_type>::value || std::is_void<_value_type>::value, "Types enabled for packed storage using enable_single_byte_value_storage must be integral types.");
  // Define stand in types for when these are void. As they are private, they
  // are disabled for SFINAE and any attempt to use them yields a useful error message.
  struct no_error_type
  {
  };
  struct no_exception_type
  {
  };
  struct empty_type
  {
  };
  template <class U, class V> using devoid = typename std::conditional<!std::is_void<U>::value, U, V>::type;

public:
  static constexpr bool has_value_type = !std::is_void<_value_type>::value;
  static constexpr bool has_error_type = false;
  static constexpr bool has_exception_type = false;
  static constexpr bool is_referenceable = false;
  static constexpr bool is_trivially_destructible = !BOOST_OUTCOME_VALUE_STORAGE_NON_TRIVIAL_DESTRUCTOR;
  typedef devoid<_value_type, unsigned char> value_type;
  typedef no_error_type error_type;
  typedef no_exception_type exception_type;
  struct storage_type
  {
    enum storage_type_t : unsigned char
    {
      empty,
      value,
      error,
      exception
    };
  };
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4624 4201)
#endif
  union {
    empty_type _empty;
    unsigned char _value_raw;
    struct
    {
      unsigned char value : 6;
      unsigned char type : 2;
    };
    error_type error;
    exception_type exception;
  };
#ifdef _MSC_VER
#pragma warning(pop)
#endif

  // Is this storage copy and move constructible?
  static constexpr bool is_copy_constructible = (!has_value_type || std::is_copy_constructible<value_type>::value) && (!has_error_type || std::is_copy_constructible<error_type>::value) && (!has_exception_type || std::is_copy_constructible<exception_type>::value);
  static constexpr bool is_move_constructible = (!has_value_type || std::is_move_constructible<value_type>::value) && (!has_error_type || std::is_move_constructible<error_type>::value) && (!has_exception_type || std::is_move_constructible<exception_type>::value);
  static constexpr bool is_copy_assignable = (!has_value_type || std::is_copy_assignable<value_type>::value) && (!has_error_type || std::is_copy_assignable<error_type>::value) && (!has_exception_type || std::is_copy_assignable<exception_type>::value);
  static constexpr bool is_move_assignable = (!has_value_type || std::is_move_assignable<value_type>::value) && (!has_error_type || std::is_move_assignable<error_type>::value) && (!has_exception_type || std::is_move_assignable<exception_type>::value);

  static constexpr bool is_nothrow_copy_constructible = (!has_value_type || std::is_nothrow_copy_constructible<value_type>::value) && (!has_error_type || std::is_nothrow_copy_constructible<error_type>::value) && (!has_exception_type || std::is_nothrow_copy_constructible<exception_type>::value);
  static constexpr bool is_nothrow_move_constructible = (!has_value_type || std::is_nothrow_move_constructible<value_type>::value) && (!has_error_type || std::is_nothrow_move_constructible<error_type>::value) && (!has_exception_type || std::is_nothrow_move_constructible<exception_type>::value);
  static constexpr bool is_nothrow_copy_assignable = (!has_value_type || std::is_nothrow_copy_assignable<value_type>::value) && (!has_error_type || std::is_nothrow_copy_assignable<error_type>::value) && (!has_exception_type || std::is_nothrow_copy_assignable<exception_type>::value);
  static constexpr bool is_nothrow_move_assignable = (!has_value_type || std::is_nothrow_move_assignable<value_type>::value) && (!has_error_type || std::is_nothrow_move_assignable<error_type>::value) && (!has_exception_type || std::is_nothrow_move_assignable<exception_type>::value);
  static constexpr bool is_nothrow_destructible = std::is_nothrow_destructible<value_type>::value && std::is_nothrow_destructible<exception_type>::value && std::is_nothrow_destructible<error_type>::value;

  constexpr BOOST_OUTCOME_VALUE_STORAGE_IMPL()
      : _value_raw(BOOST_OUTCOME_VALUE_STORAGE_IMPL_PACKED_INITIALISER(storage_type::empty, 0))
  {
  }
  constexpr BOOST_OUTCOME_VALUE_STORAGE_IMPL(empty_t) noexcept : _value_raw(BOOST_OUTCOME_VALUE_STORAGE_IMPL_PACKED_INITIALISER(storage_type::empty, 0)) {}
  constexpr BOOST_OUTCOME_VALUE_STORAGE_IMPL(value_t) noexcept(std::is_nothrow_default_constructible<value_type>::value)
      : _value_raw(BOOST_OUTCOME_VALUE_STORAGE_IMPL_PACKED_INITIALISER(storage_type::value, 0))
  {
  }
  constexpr BOOST_OUTCOME_VALUE_STORAGE_IMPL(error_t) noexcept(std::is_nothrow_default_constructible<error_type>::value)
      : _value_raw(BOOST_OUTCOME_VALUE_STORAGE_IMPL_PACKED_INITIALISER(storage_type::error, 0))
  {
  }
  constexpr BOOST_OUTCOME_VALUE_STORAGE_IMPL(exception_t) noexcept(std::is_nothrow_default_constructible<exception_type>::value)
      : _value_raw(BOOST_OUTCOME_VALUE_STORAGE_IMPL_PACKED_INITIALISER(storage_type::exception, 0))
  {
  }
  constexpr BOOST_OUTCOME_VALUE_STORAGE_IMPL(const value_type &v) noexcept(std::is_nothrow_copy_constructible<value_type>::value)
      : _value_raw(BOOST_OUTCOME_VALUE_STORAGE_IMPL_PACKED_INITIALISER(storage_type::value, v))
  {
  }
  constexpr BOOST_OUTCOME_VALUE_STORAGE_IMPL(const error_type &) noexcept(std::is_nothrow_copy_constructible<error_type>::value)
      : _value_raw(BOOST_OUTCOME_VALUE_STORAGE_IMPL_PACKED_INITIALISER(storage_type::error, 0))
  {
  }
  constexpr BOOST_OUTCOME_VALUE_STORAGE_IMPL(const exception_type &) noexcept(std::is_nothrow_copy_constructible<exception_type>::value)
      : _value_raw(BOOST_OUTCOME_VALUE_STORAGE_IMPL_PACKED_INITIALISER(storage_type::exception, 0))
  {
  }
  constexpr BOOST_OUTCOME_VALUE_STORAGE_IMPL(value_type &&v) noexcept(std::is_nothrow_move_constructible<value_type>::value)
      : _value_raw(BOOST_OUTCOME_VALUE_STORAGE_IMPL_PACKED_INITIALISER(storage_type::value, v))
  {
  }
  constexpr BOOST_OUTCOME_VALUE_STORAGE_IMPL(error_type &&) noexcept(std::is_nothrow_move_constructible<error_type>::value)
      : _value_raw(BOOST_OUTCOME_VALUE_STORAGE_IMPL_PACKED_INITIALISER(storage_type::error, 0))
  {
  }
  constexpr BOOST_OUTCOME_VALUE_STORAGE_IMPL(exception_type &&) noexcept(std::is_nothrow_move_constructible<exception_type>::value)
      : _value_raw(BOOST_OUTCOME_VALUE_STORAGE_IMPL_PACKED_INITIALISER(storage_type::exception, 0))
  {
  }
  struct emplace_t
  {
  };
  template <class... Args>
  BOOST_OUTCOME_CONSTEXPR explicit BOOST_OUTCOME_VALUE_STORAGE_IMPL(emplace_t, Args &&... args) noexcept(std::is_nothrow_constructible<value_type, Args...>::value)
      : value(std::forward<Args>(args)...)
  {
    type = storage_type::value;
  }
#if BOOST_OUTCOME_VALUE_STORAGE_NON_TRIVIAL_DESTRUCTOR
  ~BOOST_OUTCOME_VALUE_STORAGE_IMPL() noexcept(is_nothrow_destructible) { clear(); }
#endif
  BOOST_OUTCOME_CONSTEXPR void clear() noexcept(is_nothrow_destructible)
  {
    switch(type)
    {
    case storage_type::empty:
      break;
    default:
      type = storage_type::empty;
      break;
    }
  }
};
