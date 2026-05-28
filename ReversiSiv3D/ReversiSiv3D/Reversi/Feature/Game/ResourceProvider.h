#pragma once

#include "Feature/Game/SharedEnum.h"

namespace ReversiSiv3D {

    class ResourceProvider
    {
    public:

        static String GetStoneIconFilePath(StoneType);
        static String GetPlayerNameText(PlayerType);
    };
}
