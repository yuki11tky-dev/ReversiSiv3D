#include "Feature/Game/UI/Notification/StartPanel.h"

namespace ReversiSiv3D {

    StartPanel::StartPanel()
        : NotificationUI(PanelType::Start)
    {

    }

    StartPanel::~StartPanel()
    {

    }

    void StartPanel::Initialize()
    {
        NotificationUI::Initialize(U"image/start.png");
    }
}
