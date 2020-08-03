
#include "options.hpp"
#include <string_view>
#include <bac/util/menu.hpp>
#include <cassert>
#include <iostream>

namespace bac {

    template <typename T>
    void set_options_value(std::istream& in, T& option)
    {
        option = ask<T>(in).value();
    }

    void display_menu_and_configure(std::ostream& out, std::istream& in, Options& options)
    {
        Menu menu{{'0', "Back to main menu", [&] { return QuitMenu::Yes; }},
                  {'1', "Modify Maximum number of attempts per game",
                   [&] {
                       out << "Enter the Maximum number of attempts per game: "; // flush?
                       set_options_value(in, options.max_number_of_attempts);
                       return QuitMenu::No;
                   }},
                  {'2', "Modify Number of characters in a code",
                   [&] {
                       out << "Enter the Number of characters in a code: "; // flush?
                       set_options_value(in, options.number_of_characters_per_code);
                       return QuitMenu::No;
                   }},
                  {'3', "Modify Minimum allowed character",
                   [&] {
                       out << "Enter the Minimum allowed character: "; // flush?
                       set_options_value(in, options.minimum_allowed_character);
                       return QuitMenu::No;
                   }},
                  {'4', "Modify Maximum allowed character",
                   [&] {
                       out << "Enter the Maximum allowed character: "; // flush?
                       set_options_value(in, options.maximum_allowed_character);
                       return QuitMenu::No;
                   }},
                  {'5', "Save options",
                   [&] {
                       save_options(out, options);
                       return QuitMenu::No;
                   }},
                  {'6', "Load options", [&] {
                       load_options(in, options);
                       return QuitMenu::No;
                   }}};

        auto display_title = [&](std::ostream& out) {
            out << "Here are the current options:\n"
                << "Maximum number of attempts per game: " << options.max_number_of_attempts << "\n"
                << "Number of characters in a code: " << options.number_of_characters_per_code << "\n"
                << "Range of allowed characters: from '" << options.minimum_allowed_character << "' to '"
                << options.maximum_allowed_character << "'\n"
                << "Please select a menu option to configure the game options";
        };

        menu_loop(display_title, out, in, menu);
    }

    void save_options(std::ostream& output_file_stream, const Options& options)
    {
        output_file_stream << options.max_number_of_attempts << '\n'
                           << options.number_of_characters_per_code << '\n'
                           << options.minimum_allowed_character << '\n'
                           << options.maximum_allowed_character << '\n';
    }

    void load_options(std::istream& input_file_stream, Options& options)
    {
        input_file_stream >> options.max_number_of_attempts >> options.number_of_characters_per_code >>
            options.minimum_allowed_character >> options.maximum_allowed_character;
    }

} // namespace bac
