
#pragma once

#include <holdall/database.hpp>
#include <holdall/menu.hpp>

namespace mdb = musify::database;
namespace mm = musify::menu;
namespace fs = std::filesystem;

inline bool option_quit()
{
    std::cout << "Bye!\n";
    return false;
}

inline bool option_load_database(const fs::path& database_file_path, mdb::Database& database)
{
    std::cout << "Loading database file " << database_file_path << "...\n";
    const auto result = mdb::load_database(database_file_path, database);
    switch (result)
    {
    case mdb::LoadingResult::FileNotFound:
        std::cerr << "The database file does not exist\n";
        break;
    case mdb::LoadingResult::FileNotReadable:
        std::cerr << "The database file is not readable\n";
        break;
    case mdb::LoadingResult::UnknownLineType:
        std::cerr << "The database file contains a line of unknow type\n";
        break;
    case mdb::LoadingResult::IncompleteLine:
        std::cerr << "The database file contains an incomplete line\n";
        break;
    case mdb::LoadingResult::ParsingError:
        std::cerr << "The database file contains some non-parsable data\n";
        break;
    case mdb::LoadingResult::DuplicateArtist:
        std::cerr << "The database file contains a duplicate artist\n";
        break;
    case mdb::LoadingResult::UnknownArtist:
        std::cerr << "The database file refers to an unknown artist\n";
        break;
    case mdb::LoadingResult::DuplicateAlbum:
        std::cerr << "The database file contains a duplicate album\n";
        break;
    case mdb::LoadingResult::UnknownAlbum:
        std::cerr << "The database file refers to an unknown album\n";
        break;
    case mdb::LoadingResult::DuplicateSong:
        std::cerr << "The database file contains a duplicate song\n";
        break;
    case mdb::LoadingResult::Ok:
        std::cout << "The database file has been loaded\n";
        break;
    }
    return true;
}

inline bool option_display_database(const mdb::Database& database)
{
    std::cout << "Database contents:\n";
    database.display();
    return true;
}

inline bool option_find_artist(const mdb::Database& database, const std::string& text)
{
    std::cout << "Searching for artist '" << text << "'... ";
    if (const mdb::Artist* artist = database.find_artist(text); artist)
        std::cout << "Found: " << *artist << "\n";
    else
        std::cout << "Unknown artist\n";
    return true;
}

inline bool option_find_album(const mdb::Database& database, const std::string& text)
{
    std::cout << "Searching for Album '" << text << "'... ";
    if (const auto* album = database.find_album(text); album)
        std::cout << "Found: " << *album << "\n";
    else
        std::cout << "Unknown album\n";
    return true;
}

inline bool option_find_song(const mdb::Database& database, const std::string& text)
{
    std::cout << "Searching for Song '" << text << "'... ";
    if (const auto* song = database.find_song(text); song)
        std::cout << "Found: " << *song << "\n";
    else
        std::cout << "Unknown song\n";
    return true;
}

inline bool option_find_anything_which_name_equals(const mdb::Database& database, const std::string& text)
{
    std::cout << "Searching for anything which name equals '" << text << "'... ";
    if (const auto things = database.find_things(text); !things.empty())
    {
        std::cout << "Found: " << things.size() << " things\n";
        for (const auto& thing_refwrap : things)
        {
            const auto& thing = thing_refwrap.get();
            std::cout << "Found: " << thing;
            if (const auto artist = dynamic_cast<const mdb::Artist*>(&thing); artist)
            {
                std::cout << " (albums: ";
                for (const auto& artist_album : artist->albums())
                {
                    std::cout << artist_album->name() << ", ";
                }
                std::cout << ')';
            }
            std::cout << '\n';
        }
    }
    else
        std::cout << "Nothing found\n";
    return true;
}

inline bool option_find_anything_which_name_contains(const mdb::Database& database, const std::string& text)
{
    std::cout << "Searching for anything which name contains '" << text << "'... ";
    std::vector<std::reference_wrapper<const mdb::MusicalThing>> things_containing_text;
    database.visit_things([&](const mdb::MusicalThing& thing) {
        if (thing.name().find(text) != std::string::npos)
            things_containing_text.push_back(thing);
    });
    if (!things_containing_text.empty())
    {
        std::cout << "Found: " << things_containing_text.size() << " things\n";
        for (const auto& thing_refwrap : things_containing_text)
        {
            const auto& thing = thing_refwrap.get();
            std::cout << "Found: " << thing << '\n';
        }
    }
    else
        std::cout << "Nothing found\n";
    return true;
}