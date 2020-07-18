
#pragma once

#include "musical_thing.hpp"
#include "singleton.hpp"
#include <string_view>
#include <unordered_map>
#include <algorithm>
#include <filesystem>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#if defined(TRY_RISKY_FIX)
template <typename KeyT, typename DataT>
using BintreeOrHashtable = std::unordered_map<KeyT, DataT>;
#else
template <typename KeyT, typename DataT>
using BintreeOrHashtable = std::map<KeyT, DataT>;
#endif

namespace musify { namespace database {

    std::vector<std::string> ask_new_database_lines();

    void save_new_database_lines(const std::filesystem::path& database_file_path,
                                 const std::vector<std::string>& database_lines);

    enum class LoadingResult
    {
        Ok = 0,
        FileNotFound = 1,
        FileNotReadable = 2,
        UnknownLineType = 3,
        IncompleteLine = 4,
        ParsingError = 5,
        DuplicateArtist = 6,
        DuplicateAlbum,
        DuplicateSong,
    };

    enum class InsertionResult
    {
        Ok = 0,
        DuplicateThing = 6,
    };

    class Database
    {
        friend class singleton::Singleton<Database>;

        Database() = default;

    public:
        using ConstMusicalThingRef = std::reference_wrapper<MusicalThing const>;
        using MusicalThings = std::vector<ConstMusicalThingRef>;

        Database(const Database&) = delete;

        Database& operator=(const Database&) = delete;

        void display() const;

        const MusicalThing* find_thing(const std::string& name, const std::type_info& thing_type) const;

        template <typename ThingT>
        const ThingT* find_thing(const std::string& name) const
        {
            return static_cast<const ThingT*>(find_thing(name, typeid(ThingT)));
        }

        MusicalThings find_things(const std::string& name) const;

        template <typename ThingVisitorT>
        void visit_things(ThingVisitorT visitor) const
        {
            std::for_each(m_things.begin(), m_things.end(),
                          [&](const auto& name_and_thing) { visitor(*name_and_thing.second); });
        }

        void clear()
        {
            m_things.clear();
        }

        InsertionResult insert_thing(std::unique_ptr<MusicalThing> thing);

    private:
        std::multimap<std::string, std::unique_ptr<MusicalThing>> m_things{};
    };

    LoadingResult load_database(const std::filesystem::path& database_file_path, Database& database);

}} // namespace musify::database
