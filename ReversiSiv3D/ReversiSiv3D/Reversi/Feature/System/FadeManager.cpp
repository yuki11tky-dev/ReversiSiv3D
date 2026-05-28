#include "Feature/System/FadeManager.h"
#include "Feature/System/TaskId.h"
#include <cassert>

namespace ReversiSiv3D {

    FadeManager::FadeManager()
        : Task{ TaskId::FadeManager }
        , fadeDrawer_{}
    {

    }

    FadeManager::~FadeManager()
    {

    }

    void FadeManager::Update(double deltaTImeSec)
    {
        assert(fadeDrawer_);
        fadeDrawer_->Update(deltaTImeSec);
    }

    void FadeManager::Render()
    {
        assert(fadeDrawer_);
        fadeDrawer_->Render();
    }

    void FadeManager::Initialize()
    {
        fadeDrawer_ = std::make_unique<FadeDrawer>();
        fadeDrawer_->Initialize();
    }

    void FadeManager::StartFadeIn()
    {
        assert(fadeDrawer_);
        fadeDrawer_->StartFadeIn();
    }

    void FadeManager::StartFadeOut()
    {
        assert(fadeDrawer_);
        fadeDrawer_->StartFadeOut();
    }

    bool FadeManager::IsFadeFinish() const
    {
        assert(fadeDrawer_);
        return fadeDrawer_->IsFadeFinish();
    }
}
