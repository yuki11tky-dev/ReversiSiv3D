#include "Feature/Common/Math.h"
#include "Feature/Game/UI/Result/Lose.h"
#include <cassert>

namespace ReversiSiv3D {

    Lose::Lose()
        : ResultDetail{}
        , currentPhase_{}
        , elapsedTime_{}
    {

    }

    Lose::~Lose()
    {

    }

    void Lose::LoadResultIcon()
    {
        std::shared_ptr<Image> resultIcon = resultIcon_.lock();
        if (!resultIcon)
        {
            return;
        }
        resultIcon->LoadTexture(U"Image/result_lose.png");
    }

    void Lose::Start()
    {
        Visible();
        ChangeAnimationPhase(AnimationPhase::Start);
    }

    void Lose::Update(double deltaTimeSec)
    {
        switch (currentPhase_)
        {
        case AnimationPhase::Start:
        {
            elapsedTime_ = ResetTime;
            SetResultIconAngleForRadian(ResetAngle);
            ChangeAnimationPhase(AnimationPhase::Slump);
            break;
        }
        case AnimationPhase::ResetAngle:
        {
            SetResultIconAngleForRadian(ResetAngle);
            ChangeAnimationPhase(AnimationPhase::WaitStartSlump);
            break;
        }
        case AnimationPhase::WaitStartSlump:
        {
            elapsedTime_ += deltaTimeSec;
            if (elapsedTime_ < WaitStartSlumpTime)
            {
                break;
            }
            elapsedTime_ = ResetTime;

            ChangeAnimationPhase(AnimationPhase::Slump);
            break;
        }
        case AnimationPhase::Slump:
        {
            const double amountOfChangePerFrame{ AmountOfChangePerSec * deltaTimeSec };
            const double currentAngle{ GetResultIconAngleForRadian() };
            double changedAngleForRadiun{ currentAngle + amountOfChangePerFrame };
            if (changedAngleForRadiun > MaxSlumpAngle)
            {
                changedAngleForRadiun = MaxSlumpAngle;
                elapsedTime_ = ResetTime;
                ChangeAnimationPhase(AnimationPhase::WaitResetAngle);
            }
            SetResultIconAngleForRadian(changedAngleForRadiun);
            break;
        }
        case AnimationPhase::WaitResetAngle:
        {
            elapsedTime_ += deltaTimeSec;
            if (elapsedTime_ < WaitResetAngle)
            {
                break;
            }
            elapsedTime_ = ResetTime;
            ChangeAnimationPhase(AnimationPhase::ResetAngle);
            break;
        }
        }
    }

    void Lose::ChangeAnimationPhase(AnimationPhase phase)
    {
        currentPhase_ = phase;
    }
}
