#include "Feature/Game/GameInfo.h"
#include "Feature/Game/Level/BattleLevel.h"
#include "Feature/Game/Level/BootLevel.h"
#include "Feature/Game/Level/ResultLevel.h"
#include "Feature/Game/Level/TitleLevel.h"
#include "Feature/System/LevelChanger.h"

namespace ReversiSiv3D {

    LevelChanger::LevelChanger(const std::shared_ptr<TaskManager>& taskManager)
        : Task{ TaskId::LevelChanger }
        , taskManager_{ taskManager }
        , currentState_{ LevelChangerState::Initialize }
        , currentSwitchLevelPhase_{ LevelSwitchPhaseType::Wait }
        , currentLevel_{}
        , isReceivedFinishRequest_{}
    {

    }

    LevelChanger::~LevelChanger()
    {

    }

    void LevelChanger::Update(double)
    {
        switch (currentState_)
        {
        case LevelChangerState::Initialize:
        {
            Initialize();

            ChangeCurrentState(LevelChangerState::Running);
            break;
        }
        case LevelChangerState::Running:
        {
            const bool isReleased = MonitorLevelChanging();
            if (isReleased)
            {
                ChangeCurrentState(LevelChangerState::Release);
            }
            break;
        }
        case LevelChangerState::Release:
        {
            ChangeCurrentState(LevelChangerState::Released);
            break;
        }
        case LevelChangerState::Released:
        {
            break;
        }
        }
    }

    void LevelChanger::SetFinishRequest()
    {
        isReceivedFinishRequest_ = true;
    }

    bool LevelChanger::IsReleaseCompleted() const
    {
        return currentState_ == LevelChangerState::Released;
    }

    void LevelChanger::Initialize()
    {

#ifdef _DEBUG
        GameConfig gameConfig = GameInfo::GetGameConfig();
        LevelType levelType = gameConfig.GetStartupLevel();
#else
        LevelType levelType = LevelType::BootLevel;
#endif

        currentLevel_ = CreateLevel(levelType);
        ChangeCurrentLevelSwitchPhase(LevelSwitchPhaseType::NextLevelInitialize);
    }

    // レベルの切り替え監視処理
    // レベル側からの切り替え処理およびウィンドウ終了時の全体終了処理を実施
    bool LevelChanger::MonitorLevelChanging()
    {
        assert(currentLevel_);

        switch (currentSwitchLevelPhase_)
        {
        case LevelSwitchPhaseType::Wait:
        {
            // ウィンドウの閉じるボタン押下した時、
            // つまりウィンドウ終了時にリリースフェーズに移行する
            if (IsReceivedFinishRequest())
            {
                currentLevel_->SetFinish();
                ChangeCurrentLevelSwitchPhase(LevelSwitchPhaseType::Release);
            }
            else
            {
                if (currentLevel_->IsGameEnd())
                {
                    currentLevel_->SetFinish();
                    ChangeCurrentLevelSwitchPhase(LevelSwitchPhaseType::Release);
                }
                else if (currentLevel_->IsRequestFinish())
                {
                    currentLevel_->SetFinish();
                    ChangeCurrentLevelSwitchPhase(LevelSwitchPhaseType::CurrentLevelRelease);
                }
            }
            break;
        }
        case LevelSwitchPhaseType::CurrentLevelRelease:
        {
            const bool isReleased = ReleaseLevel();
            if (!isReleased)
            {
                break;
            }
            ChangeCurrentLevelSwitchPhase(LevelSwitchPhaseType::NextLevelCreate);
            break;
        }
        case LevelSwitchPhaseType::NextLevelCreate:
        {
            const LevelType levelType = currentLevel_->GetNextLevel();
            currentLevel_ = CreateLevel(levelType);
            ChangeCurrentLevelSwitchPhase(LevelSwitchPhaseType::NextLevelInitialize);
            break;
        }
        case LevelSwitchPhaseType::NextLevelInitialize:
        {
            std::shared_ptr<TaskManager> taskManager = taskManager_.lock();
            if (taskManager)
            {
                taskManager->AddTask(currentLevel_);
            }
            currentLevel_->StartInitialize();
            ChangeCurrentLevelSwitchPhase(LevelSwitchPhaseType::NextLevelInitialized);
            break;
        }
        case LevelSwitchPhaseType::NextLevelInitialized:
        {
            if (currentLevel_->IsInitialized())
            {
                currentLevel_->StartRunning();
                ChangeCurrentLevelSwitchPhase(LevelSwitchPhaseType::Wait);
            }
            break;
        }
        case LevelSwitchPhaseType::Release:
        {
            const bool isReleased = ReleaseLevel();
            if (!isReleased)
            {
                break;
            }
            ChangeCurrentLevelSwitchPhase(LevelSwitchPhaseType::Released);
            break;
        }
        case LevelSwitchPhaseType::Released:
        {
            return true;
        }
        }

        return false;
    }

    bool LevelChanger::ReleaseLevel()
    {
        if (!currentLevel_->IsFinished())
        {
            return false;
        }
        const TaskId taskId{ currentLevel_->GetTaskId() };
        std::shared_ptr<TaskManager> taskManager = taskManager_.lock();
        if (taskManager)
        {
            taskManager->ReleaseTask(taskId);
        }
        return true;
    }

    void LevelChanger::ChangeCurrentState(LevelChangerState state)
    {
        currentState_ = state;
    }

    void LevelChanger::ChangeCurrentLevelSwitchPhase(LevelSwitchPhaseType phaseType)
    {
        currentSwitchLevelPhase_ = phaseType;
    }

    std::shared_ptr<Level> LevelChanger::CreateLevel(LevelType levelType)
    {
        std::shared_ptr<TaskManager> taskManager = taskManager_.lock();
        assert(taskManager);
        switch (levelType)
        {
        case LevelType::BootLevel:
        {
            return std::make_shared<BootLevel>(taskManager);
        }
        case LevelType::TitleLevel:
        {
            return std::make_shared<TitleLevel>(taskManager);
        }
        case LevelType::BattleLevel:
        {
            return std::make_shared<BattleLevel>(taskManager);
        }
        case LevelType::ResultLevel:
        {
            return std::make_shared<ResultLevel>(taskManager);
        }
        default:
        {
            assert(false && "The Level-Type isn't exists.");
            return nullptr;
        }
        }
    }

    bool LevelChanger::IsReceivedFinishRequest() const
    {
        return isReceivedFinishRequest_;
    }
}
