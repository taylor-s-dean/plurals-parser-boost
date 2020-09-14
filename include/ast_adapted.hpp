#pragma once

#include <boost/fusion/include/adapt_struct.hpp>
#include "ast.hpp"

BOOST_FUSION_ADAPT_STRUCT(client::ast::expression, lhs, rhs)
BOOST_FUSION_ADAPT_STRUCT(client::ast::operation, op, rhs)
BOOST_FUSION_ADAPT_STRUCT(client::ast::conditional_op, lhs, rhs_true, rhs_false)
BOOST_FUSION_ADAPT_STRUCT(client::ast::binary_op, op, lhs, rhs)
