#include "Feature/Game/UI/UIBase.h"

namespace ReversiSiv3D {

    UIBase::UIBase()
        : drawerList_{}
    {

    }

    UIBase::~UIBase()
    {

    }

    void UIBase::Render()
    {
        for (const std::shared_ptr<Drawer>& drawer : drawerList_)
        {
            drawer->Render();
        }
    }

    void UIBase::AddDrawable(std::shared_ptr<Drawer> drawer)
    {
        drawerList_.push_back(std::move(drawer));
    }
}
