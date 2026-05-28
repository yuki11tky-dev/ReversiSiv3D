#include "Feature/Common/AssetManager.h"
#include "Feature/Common/Math.h"
#include "Feature/Game/ReversiLogic.h"
#include "Feature/Game/UIManager/StoneManager.h"
#include <cassert>

namespace ReversiSiv3D {

    StoneManager::StoneManager()
        : Task{ TaskId::Stone }
        , stoneDrawer_{}
        , stateObserver_{}
    {

    }

    StoneManager::~StoneManager()
    {

    }

    void StoneManager::Update(double deltaTimeSec)
    {
        std::shared_ptr<GameState> gameState;
        const bool isSuccess = TryGetGameState(gameState);
        if (!isSuccess)
        {
            return;
        }

        const std::vector<PlacedStoneInfo>& placedStoneList = gameState->GetPlacedStoneList();
        for (const PlacedStoneInfo& stoneInfo : placedStoneList)
        {
            const BoardCell& cell = stoneInfo.placedCell_;
            std::optional<Stone>& stone = gameState->GetStone(cell);
            if (stone.has_value())
            {
                stone->Update(deltaTimeSec);
            }
        }
    }

    void StoneManager::Render()
    {
        assert(stoneDrawer_);
        std::shared_ptr<GameState> gameState;
        const bool isSuccess = TryGetGameState(gameState);
        if (!isSuccess)
        {
            return;
        }

        const std::vector<PlacedStoneInfo>& placedStoneList = gameState->GetPlacedStoneList();
        for (const PlacedStoneInfo& stoneInfo : placedStoneList)
        {
            const BoardCell& cell = stoneInfo.placedCell_;
            std::optional<Stone>& stone = gameState->GetStone(cell);
            if (stone.has_value())
            {
                const RenderStoneInfo renderStoneInfo = stone->GetRenderInfo();
                if (renderStoneInfo.isHide_)
                {
                    continue;
                }
                stoneDrawer_->SetCenterPos(renderStoneInfo.placeCenterPos_);
                stoneDrawer_->SetDrawIndex(renderStoneInfo.drawIndex_);
                stoneDrawer_->Render();
            }
        }
    }

    void StoneManager::Initialize()
    {
        stoneDrawer_ = std::make_unique<StoneDrawer>();
        stoneDrawer_->Initialize();
    }

    void StoneManager::SetStateObserver(const std::shared_ptr<GameState>& observer)
    {
        stateObserver_ = observer;
    }

    bool StoneManager::TryGetGameState(std::shared_ptr<GameState>& outGameState) const
    {
        outGameState = stateObserver_.lock();
        return static_cast<bool>(outGameState);
    }
}
