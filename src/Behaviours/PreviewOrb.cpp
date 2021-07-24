#include "Behaviours/PreviewOrb.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Renderer.hpp"
#include "UnityEngine/Material.hpp"

DEFINE_TYPE(MapLoader, PreviewOrb);

using namespace UnityEngine;

namespace MapLoader
{
    void PreviewOrb::Start()
    {
        instance = this;
        Renderer* renderer = get_gameObject()->GetComponent<Renderer*>();
        renderer->set_enabled(false);
    }

    void PreviewOrb::ChangeOrb(Color color, Texture2D* texture)
    {        
        if (!instance)
        {
            return;
        }
        Renderer* renderer = instance->get_gameObject()->GetComponent<Renderer*>();
        renderer->set_enabled(true);
        
        Array<Material*>* materials = renderer->get_materials();
        materials->values[0]->set_color(color);
        materials->values[1]->set_mainTexture(texture);
    }

    void PreviewOrb::ChangeOrb(MapInfo& info)
    {
        ChangeOrb(info.packageInfo->config.mapColor, info.LoadCubeMap());
    }
}