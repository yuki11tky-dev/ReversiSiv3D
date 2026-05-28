#pragma once

#include "Feature/System/Level.h"
#include "Feature/System/Task.h"
#include "Feature/System/TaskManager.h"
#include <memory>

namespace ReversiSiv3D {

    class LevelChanger : public Task
    {
    public:

        LevelChanger(const std::shared_ptr<TaskManager>&);
        ~LevelChanger();

        void Update(double) override;
        void SetFinishRequest();
        bool IsReleaseCompleted() const;

    private:

        enum class LevelChangerState
        {
            Wait,
            Initialize,
            Running,
            Release,
            Released,
        };

        enum class LevelSwitchPhaseType
        {
            Wait,
            CurrentLevelRelease,
            NextLevelCreate,
            NextLevelInitialize,
            NextLevelInitialized,
            Release,
            Released,
        };

    private:

        void Initialize();
        bool MonitorLevelChanging();
        bool ReleaseLevel();

        void ChangeCurrentState(LevelChangerState);
        void ChangeCurrentLevelSwitchPhase(LevelSwitchPhaseType);
        std::shared_ptr<Level> CreateLevel(LevelType);
        bool IsReceivedFinishRequest() const;

    private:

        std::weak_ptr<TaskManager> taskManager_;
        LevelChangerState currentState_;
        LevelSwitchPhaseType currentSwitchLevelPhase_;
        std::shared_ptr<Level> currentLevel_;

        bool isReceivedFinishRequest_;
    };
}
