#pragma once

#include "Feature/Game/UIManager/Button/NormalButtonManager.h"
#include "Feature/System/Interface/TitleButtonEventInterface.h"

namespace ReversiSiv3D {

    class TitleButtonManager : public NormalButtonManager, public Task
    {
    public:

        TitleButtonManager(TitleButtonEventInterface&);
        ~TitleButtonManager();

        void Initialize() override;
        void Update(double) override;
        void Render() override;

    private:

        static constexpr Vec2 ButtonFirstAdjustPos{ 0.0, 300.0 };
        static constexpr Vec2 ButtonPosInterval{ 0.0, 100.0 };
        static constexpr ColorF ButtonColor{ 0.0, 0.0, 0.0, 1.0 };
        static constexpr ColorF ButtonTextColor{ 1.0, 1.0, 1.0, 1.0 };

    private:

        TitleButtonEventInterface& eventInterface_;
    };
}
