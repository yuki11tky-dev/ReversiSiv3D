#pragma once

#include "Feature/Game/GameState.h"
#include "Feature/Game/UI/Indicator/PlaceableCellIndicator.h"
#include "Feature/System/Task.h"
#include <memory>

namespace ReversiSiv3D {

    class IndicateManager : public Task
    {
    public:

        IndicateManager();
        ~IndicateManager();

        void Render() override;

        void Initialize();
        void VisiblePlaceableCellIndicator();
        void HidePlaceableCellIndicator();

        void SetStateObserver(const std::shared_ptr<GameState>&);

    private:

        bool TryGetGameState(std::shared_ptr<GameState>&) const;

    private:

        static constexpr int Radius{ 10 };
        static constexpr ColorF Red{ 1.0, 0.0, 0.0, 0.4 };

    private:

        std::unique_ptr<PlaceableCellIndicator> placeableCellIndicator_;
        std::weak_ptr<GameState> stateObserver_;
    };
}
