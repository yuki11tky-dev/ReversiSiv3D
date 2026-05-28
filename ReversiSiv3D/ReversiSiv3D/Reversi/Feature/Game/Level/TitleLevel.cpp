#include "Feature/Game/Level/TitleLevel.h"
#include "Feature/System/Fade.h"
#include "Feature/System/SoundManager.h"

namespace ReversiSiv3D {

    TitleLevel::TitleLevel(const std::shared_ptr<TaskManager>& taskManager)
        : Level(TaskId::TitleLevel, taskManager)
        , currentLevelState_{ LevelState::Wait }
        , backScreen_{}
        , titleDesigner_{}
        , buttonManager_{}
    {

    }

    TitleLevel::~TitleLevel()
    {

    }

    void TitleLevel::Initialize()
    {
        backScreen_ = std::make_shared<BackScreenDrawer>();
        AddTaskList(backScreen_);
        backScreen_->Initialize();

        titleDesigner_ = std::make_shared<TitleDesignDrawer>();
        AddTaskList(titleDesigner_);
        titleDesigner_->Initialize();

        buttonManager_ = std::make_shared<TitleButtonManager>(*this);
        AddTaskList(buttonManager_);
        buttonManager_->Initialize();

        Fade::StartFadeIn();

        SoundManager::PlayLoop(SoundType::TitleLevelBgm);

        ChangeLevelState(LevelState::Initialize);
    }

    void TitleLevel::Running(double)
    {
        switch (currentLevelState_)
        {
        case LevelState::Initialize:
        {
            const bool isFinish = Fade::IsFinished();
            if (isFinish)
            {
                ChangeLevelState(LevelState::Running);
            }
            break;
        }
        case LevelState::Running:
        {
            break;
        }
        case LevelState::GameStart:
        {
            SoundManager::Pause(SoundType::TitleLevelBgm);
            Fade::StartFadeOut();

            SetNextLevel(LevelType::BattleLevel);
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

    void TitleLevel::Finish()
    {
        SoundManager::Clear();
    }

    void TitleLevel::ChangeLevelState(LevelState state)
    {
        currentLevelState_ = state;
    }

    void TitleLevel::OnStartGame()
    {
        ChangeLevelState(LevelState::GameStart);
    }

    void TitleLevel::OnEndGame()
    {
        RequestGameEnd();
    }
}
