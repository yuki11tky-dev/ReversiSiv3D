#include "Feature/Game/UIManager/DecideTurnOrderManager.h"
#include <cassert>

namespace ReversiSiv3D {

    DecideTurnOrderManager::DecideTurnOrderManager()
        : Task{ TaskId::TurnOrder }
        , decideTurnOrder_{}
        , turnOrderButtonManager_{}
    {

    }

    DecideTurnOrderManager::~DecideTurnOrderManager()
    {

    }

    void DecideTurnOrderManager::Update(double deltaTimeSec)
    {
        assert(decideTurnOrder_ && turnOrderButtonManager_);
        decideTurnOrder_->Update(deltaTimeSec);
        turnOrderButtonManager_->Update(deltaTimeSec);
    }

    void DecideTurnOrderManager::Render()
    {
        assert(decideTurnOrder_ && turnOrderButtonManager_);
        decideTurnOrder_->Render();
        turnOrderButtonManager_->Render();
    }

    void DecideTurnOrderManager::Initialize(BattleLevelEventInterface& interface)
    {
        decideTurnOrder_ = std::make_unique<DecideTurnOrder>();
        decideTurnOrder_->Initialize();

        turnOrderButtonManager_ = std::make_unique<TurnOrderButtonManager>(interface);
        turnOrderButtonManager_->Initialize();
    }

    void DecideTurnOrderManager::Visible()
    {
        assert(decideTurnOrder_ && turnOrderButtonManager_);
        decideTurnOrder_->Visible();
        turnOrderButtonManager_->VisibleAndEnabled();
    }

    void DecideTurnOrderManager::Hide()
    {
        assert(decideTurnOrder_ && turnOrderButtonManager_);
        decideTurnOrder_->Hide();
        turnOrderButtonManager_->HideAndDisable();
    }
}
