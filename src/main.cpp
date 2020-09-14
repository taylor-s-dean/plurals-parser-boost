#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <map>
#include <boost/spirit/home/x3.hpp>

#include "CLI/App.hpp"
#include "CLI/Formatter.hpp"
#include "CLI/Config.hpp"

#include "ast.hpp"
#include "ast_adapted.hpp"
#include "parser.hpp"

typedef unsigned int uint;
namespace x3 = boost::spirit::x3;

bool
run_tests() {
    std::map<std::string, std::function<uint(uint)>> test_expressions{
        std::make_pair<std::string, std::function<uint(uint)>>(
            "0", [](uint n) { return 0; }),
        std::make_pair<std::string, std::function<uint(uint)>>(
            "(n == 0) ? 0 : ((n == 1) ? 1 : 2)",
            [](uint n) { return (n == 0) ? 0 : ((n == 1) ? 1 : 2); }),
        std::make_pair<std::string, std::function<uint(uint)>>(
            "(n == 0) ? 0 : ((n == 1) ? 1 : (((n % 100 == 2 || n % 100 == "
            "22 || n % 100 == 42 || n % 100 == 62 || n % 100 == 82) || n % "
            "1000 == 0 && (n % 100000 >= 1000 && n % 100000 <= 20000 || n "
            "% 100000 == 40000 || n % 100000 == 60000 || n % 100000 == "
            "80000) || n != 0 && n % 1000000 == 100000) ? 2 : ((n % 100 == "
            "3 || n % 100 == 23 || n % 100 == 43 || n % 100 == 63 || n % "
            "100 == 83) ? 3 : ((n != 1 && (n % 100 == 1 || n % 100 == 21 "
            "|| n % 100 == 41 || n % 100 == 61 || n % 100 == 81)) ? 4 : "
            "5))))",
            [](uint n) {
                return (n == 0)
                           ? 0
                           : ((n == 1)
                                  ? 1
                                  : (((n % 100 == 2 || n % 100 == 22 ||
                                       n % 100 == 42 || n % 100 == 62 ||
                                       n % 100 == 82) ||
                                      n % 1000 == 0 &&
                                          (n % 100000 >= 1000 &&
                                               n % 100000 <= 20000 ||
                                           n % 100000 == 40000 ||
                                           n % 100000 == 60000 ||
                                           n % 100000 == 80000) ||
                                      n != 0 && n % 1000000 == 100000)
                                         ? 2
                                         : ((n % 100 == 3 || n % 100 == 23 ||
                                             n % 100 == 43 || n % 100 == 63 ||
                                             n % 100 == 83)
                                                ? 3
                                                : ((n != 1 && (n % 100 == 1 ||
                                                               n % 100 == 21 ||
                                                               n % 100 == 41 ||
                                                               n % 100 == 61 ||
                                                               n % 100 == 81))
                                                       ? 4
                                                       : 5))));
            }),
        std::make_pair<std::string, std::function<uint(uint)>>(
            "(n == 0) ? 0 : ((n == 1) ? 1 : ((n == 2) ? 2 : ((n % 100 >= 3 "
            "&& n % 100 <= 10) ? 3 : ((n % 100 >= 11 && n % 100 <= 99) ? 4 "
            ": 5))))",
            [](uint n) {
                return (n == 0)
                           ? 0
                           : ((n == 1)
                                  ? 1
                                  : ((n == 2) ? 2
                                              : ((n % 100 >= 3 && n % 100 <= 10)
                                                     ? 3
                                                     : ((n % 100 >= 11 &&
                                                         n % 100 <= 99)
                                                            ? 4
                                                            : 5))));
            }),
        std::make_pair<std::string, std::function<uint(uint)>>(
            "(n == 0) ? 0 : ((n == 1) ? 1 : ((n == 2) ? 2 : ((n == 3) ? 3 "
            ": ((n == 6) ? 4 : 5))))",
            [](uint n) {
                return (n == 0)
                           ? 0
                           : ((n == 1)
                                  ? 1
                                  : ((n == 2)
                                         ? 2
                                         : ((n == 3) ? 3
                                                     : ((n == 6) ? 4 : 5))));
            }),
        std::make_pair<std::string, std::function<uint(uint)>>(
            "(n == 0 || n == 1) ? 0 : ((n >= 2 && n <= 10) ? 1 : 2)",
            [](uint n) {
                return (n == 0 || n == 1) ? 0 : ((n >= 2 && n <= 10) ? 1 : 2);
            }),
        std::make_pair<std::string, std::function<uint(uint)>>(
            "n != 1", [](uint n) { return n != 1; }),
        std::make_pair<std::string, std::function<uint(uint)>>(
            "n > 1", [](uint n) { return n > 1; }),
        std::make_pair<std::string, std::function<uint(uint)>>(
            "(n % 100 == 1) ? 0 : ((n % 100 == 2) ? 1 : ((n % 100 == 3 || "
            "n % 100 == 4) ? 2 : 3))",
            [](uint n) {
                return (n % 100 == 1)
                           ? 0
                           : ((n % 100 == 2)
                                  ? 1
                                  : ((n % 100 == 3 || n % 100 == 4) ? 2 : 3));
            }),
        std::make_pair<std::string, std::function<uint(uint)>>(
            "(n % 10 == 0 || n % 100 >= 11 && n % 100 <= 19) ? 0 : ((n % "
            "10 == 1 && n % 100 != 11) ? 1 : 2)",
            [](uint n) {
                return (n % 10 == 0 || n % 100 >= 11 && n % 100 <= 19)
                           ? 0
                           : ((n % 10 == 1 && n % 100 != 11) ? 1 : 2);
            }),
        std::make_pair<std::string, std::function<uint(uint)>>(
            "(n % 10 == 1) ? 0 : ((n % 10 == 2) ? 1 : ((n % 100 == 0 || n "
            "% 100 == 20 || n % 100 == 40 || n % 100 == 60 || n % 100 == "
            "80) ? 2 : 3))",
            [](uint n) {
                return (n % 10 == 1)
                           ? 0
                           : ((n % 10 == 2) ? 1
                                            : ((n % 100 == 0 || n % 100 == 20 ||
                                                n % 100 == 40 ||
                                                n % 100 == 60 || n % 100 == 80)
                                                   ? 2
                                                   : 3));
            }),
        std::make_pair<std::string, std::function<uint(uint)>>(
            "n % 10 != 1 || n % 100 == 11",
            [](uint n) { return n % 10 != 1 || n % 100 == 11; }),
        std::make_pair<std::string, std::function<uint(uint)>>(
            "(n % 10 == 1 && n % 100 != 11) ? 0 : ((n % 10 >= 2 && n % 10 "
            "<= 4 && (n % 100 < 12 || n % 100 > 14)) ? 1 : 2)",
            [](uint n) {
                return (n % 10 == 1 && n % 100 != 11)
                           ? 0
                           : ((n % 10 >= 2 && n % 10 <= 4 &&
                               (n % 100 < 12 || n % 100 > 14))
                                  ? 1
                                  : 2);
            }),
        std::make_pair<std::string, std::function<uint(uint)>>(
            "(n % 10 == 1 && (n % 100 < 11 || n % 100 > 19)) ? 0 : ((n % "
            "10 >= 2 && n % 10 <= 9 && (n % 100 < 11 || n % 100 > 19)) ? 1 "
            ": 2)",
            [](uint n) {
                return (n % 10 == 1 && (n % 100 < 11 || n % 100 > 19))
                           ? 0
                           : ((n % 10 >= 2 && n % 10 <= 9 &&
                               (n % 100 < 11 || n % 100 > 19))
                                  ? 1
                                  : 2);
            }),
        std::make_pair<std::string, std::function<uint(uint)>>(
            "(n % 10 == 1 && n % 100 != 11 && n % 100 != 71 && n % 100 != "
            "91) ? 0 : ((n % 10 == 2 && n % 100 != 12 && n % 100 != 72 && "
            "n % 100 != 92) ? 1 : ((((n % 10 == 3 || n % 10 == 4) || n % "
            "10 == 9) && (n % 100 < 10 || n % 100 > 19) && (n % 100 < 70 "
            "|| n % 100 > 79) && (n % 100 < 90 || n % 100 > 99)) ? 2 : ((n "
            "!= 0 && n % 1000000 == 0) ? 3 : 4)))",
            [](uint n) {
                return (n % 10 == 1 && n % 100 != 11 && n % 100 != 71 &&
                        n % 100 != 91)
                           ? 0
                           : ((n % 10 == 2 && n % 100 != 12 && n % 100 != 72 &&
                               n % 100 != 92)
                                  ? 1
                                  : ((((n % 10 == 3 || n % 10 == 4) ||
                                       n % 10 == 9) &&
                                      (n % 100 < 10 || n % 100 > 19) &&
                                      (n % 100 < 70 || n % 100 > 79) &&
                                      (n % 100 < 90 || n % 100 > 99))
                                         ? 2
                                         : ((n != 0 && n % 1000000 == 0) ? 3
                                                                         : 4)));
            }),
        std::make_pair<std::string, std::function<uint(uint)>>(
            "(n == 1) ? 0 : ((n == 0 || n % 100 >= 2 && n % 100 <= 10) ? 1 "
            ": ((n % 100 >= 11 && n % 100 <= 19) ? 2 : 3))",
            [](uint n) {
                return (n == 1)
                           ? 0
                           : ((n == 0 || n % 100 >= 2 && n % 100 <= 10)
                                  ? 1
                                  : ((n % 100 >= 11 && n % 100 <= 19) ? 2 : 3));
            }),
        std::make_pair<std::string, std::function<uint(uint)>>(
            "(n == 1) ? 0 : ((n == 0 || n % 100 >= 2 && n % 100 <= 19) ? 1 "
            ": 2)",
            [](uint n) {
                return (n == 1)
                           ? 0
                           : ((n == 0 || n % 100 >= 2 && n % 100 <= 19) ? 1
                                                                        : 2);
            }),
        std::make_pair<std::string, std::function<uint(uint)>>(
            "(n == 1) ? 0 : ((n % 10 >= 2 && n % 10 <= 4 && (n % 100 < 12 "
            "|| n % 100 > 14)) ? 1 : 2)",
            [](uint n) {
                return (n == 1) ? 0
                                : ((n % 10 >= 2 && n % 10 <= 4 &&
                                    (n % 100 < 12 || n % 100 > 14))
                                       ? 1
                                       : 2);
            }),
        std::make_pair<std::string, std::function<uint(uint)>>(
            "(n == 1) ? 0 : ((n == 2) ? 1 : 2)",
            [](uint n) { return (n == 1) ? 0 : ((n == 2) ? 1 : 2); }),
        std::make_pair<std::string, std::function<uint(uint)>>(
            "(n == 1) ? 0 : ((n == 2) ? 1 : ((n > 10 && n % 10 == 0) ? 2 : "
            "3))",
            [](uint n) {
                return (n == 1)
                           ? 0
                           : ((n == 2) ? 1 : ((n > 10 && n % 10 == 0) ? 2 : 3));
            }),
        std::make_pair<std::string, std::function<uint(uint)>>(
            "(n == 1) ? 0 : ((n == 2) ? 1 : ((n >= 3 && n <= 6) ? 2 : ((n "
            ">= 7 && n <= 10) ? 3 : 4)))",
            [](uint n) {
                return (n == 1)
                           ? 0
                           : ((n == 2) ? 1
                                       : ((n >= 3 && n <= 6)
                                              ? 2
                                              : ((n >= 7 && n <= 10) ? 3 : 4)));
            }),
        std::make_pair<std::string, std::function<uint(uint)>>(
            "(n == 1) ? 0 : ((n >= 2 && n <= 4) ? 1 : 2)",
            [](uint n) { return (n == 1) ? 0 : ((n >= 2 && n <= 4) ? 1 : 2); }),
        std::make_pair<std::string, std::function<uint(uint)>>(
            "(n == 1 || n == 11) ? 0 : ((n == 2 || n == 12) ? 1 : ((n >= 3 "
            "&& n <= 10 || n >= 13 && n <= 19) ? 2 : 3))",
            [](uint n) {
                return (n == 1 || n == 11)
                           ? 0
                           : ((n == 2 || n == 12)
                                  ? 1
                                  : ((n >= 3 && n <= 10 || n >= 13 && n <= 19)
                                         ? 2
                                         : 3));
            }),
        std::make_pair<std::string, std::function<uint(uint)>>(
            "n != 1 && n != 2 && n != 3 && (n % 10 == 4 || n % 10 == 6 || "
            "n % 10 == 9)",
            [](uint n) {
                return n != 1 && n != 2 && n != 3 &&
                       (n % 10 == 4 || n % 10 == 6 || n % 10 == 9);
            }),
        std::make_pair<std::string, std::function<uint(uint)>>(
            "n >= 2 && (n < 11 || n > 99)",
            [](uint n) { return n >= 2 && (n < 11 || n > 99); }),
    };

    bool success{true};
    for (const std::pair<std::string, std::function<uint(uint)>>& key_value :
         test_expressions) {
        for (uint idx = 0; idx <= 1000; ++idx) {
            const std::string& str{key_value.first};
            client::ast::operand program;
            client::ast::evaluator eval(idx);
            client::ast::printer print;

            std::string::const_iterator iter = str.begin();
            std::string::const_iterator end = str.end();
            bool r = phrase_parse(
                iter, end, client::expression(), x3::space, program);

            if (r && iter == end) {
                const uint result{eval(program)};
                const uint truth{key_value.second(idx)};

                if (result != truth) {
                    std::cout << "-------------------------" << std::endl;
                    std::cout << "Program:    ";
                    print(program);
                    std::cout << std::endl;
                    std::cout << "Expression: " << std::quoted(str)
                              << std::endl;
                    std::cout << "n: " << idx << std::endl;
                    std::cout << "Result: " << result << std::endl;
                    std::cout << "Truth: " << truth << std::endl;
                    std::cout << "-------------------------" << std::endl;

                    std::cout << "FAIL: Result did not match truth!"
                              << std::endl;
                    success = false;
                }
            } else {
                std::string rest(iter, end);
                std::cout << "Parsing failed" << std::endl;
                std::cout << "stopped at: \" " << rest << "\"" << std::endl;
                std::cout << "-------------------------" << std::endl;
                return EXIT_FAILURE;
            }
        }
    }

    return success;
}

