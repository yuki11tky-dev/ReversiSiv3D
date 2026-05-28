#include "Feature/Common/Math.h"
#include "Feature/Game/GameInfo.h"
#include "Feature/Game/LogicManager/GameMaster.h"
#include "Feature/Game/ReversiLogic.h"
#include "Feature/Game/UIManager/BattleBoard.h"
#include <cassert>
#include <queue>

namespace ReversiSiv3D {

    GameMaster::GameMaster()
        :Task{ TaskId::GameMaster }
        , cursorManager_{}
        , indicateManager_{}
        , gameState_{}
        , panelManager_{}
        , scoreBoardManager_{}
        , turnMarkerDrawer_{}
        , enemy_{}
        , currentTurn_{ TurnPhase::InitialSetup }
        , gameStartAnimPhase_{ GameStartAnimPhase::Idle }
        , elapsedTime_{}
    {

    }

    GameMaster::~GameMaster()
    {

    }

    void GameMaster::Update(double deltaTimeSec)
    {
        switch (currentTurn_)
        {
        case TurnPhase::InitialSetup:
        {
            ChangeCurrentTurn(TurnPhase::WaitStartGame);
            break;
        }
        case TurnPhase::WaitStartGame:
        {
            break;
        }
        case TurnPhase::StartGame:
        {
            const bool isFinish = GameStartAnimation(deltaTimeSec);
            if (!isFinish)
            {
                break;
            }
            CheckPlaceableAndSwitchTurn();
            ChangeCurrentTurn(TurnPhase::StartThink);
            break;
        }
        case TurnPhase::StartThink:
        {
            PrepareBeforeThinking(TurnPhase::StartThink);
            ChangeCurrentTurn(TurnPhase::Thinking);
            break;
        }
        case TurnPhase::Thinking:
        {
            const bool isFinishedThinking = Thinking();
            if (!isFinishedThinking)
            {
                break;
            }
            ChangeCurrentTurn(TurnPhase::EndThink);
            break;
        }
        case TurnPhase::EndThink:
        {
            PrepareBeforeThinking(TurnPhase::EndThink);
            ReversiLogic::CheckStoneCountAfterFlipOver(*gameState_);
            ChangeCurrentTurn(TurnPhase::StartFlipStone);
            break;
        }
        case TurnPhase::StartFlipStone:
        {
            elapsedTime_ = ResetTime;
            ChangeCurrentTurn(TurnPhase::Flipping);
            break;
        }
        case TurnPhase::Flipping:
        {
            elapsedTime_ += deltaTimeSec;
            if (elapsedTime_ < FlipOverAnimationInterval)
            {
                break;
            }
            elapsedTime_ = ResetTime;

            const bool isFinished = FlipStone();
            if (!isFinished)
            {
                break;
            }
            ChangeCurrentTurn(TurnPhase::EndFlipping);
            break;
        }
        case TurnPhase::EndFlipping:
        {
            elapsedTime_ += deltaTimeSec;
            if (elapsedTime_ < WaitFlipOverAnimation)
            {
                break;
            }
            elapsedTime_ = ResetTime;
            ChangeCurrentTurn(TurnPhase::SwitchPlayer);
            break;
        }
        case TurnPhase::SwitchPlayer:
        {
            assert(gameState_);
            gameState_->SwitchOperationPlayer();
            ChangeCurrentTurn(TurnPhase::CheckPlaceableCell);
            break;
        }
        case TurnPhase::CheckPlaceableCell:
        {
            CheckPlaceableAndSwitchTurn();
            break;
        }
        case TurnPhase::SkipTurn:
        {
            SetupPanelAnimation(PanelType::SkipTurn);
            ChangeCurrentTurn(TurnPhase::SkipNotification);
            break;
        }
        case TurnPhase::SkipNotification:
        {
            const bool isFinished = IsFinishedPanelAnimation(PanelType::SkipTurn);
            if (!isFinished)
            {
                break;
            }

            ChangeCurrentTurn(TurnPhase::SwitchPlayer);
            break;
        }
        case TurnPhase::FinishGame:
        {
            SetupPanelAnimation(PanelType::Finish);
            PrepareBeforeResultLevelTrans();
            ChangeCurrentTurn(TurnPhase::WaitFinishGame);
            break;
        }
        case TurnPhase::WaitFinishGame:
        {
            const bool isFinished = IsFinishedPanelAnimation(PanelType::Finish);
            if (!isFinished)
            {
                break;
            }
            ChangeCurrentTurn(TurnPhase::FinishedGame);
            break;
        }
        case TurnPhase::FinishedGame:
        {
            break;
        }
        case TurnPhase::Error:
        {
            // エラー発生時処理
            assert(false && "An error occurred during gameplay.");
            break;
        }
        default:
        {
            break;
        }
        }
    }

