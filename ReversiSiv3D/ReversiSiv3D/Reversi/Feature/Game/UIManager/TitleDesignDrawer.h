#pragma once

#include "Feature/Game/UI/Title/TitleDesigner.h"
#include "Feature/System/Task.h"
#include <memory>

namespace ReversiSiv3D {

    class TitleDesignDrawer : public Task
    {
    public:

        TitleDesignDrawer();
        ~TitleDesignDrawer();

        void Update(double) override;
        void Render() override;

        void Initialize();

    private:

        std::unique_ptr<TitleDesigner> titleDesigner_;
    };
}
