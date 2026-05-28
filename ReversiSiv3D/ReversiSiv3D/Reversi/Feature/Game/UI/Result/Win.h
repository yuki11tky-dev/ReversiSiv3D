#pragma once

#include "Feature/Game/UI/Result/ResultDetail.h"

namespace ReversiSiv3D {

    class Win : public ResultDetail
    {
    public:

        Win();
        ~Win();

        void LoadResultIcon() override;
        void Start() override;

        void Update(double) override;

    private:

        enum class AnimationPhase
        {
            Idle,
            Start,
            Expand,
            Shrink
        };

    private:

        void ChangeAnimationPhase(AnimationPhase);

    private:

        static constexpr double ScaleOffset{ 0.05 };
        static constexpr double AmountOfChangePerSec{ 0.15 };

    private:

        AnimationPhase currentPhase_;
        double scaleMaxValue_;
        double scaleMinValue_;
    };
}
