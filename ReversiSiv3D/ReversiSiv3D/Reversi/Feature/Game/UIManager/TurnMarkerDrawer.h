#pragma once

#include "Feature/Game/UI/HUD/TurnMarker.h"
#include "Feature/System/Task.h"

namespace ReversiSiv3D {

    class TurnMarkerDrawer : public Task
    {
    public:

        TurnMarkerDrawer();
        ~TurnMarkerDrawer();

        void Update(double) override;
        void Render() override;
        void CreateMarker();
        void SetMarkerPos(Point);

        void Hide();
        void Visible();

    private:

        enum class DrawPhase
        {
            Hide,
            StartVisible,
            WaitUpdate,
            UpdateFadeOut,
            UpdateFadeIn
        };

    private:

        void ChangeDrawPhase(DrawPhase);

    private:

        static constexpr double AddAlphaValuePerSec{ 1.0 };
        static constexpr double AlphaMinValue{ 0.0 };
        static constexpr double AlphaAdjustMinValue{ 0.3 };
        static constexpr double AlphaMaxValue{ 1.0 };
        static constexpr double ResetTime{ 0.0 };
        static constexpr double WaitUpdateTime{ 2.0 };

    private:

        std::unique_ptr<TurnMarker> turnMarker_;
        DrawPhase drawPhase_;
        double elapsedTime_;
    };
}
