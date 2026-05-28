#pragma once

#include "Feature/Game/UIManager/BackScreenDrawer.h"
#include "Feature/Game/UIManager/Button/TitleButtonManager.h"
#include "Feature/Game/UIManager/TitleDesignDrawer.h"
#include "Feature/System/Interface/TitleLevelEventInterface.h"
#include "Feature/System/Level.h"
#include <memory>

namespace ReversiSiv3D {

    class TitleLevel : public Level, public TitleLevelEventInterface
    {
    public:

        TitleLevel(const std::shared_ptr<TaskManager>&);
        ~TitleLevel();

    public:

        // TitleLevelEventInterface経由で
        // TitleButtonManagerから実行される関数
        void OnStartGame();
        void OnEndGame();

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
            GameStart,
            Release,
            Released,
        };

    private:

        void ChangeLevelState(LevelState);

    private:

        LevelState currentLevelState_;
        std::shared_ptr<BackScreenDrawer> backScreen_;
        std::shared_ptr<TitleDesignDrawer> titleDesigner_;
        std::shared_ptr<TitleButtonManager> buttonManager_;
    };
}
