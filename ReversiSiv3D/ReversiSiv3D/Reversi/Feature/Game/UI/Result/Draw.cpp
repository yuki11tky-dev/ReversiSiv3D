#include "Feature/Game/UI/Result/Draw.h"
#include <cassert>

namespace ReversiSiv3D {

    Draw::Draw()
        : ResultDetail{}
        , currentPhase_{}
        , elapsedTime_{}
        , angleMaxRadian_{}
        , angleMinRadian_{}
        , repeatCount_{}
    {

    }

    Draw::~Draw()
    {

    }

    void Draw::LoadResultIcon()
    {
        std::shared_ptr<Image> resultIcon = resultIcon_.lock();
        if (!resultIcon)
        {
            return;
        }
        resultIcon->LoadTexture(U"Image/result_draw.png");
    }

    void Draw::Start()
    {
        Visible();
        ChangeAnimationPhase(AnimationPhase::Start);
    }

    void Draw::Update(double deltaTimeSec)
    {
        switch (currentPhase_)
        {
        case AnimationPhase::Start:
        {
            elapsedTime_ = ResetTime;
            repeatCount_ = ResetRepeatCount;

            const double currentAngle{ GetResultIconAngleForRadian() };
            angleMaxRadian_ = currentAngle + SwingOffsetAngle;
            angleMinRadian_ = (currentAngle - SwingOffsetAngle);
            ChangeAnimationPhase(AnimationPhase::UpperSwing);
            break;
        }
        case AnimationPhase::UpperSwing:
        {
            const double amountOfChangePerFrame{ AmountOfChangePerSec * deltaTimeSec };
            const double currentAngle{ GetResultIconAngleForRadian() };
            double changedAngleForRadiun{ currentAngle + amountOfChangePerFrame };
            if (changedAngleForRadiun > angleMaxRadian_)
            {
                changedAngleForRadiun = angleMaxRadian_;
                elapsedTime_ = ResetTime;
                ChangeAnimationPhase(AnimationPhase::DownSwing);
            }
            SetResultIconAngleForRadian(changedAngleForRadiun);
            break;
        }
        case AnimationPhase::DownSwing:
        {
            const double amountOfChangePerFrame{ AmountOfChangePerSec * deltaTimeSec };
            const double currentAngle{ GetResultIconAngleForRadian() };
            double changedAngleForRadiun{ currentAngle - amountOfChangePerFrame };
            if (repeatCount_ < RepeatMaxCount - 1)
            {
                if (changedAngleForRadiun < angleMinRadian_)
                {
                    changedAngleForRadiun = angleMinRadian_;
                    ++repeatCount_;
                    ChangeAnimationPhase(AnimationPhase::UpperSwing);
                }
            }
            else
            {
                if (changedAngleForRadiun < ResetAngle)
                {
                    changedAngleForRadiun = ResetAngle;
                    repeatCount_ = ResetRepeatCount;
                    ChangeAnimationPhase(AnimationPhase::WaitShake);
                }
            }
            SetResultIconAngleForRadian(changedAngleForRadiun);
            break;
        }
        case AnimationPhase::WaitShake:
        {
            elapsedTime_ += deltaTimeSec;
            if (elapsedTime_ < WaitStartShakeTime)
            {
                break;
            }
            elapsedTime_ = ResetTime;
            ChangeAnimationPhase(AnimationPhase::UpperSwing);
            break;
        }
        }
    }

    void Draw::ChangeAnimationPhase(AnimationPhase phase)
    {
        currentPhase_ = phase;
    }
}
