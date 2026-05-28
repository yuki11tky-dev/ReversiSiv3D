#include "Feature/Game/UI/Notification/FinishPanel.h"
#include "Feature/Game/UI/Notification/SkipTurnPanel.h"
#include "Feature/Game/UI/Notification/StartPanel.h"
#include "Feature/Game/UIManager/PanelManager.h"

namespace ReversiSiv3D {

    PanelManager::PanelManager()
        : Task(TaskId::PanelUI)
    {

    }

    PanelManager::~PanelManager()
    {

    }

    void PanelManager::Update(double deltaTimeSec)
    {
        for (const std::unique_ptr<NotificationUI>& panel : panelList_)
        {
            panel->Update(deltaTimeSec);
        }
    }

    void PanelManager::Render()
    {
        for (const std::unique_ptr<NotificationUI>& panel : panelList_)
        {
            panel->Render();
        }
    }

    void PanelManager::CreatePanelList()
    {
        std::unique_ptr<SkipTurnPanel> skipTurnPanel = std::make_unique<SkipTurnPanel>();
        skipTurnPanel->Initialize();
        panelList_.push_back(std::move(skipTurnPanel));

        std::unique_ptr<StartPanel> startPanel = std::make_unique<StartPanel>();
        startPanel->Initialize();
        panelList_.push_back(std::move(startPanel));

        std::unique_ptr<FinishPanel> finishPanel = std::make_unique<FinishPanel>();
        finishPanel->Initialize();
        panelList_.push_back(std::move(finishPanel));
    }

    void PanelManager::Release()
    {
        panelList_.clear();
    }

    void PanelManager::StartPanelAnimation(PanelType targetPanelType)
    {
        for (const std::unique_ptr<NotificationUI>& panel : panelList_)
        {
            const PanelType panelType{ panel->GetPanelType() };
            if (panelType != targetPanelType)
            {
                continue;
            }
            panel->StartAnimation();
            break;
        }
    }

    bool PanelManager::IsFinishedPanelAnimation(PanelType targetPanelType)
    {
        for (const std::unique_ptr<NotificationUI>& panel : panelList_)
        {
            const PanelType panelType{ panel->GetPanelType() };
            if (panelType != targetPanelType)
            {
                continue;
            }
            return panel->IsFinishedAnimation();
        }

        // リストに対象のパネルが存在しない場合は終了したと返す
        return true;
    }
}
