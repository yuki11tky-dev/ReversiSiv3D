#pragma once

#include "Feature/Game/UIManager/Button/NormalButtonManager.h"
#include "Feature/System/Interface/BattleButtonEventInterface.h"

namespace ReversiSiv3D {

    class TurnOrderButtonManager : public NormalButtonManager
    {
    public:

        TurnOrderButtonManager(BattleButtonEventInterface&);
        ~TurnOrderButtonManager();

        void Initialize() override;
        void Update(double);
        void Render();

    private:

        static constexpr Vec2 ButtonFirstAdjustPos{ 0.0, 0.0 };
        static constexpr Vec2 ButtonPosInterval{ 0.0, 100.0 };
        static constexpr ColorF ButtonColor{ 0.0, 0.0, 1.0, 1.0 };
        static constexpr ColorF ButtonTextColor{ 1.0, 1.0, 1.0, 1.0 };
        static constexpr int32 ButtonTextSize{ 30 };

    private:

        BattleButtonEventInterface& eventInterface_;
    };
}
