#pragma once

#include "Feature/Game/UIManager/BackScreenDrawer.h"
#include "Feature/Game/UIManager/BootEventManager.h"
#include "Feature/System/Level.h"
#include  <memory>

namespace ReversiSiv3D {

    class BootLevel : public Level
    {
    public:

        BootLevel(const std::shared_ptr<TaskManager>&);
        ~BootLevel();

    protected:

        void Initialize() override;
        void Running(double) override;
        void Finish() override;

    private:

        enum class LevelState
        {
            Wait,
            Initialize,
            Running,
            Release,
            Released,
        };

    private:

        void ChangeLevelState(LevelState);

    private:

        LevelState currentLevelState_;
        std::shared_ptr<BackScreenDrawer> backScreen_;
        std::shared_ptr<BootEventManager> bootEventManager_;
    };
}
