#pragma once

#include "Feature/System/FadeManager.h"
#include <memory>

namespace ReversiSiv3D {

    class Fade {
    public:

        inline static void Initialize(const std::shared_ptr<FadeManager>& manager)
        {
            std::shared_ptr<FadeManager> fadeManager = fadeManager_.lock();
            if (fadeManager)
            {
                return;
            }
            fadeManager_ = manager;
        }

        inline static void StartFadeIn()
        {
            std::shared_ptr<FadeManager>fadeManager = fadeManager_.lock();
            if (!fadeManager)
            {
                return;
            }
            fadeManager->StartFadeIn();
        }

        inline static void StartFadeOut()
        {
            std::shared_ptr<FadeManager>fadeManager = fadeManager_.lock();
            if (!fadeManager)
            {
                return;
            }
            fadeManager->StartFadeOut();
        }

        inline static bool IsFinished()
        {
            std::shared_ptr<FadeManager>fadeManager = fadeManager_.lock();
            if (!fadeManager)
            {
                return true;
            }
            return fadeManager->IsFadeFinish();
        }

    public:

        static constexpr double ExecTime{ 1.0 };

    private:

        inline static std::weak_ptr<FadeManager> fadeManager_;
    };
}
