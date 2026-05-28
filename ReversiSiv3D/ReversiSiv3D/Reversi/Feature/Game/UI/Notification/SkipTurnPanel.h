#pragma once

#include "Feature/Game/UI/Notification/NotificationUI.h"

namespace ReversiSiv3D {

    class SkipTurnPanel : public NotificationUI
    {
    public:

        SkipTurnPanel();
        ~SkipTurnPanel();

        void Initialize();
    };
}
