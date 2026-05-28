#pragma once

#include "Feature/Game/UI/Image.h"
#include "Feature/Game/UI/Rectangle.h"
#include "Feature/Game/UI/Text.h"
#include "Feature/Game/UI/UIBase.h"
#include <memory>

namespace ReversiSiv3D {

    class TitleDesigner : public UIBase
    {
    public:

        TitleDesigner();
        ~TitleDesigner();

        void Update(double) override;

        void Initialize();

    protected:

        static constexpr Vec2 BoardAdjustPos{ 0.0, 100.0 };
        static constexpr SizeF BackBoardSize{ 350.0, 200.0 };
        static constexpr Vec2 TextBackBoardAdjustPos{ 0.0, 85.0 };
        static constexpr ColorF TextColor{ 1.0, 1.0, 1.0, 1.0 };
        static constexpr ColorF ImageColor{ 1.0, 1.0, 1.0, 1.0 };
        static constexpr ColorF TextBackColor{ 0.5, 0.5, 0.5, 0.7 };
        static constexpr double TitleTextSize{ 60.0 };
        static constexpr Vec2 TitleTextAdjustPos{ 0.0, 130.0 };
        static constexpr double GuideTextSize{ 30.0 };
        static constexpr Vec2 GuideTextAdjustPos{ 0.0, 20.0 };
        static constexpr double GuideTextDrawInterval{ 0.85 };
        static constexpr double ResetTime{ 0.0 };

    private:

        std::weak_ptr<Image> titleBoard_;
        std::weak_ptr<Text> titleText_;
        std::weak_ptr<Text> guideText_;
        std::weak_ptr<Rectangle> rectTextBackBoard_;

        double guideTextDrawTimer_;
    };
}
