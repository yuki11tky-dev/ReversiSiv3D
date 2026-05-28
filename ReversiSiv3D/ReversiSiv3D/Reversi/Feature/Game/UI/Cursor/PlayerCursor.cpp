#include "Feature/Common/AssetManager.h"
#include "Feature/Common/Math.h"
#include "Feature/Game/UI/Cursor/PlayerCursor.h"

namespace ReversiSiv3D {

    PlayerCursor::PlayerCursor()
        : BoardCursor{}
        , elapsedTime_{}
    {

    }

    PlayerCursor::~PlayerCursor()
    {

    }

    void PlayerCursor::Initialize()
    {
        elapsedTime_ = ResetTime;
        Hide();
    }

    void PlayerCursor::Update(double deltaTime)
    {
        elapsedTime_ += deltaTime;
        elapsedTime_ = Math::Round(elapsedTime_);
        if (elapsedTime_ < SwitchImageTime)
        {
            return;
        }
        elapsedTime_ = ResetTime;

        UpdateDrawIndex();
    }
}
