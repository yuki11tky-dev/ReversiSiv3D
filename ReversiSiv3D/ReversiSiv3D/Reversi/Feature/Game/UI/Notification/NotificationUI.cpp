#include "Feature/Common/AssetManager.h"
#include "Feature/Common/Math.h"
#include "Feature/Game/UI/Notification/NotificationUI.h"

namespace ReversiSiv3D {

    NotificationUI::NotificationUI(PanelType panelType)
        : UIBase()
        , moveOffset_{}
        , moveTime_{}
        , fadeTime_{}
        , displayTime_{}
        , panelType_{ panelType }
        , currentPhase_{ AnimationPhase::Finished }
        , elapsedTime_{}
        , targetPos_{}
        , panelImage_{}
    {

    }

    NotificationUI::~NotificationUI()
    {

    }

    PanelType NotificationUI::GetPanelType() const
    {
        return panelType_;
    }

    void NotificationUI::Update(double deltaTimeSec)
    {
        switch (currentPhase_)
        {
        case AnimationPhase::Start:
        {
            Visible();
            SetupFadeIn();
            elapsedTime_ = ResetTime;
            ChangeAnimationPhase(AnimationPhase::FadeIn);
            break;
        }
        case AnimationPhase::FadeIn:
        {
            const bool isMoveFinish{ Move(deltaTimeSec) };
            const bool isFadeFinish{ FadeIn(deltaTimeSec) };
            if (!isMoveFinish || !isFadeFinish)
            {
                break;
            }

            ChangeAnimationPhase(AnimationPhase::WaitFadeOut);
            break;
        }
        case AnimationPhase::WaitFadeOut:
        {
            elapsedTime_ += deltaTimeSec;
            if (elapsedTime_ < displayTime_)
            {
                break;
            }
            elapsedTime_ = ResetTime;

            SetupFadeOut();
            ChangeAnimationPhase(AnimationPhase::FadeOut);
            break;
        }
        case AnimationPhase::FadeOut:
        {
            const bool isMoveFinish{ Move(deltaTimeSec) };
            const bool isFadeFinish{ FadeOut(deltaTimeSec) };
            if (!isMoveFinish || !isFadeFinish)
            {
                break;
            }

            ChangeAnimationPhase(AnimationPhase::Finish);
            break;
        }
        case AnimationPhase::Finish:
        {
            Hide();
            ChangeAnimationPhase(AnimationPhase::Finished);
            break;
        }
        case AnimationPhase::Finished:
        {
            break;
        }
        }
    }

    void NotificationUI::Initialize(const String& panelFilePath)
    {
        std::shared_ptr<Image> panelImage = std::make_shared<Image>();
        panelImage->LoadTexture(panelFilePath);
        const double scale{ AssetManager::GetCurrentWindowScale() };
        panelImage->SetScale(scale);
        panelImage->SetColor(DefaultColor);

        // 所有権は基底クラスであるUIBaseが持つ
        // 本クラスでは弱参照として保持する
        panelImage_ = panelImage;
        AddDrawable(std::move(panelImage));

        const double adjustOffsetX{ AssetManager::AdjustCurrentWindowScale(MoveOffsetXFromCenter) };
        const double adjustOffsetY{ AssetManager::AdjustCurrentWindowScale(MoveOffsetYFromCenter) };
        moveOffset_ = Vec2{ adjustOffsetX, adjustOffsetY };
        moveTime_ = MoveTime;
        fadeTime_ = FadeTime;
        displayTime_ = DisplayTime;

        Hide();
    }

    void NotificationUI::StartAnimation()
    {
        ChangeAnimationPhase(AnimationPhase::Start);
    }

    bool NotificationUI::IsFinishedAnimation() const
    {
        return currentPhase_ == AnimationPhase::Finished;
    }

