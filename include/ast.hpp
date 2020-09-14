#pragma once

#include <list>
#include <string>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/foreach.hpp>

namespace client {
typedef unsigned int uint;
namespace ast {
    namespace x3 = boost::spirit::x3;
    struct nil {};
    struct binary_op;
    struct conditional_op;
    struct expression;

    struct binary_operator {
        std::string name;
        std::function<uint(uint, uint)> op;

        uint
        operator()(uint lhs, uint rhs) const {
            return op(lhs, rhs);
        }
    };

    struct operand : x3::variant<
                         nil,
                         uint,
                         std::string,
                         x3::forward_ast<binary_op>,
                         x3::forward_ast<conditional_op>,
                         x3::forward_ast<expression>> {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct binary_op {
        binary_operator op;
        operand lhs;
        operand rhs;
    };

    struct conditional_op {
        operand lhs;
        operand rhs_true;
        operand rhs_false;
    };

    struct operation {
        binary_operator op;
        operand rhs;
    };

    struct expression {
        operand lhs;
        std::list<operation> rhs;
    };

    struct printer {
        typedef void result_type;

        result_type
        operator()(operand const& ast) const {
            boost::apply_visitor(*this, ast.get());
        }

        result_type
        operator()(nil) const {}

        result_type
        operator()(expression const& ast) const {
            if (ast.rhs.size() > 0) {
                std::cout << '(';
            }
            boost::apply_visitor(*this, ast.lhs);
            BOOST_FOREACH (operation const& op, ast.rhs) { (*this)(op); }
            if (ast.rhs.size() > 0) {
                std::cout << ')';
            }
        }

        result_type
        operator()(operation const& ast) const {
            std::cout << ' ' << ast.op.name << ' ';
            boost::apply_visitor(*this, ast.rhs);
        }

        result_type
        operator()(binary_op const& ast) const {
            std::cout << '(';
            boost::apply_visitor(*this, ast.lhs);
            boost::apply_visitor(*this, ast.rhs);
            std::cout << ')';
        }

        result_type
        operator()(conditional_op const& ast) const {
            std::cout << '(';
            boost::apply_visitor(*this, ast.lhs);
            std::cout << " ? ";
            boost::apply_visitor(*this, ast.rhs_true);
            std::cout << " : ";
            boost::apply_visitor(*this, ast.rhs_false);
            std::cout << ')';
        }

        result_type
        operator()(uint const& ast) const {
            std::cout << ast;
        }

        result_type
        operator()(std::string const& ast) const {
            std::cout << ast;
        }
    };

    struct evaluator {
        typedef uint result_type;

        evaluator(const result_type variable) : variable(variable) {}
        result_type variable;

        result_type
        operator()(operand const& ast) const {
            return boost::apply_visitor(*this, ast.get());
        }

        result_type
        operator()(nil) const {
            BOOST_ASSERT(0);
            return 0;
        }

        result_type
        operator()(expression const& ast) const {
            result_type state = boost::apply_visitor(*this, ast.lhs);
            BOOST_FOREACH (operation const& op, ast.rhs) {
                state = (*this)(op, state);
            }
            return state;
        }

        result_type
        operator()(operation const& ast, uint lhs) const {
            result_type rhs = boost::apply_visitor(*this, ast.rhs);
            return ast.op(lhs, rhs);
        }

        result_type
        operator()(binary_op const& ast) const {
            result_type lhs = boost::apply_visitor(*this, ast.lhs);
            result_type rhs = boost::apply_visitor(*this, ast.rhs);
            return ast.op(lhs, rhs);
        }

        result_type
        operator()(conditional_op const& ast) const {
            bool lhs = boost::apply_visitor(*this, ast.lhs);
            if (lhs) {
                return boost::apply_visitor(*this, ast.rhs_true);
            }
            return boost::apply_visitor(*this, ast.rhs_false);
        }

        result_type
        operator()(uint const& ast) const {
            return ast;
        }

        result_type
        operator()(std::string const& ast) const {
            return variable;
        }
    };

} // namespace ast
} // namespace client
