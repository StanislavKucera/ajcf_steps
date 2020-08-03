
#include "solver.hpp"
#include "logic.hpp"
#include <bac/util/random.hpp>
#include <algorithm>
#include <cassert>
#include <iostream>

namespace bac {

    void dfs_generate(size_t i, const Options& options, std::string& str, std::vector<Code>& all_codes)
    {
        if (i == options.number_of_characters_per_code)
        {
            all_codes.push_back(Code{str});
            return;
        }
        for (str[i] = options.minimum_allowed_character; str[i] <= options.maximum_allowed_character; ++str[i])
            dfs_generate(i + 1, options, str, all_codes);
    }

    PossibleCodes generate_all_possible_codes(const Options& options)
    {
        std::string str(options.number_of_characters_per_code, ' ');
        std::vector<Code> all_codes;
        dfs_generate(0, options, str, all_codes);
        return PossibleCodes{std::move(all_codes)};
    }

    bool pretest(size_t i, const Code& att, const std::vector<AttemptAndFeedback>& attempts_and_feedbacks)
    {
        // std::cerr << "Pretesting " << att.value << ' ' << i << std::endl;
        for (const auto& aaf : attempts_and_feedbacks)
        {
            const auto fb = compare_attempt_with_secret_code(att, aaf.attempt, i);
            const size_t sum_fb = fb.bulls + fb.cows;
            const size_t sum_aaf = aaf.feedback.bulls + aaf.feedback.cows;
            // std::cerr << '(' << fb.bulls << ',' << fb.cows << ") (" << aaf.feedback.bulls << ','
            //           << aaf.feedback.cows << ')' << std::endl;
            if (fb.bulls > aaf.feedback.bulls || sum_fb > sum_aaf)
                return false;
            if (sum_fb + att.size() - i < sum_aaf)
            {
                // std::cerr << sum_fb << '+' << att.size() << '-' << i << '<' << sum_aaf << std::endl;
                return false;
            }
        }
        return true;
    }

    bool test_and_set(const Code& att, const std::vector<AttemptAndFeedback>& attempts_and_feedbacks, Code& code)
    {
        // std::cerr << "Testing " << att.value << std::endl;
        for (const auto& aaf : attempts_and_feedbacks)
        {
            const auto fb = compare_attempt_with_secret_code(att, aaf.attempt);
            if (fb.bulls != aaf.feedback.bulls || fb.cows != aaf.feedback.cows)
                return false;
        }
        code = att;
        return true;
    }

    bool dfs_find_first(size_t i, const Options& options, Code& cur, bool& initialized, const Code& init,
                        const std::vector<AttemptAndFeedback>& attempts_and_feedbacks, Code& code)
    {
        if (i == options.number_of_characters_per_code)
        {
            return test_and_set(cur, attempts_and_feedbacks, code);
        }
        for (cur[i] = initialized ? options.minimum_allowed_character : init[i];
             cur[i] <= options.maximum_allowed_character; initialized = true, ++cur[i])
            if (pretest(i + 1, cur, attempts_and_feedbacks) &&
                dfs_find_first(i + 1, options, cur, initialized, init, attempts_and_feedbacks, code))
                return true;
        return false;
    }

    Code pick_first_compatible(const Options& options, const std::vector<AttemptAndFeedback>& attempts_and_feedbacks)
    {
        return pick_first_compatible(options, attempts_and_feedbacks,
                                     Code(options.number_of_characters_per_code, options.minimum_allowed_character));
    }

    Code pick_first_compatible(const Options& options, const std::vector<AttemptAndFeedback>& attempts_and_feedbacks,
                               const Code& init)
    {
        Code cur(options.number_of_characters_per_code, ' '), ret;
        bool initialized{false};
        // std::cerr << "Init " << init.value << std::endl;
        dfs_find_first(0, options, cur, initialized, init, attempts_and_feedbacks, ret);
        assert(!ret.empty());
        // std::cerr << "Picked " << ret.value << std::endl;
        return ret;
    }

    Code pick_random_attempt(const PossibleCodes& possible_solutions)
    {
        return possible_solutions.codes[generate_random_value_between(0ul, possible_solutions.codes.size() - 1)];
    }

    void remove_incompatible_codes_from_possible_solutions(const AttemptAndFeedback& attempt_and_feedback,
                                                           PossibleCodes& possible_solutions)
    {
        std::vector<Code>& codes{possible_solutions.codes};
        codes.erase(std::remove_if(codes.begin(), codes.end(),
                                   [&](const Code& code) {
                                       return compare_attempt_with_secret_code(code, attempt_and_feedback.attempt) !=
                                              attempt_and_feedback.feedback;
                                   }),
                    codes.end());
    }

    bool test(const Code& cur, const std::vector<AttemptAndFeedback>& attempts_and_feedbacks)
    {
        for (const auto& aaf : attempts_and_feedbacks)
            if (compare_attempt_with_secret_code(cur, aaf.attempt) != aaf.feedback)
                return false;
        return true;
    }

    void dfs_count(size_t i, const Options& options, Code& cur,
                   const std::vector<AttemptAndFeedback>& attempts_and_feedbacks, const Code& attempt,
                   std::map<Feedback, size_t>& counter)
    {
        if (i == options.number_of_characters_per_code)
        {
            if (attempts_and_feedbacks.empty() || test(cur, attempts_and_feedbacks))
                ++counter[compare_attempt_with_secret_code(cur, attempt)];
            return;
        }
        for (cur[i] = options.minimum_allowed_character; cur[i] <= options.maximum_allowed_character; ++cur[i])
            if (attempts_and_feedbacks.empty() || pretest(i + 1, cur, attempts_and_feedbacks))
                dfs_count(i + 1, options, cur, attempts_and_feedbacks, attempt, counter);
    }

    std::map<Feedback, size_t> count_all(const Options& options,
                                         const std::vector<AttemptAndFeedback>& attempts_and_feedbacks,
                                         const Code& attempt)
    {
        std::map<Feedback, size_t> counter;
        Code cur(options.number_of_characters_per_code, ' ');
        dfs_count(0, options, cur, attempts_and_feedbacks, attempt, counter);
        return counter;
    }

} // namespace bac
