#pragma once

#include "Feature/Game/GameState.h"
#include "Feature/Game/UIManager/ResultDetailManager.h"
#include "Feature/Game/UIManager/ResultStoneAnimator.h"
#include "Feature/System/Task.h"
#include <memory>

namespace ReversiSiv3D {

    enum class ResultPhase
    {
        Idle,
        Start,
        StoneAnimation,
        ShowResultDetail,
        Finish,
        Finished
    };

    class GameResultManager : public Task
    {
    public:

        GameResultManager();
        ~GameResultManager();

        void Update(double) override;

        void SetStateObserver(const std::shared_ptr<GameState>&);
        void SetStoneAnimator(std::shared_ptr<ResultStoneAnimator>);
        void SetResultDetailManager(std::shared_ptr<ResultDetailManager>);
        void Start();
        bool IsFinished() const;

    private:

        void ChangeResultPhase(ResultPhase);

    private:

        std::weak_ptr<GameState> stateObserver_;
        std::shared_ptr<ResultStoneAnimator> stoneAnimator_;
        std::shared_ptr<ResultDetailManager> resultDetailManager_;

        ResultPhase currentPhase_;
    };
}
