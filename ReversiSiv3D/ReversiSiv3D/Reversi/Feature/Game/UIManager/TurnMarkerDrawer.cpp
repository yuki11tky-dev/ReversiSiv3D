#include "Feature/Game/UIManager/TurnMarkerDrawer.h"
#include <cassert>

namespace ReversiSiv3D {

    TurnMarkerDrawer::TurnMarkerDrawer()
        : Task{ TaskId::TurnMarker }
        , turnMarker_{}
        , drawPhase_{}
        , elapsedTime_{}
    {

    }

    TurnMarkerDrawer::~TurnMarkerDrawer()
    {

    }

    void TurnMarkerDrawer::Update(double deltaTimeSec)
    {
        switch (drawPhase_)
        {
        case DrawPhase::Hide:
        {
            break;
        }
        case DrawPhase::StartVisible:
        {
            assert(turnMarker_);
            turnMarker_->SetAlphaValue(AlphaMaxValue);
            elapsedTime_ = ResetTime;
            ChangeDrawPhase(DrawPhase::WaitUpdate);
            break;
        }
        case DrawPhase::WaitUpdate:
        {
            elapsedTime_ += deltaTimeSec;
            if (elapsedTime_ < WaitUpdateTime)
            {
                break;
            }
            elapsedTime_ = ResetTime;
            ChangeDrawPhase(DrawPhase::UpdateFadeOut);
            break;
        }
        case DrawPhase::UpdateFadeOut:
        {
            const double currentAlphaValue{ turnMarker_->GetAlphaValue() };
            double alphaValue{ currentAlphaValue - AddAlphaValuePerSec * deltaTimeSec };
            if (alphaValue <= AlphaAdjustMinValue)
            {
                alphaValue = AlphaAdjustMinValue;
                ChangeDrawPhase(DrawPhase::UpdateFadeIn);
            }
            turnMarker_->SetAlphaValue(alphaValue);
            break;
        }
        case DrawPhase::UpdateFadeIn:
        {
            const double currentAlphaValue{ turnMarker_->GetAlphaValue() };
            double alphaValue{ currentAlphaValue + AddAlphaValuePerSec * deltaTimeSec };
            if (alphaValue >= AlphaMaxValue)
            {
                elapsedTime_ = ResetTime;
                alphaValue = AlphaMaxValue;
                ChangeDrawPhase(DrawPhase::WaitUpdate);
            }
            turnMarker_->SetAlphaValue(alphaValue);
            break;
        }
        }
    }

    void TurnMarkerDrawer::Render()
    {
        assert(turnMarker_);
        turnMarker_->Render();
    }

    void TurnMarkerDrawer::CreateMarker()
    {
        turnMarker_ = std::make_unique<TurnMarker>();
        turnMarker_->Initialize();
        Hide();
    }

    void TurnMarkerDrawer::SetMarkerPos(Point pos)
    {
        assert(turnMarker_);
        turnMarker_->SetCenterPos(pos);
    }

    void TurnMarkerDrawer::Hide()
    {
        assert(turnMarker_);
        turnMarker_->SetAlphaValue(AlphaMinValue);
        ChangeDrawPhase(DrawPhase::Hide);
    }

    void TurnMarkerDrawer::Visible()
    {
        assert(turnMarker_);
        ChangeDrawPhase(DrawPhase::StartVisible);
    }

    void TurnMarkerDrawer::ChangeDrawPhase(DrawPhase drawPhase)
    {
        drawPhase_ = drawPhase;
    }
}
