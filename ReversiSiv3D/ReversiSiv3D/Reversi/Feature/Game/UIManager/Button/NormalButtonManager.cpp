#include "Feature/Game/UIManager/Button/NormalButtonManager.h"

namespace ReversiSiv3D {

    NormalButtonManager::NormalButtonManager()
        : buttonList_{}
    {

    }

    NormalButtonManager::~NormalButtonManager()
    {

    }

    void NormalButtonManager::AddButtonList(std::unique_ptr<NormalButton> button)
    {
        buttonList_.push_back(std::move(button));
    }

    void NormalButtonManager::UpdateButton(double deltaTime)
    {
        for (const std::unique_ptr<NormalButton>& button : buttonList_)
        {
            if (!button)
            {
                continue;
            }
            button->Update(deltaTime);
        }
    }

    void NormalButtonManager::RenderButton()
    {
        for (const std::unique_ptr<NormalButton>& button : buttonList_)
        {
            if (!button)
            {
                continue;
            }
            button->Render();
        }
    }

    NormalButtonType NormalButtonManager::TryGetDecidedButton()
    {
        for (const std::unique_ptr<NormalButton>& button : buttonList_)
        {
            if (!button)
            {
                continue;
            }
            const bool isDecided = button->IsDecided();
            if (!isDecided)
            {
                continue;
            }
            return button->GetButtonType();
        }

        return NormalButtonType::None;
    }

    void NormalButtonManager::ResetAndDisabledAllButton()
    {
        for (const std::unique_ptr<NormalButton>& button : buttonList_)
        {
            if (!button)
            {
                continue;
            }
            button->ResetDecided();
            button->Disabled();
        }
    }

    void NormalButtonManager::VisibleAndEnabled()
    {
        for (const std::unique_ptr<NormalButton>& button : buttonList_)
        {
            if (!button)
            {
                continue;
            }
            button->Visible();
            button->Enabled();
        }
    }

    void NormalButtonManager::HideAndDisable()
    {
        for (const std::unique_ptr<NormalButton>& button : buttonList_)
        {
            if (!button)
            {
                continue;
            }
            button->Hide();
            button->Disabled();
        }
    }
}
