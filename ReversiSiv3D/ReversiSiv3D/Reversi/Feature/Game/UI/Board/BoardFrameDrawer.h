#pragma once

#include "Feature/Game/UI/ImageSprite.h"
#include "Feature/Game/UI/UIBase.h"
#include <memory>

namespace ReversiSiv3D {

    class BoardFrameDrawer : public UIBase
    {
    public:

        BoardFrameDrawer();
        ~BoardFrameDrawer();

        void Initialize();
        void SetCenterPos(Vec2);
        void SetAngle(double);
        void SetDrawIndex(int);
        Size GetOriginImageSize() const;
        Size GetPieceSize() const;

    private:

        static constexpr ColorF DefaultColor{ 1.0, 1.0, 1.0, 1.0 };

    private:

        std::shared_ptr<ImageSprite> spriteBoardFrame_;
    };
}
