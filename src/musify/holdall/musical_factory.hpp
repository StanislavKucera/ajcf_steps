
#pragma once

#include "strong_types.hpp"
#include <string_view>
#include <functional>
#include <map>
#include <memory>
#include <string>

namespace musify::database {

    class MusicalThing;

    class MusicalFactory
    {
    public:
        virtual ~MusicalFactory() = default;
        virtual std::unique_ptr<MusicalThing> create_thing(std::string_view thing_type_label,
                                                           std::string thing_name) = 0;
    };

    class MusicalFactoryWithIfElse : public MusicalFactory
    {
    public:
        std::unique_ptr<MusicalThing> create_thing(std::string_view thing_type_label, std::string thing_name);
    };

    template <typename AbstractThingT, typename TypeLabelT, typename... ConstructionParams>
    class Factory
    {
        using Creator = std::function<std::unique_ptr<AbstractThingT>(ConstructionParams...)>;
        std::map<std::string_view, Creator> m_creators;

    public:
        std::unique_ptr<AbstractThingT> create_thing(TypeLabelT type_label, ConsutructionParams... params)
        {
            const auto iter_creator = m_creators.find(type_label);
            if (iter_creator == m_creators.end())
                return nullptr;
            const auto& creator = iter_creator->second;
            std::unique_ptr<AbstractThingT> thing = creator(params...);
            return thing;
        }

        void register_creator(TypeLabelT type_label, Creator creator)
        {
            m_creators[type_label] = creator;
        }
    };

    class MusicalFactoryWithCreators : public MusicalFactory
    {
        using Creator = std::function<std::unique_ptr<MusicalThing>(std::string thing_name)>;
        std::map<std::string_view, Creator> m_creators;

    public:
        std::unique_ptr<MusicalThing> create_thing(std::string_view thing_type_label, std::string thing_name);

        void register_creator(std::string_view thing_type_label, Creator thing_creator);
    };

    template <typename ConcreteClassT>
    struct ClassRegistrator
    {
        ClassRegistrator()
        {
            singleton::Singleton<MusicalFactoryWithCreators>::get_instance().register_creator(
                ConcreteClassT::type_label,
                [](std::string thing_name) { return std::make_unique<ConcreteClassT>(thing_name); });
        }
    };

} // namespace musify::database
