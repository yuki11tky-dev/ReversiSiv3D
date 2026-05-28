#include "Feature/Game/Level/BootLevel.h"
#include "Feature/System/Fade.h"

namespace ReversiSiv3D {

        BootLevel::BootLevel(const std::shared_ptr<TaskManager>& taskManager)
            : Level(TaskId::BootLevel, taskManager)
            , currentLevelState_{ LevelState::Wait }
            , backScreen_{}
            , bootEventManager_{}
        {

        }

        BootLevel::~BootLevel()
        {

        }

        void BootLevel::Initialize()
        {
            backScreen_ = std::make_shared<BackScreenDrawer>();
            AddTaskList(backScreen_);
            backScreen_->Initialize();

            bootEventManager_ = std::make_shared<BootEventManager>();
            AddTaskList(bootEventManager_);
            bootEventManager_->Initialize();

            Fade::StartFadeIn();

            ChangeLevelState(LevelState::Initialize);
        }

        void BootLevel::Running(double)
        {
            switch (currentLevelState_)
            {
            case LevelState::Initialize:
            {
                const bool isFinish = Fade::IsFinished();
                if (isFinish)
                {
                    if (bootEventManager_)
                    {
                        bootEventManager_->StartEvent();
                    }
                    ChangeLevelState(LevelState::Running);
                }
                break;
            }
            case LevelState::Running:
            {
                if (bootEventManager_)
                {
                    const bool isFinish = bootEventManager_->IsFinish();
                    if (!isFinish)
                    {
                        break;
                    }
                }

                Fade::StartFadeOut();

                SetNextLevel(LevelType::TitleLevel);
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

        void BootLevel::Finish()
        {

        }

        void BootLevel::ChangeLevelState(LevelState state)
        {
            currentLevelState_ = state;
        }
}
