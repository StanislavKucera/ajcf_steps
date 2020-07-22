
#include "database.hpp"
#include "database.impl.hpp"
#include "musical_factory.hpp"
#include "musical_things.hpp"
#include <catch2/catch.hpp>
#include <sstream>
#include <typeindex>

namespace musify { namespace database {

    using namespace std::literals::chrono_literals;
    using namespace date::literals;

    TEST_CASE("TEST musify::database::ask_new_database_lines WITH a few lines", "[database]")
    {
        // ARRANGE
        std::stringstream input_stream{
            "y\n"
            "Line 1\n"
            "Y\n"
            "Line 2\n"
            "y\n"
            "Line 3\n"
            "n\n"};
        std::stringstream output_stream;

        // ACT
        const auto result = ask_new_database_lines(output_stream, input_stream);

        // ASSERT
        REQUIRE(result == std::vector<std::string>{"Line 1", "Line 2", "Line 3"});
        REQUIRE((input_stream.good() || input_stream.eof()));
        REQUIRE(input_stream.peek() == std::stringstream::traits_type::eof());
        REQUIRE(output_stream.str() ==
                "Would you like to add another line to the database (Y/N) ? "
                "What is the new database line (please follow the database syntax) ? "
                "Would you like to add another line to the database (Y/N) ? "
                "What is the new database line (please follow the database syntax) ? "
                "Would you like to add another line to the database (Y/N) ? "
                "What is the new database line (please follow the database syntax) ? "
                "Would you like to add another line to the database (Y/N) ? ");
    }

    TEST_CASE("TEST musify::database::save_new_database_lines WITH a few lines", "[database]")
    {
        // ARRANGE
        namespace fs = std::filesystem;
        fs::path database_file_path{"test_database.txt"};
        std::error_code ec{};
        fs::remove(database_file_path, ec);
        std::vector<std::string> lines{"Line 1", "Line 2", "Line 3", "Line 4"};

        // ACT
        save_new_database_lines(database_file_path, lines);

        // ASSERT
        REQUIRE(fs::is_regular_file(database_file_path));
        REQUIRE(fs::file_size(database_file_path) > 0);
    }

    TEST_CASE("TEST musify::database::load_database WITH file containing a few entities of unknown type", "[database]")
    {
        // ARRANGE
        namespace fs = std::filesystem;
        fs::path database_file_path{"test_database.txt"};
        std::error_code ec{};
        fs::remove(database_file_path, ec);
        std::vector<std::string> lines{"Line 1", "Line 2", "Line 3", "Line 4"};
        save_new_database_lines(database_file_path, lines);
        Database& database = singleton::Singleton<Database>::get_instance();
        database.clear();
        MusicalFactoryWithCreators factory{};

        // ACT
        const auto result = load_database(database_file_path, database, factory);

        // ASSERT
        REQUIRE(result == LoadingResult::UnknownLineType);
    }

    TEST_CASE(
        "TEST musify::database::load_database WITH file containing a few entities, some of them of incomplete type",
        "[database]")
    {
        // ARRANGE
        namespace fs = std::filesystem;
        fs::path database_file_path{"test_database.txt"};
        std::error_code ec{};
        fs::remove(database_file_path, ec);
        std::vector<std::string> lines{"Artist=1", "Album=2", "Song=3", "Song="};
        save_new_database_lines(database_file_path, lines);
        Database& database = singleton::Singleton<Database>::get_instance();
        database.clear();
        MusicalFactoryWithCreators factory{};

        // ACT
        const auto result = load_database(database_file_path, database, factory);

        // ASSERT
        REQUIRE(result == LoadingResult::IncompleteLine);
    }

    TEST_CASE("TEST musify::database::load_database WITH file containing a few entities", "[database]")
    {
        // ARRANGE
        namespace fs = std::filesystem;
        fs::path database_file_path{"test_database.txt"};
        std::error_code ec{};
        fs::remove(database_file_path, ec);
        std::vector<std::string> lines{"Artist=Artist1,2001,4.5,Rock", "Album=Album1,Artist1,2020/03/09",
                                       "Song=Song1,Album1,Artist1,3:45"};
        save_new_database_lines(database_file_path, lines);
        Database& database = singleton::Singleton<Database>::get_instance();
        database.clear();
        MusicalFactoryWithCreators factory{};

        // ACT
        const auto result = load_database(database_file_path, database, factory);

        // ASSERT
        REQUIRE(result == LoadingResult::Ok);
    }

    TEST_CASE("TEST musify::database::display_music_entities with artists", "[database]")
    {
        // ARRANGE
        std::stringstream output_stream;
        std::vector<Artist> music_artists{{"a1", strong::Year{2000_y}, strong::Rating{1.5f}, strong::Genre::Pop},
                                          {"a2", strong::Year{2001_y}, strong::Rating{3.f}, strong::Genre::Rock},
                                          {"a3", strong::Year{2002_y}, strong::Rating{5.0f}, strong::Genre::Jazz}};

        // ACT
        display_music_entities(output_stream, music_artists);

        // ASSERT
        REQUIRE(output_stream.str() ==
                "-----------------\n"
                "Thing #1: {{a1, Artist}, 2000, 1.5, Pop}\n"
                "-----------------\n"
                "Thing #2: {{a2, Artist}, 2001, 3.0, Rock}\n"
                "-----------------\n"
                "Thing #3: {{a3, Artist}, 2002, 5.0, Jazz}\n"
                "-----------------\n"
                "--> 3 things\n"
                "-----------------\n");
    }

    TEST_CASE("TEST musify::database::display_music_entities with albums", "[database]")
    {
        // TODO
    }

    TEST_CASE("TEST musify::database::display_music_entities with songs", "[database]")
    {
        // TODO
    }

}} // namespace musify::database
