#pragma once

#include "Feature/Game/UI/Notification/NotificationUI.h"

namespace ReversiSiv3D {

    class StartPanel : public NotificationUI
    {
    public:

        StartPanel();
        ~StartPanel();

        void Initialize();
    };
}
