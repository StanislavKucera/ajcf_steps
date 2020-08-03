
#include "game.hpp"
#include "codebreaker.hpp"
#include "codemaker.hpp"
#include "logic.hpp"
#include "options.hpp"
#include <bac/util/menu.hpp>

namespace bac {

    void configure_options(std::ostream& out, [[maybe_unused]] std::istream& in, [[maybe_unused]] Options& options)
    {
        display_menu_and_configure(out, in, options);
    }

    void human_plays_against_computer(std::ostream& out, [[maybe_unused]] std::istream& in,
                                      [[maybe_unused]] const Options& options)
    {
        State state{std::make_unique<HumanCodebreaker>(), std::make_unique<Codemaker>(), std::make_unique<Board>()};
        play(out, in, options, state);
    }

    void computer_plays_against_computer(std::ostream& out, [[maybe_unused]] std::istream& in,
                                         [[maybe_unused]] const Options& options)
    {
        Menu menu{{'0', "Back to main menu", [&] { return QuitMenu::Yes; }},
                  {'1', "Random vs Random",
                   [&] {
                       State state{std::make_unique<RandomComputerCodebreaker>(), std::make_unique<Codemaker>(),
                                   std::make_unique<ComputerBoard>()};
                       play(out, in, options, state);
                       return QuitMenu::No;
                   }},
                  {'2', "Random vs Clever",
                   [&] {
                       State state{std::make_unique<RandomComputerCodebreaker>(),
                                   std::make_unique<EliminatingCodemaker>(), std::make_unique<ComputerBoard>()};
                       play(out, in, options, state);
                       return QuitMenu::No;
                   }},
                  {'3', "Greedy vs Random",
                   [&] {
                       State state{std::make_unique<GreedyComputerCodebreaker>(), std::make_unique<Codemaker>(),
                                   std::make_unique<ComputerBoard>()};
                       play(out, in, options, state);
                       return QuitMenu::No;
                   }},
                  {'4', "Greedy vs Clever",
                   [&] {
                       State state{std::make_unique<GreedyComputerCodebreaker>(),
                                   std::make_unique<EliminatingCodemaker>(), std::make_unique<ComputerBoard>()};
                       play(out, in, options, state);
                       return QuitMenu::No;
                   }},
                  {'5', "RandomAndGreedy vs Random",
                   [&] {
                       State state{std::make_unique<RandomAndGreedyComputerCodebreaker>(),
                                   std::make_unique<Codemaker>(), std::make_unique<ComputerBoard>()};
                       play(out, in, options, state);
                       return QuitMenu::No;
                   }},
                  {'6', "RandomAndGreedy vs Clever", [&] {
                       State state{std::make_unique<RandomAndGreedyComputerCodebreaker>(),
                                   std::make_unique<EliminatingCodemaker>(), std::make_unique<ComputerBoard>()};
                       play(out, in, options, state);
                       return QuitMenu::No;
                   }}};

        auto display_title = [](std::ostream& out) {
            out << "Pick which computers play the game:\n"
                << "  Codebreaker:\n"
                << "    Random: (may be slow) Picks random code consistent with previous attempts.\n"
                << "    Greedy: Picks alphabetically smallest code consistent with previous attempts.\n"
                << "    RandomAndGreedy: Picks randomly the first code and then same as Greedy.\n"
                << "  Codemaker:\n"
                << "    Random: Picks a random code before the game starts.\n"
                << "    Clever: (may be slow) Dynamically adjusts the feedback to maximize number of consistent codes.";
        };

        menu_loop(display_title, out, in, menu);
        State state{std::make_unique<RandomComputerCodebreaker>(), std::make_unique<Codemaker>(),
                    std::make_unique<ComputerBoard>()};
        play(out, in, options, state);
    }

    void greedy_computer_plays_against_computer(std::ostream& out, [[maybe_unused]] std::istream& in,
                                                [[maybe_unused]] const Options& options)
    {
        State state{std::make_unique<GreedyComputerCodebreaker>(), std::make_unique<Codemaker>(),
                    std::make_unique<ComputerBoard>()};
        play(out, in, options, state);
    }

    void random_and_greedy_computer_plays_against_computer(std::ostream& out, [[maybe_unused]] std::istream& in,
                                                           [[maybe_unused]] const Options& options)
    {
        State state{std::make_unique<RandomAndGreedyComputerCodebreaker>(), std::make_unique<Codemaker>(),
                    std::make_unique<ComputerBoard>()};
        play(out, in, options, state);
    }

    void display_menu_and_play(std::ostream& out, std::istream& in)
    {
        Options options{};

        Menu menu{{'0', "Quit",
                   [&] {
                       out << "\nBye bye!\n";
                       return QuitMenu::Yes;
                   }},
                  {'1', "Configure Options",
                   [&] {
                       configure_options(out, in, options);
                       return QuitMenu::No;
                   }},
                  {'2', "Human vs Computer",
                   [&] {
                       human_plays_against_computer(out, in, options);
                       return QuitMenu::No;
                   }},
                  {'3', "Computer vs Computer", [&] {
                       computer_plays_against_computer(out, in, options);
                       return QuitMenu::No;
                   }}};

        auto display_title = [](std::ostream& out) { out << "Welcome to Bulls And Cows\nPlease select a menu option"; };

        menu_loop(display_title, out, in, menu);
    }

} // namespace bac
