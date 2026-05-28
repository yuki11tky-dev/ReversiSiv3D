#pragma once

#include "Feature/Game/UI/Rectangle.h"
#include "Feature/Game/UI/Text.h"
#include "Feature/Game/UI/UIBase.h"
#include <memory>

namespace ReversiSiv3D {

    class DecideTurnOrder : public UIBase
    {
    public:

        DecideTurnOrder();
        ~DecideTurnOrder();

        void Initialize();
        void Visible();
        void Hide();

    private:

        static constexpr Size BackBoardSize{ 400, 400 };
        static constexpr ColorF BackBoardColor{ 0.0, 0.0, 1.0, 0.6 };
        static constexpr ColorF ShadowColor{ 0.0, 0.0, 0.0, 0.3 };
        static constexpr Point ShadowOffset{ 5, 5 };

        static constexpr int32 GuideTextSize{ 40 };
        static constexpr ColorF GuideTextColor{ 1.0, 1.0, 1.0, 1.0 };
        static constexpr Vec2 GuideTextOffsetPos{ 0.0, -125.0 };
        static constexpr double AlphaMinValue{ 0.0 };

    private:

        std::weak_ptr<Rectangle> rectBoard_;
        std::weak_ptr<Rectangle> rectBoardShadow_;
        std::weak_ptr<Text> guideText_;
    };
}
