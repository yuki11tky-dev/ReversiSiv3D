#pragma once

#include "Feature/Game/GameState.h"
#include "Feature/Game/LogicManager/Enemy.h"
#include "Feature/Game/SharedEnum.h"
#include "Feature/Game/UIManager/CursorManager.h"
#include "Feature/Game/UIManager/IndicateManager.h"
#include "Feature/Game/UIManager/PanelManager.h"
#include "Feature/Game/UIManager/ScoreBoardManager.h"
#include "Feature/Game/UIManager/TurnMarkerDrawer.h"
#include "Feature/System/Task.h"
#include <unordered_map>

namespace ReversiSiv3D {

    enum class TurnPhase
    {
        None,
        InitialSetup,
        WaitStartGame,
        StartGame,
        StartThink,
        Thinking,
        EndThink,
        StartFlipStone,
        Flipping,
        EndFlipping,
        SwitchPlayer,
        CheckPlaceableCell,
        SkipTurn,
        SkipNotification,
        FinishGame,
        WaitFinishGame,
        FinishedGame,
        Error
    };

    enum class GameStartAnimPhase
    {
        Idle,
        Start,
        PlaceFirst,
        PlaceSecond,
        PlaceThird,
        PlaceFourth,
        StartPanelAnim,
        WaitPanelAnim,
        FinishPanelAnim,
        FinishAll,
        FinishedAll
    };

    class GameMaster : public Task
    {
    public:

        GameMaster();
        ~GameMaster();

        void Update(double) override;

        void InitialSetup(PlayerType, PlayerType);

        void StartGame();
        bool IsFinishedStartGameAnim() const;
        bool IsThinking() const;
        bool IsFinishedGame() const;

    public:

        void SetCursorManager(std::shared_ptr<CursorManager>);
        void SetIndicateManager(std::shared_ptr<IndicateManager>);
        void SetStateObserver(std::shared_ptr<GameState>);
        void SetPanelManager(std::shared_ptr<PanelManager>);
        void SetScoreBoardManager(std::shared_ptr<ScoreBoardManager>);
        void SetTurnMarkerDrawer(std::shared_ptr<TurnMarkerDrawer>);
        void SetEnemy(std::shared_ptr<Enemy>);

    private:

        bool Thinking();

        void InitPlacedStone(StoneType, const BoardCell&);
        bool TryPlacedStone(GameState&, StoneType, const BoardCell&);
        void CheckPlaceableAndSwitchTurn();
        void PrepareBeforeThinking(TurnPhase);

        bool FlipStone();

        void ChangeCurrentTurn(TurnPhase);
        void VisibleCursorAndIndicater();
        void HideCursorAndIndicater();

        void PrepareBeforeResultLevelTrans();

        bool GameStartAnimation(double);
        void SetupPanelAnimation(PanelType);
        bool IsFinishedPanelAnimation(PanelType);
        void ChangeGameStartAnimPhase(GameStartAnimPhase);

    private:

        static constexpr BoardCell InitPlacedLeftTopWhite{ 3, 3 };
        static constexpr BoardCell InitPlacedRightTopBlack{ 4, 3 };
        static constexpr BoardCell InitPlacedLeftBottomBlack{ 3, 4 };
        static constexpr BoardCell InitPlacedRightBottomWhite{ 4, 4 };
        static constexpr BoardCell InitPlayerCursorCell{ 4, 5 };

        static constexpr double InitPlaceStoneWaitTime{ 0.25 };
        static constexpr double FlipOverAnimationInterval{ 0.07 };
        static constexpr double WaitFlipOverAnimation{ 0.5 };
        static constexpr double ResetTime{ 0.0 };

    private:

        std::shared_ptr<CursorManager> cursorManager_;
        std::shared_ptr<IndicateManager> indicateManager_;
        std::shared_ptr<GameState> gameState_;
        std::shared_ptr<PanelManager> panelManager_;
        std::shared_ptr<ScoreBoardManager> scoreBoardManager_;
        std::shared_ptr<TurnMarkerDrawer> turnMarkerDrawer_;
        std::shared_ptr<Enemy> enemy_;

        TurnPhase currentTurn_;
        GameStartAnimPhase gameStartAnimPhase_;
        double elapsedTime_;
    };
}
