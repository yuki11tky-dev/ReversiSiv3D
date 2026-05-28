#pragma once

#include "Feature/System/GameConfig.h"

namespace ReversiSiv3D {

    class GameConfigLoader
    {
    public:

        GameConfigLoader();
        ~GameConfigLoader();

        static bool LoadGameConfig();

    private:

        static bool CheckLoadData(JSON&);
        static bool CheckConfigData(GameConfig&);
        static bool CheckWindowSettings(GameWindowSettings);
        static bool CheckSoundVolume(double);
        static bool CheckStartupLevel(LevelType);

        static JSON SaveConfig(const GameConfig&);

    private:

        inline static const String ConfigJsonPath{ U"config/config.json" };
    };
}
