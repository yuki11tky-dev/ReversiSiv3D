#pragma once

#include "Feature/Game/GameState.h"
#include "Feature/System/Task.h"
#include <memory>

namespace ReversiSiv3D {

    class Enemy : public Task
    {
    public:

        Enemy();
        ~Enemy();

        void Update(double) override;

        void ResetThinking();
        void StartThinking();
        bool IsFinishThinking() const;

        void SetStateObserver(const std::shared_ptr<GameState>&);
        const BoardCell& GetPlacedCell() const;

    private:

        enum class ThinkingPhase
        {
            Idle,
            Start,
            Calculate,
            Evaluate,
            WaitFinish,
            Finish,
        };

    private:

        bool ComputeScore();
        BoardCell EvaluateScore();
        void ChangeThinkingPhase(ThinkingPhase);
        bool TryGetGameState(std::shared_ptr<GameState>&) const;

    private:

        static constexpr double ResetTime{ 0.0 };
        static constexpr double WaitTime{ 2.0 };
        static constexpr int ResetIndex{ 0 };

        static constexpr int EvaMaxInitValue{ -10000 };
        static constexpr int EvaBoardCoeffient{ 2 };
        static constexpr int ConfirmStoneCoeffient{ 55 };

    private:

        ThinkingPhase currentPhase_;
        std::weak_ptr<GameState> stateObserver_;
        BoardCell placedCell_;
        double elapsedTime_;
        int checkIndex_;
        std::unordered_map<BoardCell, EvaluateResult> evaluationResultList_;
    };
}
