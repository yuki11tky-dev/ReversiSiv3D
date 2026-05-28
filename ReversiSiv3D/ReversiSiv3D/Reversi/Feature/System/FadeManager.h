#pragma once

#include "Feature/Game/UI/Fade/FadeDrawer.h"
#include "Feature/System/Task.h"
#include <memory>

namespace ReversiSiv3D {

    class FadeManager : public Task {
    public:

        FadeManager();
        ~FadeManager();

    public:

        void Update(double) override;
        void Render() override;

        void Initialize();

        void StartFadeIn();
        void StartFadeOut();
        bool IsFadeFinish() const;

    private:

        std::unique_ptr<FadeDrawer> fadeDrawer_;
    };
}
