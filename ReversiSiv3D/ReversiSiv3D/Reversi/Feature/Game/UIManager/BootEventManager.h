#pragma once

#include "Feature/Game/UI/Boot/BootEvent.h"
#include "Feature/System/Task.h"
#include <memory>

namespace ReversiSiv3D {

    class BootEventManager : public Task
    {
    public:

        BootEventManager();
        ~BootEventManager();

        void Initialize();
        void Update(double) override;
        void Render() override;

        void StartEvent();
        bool IsFinish() const;

    private:

        std::unique_ptr<BootEvent> bootEvent_;
    };
}
