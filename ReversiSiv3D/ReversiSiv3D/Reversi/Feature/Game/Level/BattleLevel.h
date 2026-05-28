#pragma once

#include "Feature/Game/GameState.h"
#include "Feature/Game/LogicManager/Enemy.h"
#include "Feature/Game/LogicManager/GameMaster.h"
#include "Feature/Game/LogicManager/MouseController.h"
#include "Feature/Game/UIManager/BackScreenDrawer.h"
#include "Feature/Game/UIManager/BattleBoard.h"
#include "Feature/Game/UIManager/CursorManager.h"
#include "Feature/Game/UIManager/DecideTurnOrderManager.h"
#include "Feature/Game/UIManager/IndicateManager.h"
#include "Feature/Game/UIManager/PanelManager.h"
#include "Feature/Game/UIManager/ScoreBoardManager.h"
#include "Feature/Game/UIManager/StoneManager.h"
#include "Feature/Game/UIManager/TurnMarkerDrawer.h"
#include "Feature/System/Interface/BattleLevelEventInterface.h"
#include "Feature/System/Level.h"
#include  <memory>

namespace ReversiSiv3D {

    class BattleLevel : public Level, public BattleLevelEventInterface
    {
    public:

        BattleLevel(const std::shared_ptr<TaskManager>&);
        ~BattleLevel();

    public:

        // BattleLevelEventInterface経由で
        // TurnOrderButtonManagerから実行される関数
        void OnDecideFirstPlayer();
        void OnDecideSecondPlayer();

    protected:

        void Initialize() override;
        void Running(double) override;
        void Finish() override;

    private:

        enum class LevelState
        {
            Wait,
            Initialize,
            SelectTurnOrder,
            StartGame,
            WaitStartGameAnim,
            Running,
            SwitchResult,
            Release,
            Released,
        };
    private:

        void ChangeLevelState(LevelState);

    private:

        static constexpr double MuteAudioValue{ 0.0 };

    private:

        LevelState currentLevelState_;
        std::shared_ptr<BackScreenDrawer> backScreen_;
        std::shared_ptr<BattleBoard> battleBoard_;
        std::shared_ptr<MouseController> mouseController_;
        std::shared_ptr<CursorManager> cursorManager_;
        std::shared_ptr<StoneManager> stoneManager_;
        std::shared_ptr<GameMaster> gameMaster_;
        std::shared_ptr<IndicateManager> indicateManager_;
        std::shared_ptr<GameState> gameState_;
        std::shared_ptr<PanelManager> panelManager_;
        std::shared_ptr<ScoreBoardManager> scoreBoardManager_;
        std::shared_ptr<TurnMarkerDrawer> turnMarkerDrawer_;
        std::shared_ptr<Enemy> enemy_;
        std::shared_ptr<DecideTurnOrderManager> decideTurnOrderManager_;
    };
}
