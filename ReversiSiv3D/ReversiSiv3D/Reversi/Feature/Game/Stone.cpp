#include "Feature/Common/Math.h"
#include "Feature/Game/Stone.h"
#include "Feature/System/SoundManager.h"

namespace ReversiSiv3D {

    Stone::Stone()
        : imagePos_{}
        , drawIndex_{}
        , elapsedTime_{}
        , currentPhase_{ PhaseType::Hide }
        , stoneType_{}
    {

    }

    Stone::~Stone()
    {

    }

    void Stone::Update(double deltaTime)
    {
        switch (currentPhase_)
        {
        case PhaseType::Wait:
        {
            break;
        }
        case PhaseType::Hide:
        {
            elapsedTime_ = ResetTime;
            ChangeCurrentPhase(PhaseType::Wait);
            break;
        }
        case PhaseType::Placed:
        {
            SoundManager::PlayOneShot(SoundType::PlacedStoneSe);
            ChangeCurrentPhase(PhaseType::Wait);
            break;
        }
        case PhaseType::StartFlipping:
        {
            elapsedTime_ = ResetTime;
            ChangeCurrentPhase(PhaseType::Flipping);
            break;
        }
        case PhaseType::Flipping:
        {
            elapsedTime_ += deltaTime;
            if (elapsedTime_ < SwitchAnimation)
            {
                break;
            }
            elapsedTime_ = ResetTime;

            const bool isFinished = FlipOverAnimation();
            if (!isFinished)
            {
                break;
            }
            SoundManager::PlayOneShot(SoundType::PlacedStoneSe);

            ChangeCurrentPhase(PhaseType::EndFlipping);
            break;
        }
        case PhaseType::EndFlipping:
        {

            ChangeCurrentPhase(PhaseType::Wait);
            break;
        }
        default:
        {
            break;
        }
        }
    }

    RenderStoneInfo Stone::GetRenderInfo()
    {
        RenderStoneInfo stoneInfo{};
        stoneInfo.isHide_ = currentPhase_ == PhaseType::Hide;
        stoneInfo.drawIndex_ = drawIndex_;
        stoneInfo.placeCenterPos_ = imagePos_;

        return stoneInfo;
    }

    void Stone::SetPos(Vec2 pos)
    {
        imagePos_ = pos;
    }

    void Stone::SetStoneType(StoneType stoneType)
    {
        stoneType_ = stoneType;
        switch (stoneType_)
        {
        case StoneType::Black:
        {
            drawIndex_ = BlackStoneIndex;
            break;
        }
        case StoneType::White:
        {
            drawIndex_ = WhiteStoneIndex;
            break;
        }
        }
    }

    void Stone::ChangeStoneType()
    {
        switch (stoneType_)
        {
        case StoneType::Black:
        {
            stoneType_ = StoneType::White;
            break;
        }
        case StoneType::White:
        {
            stoneType_ = StoneType::Black;
            break;
        }
        }
    }

    StoneType Stone::GetStoneType() const
    {
        return stoneType_;
    }

    void Stone::Hide()
    {
        ChangeCurrentPhase(PhaseType::Hide);
    }

    void Stone::Visible()
    {
        ChangeCurrentPhase(PhaseType::Placed);
    }

    void Stone::StartFlipping()
    {
        ChangeCurrentPhase(PhaseType::StartFlipping);
    }

    bool Stone::FlipOverAnimation()
    {
        switch (stoneType_)
        {
        case StoneType::Black:
        {
            --drawIndex_;
            if (drawIndex_ <= BlackStoneIndex)
            {
                drawIndex_ = BlackStoneIndex;
                return true;
            }
            break;
        }
        case StoneType::White:
        {
            ++drawIndex_;
            if (drawIndex_ >= WhiteStoneIndex)
            {
                drawIndex_ = WhiteStoneIndex;
                return true;
            }
            break;
        }
        }

        return false;
    }

    void Stone::ChangeCurrentPhase(PhaseType phaseType)
    {
        currentPhase_ = phaseType;
    }
}
