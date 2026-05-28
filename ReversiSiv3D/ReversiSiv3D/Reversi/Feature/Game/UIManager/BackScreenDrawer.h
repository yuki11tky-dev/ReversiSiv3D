#pragma once

#include "Feature/Game/UI/BackScreen/BackScreenNormal.h"
#include "Feature/System/Task.h"
#include <memory>

namespace ReversiSiv3D {

    class BackScreenDrawer : public Task
    {
    public:

        BackScreenDrawer();
        ~BackScreenDrawer();

        void Render() override;

        void Initialize();

    private:

        std::unique_ptr<BackScreenNormal> backScreen_;
    };
}
