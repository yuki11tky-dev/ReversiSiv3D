#include "Feature/Common/Math.h"
#include "Feature/Game/UI/Fade/FadeDrawer.h"
#include "Feature/System/Fade.h"
#include <cassert>

namespace ReversiSiv3D {

    FadeDrawer::FadeDrawer()
        : UIBase{}
        , fadeScreen_{}
        , currentState_{}
    {

    }

    FadeDrawer::~FadeDrawer()
    {

    }

    void FadeDrawer::Update(double deltaTimeSec)
    {
        switch (currentState_)
        {
        case FadeState::Idle:
        case FadeState::Finish:
        {
            break;
        }
        case FadeState::StartFadeIn:
        {
            SetAlpha(MaxAlpha);
            ChangeCurrentState(FadeState::FadeIn);
            break;
        }
        case FadeState::FadeIn:
        {
            double alphaValue{ GetAlpha() };
            const double amountValuePerSec{ Math::Divide(MaxAlpha, Fade::ExecTime) };
            const double subAlphaValue{ Math::Multiply(amountValuePerSec, deltaTimeSec) };
            alphaValue = Math::Round(alphaValue - subAlphaValue);
            if (alphaValue <= MinAlpha)
            {
                alphaValue = MinAlpha;
                ChangeCurrentState(FadeState::FinishFadeIn);
            }
            SetAlpha(alphaValue);
            break;
        }
        case FadeState::StartFadeOut:
        {
            SetAlpha(MinAlpha);
            ChangeCurrentState(FadeState::FadeOut);
            break;
        }
        case FadeState::FadeOut:
        {
            double alphaValue{ GetAlpha() };
            const double amountValuePerSec{ Math::Divide(MaxAlpha, Fade::ExecTime) };
            const double subAlphaValue{ Math::Multiply(amountValuePerSec, deltaTimeSec) };
            alphaValue = Math::Round(alphaValue + subAlphaValue);
            if (alphaValue >= MaxAlpha)
            {
                alphaValue = MaxAlpha;
                ChangeCurrentState(FadeState::FinishFadeOut);
            }
            SetAlpha(alphaValue);
            break;
        }
        case FadeState::FinishFadeIn:
        case FadeState::FinishFadeOut:
        {
            ChangeCurrentState(FadeState::Finish);
            break;
        }
        default:
        {
            break;
        }
        }
    }

    void FadeDrawer::Initialize()
    {
        std::shared_ptr<Rectangle> fadeScreen = std::make_shared<Rectangle>();
        fadeScreen->Create(Scene::Rect());

        fadeScreen->SetColor(DefaultColor);

        // 所有権は基底クラスであるUIBaseと本クラスが保持する
        // 本クラスでの設定更新処理が頻繁に実施されるため、
        // 本クラスにも所有権を保持する
        fadeScreen_ = fadeScreen;
        AddDrawable(fadeScreen);
    }

    void FadeDrawer::SetCenterPos(Point centerPos)
    {
        assert(fadeScreen_);
        fadeScreen_->SetCenterPos(centerPos);
    }

    void FadeDrawer::SetAlpha(double alpha)
    {
        assert(fadeScreen_);
        fadeScreen_->SetAlpha(alpha);
    }

    double FadeDrawer::GetAlpha() const
    {
        assert(fadeScreen_);
        return fadeScreen_->GetAlpha();
    }

    void FadeDrawer::StartFadeIn()
    {
        currentState_ = FadeState::StartFadeIn;
    }

    void FadeDrawer::StartFadeOut()
    {
        currentState_ = FadeState::StartFadeOut;
    }

    bool FadeDrawer::IsFadeFinish() const
    {
        return currentState_ == FadeState::Finish;
    }

    void FadeDrawer::ChangeCurrentState(FadeState state)
    {
        currentState_ = state;
    }
}
