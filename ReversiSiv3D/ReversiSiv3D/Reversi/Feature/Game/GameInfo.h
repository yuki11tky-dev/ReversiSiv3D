#pragma once

#include "Feature/System/GameConfig.h"

namespace ReversiSiv3D {

    class GameInfo
    {
    public:

        static void SetGameConfig(const GameConfig&);
        static GameConfig& GetGameConfig();

    private:

        GameInfo();
        ~GameInfo();

    private:

        static GameConfig gameConfig_;
    };
}
