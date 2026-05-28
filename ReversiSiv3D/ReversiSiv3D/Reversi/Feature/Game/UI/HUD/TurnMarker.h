#pragma once

#include "Feature/Game/UI/HUD/HUDBase.h"
#include "Feature/Game/UI/Rectangle.h"

namespace ReversiSiv3D {

    class TurnMarker : public HUDBase
    {
    public:

        TurnMarker();
        ~TurnMarker();

        void Initialize();
        void SetCenterPos(Point);
        void SetSize(Size);
        void SetImageColor(ColorF);

        void SetAlphaValue(double);
        double GetAlphaValue() const;

    private:

        static constexpr Size MarkerSize{ 400, 400 };
        static constexpr ColorF Red{ 1.0, 0.0, 0.0, 1.0 };
        static constexpr double FullHDWindowScale{ 1.0 };

    private:

        std::shared_ptr<Rectangle> rectTurnMarker_;
    };
}
