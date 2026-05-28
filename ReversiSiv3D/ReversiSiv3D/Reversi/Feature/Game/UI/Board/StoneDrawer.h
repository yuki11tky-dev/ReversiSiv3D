#pragma once

#include "Feature/Game/UI/ImageSprite.h"
#include "Feature/Game/UI/UIBase.h"
#include <memory>

namespace ReversiSiv3D {

    class StoneDrawer : public UIBase
    {
    public:

        StoneDrawer();
        ~StoneDrawer();

        void Initialize();
        void SetCenterPos(Vec2);
        void SetDrawIndex(int);

    private:

        static constexpr ColorF DefaultColor{ 1.0, 1.0, 1.0, 1.0 };

    private:

        std::shared_ptr<ImageSprite> spriteStone_;
    };
}
