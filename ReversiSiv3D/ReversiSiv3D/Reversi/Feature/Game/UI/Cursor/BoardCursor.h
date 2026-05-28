#pragma once

#include "Feature/Game/UI/ImageSprite.h"
#include "Feature/Game/UI/UIBase.h"
#include <memory>

namespace ReversiSiv3D {

    class BoardCursor : public UIBase
    {
    public:

        BoardCursor();
        virtual ~BoardCursor();

        void LoadTexture(const String&);

        void SetCenterPos(Vec2);
        void UpdateDrawIndex();
        void Visible();
        void Hide();

    protected:

        static constexpr ColorF DefaultColor{ 1.0, 1.0, 1.0, 1.0 };

    protected:

        std::weak_ptr<ImageSprite> spriteCursor_;
    };
}