    void GameMaster::SetCursorManager(std::shared_ptr<CursorManager> cursorManager)
    {
        cursorManager_ = std::move(cursorManager);
    }
    void GameMaster::SetIndicateManager(std::shared_ptr<IndicateManager> indicateManager)
    {
        indicateManager_ = std::move(indicateManager);
    }
    void GameMaster::SetStateObserver(std::shared_ptr<GameState> gameState)
    {
        gameState_ = std::move(gameState);
    }
    void GameMaster::SetPanelManager(std::shared_ptr<PanelManager> panelManager)
    {
        panelManager_ = std::move(panelManager);
    }
    void GameMaster::SetScoreBoardManager(std::shared_ptr<ScoreBoardManager> scoreBoardManager)
    {
        scoreBoardManager_ = std::move(scoreBoardManager);
    }
    void GameMaster::SetTurnMarkerDrawer(std::shared_ptr<TurnMarkerDrawer> turnMarkerDrawer)
    {
        turnMarkerDrawer_ = std::move(turnMarkerDrawer);
    }
    void GameMaster::SetEnemy(std::shared_ptr<Enemy> enemy)
    {
        enemy_ = std::move(enemy);
    }

    void GameMaster::StartGame()
    {
        ChangeCurrentTurn(TurnPhase::StartGame);
        ChangeGameStartAnimPhase(GameStartAnimPhase::Start);
    }

    bool GameMaster::IsFinishedStartGameAnim() const
    {
        return gameStartAnimPhase_ == GameStartAnimPhase::FinishedAll;
    }
    bool GameMaster::IsThinking() const
    {
        assert(gameState_);
        PlayerData currentPlayerData = gameState_->GetCurrentPlayData();
        const bool isThinking{ currentTurn_ == TurnPhase::Thinking };
        const bool isManualOperation{ currentPlayerData.playerType_ != PlayerType::Cpu };
        return isThinking && isManualOperation;
    }
    bool GameMaster::IsFinishedGame() const
    {
        return currentTurn_ == TurnPhase::FinishedGame;
    }

    void GameMaster::SetupPanelAnimation(PanelType panelType)
    {
        assert(panelManager_);
        panelManager_->StartPanelAnimation(panelType);
    }

    bool GameMaster::IsFinishedPanelAnimation(PanelType panelType)
    {
        assert(panelManager_);
        return panelManager_->IsFinishedPanelAnimation(panelType);
    }

    void GameMaster::ChangeGameStartAnimPhase(GameStartAnimPhase phase)
    {
        gameStartAnimPhase_ = phase;
    }

    void GameMaster::InitialSetup(PlayerType firstAttack, PlayerType secondAttack)
    {
        assert(gameState_ && scoreBoardManager_ && cursorManager_);
        gameState_->SetTurnOrder(firstAttack, secondAttack);
        scoreBoardManager_->Assigned(firstAttack, secondAttack);

        const BoardData& boardData = gameState_->GetBoardData();
        const Vec2 cursorPos{ ReversiLogic::ConvertGridCellToCenterCoord(boardData, InitPlayerCursorCell) };
        cursorManager_->SetCurrentCursorPos(cursorPos);
    }

    bool GameMaster::Thinking()
    {
        assert(gameState_ && enemy_ && cursorManager_);

        const PlayerType currentPlayerType = gameState_->GetCurrentPlayData().playerType_;
        const bool isCpu = currentPlayerType == PlayerType::Cpu;
        if (isCpu)
        {
            const bool isFinish = enemy_->IsFinishThinking();
            if (!isFinish)
            {
                return false;
            }
            const BoardCell& placedCell = enemy_->GetPlacedCell();
            const StoneType stoneType{ gameState_->GetCurrentTurn() };
            return TryPlacedStone(*gameState_, stoneType, placedCell);
        }
        else
        {
            const BoardData& boardData = gameState_->GetBoardData();
            const Vec2 mousePos{ gameState_->GetMouseCursorPos() };
            const bool isOnBoard = ReversiLogic::IsMouseCursorOnTheBoard(boardData, mousePos);
            if (!isOnBoard)
            {
                return false;
            }

            const Vec2 cursorPos{ ReversiLogic::ConvertCoordPosToCenterCoord(boardData, mousePos) };
            cursorManager_->SetCurrentCursorPos(cursorPos);

            const bool isPressLeft = gameState_->GetPressLeftButton();
            if (isPressLeft)
            {
                // 石の配置試行イベント
                // 配置可能ではない場所の場合は false が返り配置処理を継続
                // 配置可能な場所の場合は true が返り次のフェーズへ進む
                const StoneType stoneType{ gameState_->GetCurrentTurn() };
                const BoardCell cell{ ReversiLogic::ConvertCoordinatesToGridCell(boardData, mousePos) };
                return TryPlacedStone(*gameState_, stoneType, cell);
            }
        }
        return false;
    }

