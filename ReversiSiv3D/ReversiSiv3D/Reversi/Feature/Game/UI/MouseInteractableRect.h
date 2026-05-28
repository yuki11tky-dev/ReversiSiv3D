#pragma once

#include "Feature/Game/UI/Rectangle.h"

namespace ReversiSiv3D
{
    // 四角形クラスについてマウスによる操作を扱う必要がある際に使用するクラス
    class MouseInteractableRect : public Rectangle
    {
    public:

        MouseInteractableRect();
        ~MouseInteractableRect();

        bool OnHover();
        bool OnLeftClicked();
        bool OnRightClicked();
        bool OnLeftPressed();
        bool OnRightPressed();
        bool OnLeftReleased();
        bool OnRightReleased();
    };
}
