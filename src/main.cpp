#include <iostream>
#include <iomanip>
#include <map>
#include <boost/spirit/home/x3.hpp>
#include "ast.hpp"
#include "ast_adapted.hpp"
#include "parser.hpp"

int
main() {
    namespace x3 = boost::spirit::x3;
    typedef unsigned int uint;

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
                    return EXIT_FAILURE;
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

    std::cout << "///////////////////////////////////////////////////\n\n";
    std::cout << "Expression parser...\n\n";
    std::cout << "///////////////////////////////////////////////////\n\n";

    std::string str;
    while (true) {
        std::cout << "Type an expression...or [q or Q] to quit\n\n";
        std::getline(std::cin, str);

        if (str.empty() || str[0] == 'q' || str[0] == 'Q')
            break;

        std::cout << "Enter variable value: ";
        std::string variable_str;
        std::getline(std::cin, variable_str);
        uint variable = std::stod(variable_str);

        client::ast::operand program;
        client::ast::evaluator eval(variable);
        client::ast::printer print;

        std::string::const_iterator iter = str.begin();
        std::string::const_iterator end = str.end();
        bool r =
            phrase_parse(iter, end, client::expression(), x3::space, program);

        if (r && iter == end) {
            std::cout << "-------------------------" << std::endl;
            std::cout << "Program:    ";
            print(program);
            std::cout << std::endl;
            std::cout << "Expression: " << std::quoted(str) << std::endl;
            std::cout << "Result: " << eval(program) << std::endl;
            std::cout << "-------------------------" << std::endl;
        } else {
            std::string rest(iter, end);
            std::cout << "-------------------------" << std::endl;
            std::cout << "Parsing failed" << std::endl;
            std::cout << "stopped at: \" " << rest << "\"" << std::endl;
            std::cout << "-------------------------" << std::endl;
        }
    }

    std::cout << "Bye... :-) \n\n";
    return EXIT_SUCCESS;
}
