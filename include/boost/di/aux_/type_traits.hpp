//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_CPP_14_TYPE_TRAITS_HPP
#define BOOST_DI_AUX_CPP_14_TYPE_TRAITS_HPP

#include "boost/di/aux_/utility.hpp"

#include <type_traits>
#include <boost/mpl/aux_/yes_no.hpp>

#define BOOST_DI_HAS_TYPE(name) BOOST_DI_HAS_TYPE_IMPL(name)
#define BOOST_DI_HAS_TYPE_IMPL(name)                                            \
    template<class > std::false_type has_##name##_impl(...);                    \
    template<class T> std::true_type has_##name##_impl(typename T::name*);      \
    template<class T> struct has_##name : decltype(has_##name##_impl<T>(0)) { }

#define BOOST_DI_HAS_METHOD(name) BOOST_DI_HAS_METHOD_IMPL(name, name)
#define BOOST_DI_HAS_METHOD_(name, f) BOOST_DI_HAS_METHOD_IMPL(name, f)
#define BOOST_DI_HAS_METHOD_IMPL(name, f)                                                                   \
	template<class T, class... TArgs>                                                                       \
    decltype(std::declval<T>().f(std::declval<TArgs>()...), std::true_type()) has_##name##_impl(int);       \
	template<class, class...> std::false_type has_##name##_impl(...);                                       \
	template<class T, class... TArgs> struct has_##name : decltype(has_##name##_impl<T, TArgs...>(0)) { }

#define BOOST_DI_HAS_METHOD_CALL(name) BOOST_DI_HAS_METHOD_CALL_IMPL(name, name)
#define BOOST_DI_HAS_METHOD_CALL_(name, f) BOOST_DI_HAS_METHOD_CALL_IMPL(name, f)
#define BOOST_DI_HAS_METHOD_CALL_IMPL(name, f)                                                                      \
    struct has_##name##_base { void f() { } };                                                                      \
    template<class T> std::false_type has_##name##_impl(T*, non_type<void (has_##name##_base::*)(), &T::f>* = 0);                \
    template<class> std::true_type has_##name##_impl(...);                                                                       \
    template<class T, typename = void> struct has_##name : decltype(has_##name##_impl<inherit<T, has_##name##_base>>(0)) { };    \
    template<class T> struct has_##name<T, std::enable_if_t<!std::is_class<T>::value>> : std::false_type { }        \

#define BOOST_DI_HAS_METHOD_SIGN(name) BOOST_DI_HAS_METHOD_SIGN_IMPL(name, name)
#define BOOST_DI_HAS_METHOD_SIGN_(name, f) BOOST_DI_HAS_METHOD_SIGN_IMPL(name, f)
#define BOOST_DI_HAS_METHOD_SIGN_IMPL(name, f)                                                                      \
    template<class T, class TSign> std::true_type has_##name##_impl(non_type<TSign, &T::f>* = 0);                                \
    template<class, class> std::false_type has_##name##_impl(...);                                                               \
    template<class T, class TSign> struct has_##name : decltype(has_##name##_impl<T, TSign>(0)) { };    \

namespace boost {
namespace di {
namespace aux {

template <bool b, class T=void>
  using enable_if_t       = typename std::enable_if<b,T>::type;  // C++14

template <bool b, class T, class F>
  using conditional_t     = typename std::conditional<b,T,F>::type;  // C++14

template<class T, class... TArgs>
using is_constructible_t = typename std::is_constructible<T, TArgs...>::type;

template<class T>
using remove_pointer_t = typename std::remove_pointer<T>::type;

template<class T>
using remove_reference_t = typename std::remove_reference<T>::type;

template<class T>
using remove_accessors = std::remove_cv<aux::remove_pointer_t<aux::remove_reference_t<T>>>;

template<class T>
using remove_accessors_t = typename remove_accessors<T>::type;

template<class>
struct make_plain;

BOOST_DI_HAS_TYPE(element_type);
BOOST_DI_HAS_TYPE(named_type);

template<class T, class = void>
struct deref_type {
    using type = T;
};

template<class T>
struct deref_type<T, typename std::enable_if<has_element_type<T>::value>::type> {
    using type = typename T::element_type;
};

template<class T>
struct deref_type<T, typename std::enable_if<has_named_type<T>::value>::type> {
    using type = typename make_plain<typename T::named_type>::type;
};

template<class T>
struct make_plain
    : deref_type<
          typename remove_accessors<
              typename deref_type<
                  typename remove_accessors<T>::type
              >::type
          >::type
      >
{ };

template<class T>
using make_plain_t = typename make_plain<T>::type;

template<class T>
struct function_traits
    : function_traits<decltype(&T::operator())>
{ };

template<class R, class... TArgs>
struct function_traits<R(*)(TArgs...)> {
    using result_type = R;
    using base_type = none_t;
    using args = type_list<TArgs...>;
};

template<class R, class T, class... TArgs>
struct function_traits<R(T::*)(TArgs...)> {
    using result_type = R;
    using base_type = T;
    using args = type_list<TArgs...>;
};

template<class R, class T, class... TArgs>
struct function_traits<R(T::*)(TArgs...) const> {
    using result_type = R;
    using base_type = T;
    using args = type_list<TArgs...>;
};

} // namespace aux
} // namespace boost
} // namespace di

#endif

