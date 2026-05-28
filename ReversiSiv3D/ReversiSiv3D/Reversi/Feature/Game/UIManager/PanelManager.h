#pragma once

#include "Feature/Game/UI/Notification/NotificationUI.h"
#include "Feature/System/Task.h"
#include <memory>
#include <vector>

namespace ReversiSiv3D {

    class PanelManager : public Task
    {
    public:

        PanelManager();
        ~PanelManager();

        void Update(double) override;
        void Render() override;
        void Release() override;

        void CreatePanelList();

        void StartPanelAnimation(PanelType);
        bool IsFinishedPanelAnimation(PanelType);

    private:

        std::vector<std::unique_ptr<NotificationUI>> panelList_;
    };
}
