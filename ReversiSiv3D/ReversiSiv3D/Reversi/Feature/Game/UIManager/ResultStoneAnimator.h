#pragma once

#include "Feature/Game/GameState.h"
#include "Feature/System/Task.h"
#include <memory>

namespace ReversiSiv3D {

    class ResultStoneAnimator : public Task
    {
    public:

        ResultStoneAnimator();
        ~ResultStoneAnimator();

        void Update(double) override;

        void SetStateObserver(const std::shared_ptr<GameState>&);
        void Start();
        bool IsFinished() const;

    private:

        enum class AnimationPhase
        {
            None,
            Start,
            ShowResult,
            WaitOperate,
            Finish,
            Finished
        };

        bool ShowResult();
        BoardCell PlaceNext(ResultLayoutSide);
        void ChangeAnimationPhase(AnimationPhase);
        bool TryGetGameState(std::shared_ptr<GameState>&) const;

    private:

        static constexpr double ResetTime{ 0.0 };
        static constexpr double PlaceStoneInterval{ 0.05 };
        static constexpr BoardCell StartCellLeftSide{ 0, 0 };
        static constexpr BoardCell StartCellRightSide{ 7, 0 };

    private:

        std::weak_ptr<GameState> stateObserver_;
        AnimationPhase currentPhase_;
        double elapsedTime_;
        int placedCount_;
    };
}
