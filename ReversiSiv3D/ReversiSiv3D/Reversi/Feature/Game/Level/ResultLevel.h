#pragma once

#include "Feature/Game/GameState.h"
#include "Feature/Game/LogicManager/GameResultManager.h"
#include "Feature/Game/UIManager/BackScreenDrawer.h"
#include "Feature/Game/UIManager/BattleBoard.h"
#include "Feature/Game/UIManager/Button/ResultButtonManager.h"
#include "Feature/Game/UIManager/ResultDetailManager.h"
#include "Feature/Game/UIManager/ResultStoneAnimator.h"
#include "Feature/Game/UIManager/StoneManager.h"
#include "Feature/System/Interface/ResultLevelEventInterface.h"
#include "Feature/System/Level.h"
#include <memory>

namespace ReversiSiv3D {

    class ResultLevel : public Level, public ResultLevelEventInterface
    {
    public:

        ResultLevel(const std::shared_ptr<TaskManager>&);
        ~ResultLevel();

    public:

        // ResultButtonEventInterface経由で
        // ResultButtonManagerから実行される関数
        void OnStartGame() override;
        void OnReturnTitle() override;

    protected:

        void Initialize() override;
        void Running(double) override;
        void Finish() override;

    private:

        enum class LevelState
        {
            Wait,
            Initialize,
            Running,
            WaitOperation,
            TransferBattleLevel,
            TransferTitleLevel,
            BeforeRelease,
            Release,
            Released,
        };
    private:

        void ChangeLevelState(LevelState);

    private:

        static constexpr double MuteAudioValue{ 0.0 };

    private:

        LevelState currentLevelState_;
        std::shared_ptr<GameState> gameState_;
        std::shared_ptr<BackScreenDrawer> backScreen_;
        std::shared_ptr<BattleBoard> battleBoard_;
        std::shared_ptr<StoneManager> stoneManager_;
        std::shared_ptr<ResultStoneAnimator> stoneAnimator_;
        std::shared_ptr<ResultDetailManager> resultDetailManager_;
        std::shared_ptr<GameResultManager> gameResultManager_;
        std::shared_ptr<ResultButtonManager> resultButtonManager_;
    };
}
