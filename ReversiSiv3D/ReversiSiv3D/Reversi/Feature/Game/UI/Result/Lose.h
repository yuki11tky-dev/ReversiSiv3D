#pragma once

#include "Feature/Game/UI/Result/ResultDetail.h"

namespace ReversiSiv3D {

    class Lose : public ResultDetail
    {
    public:

        Lose();
        ~Lose();

        void LoadResultIcon() override;
        void Start() override;

        void Update(double) override;

    private:

        enum class AnimationPhase
        {
            Idle,
            Start,
            ResetAngle,
            WaitStartSlump,
            Slump,
            WaitResetAngle
        };

    private:

        void ChangeAnimationPhase(AnimationPhase);

    private:

        static constexpr double ResetTime{ 0.0 };
        static constexpr double WaitStartSlumpTime{ 1.5 };
        static constexpr double WaitResetAngle{ 1.0 };
        static constexpr double ResetAngle{ 0_deg };
        static constexpr double MaxSlumpAngle{ 15_deg };
        static constexpr double AmountOfChangePerSec{ 40_deg };

    private:

        AnimationPhase currentPhase_;
        double elapsedTime_;
    };
}
