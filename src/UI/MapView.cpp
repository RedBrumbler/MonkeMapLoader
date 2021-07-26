#include "UI/MapView.hpp"
#include "monkecomputer/shared/ViewLib/CustomComputer.hpp"
#include "Behaviours/MapLoader.hpp"
#include "UI/MapSelectorViewManager.hpp"
#include "cpp-semver/include/cpp-semver.hpp"
#include "static-defines.hpp"

DEFINE_TYPE(MapLoader, MapView);

extern Logger& getLogger();

using namespace GorillaUI;

namespace MapLoader
{
    void MapView::Awake()
    {
        isUpdated = false;
        loaded = false;
        mapInfo = nullptr;

        missingModIDs.clear();
    }

    void MapView::DidActivate(bool firstActivation)
    {
        loaded = false;

        // check wether the map plugin version is higher than the map info 
        static std::string pluginVersionStatic = pluginVersion;
        isUpdated = mapInfo ? semver::satisfies(pluginVersionStatic, "^" + ((MapInfo*)mapInfo)->packageInfo->descriptor.androidRequiredVersion) : false;
        
        // get any missing mod IDs
        missingModIDs = mapInfo ? ((MapInfo*)mapInfo)->packageInfo->config.GetMissingModIDs() : std::vector<std::string>{};
        Redraw();
    }

    void MapView::Load()
    {
        if (!mapInfo)
        {
            return;
        }
        
        Loader* loader = Object::FindObjectOfType<Loader*>();
        if (loader) loader->LoadMap(*(MapInfo*)mapInfo);
    }

    void MapView::Redraw()
    {
        text = "";

        if (!loaded)
        {
            DrawHeader();
            DrawMap();
        }
        else
        {
            /*
            text += "                                                          <size=25>\n";
            text += "                                                                <color=#2222ff>##########\n";
            text += "                                                          ######<color=#00dd00>####</color>############\n";
            text += "                                                      ########<color=#00dd00>######</color>################\n";
            text += "                                                    <color=#00dd00>####</color>########<color=#00dd00>####</color>################<color=#00dd00>##</color>\n";
            text += "                                                  <color=#00dd00>########</color>####<color=#00dd00>##</color>##<color=#00dd00>##</color>##############<color=#00dd00>######</color>\n";
            text += "                                                  <color=#00dd00>##########</color>##################<color=#00dd00>##########</color>\n";
            text += "                                                <color=#00dd00>####################</color>########<color=#00dd00>##############</color>\n";
            text += "                                                <color=#00dd00>############</color>##<color=#00dd00>######</color>########<color=#00dd00>##############</color>\n";
            text += "                                                <color=#00dd00>##########</color>##<color=#00dd00>########</color>######<color=#00dd00>##########</color>####<color=#00dd00>##</color>\n";
            text += "                                              <color=#00dd00>####################</color>########<color=#00dd00>########</color>########<color=#00dd00>##</color>\n";
            text += "                                              <color=#00dd00>####################</color>##########<color=#00dd00>####</color>####<color=#00dd00>########</color>\n";
            text += "                                              <color=#00dd00>##################</color>################<color=#00dd00>############</color>\n";
            text += "                                              <color=#00dd00>##################</color>##############<color=#00dd00>##############</color>\n";
            text += "                                              ##########<color=#00dd00>######</color>################<color=#00dd00>##############</color>\n";
            text += "                                                ############<color=#00dd00>##</color>##############<color=#00dd00>##############</color>\n";
            text += "                                                ######<color=#00dd00>##</color>####<color=#00dd00>##</color>##############<color=#00dd00>##############</color>\n";
            text += "                                                ####<color=#00dd00>##</color>######################<color=#00dd00>##############</color>\n";
            text += "                                                  ##########################<color=#00dd00>############</color>\n";
            text += "                                                  ############################<color=#00dd00>##########</color>\n";
            text += "                                                    ####<color=#00dd00>##</color>####################<color=#00dd00>########</color>\n";
            text += "                                                      <color=#00dd00>####</color>####################<color=#00dd00>######</color>\n";
            text += "                                                          ##<color=#00dd00>####</color>################\n";
            text += "                                                                ##########</color>\n";
            text += "                                              </size>\n";
            text += "                 MAP LOADED\n";
            */
            text += "\n";
            text += "<size=12><color=#006ca2>\n";
            text += "                                                                                                                              ####################\n";
            text += "                                                                                                                        ################################\n";
            text += "                                                                                                                   ###########################################\n";
            text += "                                                                                                               ##################################################\n";
            text += "                                                                                                            ###########    ######### ###### #########    ############\n";
            text += "                                                                                                          ##########      #######    ######    #######      ##########\n";
            text += "                                                                                                       #########        #######      ######      #######         ########\n";
            text += "                                                                                                     #########         #######       ######       #######         #########\n";
            text += "                                                                                                   ########           ######       ##########      #######           ########\n";
            text += "                                                                                                 #######            ########################################            ########\n";
            text += "                                                                                                ########           ##########################################            #######\n";
            text += "                                                                                              #############       #######                              #######       #############\n";
            text += "                                                                                              ###############  ########                                   #######  ################\n";
            text += "                                                                                            ######    #############                                          #############    ######\n";
            text += "                                                                                           ######        #########                                            #########        ######\n";
            text += "                                                                                          ######           ####                                                  ####           ######\n";
            text += "                                                                                          ######          #####                                                  #####          #######\n";
            text += "                                                                                         ##############  #####                                                    #####  ##############\n";
            text += "                                                                                        #####################                                                      #####################\n";
            text += "                                                                                       ######        #######                                                         ######        ######\n";
            text += "                                                                                       ####                                                                                          ####\n";
            text += "                                                                                      #####                                                                                          #####\n";
            text += "                                                                                      #####                                                                                          #####\n";
            text += "                                                                                      ######                                                                                        ######\n";
            text += "                                                                                      ########                                                                                     #######\n";
            text += "                                                                                      ###########                                                                               ##########\n";
            text += "                                                                                      ###############                                                                       ##############\n";
            text += "                                                                                      ######   ########################                                   #######################   ######\n";
            text += "                                                                                      ######       ######################                              ######################       ######\n";
            text += "                                                                                       ######                ######  ########                      ########   #####                ######\n";
            text += "                                                                                       ######                ######    #######                    #######     #####                ######\n";
            text += "                                                                                        ######                #####       ######                ######       #####                 #####\n";
            text += "                                                                                        ######                #####         #####               ####         #####                ######\n";
            text += "                                                                                         ######                ##################               #################                ######\n";
            text += "                                                                                          ######              ##################                ##################              ######\n";
            text += "                                                                                          #######          #####################                #####################           ######\n";
            text += "                                                                                            ######     ################    ####                  ####     ###############     ######\n";
            text += "                                                                                             ####### ##########  ######   #####                  #####   ######  ##################\n";
            text += "                                                                                              #############       ######  ####                    ####  ######       #############\n";
            text += "                                                                                               #########           ###### ####                    #### #######          #########\n";
            text += "                                                                                                 #######            ##########                    ##########            ########\n";
            text += "                                                                                                  ########           #########                    #########           ########\n";
            text += "                                                                                                     #######           ###################################          ########\n";
            text += "                                                                                                       ########         ################################         ########\n";
            text += "                                                                                                        ###########       #######    #######   ########       ##########\n";
            text += "                                                                                                            ###########    ######### ###### #########    ############\n";
            text += "                                                                                                              ####################################################\n";
            text += "                                                                                                                   ###########################################\n";
            text += "                                                                                                                      ####################################\n";
            text += "                                                                                                                              ####################\n";
            text += "</color></size>\n";
            text += "                 <color=#fdfdfd>MAP LOADED\n";
            text += "       GET MORE MAPS AT</color> <color=#5555ff>MONKEMAPHUB.COM</color>\n";

        }

        CustomComputer::Redraw();
    }
    
