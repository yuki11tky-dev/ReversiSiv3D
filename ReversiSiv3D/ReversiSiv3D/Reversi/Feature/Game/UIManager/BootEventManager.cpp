#include "Feature/Common/AssetManager.h"
#include "Feature/Common/Math.h"
#include "Feature/Game/UIManager/BootEventManager.h"
#include <cassert>

namespace ReversiSiv3D {

    BootEventManager::BootEventManager()
        : Task{ TaskId::BootEvent }
    {

    }

    BootEventManager::~BootEventManager()
    {

    }

    void BootEventManager::Initialize()
    {
        bootEvent_ = std::make_unique<BootEvent>();
        bootEvent_->Initialize();
    }

    void BootEventManager::Update(double deltaTimeSec)
    {
        assert(bootEvent_);
        bootEvent_->Update(deltaTimeSec);
    }

    void BootEventManager::Render()
    {
        assert(bootEvent_);
        bootEvent_->Render();
    }

    void BootEventManager::StartEvent()
    {
        assert(bootEvent_);
        bootEvent_->Start();
    }

    bool BootEventManager::IsFinish() const
    {
        assert(bootEvent_);
        return bootEvent_->IsFinished();
    }
}
