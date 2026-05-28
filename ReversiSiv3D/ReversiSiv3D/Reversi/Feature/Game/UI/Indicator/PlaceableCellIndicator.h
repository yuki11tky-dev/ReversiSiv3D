#pragma once

#include "Feature/Game/UI/CircleBase.h"
#include "Feature/Game/UI/UIBase.h"

namespace ReversiSiv3D {

    class PlaceableCellIndicator : public UIBase
    {
    public:

        PlaceableCellIndicator();
        ~PlaceableCellIndicator();

        void Initialize(int32, ColorF color = DefaultColor);
        void SetCenterPos(Vec2);
        void SetColor(ColorF);
        void Visible(double alphaValue = AlphaMaxValue);
        void Hide();

    private:

        static constexpr double AlphaMaxValue{ 1.0 };
        static constexpr ColorF DefaultColor{ 0.0, 0.0, 0.0, 0.0 };

    private:

        std::weak_ptr<CircleBase> indicator_;
    };
}