    void MapView::DrawHeader()
    {
        text += string_format("<color=#ffff00>== <color=#fdfdfd>Map Details</color> ==</color>\n");
    }
    
    void MapView::DrawMap()
    {
        if (!mapInfo) return;

        text += "<color=#fdfdfd>";
        text += "<size=40>";
        if (!isUpdated) // if map is of newver version than the maploader
        {
            text += "\n  Your map loader is outdated,";
            text += "\n  please update it!";
            text += "\n  This map will not be allowed to be loaded";
            text += string_format("\n  Required: %s", ((MapInfo*)mapInfo)->packageInfo->descriptor.androidRequiredVersion.c_str());
            text += string_format("\n  You Have: %s", pluginVersion);
        }

        int count = missingModIDs.size();
        if (count > 0)
        {
            text += string_format("\n  You are missing %d mod%s.", count, count > 1 ? "s" : "");
            text += count > 1 ? "\n  These are their IDs:" : "\n  This is the ID: ";
            
            for (auto& s : missingModIDs)
            {
                text += string_format("\n  %s", s.c_str());
            }

            text += "\n\n  Please install these mods in order to play on this map,";
            text += "\n  This map will not be allowed to be loaded";
        }   
        
        text += "\n</size>";
        text += "</color>";

        text += "\n<size=60>";
        text += string_format("   Author: <color=#fdfdfd>%s</color>\n", ((MapInfo*)mapInfo)->packageInfo->descriptor.author.c_str());
        text += string_format("   Map Name: <color=#fdfdfd>%s</color>\n", ((MapInfo*)mapInfo)->packageInfo->descriptor.mapName.c_str());
        text += string_format("   Description: <color=#fdfdfd>%s</color>\n", ((MapInfo*)mapInfo)->packageInfo->descriptor.description.c_str());
        text += "\n</size>";
    }
    
    void MapView::OnKeyPressed(int key)
    {
        if (loaded || !isUpdated || missingModIDs.size() > 0) return;
        if (key == (int)EKeyboardKey::Enter)
        {
            loaded = true;
            Load();
        }

        Redraw();
    }
}