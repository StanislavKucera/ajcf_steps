
#pragma once

#include <iosfwd>
#include <string>
#include <vector>

namespace bac {

    struct Options;

    struct Code
    {
        Code() = default;
        Code(const Code&) = default;
        Code(Code&&) = default;
        Code& operator=(const Code&) = default;
        Code& operator=(Code&&) = default;
        ~Code() = default;
        Code(const std::string& str) : value(str)
        {
        }
        Code(std::string&& str) : value(std::move(str))
        {
        }
        Code(size_t len, char c) : value(len, c)
        {
        }
        auto begin()
        {
            return value.begin();
        }
        auto begin() const
        {
            return value.begin();
        }
        auto end()
        {
            return value.end();
        }
        auto end() const
        {
            return value.end();
        }
        std::string::reference operator[](size_t i)
        {
            return value[i];
        }
        auto operator[](size_t i) const
        {
            return value[i];
        }
        auto size() const
        {
            return value.size();
        }
        auto empty() const
        {
            return value.empty();
        }
        auto operator==(const Code& code) const
        {
            return value == code.value;
        }
        auto operator!=(const Code& code) const
        {
            return value != code.value;
        }
        std::string value{};
    };

    struct Feedback
    {
        auto operator==(const Feedback& fb) const
        {
            return bulls == fb.bulls && cows == fb.cows;
        }
        auto operator!=(const Feedback& fb) const
        {
            return bulls != fb.bulls || cows != fb.cows;
        }
        auto operator<(const Feedback& fb) const
        {
            return bulls < fb.bulls || (bulls == fb.bulls && cows < fb.cows);
        }
        // Number of good and well-placed characters
        unsigned int bulls{};
        // Number of good but not well-placed characters
        unsigned int cows{};
    };

    // Codebreaker's attempt and associated codemaker's feedback
    struct AttemptAndFeedback
    {
        Code attempt{};
        Feedback feedback{};
    };

    enum class DisplaySecretCode
    {
        No,
        Yes,
    };

    struct Board
    {
        Code secret_code{};
        std::vector<AttemptAndFeedback> attempts_and_feedbacks{};
        virtual void display(std::ostream&, const Options&, DisplaySecretCode) const;
    };

    struct ComputerBoard : public Board
    {
        void display(std::ostream&, const Options&, DisplaySecretCode) const final;
    };

    void display_board(std::ostream&, const Options&, const Board&, DisplaySecretCode);

    void display_board_line_by_line(std::ostream&, const Options&, const Board&, DisplaySecretCode);

} // namespace bac
