#include "Models/MapPackageInfo.hpp"

/// finds a json property, or if it doesnt exist just doesnt assign a new value
#define GetJSONProperty(name, getter) \
    auto __##name ##_itr = doc.FindMember(#name);\
    if (__##name ##_itr != doc.MemberEnd()) {\
        name = __##name ##_itr->value.getter;\
    }

namespace MapLoader
{
    MapPackageInfo::MapPackageInfo(rapidjson::Document& doc) : descriptor(Descriptor(doc["descriptor"])) , config(MapConfig(doc["config"]))
    {
        androidFileName = "";
        previewCubeMap = nullptr;

        GetJSONProperty(androidFileName, GetString());
    }
}