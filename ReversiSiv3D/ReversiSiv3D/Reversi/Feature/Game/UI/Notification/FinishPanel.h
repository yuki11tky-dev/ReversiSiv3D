#pragma once

#include "Feature/Game/UI/Notification/NotificationUI.h"

namespace ReversiSiv3D {

    class FinishPanel : public NotificationUI
    {
    public:

        FinishPanel();
        ~FinishPanel();

        void Initialize();
    };
}
