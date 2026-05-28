#pragma once

#include "Feature/Game/GameState.h"
#include "Feature/Game/UI/Board/StoneDrawer.h"
#include "Feature/System/Task.h"

namespace ReversiSiv3D {

    class StoneManager : public Task
    {
    public:

        StoneManager();
        ~StoneManager();

        void Update(double) override;
        void Render() override;

        void Initialize();
        void SetStateObserver(const std::shared_ptr<GameState>&);

    private:

        bool TryGetGameState(std::shared_ptr<GameState>&) const;

    private:

        std::unique_ptr<StoneDrawer> stoneDrawer_;
        std::weak_ptr<GameState> stateObserver_;
    };
}