    void NotificationUI::SetupFadeIn()
    {
        std::shared_ptr<Image> panelImage = panelImage_.lock();
        if (!panelImage)
        {
            return;
        }

        const Vec2 centerPos{ Scene::CenterF() };
        const Vec2 imageStartPos{ centerPos - moveOffset_ };
        targetPos_ = centerPos;

        panelImage->SetCenterPos(imageStartPos);
        panelImage->SetAlpha(AlphaMinValue);
    }

    void NotificationUI::SetupFadeOut()
    {
        targetPos_ = Scene::CenterF() + moveOffset_;
    }

    bool NotificationUI::Move(double deltaTimeSec)
    {
        std::shared_ptr<Image> panelImage = panelImage_.lock();
        if (!panelImage)
        {
            return true;
        }

        bool isArrivedX{ false };
        bool isArrivedY{ false };
        bool isFinished{ false };
        Vec2 imagePos{ panelImage->GetCenterPos() };
        const double adjustMoveXOffset{ moveOffset_.x * (deltaTimeSec / moveTime_) };
        const double adjustMoveYOffset{ moveOffset_.x * (deltaTimeSec / moveTime_) };
        imagePos.x = CalcMove(imagePos.x, targetPos_.x, adjustMoveXOffset);
        imagePos.y = CalcMove(imagePos.y, targetPos_.y, adjustMoveYOffset);
        if (Math::AlmostEqual(imagePos.x, targetPos_.x))
        {
            isArrivedX = true;
        }

        if (Math::AlmostEqual(imagePos.y, targetPos_.y))
        {
            isArrivedY = true;
        }

        if (isArrivedX && isArrivedY)
        {
            isFinished = true;
        }

        panelImage->SetCenterPos(imagePos);
        return isFinished;
    }

    bool NotificationUI::FadeIn(double deltaTimeSec)
    {
        std::shared_ptr<Image> panelImage = panelImage_.lock();
        if (!panelImage)
        {
            return true;
        }

        bool isFinished{ false };
        double alpha{ panelImage->GetAlpha() };
        alpha += deltaTimeSec / fadeTime_;
        if (alpha > AlphaMaxValue)
        {
            alpha = AlphaMaxValue;
            isFinished = true;
        }
        panelImage->SetAlpha(alpha);
        return isFinished;
    }

    bool NotificationUI::FadeOut(double deltaTimeSec)
    {
        std::shared_ptr<Image> panelImage = panelImage_.lock();
        if (!panelImage)
        {
            return true;
        }

        bool isFinished{ false };
        double alpha{ panelImage->GetAlpha() };
        static_assert(FadeTime > 0.0);
        alpha -= deltaTimeSec / FadeTime;
        if (alpha < AlphaMinValue)
        {
            alpha = AlphaMinValue;
            isFinished = true;
        }
        panelImage->SetAlpha(alpha);
        return isFinished;
    }

    void NotificationUI::Visible()
    {
        std::shared_ptr<Image> panelImage = panelImage_.lock();
        if (!panelImage)
        {
            return;
        }
        constexpr double maxAlpha{ 1.0 };
        panelImage->SetAlpha(maxAlpha);
    }

    void NotificationUI::Hide()
    {
        std::shared_ptr<Image> panelImage = panelImage_.lock();
        if (!panelImage)
        {
            return;
        }
        constexpr double minAlpha{ 0.0 };
        panelImage->SetAlpha(minAlpha);
    }

    double NotificationUI::CalcMove(double value, double targetValue, double adjustValue)
    {
        double resultValue{};
        if (value > targetValue)
        {
            resultValue = Math::Round(value - adjustValue);
            if (resultValue <= targetValue)
            {
                resultValue = targetValue;
            }
        }
        else if (value < targetValue)
        {
            resultValue = Math::Round(value + adjustValue);
            if (resultValue >= targetValue)
            {
                resultValue = targetValue;
            }
        }
        else
        {
            resultValue = targetValue;
        }
        return resultValue;
    }

    void NotificationUI::ChangeAnimationPhase(AnimationPhase phase)
    {
        currentPhase_ = phase;
    }
}
