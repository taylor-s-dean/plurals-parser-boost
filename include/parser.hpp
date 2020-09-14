#pragma once

#include <iomanip>
#include <boost/spirit/home/x3.hpp>

#include "ast.hpp"

namespace client {
namespace parser {
    namespace x3 = boost::spirit::x3;

    using expression_type = x3::rule<class expression, ast::operand>;
    using conditional_type = x3::rule<class conditional, ast::operand>;
    using primary_type = x3::rule<class primary, ast::operand>;
    using logical_type = x3::rule<class logical, ast::expression>;
    using equality_type = x3::rule<class equality, ast::expression>;
    using relational_type = x3::rule<class relational, ast::expression>;
    using multiplicative_type = x3::rule<class multiplicative, ast::expression>;
    using variable_type = x3::rule<class variable, std::string>;
    // clang-format on

    BOOST_SPIRIT_DECLARE(expression_type);

} // namespace parser

parser::expression_type expression();

} // namespace client
