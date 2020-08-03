
#include "board.hpp"
#include "options.hpp"
#include <iostream>

namespace bac {

    void Board::display(std::ostream& out, const Options& options, DisplaySecretCode display_secret_code) const
    {
        const size_t n = secret_code.size();
        std::string pre = "| #        ";            // | #01
        std::string code = std::string(2 * n, ' '); // A B C D E
        std::string post = "|       |      |";      // |  1  |   3    |

        const std::string sep(pre.size() + 2 * n + post.size(), '-');

        out << sep << '\n';
        for (size_t i = 0; i < n; ++i)
            code[2 * i] = display_secret_code == DisplaySecretCode::Yes ? secret_code[i] : '*';
        out << "| SECRET   " << code << "|              |" << '\n';
        out << sep << '\n';
        code = std::string(2 * n, ' ');
        out << "| ATTEMPTS " << code << "| BULLS | COWS |" << '\n';
        out << sep << '\n';

        size_t round{options.max_number_of_attempts};
        for (size_t i = 0; i < n; ++i)
            code[2 * i] = '.';
        while (round > 0)
        {
            pre[3] = '0' + round / 10; // expecting max_number_of_attempts < 100
            pre[4] = '0' + round % 10;
            --round;
            if (attempts_and_feedbacks.size() > round)
            {
                for (size_t i = 0; i < n; ++i)
                    code[2 * i] = attempts_and_feedbacks[round].attempt[i];
                post[4] = '0' + attempts_and_feedbacks[round].feedback.bulls; // expecting secret_size < 10
                post[11] = '0' + attempts_and_feedbacks[round].feedback.cows;
            }
            out << pre << code << post << '\n';
        }
        out << sep << '\n';
    }

    void display_board(std::ostream& out, const Options& options, const Board& board,
                       DisplaySecretCode display_secret_code)
    {
        board.display(out, options, display_secret_code);
    }

    void ComputerBoard::display(std::ostream& out, const Options& options, DisplaySecretCode display_secret_code) const
    {
        const size_t n = secret_code.size();
        std::string pre = "| #        ";            // | #01
        std::string code = std::string(2 * n, ' '); // A B C D E
        std::string post = "|       |      |";      // |  1  |   3    |

        const std::string sep(pre.size() + 2 * n + post.size(), '-');

        if (attempts_and_feedbacks.empty())
        {
            out << sep << '\n';
            for (size_t i = 0; i < n; ++i)
                code[2 * i] = secret_code[i];
            out << "| SECRET   " << code << "|              |" << '\n';
            out << sep << '\n';
            code = std::string(2 * n, ' ');
            out << "| ATTEMPTS " << code << "| BULLS | COWS |" << '\n';
            out << sep << '\n';
        }
        else
        {
            size_t round{attempts_and_feedbacks.size()};
            pre[3] = '0' + round / 10; // expecting max_number_of_attempts < 100
            pre[4] = '0' + round % 10;
            --round;
            for (size_t i = 0; i < n; ++i)
                code[2 * i] = attempts_and_feedbacks[round].attempt[i];
            post[4] = '0' + attempts_and_feedbacks[round].feedback.bulls; // expecting secret_size < 10
            post[11] = '0' + attempts_and_feedbacks[round].feedback.cows;
            out << pre << code << post << '\n';
            if (attempts_and_feedbacks.back().feedback.bulls == secret_code.size())
                out << sep << '\n';
        }
    }

} // namespace bac
