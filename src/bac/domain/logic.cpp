
#include "logic.hpp"
#include "board.hpp"
#include "codebreaker.hpp"
#include "codemaker.hpp"
#include <bac/util/random.hpp>
#include <cassert>
#include <iostream>
#include <map>

namespace bac {

    Code generate_random_secret_code(const Options& options)
    {
        Code ret(options.number_of_characters_per_code, options.minimum_allowed_character);
        for (char& c : ret)
            c = generate_random_value_between(options.minimum_allowed_character, options.maximum_allowed_character);
        return ret;
    }

    bool validate_attempt(const Options& options, const Code& attempt)
    {
        if (attempt.size() != options.number_of_characters_per_code)
            return false;
        for (char c : attempt)
            if (options.minimum_allowed_character > c || options.maximum_allowed_character < c)
                return false;
        return true;
    }

    Feedback compare_attempt_with_secret_code(const Code& attempt, const Code& secret_code, size_t len)
    {
        assert(attempt.size() == secret_code.size());
        std::map<char, int> a_m, s_m; // could be normalized vector
        Feedback fb;
        for (size_t i = 0; i < attempt.size(); ++i)
        {
            ++s_m[secret_code[i]];
            if (i < len)
            {
                ++a_m[attempt[i]];
                if (attempt[i] == secret_code[i])
                    ++fb.bulls;
            }
        }
        for (const auto& [k, v] : a_m)
            fb.cows += std::min(v, s_m[k]);
        fb.cows -= fb.bulls;
        return fb;
    }

    Feedback compare_attempt_with_secret_code(const Code& attempt, const Code& secret_code)
    {
        assert(attempt.size() == secret_code.size());
        const Code& a{attempt};
        const Code& s{secret_code};
        std::map<char, int> a_m, s_m;
        Feedback fb;
        for (size_t i = 0; i < a.size(); ++i)
        {
            ++a_m[a[i]];
            ++s_m[s[i]];
            if (a[i] == s[i])
                ++fb.bulls;
        }
        for (const auto& [k, v] : a_m)
            fb.cows += std::min(v, s_m[k]);
        fb.cows -= fb.bulls;
        return fb;
    }

    bool has_codemaker_won(const Options& options, const Board& board)
    {
        return (options.max_number_of_attempts == board.attempts_and_feedbacks.size() &&
                board.attempts_and_feedbacks.back().feedback.bulls != options.number_of_characters_per_code);
    }

    bool has_codebreaker_won(const Options& options, const Board& board)
    {
        return (!board.attempts_and_feedbacks.empty() &&
                board.attempts_and_feedbacks.back().feedback.bulls == options.number_of_characters_per_code);
    }

    void play(std::ostream& out, std::istream& in, const Options& options, State& state)
    {
        Codebreaker& breaker{*state.codebreaker};
        Codemaker& maker{*state.codemaker};
        Board& board{*state.board};

        board.secret_code = maker.choose_secret_code(out, options);
        do
        {
            board.display(out, options, DisplaySecretCode::No);
            const auto attempt = breaker.ask_attempt(out, in, options, board.attempts_and_feedbacks);
            board.attempts_and_feedbacks.push_back({attempt, maker.compare_attempt_with_secret_code(out, attempt)});
        } while (!has_codebreaker_won(options, board) && !has_codemaker_won(options, board));
        board.display(out, options, DisplaySecretCode::Yes);
        const std::string winner = has_codebreaker_won(options, board) ? "Codebreaker" : "Codemaker";
        out << winner << " wins!\n\n";
    }

} // namespace bac
