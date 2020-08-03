
#pragma once

#include "board.hpp"
#include "options.hpp"

namespace bac {

    struct Options;

    class Codemaker
    {
    public:
        virtual Feedback compare_attempt_with_secret_code(std::ostream& out, const Code& attempt);

        virtual Code choose_secret_code(std::ostream& out, const Options& options);

    private:
        Code m_secret_code{};
    };

    class EliminatingCodemaker : public Codemaker
    {
    public:
        Feedback compare_attempt_with_secret_code(std::ostream& out, const Code& attempt) final;

        Code choose_secret_code(std::ostream& out, const Options& options) final;

    private:
        Options m_options;
        std::vector<AttemptAndFeedback> attempts_and_feedbacks{};
    };

} // namespace bac
