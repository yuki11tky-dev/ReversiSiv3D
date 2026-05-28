#include "Feature/Game/UI/Notification/SkipTurnPanel.h"

namespace ReversiSiv3D {

    SkipTurnPanel::SkipTurnPanel()
        : NotificationUI(PanelType::SkipTurn)
    {

    }

    SkipTurnPanel::~SkipTurnPanel()
    {

    }

    void SkipTurnPanel::Initialize()
    {
        NotificationUI::Initialize(U"image/skipturn.png");
    }
}
