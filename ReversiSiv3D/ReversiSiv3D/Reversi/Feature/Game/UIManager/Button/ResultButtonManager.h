#pragma once

#include "Feature/Game/UIManager/Button/NormalButtonManager.h"
#include "Feature/System/Interface/ResultButtonEventInterface.h"

namespace ReversiSiv3D {

    class ResultButtonManager : public NormalButtonManager, public Task
    {
    public:

        ResultButtonManager(ResultButtonEventInterface&);
        ~ResultButtonManager();

        void Initialize() override;
        void Update(double) override;
        void Render() override;

    private:

        static constexpr ColorF ButtonColor{ 0.0, 0.0, 0.0, 1.0 };
        static constexpr ColorF ButtonTextColor{ 1.0, 1.0, 1.0, 1.0 };
        static constexpr Point ButtonOffsetPos{ 200, 475 };
        static constexpr Point ReverseXAxis{ -1, 1 };

    private:

        ResultButtonEventInterface& eventInterface_;
    };
}
