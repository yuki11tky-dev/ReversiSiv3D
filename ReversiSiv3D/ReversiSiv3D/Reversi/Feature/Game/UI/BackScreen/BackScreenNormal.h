#pragma once

#include "Feature/Game/UI/Image.h"
#include "Feature/Game/UI/UIBase.h"
#include <memory>

namespace ReversiSiv3D {

    class BackScreenNormal : public UIBase
    {
    public:

        BackScreenNormal();
        ~BackScreenNormal();

        void Initialize();

    private:

        static constexpr ColorF DefaultColor{ 1.0, 1.0, 1.0, 1.0 };

    private:

        std::weak_ptr<Image> backScreen_;
    };
}
