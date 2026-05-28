#pragma once

#include "Feature/Game/UI/Image.h"
#include "Feature/Game/UI/UIBase.h"
#include <memory>

namespace ReversiSiv3D {

    class BoardDrawer : public UIBase
    {
    public:

        BoardDrawer();
        ~BoardDrawer();

        void Initialize();
        void SetCenterPos(Vec2);
        Size GetImageSize() const;

    private:

        static constexpr ColorF DefaultColor{ 1.0, 1.0, 1.0, 1.0 };

    private:

        std::shared_ptr<Image> board_;
    };
}
