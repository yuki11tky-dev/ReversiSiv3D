#pragma once

#include "Feature/Game/UI/Button/NormalButton.h"
#include "Feature/System/Task.h"

namespace ReversiSiv3D {

    class NormalButtonManager
    {
    public:

        NormalButtonManager();
        virtual ~NormalButtonManager();

        virtual void Initialize() = 0;
        NormalButtonType TryGetDecidedButton();
        void VisibleAndEnabled();
        void HideAndDisable();

    protected:

        void AddButtonList(std::unique_ptr<NormalButton>);
        void ResetAndDisabledAllButton();

        void UpdateButton(double);
        void RenderButton();

    protected:

        std::vector<std::unique_ptr<NormalButton>> buttonList_;
    };
}