bool
evaluate_plural_forms(
    std::string plural_forms, uint n, uint& result, bool verbose) {
    client::ast::operand program;
    client::ast::evaluator eval(n);
    client::ast::printer print;

    std::string::const_iterator iter = plural_forms.begin();
    std::string::const_iterator end = plural_forms.end();
    bool r = phrase_parse(iter, end, client::expression(), x3::space, program);

    if (r && iter == end) {
        result = eval(program);
        if (verbose) {
            std::cout << "Program:    ";
            print(program);
            std::cout << std::endl;
            std::cout << "Expression: " << std::quoted(plural_forms)
                      << std::endl;
            std::cout << "Result: " << result << std::endl;
        }
    } else {
        if (verbose) {
            std::string rest(iter, end);
            std::cout << "Parsing failed" << std::endl;
            std::cout << "stopped at: \" " << rest << "\"" << std::endl;
        }
        return false;
    }

    return true;
}

int
main(int argc, char** argv) {
    CLI::App app{
        "plurals-parser is a CLI tool that computes the result of a Gettext "
        "plural-forms ternary."};

    CLI::App* eval{
        app.add_subcommand("eval", "Evaluate a plural-forms ternary.")};

    std::string plural_forms;
    eval->add_option(
            "plural-forms", plural_forms, "A Gettext plural-forms ternary.")
        ->required(true);

    uint n;
    eval->add_option("-n,--n", n, "The value of n.")->required(true);

    bool verbose;
    eval->add_flag("-v,--verbose", verbose, "Be verbose.")->required(false);

    CLI::App* test{app.add_subcommand("test", "Run test suite.")};
    test->add_flag("-v,--verbose", verbose, "Be verbose.")->required(false);

    CLI11_PARSE(app, argc, argv);

    if (app.got_subcommand("test")) {
        if (run_tests()) {
            std::cout << "All tests passed." << std::endl;
        } else {
            std::cout << "Tests failed." << std::endl;
        }
    }

    if (app.got_subcommand("eval")) {
        uint result;
        if (!evaluate_plural_forms(plural_forms, n, result, verbose)) {
            std::cout << "Failed to parse plural-forms expression. Try running "
                         "with --verbose for more information."
                      << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << result << std::endl;
    }

    return EXIT_SUCCESS;
}
