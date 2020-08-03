
#include "codemaker.hpp"
#include "logic.hpp"
#include "solver.hpp"
#include <cassert>

namespace bac {

    Feedback Codemaker::compare_attempt_with_secret_code(std::ostream& out, const Code& attempt)
    {
        return bac::compare_attempt_with_secret_code(attempt, m_secret_code);
    }

    Code Codemaker::choose_secret_code(std::ostream& out, const Options& options)
    {
        m_secret_code = generate_random_secret_code(options);
        return m_secret_code;
    }

    Feedback EliminatingCodemaker::compare_attempt_with_secret_code(std::ostream& out, const Code& attempt)
    {
        const auto counter = count_all(m_options, attempts_and_feedbacks, attempt);
        assert(!counter.empty());
        size_t best = 0;
        Feedback best_fb;
        for (const auto& [k, v] : counter)
            if (v > best)
            {
                best = v;
                best_fb = k;
            }
        attempts_and_feedbacks.push_back({attempt, best_fb});
        return best_fb;
    }

    Code EliminatingCodemaker::choose_secret_code(std::ostream& out, const Options& options)
    {
        m_options = options;
        return Code(options.number_of_characters_per_code, '?');
    }

} // namespace bac
