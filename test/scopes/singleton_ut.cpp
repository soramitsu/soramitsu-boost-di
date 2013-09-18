//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/scopes/singleton.hpp"

#include <boost/test/unit_test.hpp>

namespace boost {
namespace di {
namespace scopes {

BOOST_AUTO_TEST_CASE(create) {
    singleton<>::scope<int> singleton_;

    BOOST_CHECK((
        static_cast<shared_ptr<int>>(
            singleton_.create())
        ==
        static_cast<shared_ptr<int>>(
            singleton_.create())
    ));
}

BOOST_AUTO_TEST_CASE(create_args) {
    singleton<>::scope<c2> singleton_;

    BOOST_CHECK((
        static_cast<shared_ptr<c2>>(
            singleton_.create<int, double, char>(0, 0.0, '0'))
        ==
        static_cast<shared_ptr<c2>>(
            singleton_.create<int, double, char>(0, 0.0, '0'))
    ));
}

} // namespace scopes
} // namespace di
} // namespace boost

