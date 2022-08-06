#pragma once
#include <string>
#include <variant>
#include <iostream>
#include <iomanip>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
namespace boost::spirit::x3::traits {
    template<typename... t>
    struct is_variant<std::variant<t...> >
        : mpl::true_ {};

    template <typename attribute, typename... t>
    struct variant_has_substitute_impl<std::variant<t...>, attribute>
    {
        typedef std::variant<t...> variant_type;
        typedef typename mpl::transform<
              mpl::list<t...>
            , unwrap_recursive<mpl::_1>
            >::type types;
        typedef typename mpl::end<types>::type end;

        typedef typename mpl::find<types, attribute>::type iter_1;

        typedef typename
            mpl::eval_if<
                is_same<iter_1, end>,
                mpl::find_if<types, traits::is_substitute<mpl::_1, attribute>>,
                mpl::identity<iter_1>
            >::type
        iter;

        typedef mpl::not_<is_same<iter, end>> type;
    };


    template <typename attribute, typename... t>
    struct variant_find_substitute<std::variant<t...>, attribute>
    {
        typedef std::variant<t...> variant_type;
        typedef typename mpl::transform<
              mpl::list<t...>
            , unwrap_recursive<mpl::_1>
            >::type types;

        typedef typename mpl::end<types>::type end;

        typedef typename mpl::find<types, attribute>::type iter_1;

        typedef typename
            mpl::eval_if<
                is_same<iter_1, end>,
                mpl::find_if<types, traits::is_substitute<mpl::_1, attribute> >,
                mpl::identity<iter_1>
            >::type
        iter;

        typedef typename
            mpl::eval_if<
                is_same<iter, end>,
                mpl::identity<attribute>,
                mpl::deref<iter>
            >::type
        type;
    };

    template <typename... t>
    struct variant_find_substitute<std::variant<t...>, std::variant<t...> >
        : mpl::identity<std::variant<t...> > {};
}
