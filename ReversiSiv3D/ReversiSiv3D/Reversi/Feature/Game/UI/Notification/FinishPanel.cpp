#include "Feature/Game/UI/Notification/FinishPanel.h"

namespace ReversiSiv3D {

    FinishPanel::FinishPanel()
        : NotificationUI(PanelType::Finish)
    {

    }

    FinishPanel::~FinishPanel()
    {

    }

    void FinishPanel::Initialize()
    {
        NotificationUI::Initialize(U"image/finish.png");
    }
}
