#pragma once

#include "Feature/Game/UI/Result/ResultDetail.h"

namespace ReversiSiv3D {

    class Draw : public ResultDetail
    {
    public:

        Draw();
        ~Draw();

        void LoadResultIcon() override;
        void Start() override;

        void Update(double) override;

    private:

        enum class AnimationPhase
        {
            Idle,
            Start,
            UpperSwing,
            DownSwing,
            WaitShake
        };

    private:

        void ChangeAnimationPhase(AnimationPhase);

    private:

        static constexpr double ResetTime{ 0.0 };
        static constexpr double WaitStartShakeTime{ 1.5 };
        static constexpr double ResetAngle{ 0_deg };
        static constexpr double SwingOffsetAngle{ 5_deg };
        static constexpr double AmountOfChangePerSec{ 180_deg };
        static constexpr double ResetRepeatCount{ 0 };
        static constexpr double RepeatMaxCount{ 5 };

    private:

        AnimationPhase currentPhase_;
        double elapsedTime_;
        double angleMaxRadian_;
        double angleMinRadian_;
        double repeatCount_;
    };
}
