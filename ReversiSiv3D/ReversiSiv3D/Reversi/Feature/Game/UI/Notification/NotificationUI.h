#pragma once

#include "Feature/Game/UI/Image.h"
#include "Feature/Game/UI/Text.h"
#include "Feature/Game/UI/UIBase.h"

namespace ReversiSiv3D {

    enum class PanelType
    {
        None,
        SkipTurn,
        Start,
        Finish
    };

    class NotificationUI : public UIBase
    {
    public:

        NotificationUI(PanelType);
        ~NotificationUI();

        void Initialize(const String&);
        void SetScale(double);
        void SetImageColor(ColorF);
        void SetPlayerName(const String&);
        void SetStoneCount(const String&);
        void SetPlayerNameFont(FontMethod, int32, Typeface);
        void SetStoneCountFont(FontMethod, int32, Typeface);
        void SetPlayerNameColor(ColorF);
        void SetStoneCountColor(ColorF);

    public:

        PanelType GetPanelType() const;

        void Initialize();
        void Update(double) override;
        void StartAnimation();
        bool IsFinishedAnimation() const;

    private:

        enum class AnimationPhase
        {
            Start,
            FadeIn,
            WaitFadeOut,
            FadeOut,
            Finish,
            Finished
        };

    private:

        void SetupFadeIn();
        void SetupFadeOut();

        bool Move(double);
        bool FadeIn(double);
        bool FadeOut(double);
        void Visible();
        void Hide();

        double CalcMove(double, double, double);
        void ChangeAnimationPhase(AnimationPhase);

    private:

        static constexpr double AlphaMaxValue{ 1.0 };
        static constexpr double AlphaMinValue{ 0.0 };
        static constexpr double MoveOffsetXFromCenter{ 50.0 };
        static constexpr double MoveOffsetYFromCenter{ 0.0 };
        static constexpr double ResetTime{ 0.0 };
        static constexpr double FadeTime{ 0.4 };
        static constexpr double MoveTime{ 0.5 };
        static constexpr double DisplayTime{ 1.5 };
        static constexpr ColorF DefaultColor{ 1.0, 1.0, 1.0, 1.0 };

    protected:

        Vec2 moveOffset_;
        double moveTime_;
        double fadeTime_;
        double displayTime_;

    private:

        PanelType panelType_;
        AnimationPhase currentPhase_;
        double elapsedTime_;
        Vec2 targetPos_;

        std::weak_ptr<Image> panelImage_;
    };
}
