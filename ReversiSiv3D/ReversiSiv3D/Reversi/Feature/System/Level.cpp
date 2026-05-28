#include "Feature/System/Level.h"
#include <cassert>

namespace ReversiSiv3D {

    Level::Level(TaskId taskId, const std::shared_ptr<TaskManager>& taskManager)
        : Task{ taskId }
        , taskManager_{ taskManager }
        , currentPhase_{}
        , nextLevelId_{}
        , addTaskIdList_{}
    {

    }

    Level::~Level()
    {

    }

    void Level::Update(double deltaTimeSec)
    {
        switch (currentPhase_)
        {
        case PhaseType::Initialize:
        {
            Initialize();

            SwitchCurrentPhase(PhaseType::Initialized);
            break;
        }
        case PhaseType::Running:
        case PhaseType::RequestFinish:
        case PhaseType::GameEnd:
        {
            Running(deltaTimeSec);
            break;
        }
        case PhaseType::Finish:
        {
            Finish();
            ReleaseAllTask();

            SwitchCurrentPhase(PhaseType::Finished);
            break;
        }
        default:
        {
            break;
        }
        }
    }

    void Level::StartInitialize()
    {
        SwitchCurrentPhase(PhaseType::Initialize);
    }

    void Level::StartRunning()
    {
        SwitchCurrentPhase(PhaseType::Running);
    }

    void Level::SetFinish()
    {
        SwitchCurrentPhase(PhaseType::Finish);
    }

    void Level::RequestFinish()
    {
        SwitchCurrentPhase(PhaseType::RequestFinish);
    }

    void Level::RequestGameEnd()
    {
        SwitchCurrentPhase(PhaseType::GameEnd);
    }

    void Level::SwitchCurrentPhase(PhaseType phaseType)
    {
        currentPhase_ = phaseType;
    }

    void Level::SetNextLevel(LevelType levelType)
    {
        nextLevelId_ = levelType;
    }

    LevelType Level::GetNextLevel() const
    {
        return nextLevelId_;
    }

    void Level::AddTaskList(const std::shared_ptr<Task>& task)
    {
        assert(task);
        std::shared_ptr<TaskManager> taskManager = taskManager_.lock();
        if (!taskManager)
        {
            return;
        }
        taskManager->AddTask(task);

        const TaskId taskId = task->GetTaskId();
        assert(taskId != TaskId::Empty);
        addTaskIdList_.push_back(taskId);
    }

    void Level::ReleaseTask(TaskId taskId)
    {
        std::shared_ptr<TaskManager> taskManager = taskManager_.lock();
        if (!taskManager)
        {
            return;
        }
        taskManager->ReleaseTask(taskId);
        std::erase(addTaskIdList_, taskId);
    }

    void Level::ReleaseAllTask()
    {
        std::shared_ptr<TaskManager> taskManager = taskManager_.lock();
        if (!taskManager)
        {
            return;
        }
        for (const TaskId taskId : addTaskIdList_)
        {
            taskManager->ReleaseTask(taskId);
        }
    }

    bool Level::IsInitialized() const
    {
        return currentPhase_ == PhaseType::Initialized;
    }
    bool Level::IsRequestFinish() const
    {
        return currentPhase_ == PhaseType::RequestFinish;
    }
    bool Level::IsGameEnd() const
    {
        return currentPhase_ == PhaseType::GameEnd;
    }
    bool Level::IsFinished() const
    {
        return currentPhase_ == PhaseType::Finished;
    }
}
