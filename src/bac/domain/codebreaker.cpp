
#include "codebreaker.hpp"
#include "logic.hpp"
#include "solver.hpp"
#include <bac/util/input.hpp>
#include <iostream>

namespace bac {

    Code HumanCodebreaker::ask_attempt(std::ostream& out, std::istream& in, const Options& options,
                                       const std::vector<AttemptAndFeedback>& attempts_and_feedbacks)
    {
        if (attempts_and_feedbacks.empty())
            out << "Hey, codebreaker!\n";
        bool first{true};
        Code att;
        do
        {
            if (!first)
                out << "Your guess has an invalid length or contains non-allowed characters, please try again\n";
            else
                first = false;
            out << "What is your guess #" << attempts_and_feedbacks.size() + 1 << " ("
                << options.number_of_characters_per_code << " characters between '" << options.minimum_allowed_character
                << "' and '" << options.maximum_allowed_character << "')\n";
            out << "? ";
            att = Code{std::move(ask_string(in))};
        } while (!validate_attempt(options, att));
        return Code{att};
    }

    Code RandomComputerCodebreaker::ask_attempt(std::ostream& out, std::istream& in, const Options& options,
                                                const std::vector<AttemptAndFeedback>& attempts_and_feedbacks)
    {
        if (attempts_and_feedbacks.empty())
            m_remaining_possible_codes = generate_all_possible_codes(options);
        else
            remove_incompatible_codes_from_possible_solutions(attempts_and_feedbacks.back(),
                                                              m_remaining_possible_codes);
        return pick_random_attempt(m_remaining_possible_codes);
    }

    Code GreedyComputerCodebreaker::ask_attempt(std::ostream& out, std::istream& in, const Options& options,
                                                const std::vector<AttemptAndFeedback>& attempts_and_feedbacks)
    {
        if (attempts_and_feedbacks.empty())
            return pick_first_compatible(options, attempts_and_feedbacks);
        return pick_first_compatible(options, attempts_and_feedbacks, attempts_and_feedbacks.back().attempt);
    }

    Code RandomAndGreedyComputerCodebreaker::ask_attempt(std::ostream& out, std::istream& in, const Options& options,
                                                         const std::vector<AttemptAndFeedback>& attempts_and_feedbacks)
    {
        if (attempts_and_feedbacks.empty())
            return generate_random_secret_code(options);
        else if (attempts_and_feedbacks.size() == 1)
            return pick_first_compatible(options, attempts_and_feedbacks);
        return pick_first_compatible(options, attempts_and_feedbacks, attempts_and_feedbacks.back().attempt);
    }

} // namespace bac