    bool GameMaster::GameStartAnimation(double deltaTimeSec)
    {
        switch (gameStartAnimPhase_)
        {
        case GameStartAnimPhase::Start:
        {
            elapsedTime_ = ResetTime;

            ChangeGameStartAnimPhase(GameStartAnimPhase::PlaceFirst);
            break;
        }
        case GameStartAnimPhase::PlaceFirst:
        {
            elapsedTime_ += deltaTimeSec;
            if (elapsedTime_ < InitPlaceStoneWaitTime)
            {
                break;
            }
            elapsedTime_ = ResetTime;

            InitPlacedStone(StoneType::White, InitPlacedLeftTopWhite);

            ChangeGameStartAnimPhase(GameStartAnimPhase::PlaceSecond);
            break;
        }
        case GameStartAnimPhase::PlaceSecond:
        {
            elapsedTime_ += deltaTimeSec;
            if (elapsedTime_ < InitPlaceStoneWaitTime)
            {
                break;
            }
            elapsedTime_ = ResetTime;

            InitPlacedStone(StoneType::Black, InitPlacedRightTopBlack);

            ChangeGameStartAnimPhase(GameStartAnimPhase::PlaceThird);
            break;
        }
        case GameStartAnimPhase::PlaceThird:
        {
            elapsedTime_ += deltaTimeSec;
            if (elapsedTime_ < InitPlaceStoneWaitTime)
            {
                break;
            }
            elapsedTime_ = ResetTime;

            InitPlacedStone(StoneType::Black, InitPlacedLeftBottomBlack);

            ChangeGameStartAnimPhase(GameStartAnimPhase::PlaceFourth);
            break;
        }
        case GameStartAnimPhase::PlaceFourth:
        {
            elapsedTime_ += deltaTimeSec;
            if (elapsedTime_ < InitPlaceStoneWaitTime)
            {
                break;
            }
            elapsedTime_ = ResetTime;

            InitPlacedStone(StoneType::White, InitPlacedRightBottomWhite);

            ChangeGameStartAnimPhase(GameStartAnimPhase::StartPanelAnim);
            break;
        }
        case GameStartAnimPhase::StartPanelAnim:
        {
            elapsedTime_ += deltaTimeSec;
            if (elapsedTime_ < InitPlaceStoneWaitTime)
            {
                break;
            }
            elapsedTime_ = ResetTime;

            SetupPanelAnimation(PanelType::Start);

            ChangeGameStartAnimPhase(GameStartAnimPhase::WaitPanelAnim);
            break;
        }
        case GameStartAnimPhase::WaitPanelAnim:
        {
            const bool isFinished = IsFinishedPanelAnimation(PanelType::Start);
            if (!isFinished)
            {
                break;
            }
            ChangeGameStartAnimPhase(GameStartAnimPhase::FinishPanelAnim);
            break;
        }
        case GameStartAnimPhase::FinishPanelAnim:
        {

            ChangeGameStartAnimPhase(GameStartAnimPhase::FinishAll);
            break;
        }
        case GameStartAnimPhase::FinishAll:
        {

            ChangeGameStartAnimPhase(GameStartAnimPhase::FinishedAll);
            break;
        }
        case GameStartAnimPhase::FinishedAll:
        {
            return true;
        }
        }

        return false;
    }

    void GameMaster::InitPlacedStone(StoneType stoneType, const BoardCell& cell)
    {
        assert(gameState_);

        // リバーシの石の初期配置
        ReversiLogic::PlacedStone(*gameState_, stoneType, cell);

        int32 stoneCount{};
        switch (stoneType)
        {
        case StoneType::Black:
        {
            stoneCount = gameState_->GetBlackStoneCount();
            break;
        }
        case StoneType::White:
        {
            stoneCount = gameState_->GetWhiteStoneCount();
            break;
        }
        }
        ++stoneCount;
        gameState_->SetStoneCount(stoneType, stoneCount);
    }

    bool GameMaster::TryPlacedStone(GameState& gameState, StoneType stoneType, const BoardCell& cell)
    {
        // 石を配置しようとしている座標が盤上であることは関数外でチェックする必要あり
        // 対象のマスが配置可能かどうかをチェックして配置可能なら配置する
        const bool isPlaceable = gameState.IsCellPlaceable(cell);
        if (!isPlaceable)
        {
            return false;
        }

        ReversiLogic::PlacedStone(gameState, stoneType, cell);
        return true;
    }

