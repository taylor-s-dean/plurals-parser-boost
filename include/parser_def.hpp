#pragma once

#include <iostream>
#include <functional>
#include <iomanip>

#include <boost/spirit/home/x3.hpp>

#include "ast.hpp"
#include "ast_adapted.hpp"
#include "parser.hpp"

namespace client {
namespace parser {
    namespace x3 = boost::spirit::x3;

    expression_type const expression{"expression"};
    conditional_type const conditional{"conditional"};
    primary_type const primary{"primary"};
    logical_type const logical{"logical"};
    equality_type const equality{"equality"};
    relational_type const relational{"relational"};
    multiplicative_type const multiplicative{"multiplicative"};
    variable_type const variable{"variable"};

#define add_operation(NAME, OP) this->add(NAME, {NAME, OP});

    struct multiplicative_op_ : x3::symbols<ast::binary_operator> {
        multiplicative_op_() {
            add_operation("%", [](uint a, uint b) { return std::fmod(a, b); });
        }
    } multiplicative_op;

    struct logical_op_ : x3::symbols<ast::binary_operator> {
        logical_op_() {
            add_operation("&&", std::logical_and<uint>{});
            add_operation("||", std::logical_or<uint>{});
        }
    } logical_op;

    struct relational_op_ : x3::symbols<ast::binary_operator> {
        relational_op_() {
            add_operation("<", std::less<uint>{});
            add_operation("<=", std::less_equal<uint>{});
            add_operation(">", std::greater<uint>{});
            add_operation(">=", std::greater_equal<uint>{});
        }
    } relational_op;

    struct equality_op_ : x3::symbols<ast::binary_operator> {
        equality_op_() {
            add_operation("==", std::equal_to<uint>{});
            add_operation("!=", std::not_equal_to<uint>{});
        }
    } equality_op;
#undef add_operation

    auto make_conditional_op = [](auto& ctx) {
        using boost::fusion::at_c;
        x3::_val(ctx) = ast::conditional_op{
            x3::_val(ctx), at_c<0>(x3::_attr(ctx)), at_c<1>(x3::_attr(ctx))};
    };

    // Rule defintions
    auto const expression_def = conditional;

    auto const conditional_def =
        logical[([](auto& ctx) { _val(ctx) = _attr(ctx); })] >>
        -('?' > expression > ':' > expression)[make_conditional_op];

    auto const logical_def = equality >> *((logical_op > logical) |
                                           (logical_op > equality));

    auto const equality_def = relational >> *(equality_op > relational);

    auto const relational_def = multiplicative >>
                                *(relational_op > multiplicative);

    auto const multiplicative_def = primary >> *(multiplicative_op > primary);

    auto const primary_def = x3::uint_ | ('(' > expression > ')') | variable;

    auto const variable_def = x3::lexeme[x3::alpha >> *x3::alnum];
    // clang-format on

    BOOST_SPIRIT_DEFINE(
        expression,
        logical,
        equality,
        relational,
        multiplicative,
        primary,
        conditional,
        variable);

} // namespace parser

parser::expression_type
expression() {
    return parser::expression;
}

} // namespace client
