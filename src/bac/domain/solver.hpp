
#pragma once

#include "board.hpp"
#include "options.hpp"
#include <map>

namespace bac {

    struct PossibleCodes
    {
        std::vector<Code> codes;
    };

    PossibleCodes generate_all_possible_codes(const Options& options);

    // returns the lexicographically smallest Code consistent with all previous feedbacks
    // init is the first Code to be considered and the caller guarantees there is no Code before init that is consistent
    Code pick_first_compatible(const Options& options, const std::vector<AttemptAndFeedback>& attempts_and_feedbacks);
    Code pick_first_compatible(const Options& options, const std::vector<AttemptAndFeedback>& attempts_and_feedbacks,
                               const Code& init);

    Code pick_random_attempt(const PossibleCodes& possible_solutions);

    // Given the codemaker's feedback,
    // Remove from all the possibles codes the ones that are not possible anymore
    void remove_incompatible_codes_from_possible_solutions(const AttemptAndFeedback& attempt_and_feedback,
                                                           PossibleCodes& possible_solutions);

    // given an attempt and the history of previous attempts and feedbacks
    // returns a map in format [key, value] where:
    //  key = feedback for the attempt
    //  value = how many solutions are consistent with the history and this new feedback
    std::map<Feedback, size_t> count_all(const Options& options,
                                         const std::vector<AttemptAndFeedback>& attempts_and_feedbacks,
                                         const Code& attempt);

} // namespace bac