    void GameMaster::CheckPlaceableAndSwitchTurn()
    {
        assert(gameState_);
        const StoneType stoneType{ gameState_->GetCurrentTurn() };
        const TurnResult result{ ReversiLogic::CheckAvailablePlaceCell(*gameState_, stoneType) };
        switch (result)
        {
        case TurnResult::Continue:
        {
            ChangeCurrentTurn(TurnPhase::StartThink);
            break;
        }
        case TurnResult::Skip:
        {
            ChangeCurrentTurn(TurnPhase::SkipTurn);
            break;
        }
        case TurnResult::GameEnd:
        {
            ChangeCurrentTurn(TurnPhase::FinishGame);
            break;
        }
        default:
        {
            assert(false && "CheckPlaceableAndSwitchTurn : The return value is an abnormal value.");
            ChangeCurrentTurn(TurnPhase::Error);
            break;
        }
        }
    }

    void GameMaster::PrepareBeforeThinking(TurnPhase turnPhase)
    {
        assert(gameState_ && turnMarkerDrawer_ && enemy_);

        const PlayerType playerType{ gameState_->GetCurrentPlayData().playerType_ };
        const bool isCpu = playerType == PlayerType::Cpu;
        switch (turnPhase)
        {
        case TurnPhase::StartThink:
        {
            constexpr int roundingDigitsZero{ 0 };
            const StoneType stoneType = gameState_->GetCurrentTurn();
            const Vec2 scoreBoardPos = Math::RoundForVec2(scoreBoardManager_->GetScoreBoardCenterPos(stoneType), roundingDigitsZero);
            turnMarkerDrawer_->SetMarkerPos(scoreBoardPos.asPoint());
            turnMarkerDrawer_->Visible();

            if (isCpu)
            {
                enemy_->StartThinking();
            }
            else
            {
                VisibleCursorAndIndicater();
            }
            break;
        }
        case TurnPhase::EndThink:
        {
            if (isCpu)
            {
                enemy_->ResetThinking();
            }
            else
            {
                HideCursorAndIndicater();
            }
            break;
        }
        default:
        {
            assert(false && "PrepareBeforeThinking : An incorrect argument was passed.");
            break;
        }
        }
    }

    bool GameMaster::FlipStone()
    {
        assert(gameState_);
        const StoneType stoneType{ gameState_->GetCurrentTurn() };
        return ReversiLogic::FlipOverStone(*gameState_, stoneType);
    }

    void GameMaster::ChangeCurrentTurn(TurnPhase turnPhase)
    {
        currentTurn_ = turnPhase;
    }

    void GameMaster::VisibleCursorAndIndicater()
    {
        assert(indicateManager_ && cursorManager_ && gameState_);
        CursorType cursorType{};
        const PlayerData currentPlayerData{ gameState_->GetCurrentPlayData() };
        switch (currentPlayerData.playerType_)
        {
        case PlayerType::Player1:
        {
            cursorType = CursorType::Player1;
            break;
        }
        case PlayerType::Player2:
        {
            cursorType = CursorType::Player2;
            break;
        }
        default:
        {
            return;
        }
        }

        indicateManager_->VisiblePlaceableCellIndicator();
        cursorManager_->VisibleCursor(cursorType);

        const BoardData& boardData = gameState_->GetBoardData();
        const Vec2 mousePos{ gameState_->GetMouseCursorPos() };
        const bool isOnBoard = ReversiLogic::IsMouseCursorOnTheBoard(boardData, mousePos);
        if (isOnBoard)
        {
            const Vec2 cursorPos = ReversiLogic::ConvertCoordPosToCenterCoord(boardData, mousePos);
            cursorManager_->SetCurrentCursorPos(cursorPos);
        }
    }

    void GameMaster::HideCursorAndIndicater()
    {
        assert(indicateManager_ && cursorManager_);
        indicateManager_->HidePlaceableCellIndicator();
        cursorManager_->HideAllCursor();
    }

    void GameMaster::PrepareBeforeResultLevelTrans()
    {
        assert(gameState_ && turnMarkerDrawer_);

        const int32 blackStoneCount{ gameState_->GetBlackStoneCount() };
        const int32 whiteStoneCount{ gameState_->GetWhiteStoneCount() };
        GameConfig& gameConfig = GameInfo::GetGameConfig();
        gameConfig.UpdateStoneCount(StoneType::Black, blackStoneCount);
        gameConfig.UpdateStoneCount(StoneType::White, whiteStoneCount);

        turnMarkerDrawer_->Hide();
    }
}
