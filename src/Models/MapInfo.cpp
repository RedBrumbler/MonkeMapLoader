#include "Models/MapInfo.hpp"

#include "zip/shared/zip.h"
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "Utils/FileUtils.hpp"

using namespace UnityEngine;

std::string toLower(std::string aString)
{
    std::string result = "";

    for (auto c : aString)
    {
        result += tolower(c);
    }

    return result;
}

extern Logger& getLogger();
namespace MapLoader
{
    void MapInfo::Load()
    {
        if (!fileexists(filePath)) return;
        int err = 0;
        zip *z = zip_open(filePath.c_str(), 0, &err);
        
        struct zip_stat st;
        zip_stat_init(&st);
        zip_stat(z, "package.json", 0, &st);
        zip_file* f = zip_fopen(z, st.name, 0);
        char* package = new char[st.size];
        zip_fread(f, package, st.size);
        zip_fclose(f);
        
        std::string json = "";
        for (int i = 0; i < st.size; i++)
        {
            json += package[i];
        }
        
        rapidjson::Document doc;
        doc.Parse(json.c_str());
        packageInfo = new MapPackageInfo(doc);
        delete[](package);
        zip_close(z);
    }

    Texture2D* MapInfo::LoadCubeMap()
    {
        if (packageInfo->previewCubeMap) return (Texture2D*)packageInfo->previewCubeMap;
        using LoadImage = function_ptr_t<unsigned int, Texture2D*, Array<uint8_t>*, bool>;
        static LoadImage loadImage = reinterpret_cast<LoadImage>(il2cpp_functions::resolve_icall("UnityEngine.ImageConversion::LoadImage"));

        int err = 0;
        zip *z = zip_open(filePath.c_str(), 0, &err);
        
        struct zip_stat st;
        zip_stat_init(&st);
        zip_stat(z, packageInfo->config.cubeMapImagePath.c_str(), 0, &st);

        zip_file* f = zip_fopen(z, st.name, 0);
        
        uint8_t* cubeMap = new uint8_t[st.size];
        static_assert(sizeof(char) == sizeof(uint8_t));
        zip_fread(f, (char*)cubeMap, st.size);
        zip_fclose(f);
        zip_close(z);

        std::vector<uint8_t> byteVector(cubeMap, cubeMap + st.size);
        Array<uint8_t>* byteArray = il2cpp_utils::vectorToArray(byteVector);

        Texture2D* texture = Texture2D::New_ctor(0, 0, 4, false);
        loadImage(texture, byteArray, false);
        delete[](cubeMap);
        packageInfo->previewCubeMap = texture;
        return texture;
    }

    bool MapInfo::operator<(const MapInfo& second) const
    {
        std::string firstName = toLower(this->packageInfo->descriptor.mapName);
        std::string secondName = toLower(second.packageInfo->descriptor.mapName);
        return firstName < secondName;
    }

    std::string MapInfo::get_mapString()
    {
        if (packageInfo->config.guid != "")
        {
            return string_format("%s_%d", packageInfo->config.guid.c_str(), packageInfo->config.version);
        }
        else 
        {
            return string_format("%s_%s", packageInfo->descriptor.author.c_str(), packageInfo->descriptor.mapName.c_str());
        }
    }
}