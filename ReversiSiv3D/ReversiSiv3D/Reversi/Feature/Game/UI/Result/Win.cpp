#include "Feature/Game/UI/Result/Win.h"
#include <cassert>

namespace ReversiSiv3D {

    Win::Win()
        : ResultDetail{}
        , currentPhase_{}
        , scaleMaxValue_{}
        , scaleMinValue_{}
    {

    }

    Win::~Win()
    {

    }

    void Win::LoadResultIcon()
    {
        std::shared_ptr<Image> resultIcon = resultIcon_.lock();
        if (!resultIcon)
        {
            return;
        }
        resultIcon->LoadTexture(U"Image/result_winner.png");
    }

    void Win::Start()
    {
        Visible();
        ChangeAnimationPhase(AnimationPhase::Start);
    }

    void Win::Update(double deltaTimeSec)
    {
        switch (currentPhase_)
        {
        case AnimationPhase::Start:
        {
            const double resultIconScale{ GetResultIconScale() };
            assert(resultIconScale - ScaleOffset >= 0.0);
            scaleMaxValue_ = resultIconScale + ScaleOffset;
            scaleMinValue_ = resultIconScale - ScaleOffset;
            ChangeAnimationPhase(AnimationPhase::Expand);
            break;
        }
        case AnimationPhase::Expand:
        {
            const double amountOfChangePerFrame{ AmountOfChangePerSec * deltaTimeSec };
            const double currentScale{ GetResultIconScale() };
            double changedScale{ currentScale + amountOfChangePerFrame };
            if (changedScale > scaleMaxValue_)
            {
                changedScale = scaleMaxValue_;
                ChangeAnimationPhase(AnimationPhase::Shrink);
            }
            SetResultIconScale(changedScale);
            break;
        }
        case AnimationPhase::Shrink:
        {
            const double amountOfChangePerFrame{ AmountOfChangePerSec * deltaTimeSec };
            const double currentScale{ GetResultIconScale() };
            double changedScale{ currentScale - amountOfChangePerFrame };
            if (changedScale < scaleMinValue_)
            {
                changedScale = scaleMinValue_;
                ChangeAnimationPhase(AnimationPhase::Expand);
            }
            SetResultIconScale(changedScale);
            break;
        }
        }
    }

    void Win::ChangeAnimationPhase(AnimationPhase phase)
    {
        currentPhase_ = phase;
    }
}
