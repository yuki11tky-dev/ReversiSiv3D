#pragma once

#include "Feature/Game/UI/Battle/DecideTurnOrder.h"
#include "Feature/Game/UIManager/Button/TurnOrderButtonManager.h"
#include "Feature/System/Interface/BattleLevelEventInterface.h"
#include "Feature/System/Task.h"
#include <memory>

namespace ReversiSiv3D {

    class DecideTurnOrderManager : public Task
    {
    public:

        DecideTurnOrderManager();
        ~DecideTurnOrderManager();

        void Update(double) override;
        void Render() override;

        void Visible();
        void Hide();

        void Initialize(BattleLevelEventInterface&);

    private:

        std::unique_ptr<DecideTurnOrder> decideTurnOrder_;
        std::unique_ptr<TurnOrderButtonManager> turnOrderButtonManager_;
    };
}
