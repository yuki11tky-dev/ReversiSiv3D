#include "Feature/Game/Level/ResultLevel.h"
#include "Feature/System/Fade.h"
#include "Feature/System/SoundManager.h"
#include <cassert>

namespace ReversiSiv3D {

    ResultLevel::ResultLevel(const std::shared_ptr<TaskManager>& taskManager)
        : Level(TaskId::ResultLevel, taskManager)
        , currentLevelState_{ LevelState::Wait }
        , gameState_{}
        , backScreen_{}
        , battleBoard_{}
        , stoneManager_{}
        , stoneAnimator_{}
        , resultDetailManager_{}
        , gameResultManager_{}
        , resultButtonManager_{}
    {

    }

    ResultLevel::~ResultLevel()
    {

    }

    void ResultLevel::Initialize()
    {
        gameState_ = std::make_shared<GameState>();
        gameState_->Initialize();

#ifndef NDEBUG

        // デバッグ時にリザルト画面から実行した場合に
        // 対戦結果が存在しないためここで格納している

        GameConfig& gameConfig = GameInfo::GetGameConfig();

        constexpr int blackStoneCount = 64;
        if (!gameConfig.IsExistsPlayerData(StoneType::Black))
        {
            PlayerData playerData{};
            playerData.playerType_ = PlayerType::Cpu;
            playerData.stoneCount_ = blackStoneCount;
            playerData.resultLayoutSide_ = ResultLayoutSide::Right;
            gameConfig.AddPlayData(StoneType::Black, playerData);
        }

        if (!gameConfig.IsExistsPlayerData(StoneType::White))
        {
            PlayerData playerData{};
            playerData.playerType_ = PlayerType::Player1;
            playerData.stoneCount_ = 64 - blackStoneCount;
            playerData.resultLayoutSide_ = ResultLayoutSide::Left;
            gameConfig.AddPlayData(StoneType::White, playerData);
        }

#endif

        backScreen_ = std::make_shared<BackScreenDrawer>();
        AddTaskList(backScreen_);
        backScreen_->Initialize();

        battleBoard_ = std::make_shared<BattleBoard>();
        AddTaskList(battleBoard_);
        battleBoard_->Initialize(gameState_);

        stoneManager_ = std::make_shared<StoneManager>();
        AddTaskList(stoneManager_);
        stoneManager_->SetStateObserver(gameState_);
        stoneManager_->Initialize();

        stoneAnimator_ = std::make_shared<ResultStoneAnimator>();
        AddTaskList(stoneAnimator_);
        stoneAnimator_->SetStateObserver(gameState_);

        resultDetailManager_ = std::make_shared<ResultDetailManager>();
        AddTaskList(resultDetailManager_);
        resultDetailManager_->Initialize();

        gameResultManager_ = std::make_shared<GameResultManager>();
        AddTaskList(gameResultManager_);
        gameResultManager_->SetStateObserver(gameState_);
        gameResultManager_->SetStoneAnimator(stoneAnimator_);
        gameResultManager_->SetResultDetailManager(resultDetailManager_);

        resultButtonManager_ = std::make_shared<ResultButtonManager>(*this);
        AddTaskList(resultButtonManager_);
        resultButtonManager_->Initialize();

        Fade::StartFadeIn();
        ChangeLevelState(LevelState::Initialize);
    }

    void ResultLevel::Running(double)
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
            assert(gameResultManager_);
            gameResultManager_->Start();
            ChangeLevelState(LevelState::Running);
            break;
        }
        case LevelState::Running:
        {
            assert(gameResultManager_ && resultButtonManager_);
            const bool isFinished = gameResultManager_->IsFinished();
            if (!isFinished)
            {
                break;
            }

            constexpr Duration startTime{ 2s };
            constexpr Duration cycleStartTime{ 26s };
            constexpr Duration cycleEndTime{ 147.6s };
            SoundManager::PlayLoop(SoundType::ResultLevelBgm, startTime, cycleStartTime, cycleEndTime);

            resultButtonManager_->VisibleAndEnabled();

            ChangeLevelState(LevelState::WaitOperation);
            break;
        }
        case LevelState::WaitOperation:
        {
            break;
        }
        case LevelState::TransferBattleLevel:
        {
            SetNextLevel(LevelType::BattleLevel);
            ChangeLevelState(LevelState::BeforeRelease);
            break;
        }
        case LevelState::TransferTitleLevel:
        {
            SetNextLevel(LevelType::TitleLevel);
            ChangeLevelState(LevelState::BeforeRelease);
            break;
        }
        case LevelState::BeforeRelease:
        {
            Fade::StartFadeOut();

            SoundManager::FadeVolume(SoundType::ResultLevelBgm, MuteAudioValue, SecondsF{ Fade::ExecTime });

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

    void ResultLevel::Finish()
    {
        SoundManager::Clear();
    }

    void ResultLevel::OnStartGame()
    {
        ChangeLevelState(LevelState::TransferBattleLevel);
    }

    void ResultLevel::OnReturnTitle()
    {
        ChangeLevelState(LevelState::TransferTitleLevel);
    }

    void ResultLevel::ChangeLevelState(LevelState state)
    {
        currentLevelState_ = state;
    }
}
