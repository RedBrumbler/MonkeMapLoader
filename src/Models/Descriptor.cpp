#include "Models/Descriptor.hpp"

/// finds a json property, or if it doesnt exist just doesnt assign a new value
#define GetJSONProperty(name, getter) \
    auto __##name ##_itr = val.FindMember(#name);\
    if (__##name ##_itr != val.MemberEnd()) {\
        name = __##name ##_itr->value.getter;\
    }
    

namespace MapLoader
{
    Descriptor::Descriptor(rapidjson::Value& val)
    {
        author = val["author"].GetString();
        mapName = val["objectName"].GetString();
        description = val["description"].GetString();
        GetJSONProperty(androidRequiredVersion, GetString());
    }
}