#include "Feature/Game/Level/BattleLevel.h"
#include "Feature/Game/ReversiLogic.h"
#include "Feature/System/Fade.h"
#include "Feature/System/SoundManager.h"
#include <cassert>

namespace ReversiSiv3D {

    BattleLevel::BattleLevel(const std::shared_ptr<TaskManager>& taskManager)
        : Level(TaskId::BattleLevel, taskManager)
        , currentLevelState_{ LevelState::Wait }
        , backScreen_{}
        , battleBoard_{}
        , mouseController_{}
        , cursorManager_{}
        , stoneManager_{}
        , gameMaster_{}
        , indicateManager_{}
        , gameState_{}
        , panelManager_{}
        , scoreBoardManager_{}
        , turnMarkerDrawer_{}
        , enemy_{}
        , decideTurnOrderManager_{}
    {

    }

    BattleLevel::~BattleLevel()
    {

    }

    void BattleLevel::Initialize()
    {
        gameState_ = std::make_shared<GameState>();
        gameState_->Initialize();

        mouseController_ = std::make_shared<MouseController>();
        AddTaskList(mouseController_);

        backScreen_ = std::make_shared<BackScreenDrawer>();
        AddTaskList(backScreen_);
        backScreen_->Initialize();

        battleBoard_ = std::make_shared<BattleBoard>();
        AddTaskList(battleBoard_);
        battleBoard_->Initialize(gameState_);

        cursorManager_ = std::make_shared<CursorManager>();
        AddTaskList(cursorManager_);
        cursorManager_->CreatePlayerCursor();
        cursorManager_->CreateEnemyCursor();
        cursorManager_->HideAllCursor();

        stoneManager_ = std::make_shared<StoneManager>();
        AddTaskList(stoneManager_);
        stoneManager_->SetStateObserver(gameState_);
        stoneManager_->Initialize();

        indicateManager_ = std::make_shared<IndicateManager>();
        AddTaskList(indicateManager_);
        indicateManager_->SetStateObserver(gameState_);
        indicateManager_->Initialize();

        panelManager_ = std::make_shared<PanelManager>();
        AddTaskList(panelManager_);
        panelManager_->CreatePanelList();

        scoreBoardManager_ = std::make_shared<ScoreBoardManager>();
        AddTaskList(scoreBoardManager_);

        scoreBoardManager_->SetStateObserver(gameState_);
        scoreBoardManager_->CreateBoard();

        turnMarkerDrawer_ = std::make_shared<TurnMarkerDrawer>();
        AddTaskList(turnMarkerDrawer_);
        turnMarkerDrawer_->CreateMarker();

        enemy_ = std::make_shared<Enemy>();
        AddTaskList(enemy_);
        enemy_->SetStateObserver(gameState_);

        gameMaster_ = std::make_shared<GameMaster>();
        AddTaskList(gameMaster_);
        gameMaster_->SetStateObserver(gameState_);
        gameMaster_->SetCursorManager(cursorManager_);
        gameMaster_->SetIndicateManager(indicateManager_);
        gameMaster_->SetPanelManager(panelManager_);
        gameMaster_->SetScoreBoardManager(scoreBoardManager_);
        gameMaster_->SetTurnMarkerDrawer(turnMarkerDrawer_);
        gameMaster_->SetEnemy(enemy_);

        decideTurnOrderManager_ = std::make_shared<DecideTurnOrderManager>();
        AddTaskList(decideTurnOrderManager_);
        decideTurnOrderManager_->Initialize(*this);
        decideTurnOrderManager_->Hide();

        Fade::StartFadeIn();

        ChangeLevelState(LevelState::Initialize);
    }

    void BattleLevel::Running(double)
    {
        switch (currentLevelState_)
        {
        case LevelState::Initialize:
        {
            const bool isFinish = Fade::IsFinished();
            if (!isFinish)
            {
                break;
            }
            decideTurnOrderManager_->Visible();
            ChangeLevelState(LevelState::SelectTurnOrder);
            break;
        }
        case LevelState::SelectTurnOrder:
        {

            break;
        }
        case LevelState::StartGame:
        {
            assert(gameMaster_);
            gameMaster_->StartGame();
            ChangeLevelState(LevelState::WaitStartGameAnim);
            break;
        }
        case LevelState::WaitStartGameAnim:
        {
            const bool isFinish = gameMaster_->IsFinishedStartGameAnim();
            if (!isFinish)
            {
                break;
            }
            SoundManager::PlayLoop(SoundType::BattleLevelBgm);
            ChangeLevelState(LevelState::Running);
            break;
        }
        case LevelState::Running:
        {
            assert(gameMaster_ && gameState_ && mouseController_ && cursorManager_);

            const Vec2 mousePos{ mouseController_->GetCurrentMouseCursorPos() };
            gameState_->SetMouseCursorPos(mousePos);

            const bool isThinking = gameMaster_->IsThinking();
            const bool isFinishedGame = gameMaster_->IsFinishedGame();
            if (isThinking)
            {
                // マウスカーソルの位置と左クリックされたかどうかを取得する
                const bool isPressedLeftButton = mouseController_->IsPressLeftButton();
                gameState_->SetPressLeftButton(isPressedLeftButton);

                // リバーシの盤上のカーソル位置は全てのカーソルに対して毎フレーム更新し続ける
                // 表示するカーソルの選択をGameMasterクラスのプレイヤー切り替え時に実施する
                const BoardData& boardData = gameState_->GetBoardData();
                const bool isOnBoard = ReversiLogic::IsMouseCursorOnTheBoard(boardData, mousePos);
                if (isOnBoard)
                {
                    const Vec2 cursorPos = ReversiLogic::ConvertCoordPosToCenterCoord(boardData, mousePos);
                    cursorManager_->SetCurrentCursorPos(cursorPos);
                }
            }
            else if (isFinishedGame)
            {
                ChangeLevelState(LevelState::SwitchResult);
            }

            break;
        }
        case LevelState::SwitchResult:
        {
            Fade::StartFadeOut();

            SoundManager::FadeVolume(SoundType::BattleLevelBgm, MuteAudioValue, SecondsF{ Fade::ExecTime });

            SetNextLevel(LevelType::ResultLevel);
            ChangeLevelState(LevelState::Release);
            break;
        }
        case LevelState::Release:
        {
            const bool isFinish = Fade::IsFinished();
            if (isFinish)
            {
                RequestFinish();

                ChangeLevelState(LevelState::Released);
            }
            break;
        }
        case LevelState::Released:
        {

            break;
        }
        }
    }

    void BattleLevel::OnDecideFirstPlayer()
    {
        assert(gameMaster_ && decideTurnOrderManager_);
        gameMaster_->InitialSetup(PlayerType::Player1, PlayerType::Cpu);

        decideTurnOrderManager_->Hide();
        TaskId taskId = decideTurnOrderManager_->GetTaskId();
        ReleaseTask(taskId);
        ChangeLevelState(LevelState::StartGame);
    }

    void BattleLevel::OnDecideSecondPlayer()
    {
        assert(gameMaster_ && decideTurnOrderManager_);
        gameMaster_->InitialSetup(PlayerType::Cpu, PlayerType::Player1);

        decideTurnOrderManager_->Hide();
        TaskId taskId = decideTurnOrderManager_->GetTaskId();
        ReleaseTask(taskId);
        ChangeLevelState(LevelState::StartGame);
    }

    void BattleLevel::Finish()
    {
        SoundManager::Clear();

        if (gameState_)
        {
            gameState_->ReleaseData();
        }
    }

    void BattleLevel::ChangeLevelState(LevelState state)
    {
        currentLevelState_ = state;
    }
}
