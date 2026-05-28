#pragma once
#include "Feature/Game/UI/Rectangle.h"
#include "Feature/Game/UI/UIBase.h"
#include <memory>

namespace ReversiSiv3D {

    class FadeDrawer : public UIBase
    {
    public:

        FadeDrawer();
        ~FadeDrawer();

        void Update(double) override;

        void Initialize();
        void SetCenterPos(Point);
        void SetAlpha(double);

        double GetAlpha() const;

        void StartFadeIn();
        void StartFadeOut();
        bool IsFadeFinish() const;

    private:

        enum class FadeState
        {
            Idle,
            StartFadeIn,
            FadeIn,
            FinishFadeIn,
            StartFadeOut,
            FadeOut,
            FinishFadeOut,
            Finish,
        };

    private:

        void ChangeCurrentState(FadeState);

    private:

        static constexpr ColorF DefaultColor{ 0.0, 0.0, 0.0, 1.0 };
        static constexpr double ResetTime{ 0.0 };
        static constexpr double MaxAlpha{ 1.0 };
        static constexpr double MinAlpha{ 0.0 };

    private:

        std::shared_ptr<Rectangle> fadeScreen_;
        FadeState currentState_;
        double alpha_;
    };
}
