#include "Feature/Game/UIManager/IndicateManager.h"
#include <cassert>

namespace ReversiSiv3D {

    IndicateManager::IndicateManager()
        : Task{ TaskId::Indicator }
        , placeableCellIndicator_ {}
        , stateObserver_{}
    {

    }

    IndicateManager::~IndicateManager()
    {

    }

    void IndicateManager::Render()
    {
        assert(placeableCellIndicator_);
        std::shared_ptr<GameState> gameState;
        const bool isSuccess = TryGetGameState(gameState);
        if (isSuccess)
        {
            const std::vector<Vec2> placeableCellList{ gameState->GetPlaceableCellPosList() };
            for (const Vec2& placeableCell : placeableCellList)
            {
                placeableCellIndicator_->SetCenterPos(placeableCell);
                placeableCellIndicator_->Render();
            }
        }
    }

    void IndicateManager::Initialize()
    {
        placeableCellIndicator_ = std::make_unique<PlaceableCellIndicator>();
        placeableCellIndicator_->Initialize(Radius, Red);
        placeableCellIndicator_->Hide();
    }

    void IndicateManager::VisiblePlaceableCellIndicator()
    {
        assert(placeableCellIndicator_);
        placeableCellIndicator_->Visible(Red.a);
    }

    void IndicateManager::HidePlaceableCellIndicator()
    {
        assert(placeableCellIndicator_);
        placeableCellIndicator_->Hide();
    }

    void IndicateManager::SetStateObserver(const std::shared_ptr<GameState>& observer)
    {
        stateObserver_ = observer;
    }

    bool IndicateManager::TryGetGameState(std::shared_ptr<GameState>& outGameState) const
    {
        outGameState = stateObserver_.lock();
        return static_cast<bool>(outGameState);
    }
}
