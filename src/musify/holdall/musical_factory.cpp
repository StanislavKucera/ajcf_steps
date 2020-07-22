
#include "musical_factory.hpp"
#include "musical_things.hpp"

namespace musify::database {

    std::unique_ptr<MusicalThing> MusicalFactoryWithIfElse::create_thing(std::string_view thing_type_label,
                                                                         std::string thing_name)
    {
        if (thing_type_label == Artist::type_label)
            return std::make_unique<Artist>(std::move(thing_name));
        if (thing_type_label == Album::type_label)
            return std::make_unique<Album>(std::move(thing_name));
        if (thing_type_label == Song::type_label)
            return std::make_unique<Song>(std::move(thing_name));
        return nullptr;
    }

    std::unique_ptr<MusicalThing> MusicalFactoryWithCreators::create_thing(std::string_view thing_type_label,
                                                                           std::string thing_name)
    {
        const auto iter_creator = m_creators.find(thing_type_label);
        if (iter_creator == m_creators.end())
            return nullptr;
        const auto& creator = iter_creator->second;
        std::unique_ptr<MusicalThing> thing = creator(thing_name);
        return thing;
    }

    void MusicalFactoryWithCreators::register_creator(std::string_view thing_type_label, Creator thing_creator)
    {
        m_creators[thing_type_label] = thing_creator;
    }

} // namespace musify::database
