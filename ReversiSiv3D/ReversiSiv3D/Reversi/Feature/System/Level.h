#pragma once

#include "Feature/System/Task.h"
#include "Feature/System/TaskManager.h"
#include <memory>
#include <vector>

namespace ReversiSiv3D {

    class Level : public Task
    {
    protected:

        enum class PhaseType
        {
            None,
            Initialize,
            Initialized,
            Running,
            RequestFinish,
            GameEnd,
            Finish,
            Finished,
        };

    public:

        Level(TaskId, const std::shared_ptr<TaskManager>&);
        virtual ~Level();

        void Update(double) override;

        void StartInitialize();
        void StartRunning();
        void SetFinish();

        void SetNextLevel(LevelType);
        LevelType GetNextLevel() const;
        void AddTaskList(const std::shared_ptr<Task>&);
        void ReleaseTask(TaskId);

        bool IsInitialized() const;
        bool IsRequestFinish() const;
        bool IsGameEnd() const;
        bool IsFinished() const;

    protected:

        virtual void Initialize() = 0;
        virtual void Running(double) = 0;
        virtual void Finish() = 0;

        void RequestFinish();
        void RequestGameEnd();

    private:

        void ReleaseAllTask();
        void SwitchCurrentPhase(PhaseType);

    private:

        std::weak_ptr<TaskManager> taskManager_;
        PhaseType currentPhase_;
        LevelType nextLevelId_;
        std::vector<TaskId> addTaskIdList_;
    };
}
