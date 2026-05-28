#pragma once

#include "Feature/Game/UI/ImageSprite.h"
#include "Feature/Game/UI/Text.h"
#include "Feature/Game/UI/UIBase.h"
#include <memory>
#include <vector>

namespace ReversiSiv3D {

    class BootEvent : public UIBase
    {
    public:

        BootEvent();
        ~BootEvent();

        void Update(double) override;

        void Initialize();
        void Start();
        bool IsFinished() const;

    private:

        bool UpdatePeriodText(double);
        bool UpdateStoneJumpAnimation(double);
        void UpdateStoneSpinAnimation(double);
        double ResetVelocity();

    private:

        enum class EventState
        {
            Idle,
            Start,
            AddPeriod,
            Animation,
            Finish,
            Finished,
        };

    private:

        void ChangeEventState(EventState);

    private:

        static constexpr Vec2 BootTextAdjustPos{ 150.0, 0.0 };
        static constexpr double BootTextSize{ 128.0 };
        static constexpr ColorF TextColor{ 0.0, 0.0, 0.0, 1.0 };
        static constexpr Vec2 PeriodTextAdjustFirstPos{ 25.0, 0.0 };
        static constexpr Vec2 PeriodTextDisplayInterval{ 40.0, 0.0 };
        static constexpr ColorF SpinStoneColor{ 1.0, 1.0, 1.0, 1.0 };
        static constexpr Vec2 StoneAdjustPos{ 180.0, -10.0 };
        static constexpr double BounceHeights[3]{ 150.0, 75.0, 20.0 };

        static constexpr double CoefficientValue{ 2.0 };
        static constexpr double GroundY{ 530.0 };
        static constexpr double Gravity{ 4500.0 };

        static constexpr double ResetTime{ 0.0 };
        static constexpr double PeriodAddTime{ 0.45 };
        static constexpr double SwitchImageTime{ 0.030 };

    private:

        std::weak_ptr<ImageSprite> spriteStone_;
        std::weak_ptr<Text> bootText_;
        std::vector<std::weak_ptr<Text>> periodTextList_;

        EventState currentState_;
        double elapsedTimeForPeriod_;
        double elapsedTimeForImage_;

        double velocity_;
        int bounceIndex_;
        std::vector<double> bounceHeightList_;
        double bounceGroundPosY_;

        int appearedPeriodIndex_;
    };
}

