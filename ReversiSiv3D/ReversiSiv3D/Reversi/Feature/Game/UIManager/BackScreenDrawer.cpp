#include "Feature/Game/UIManager/BackScreenDrawer.h"
#include <cassert>

namespace ReversiSiv3D {

    BackScreenDrawer::BackScreenDrawer()
        : Task{ TaskId::BackGround }
        , backScreen_{}
    {

    }

    BackScreenDrawer::~BackScreenDrawer()
    {

    }

    void BackScreenDrawer::Render()
    {
        assert(backScreen_);
        backScreen_->Render();
    }

    void BackScreenDrawer::Initialize()
    {
        backScreen_ = std::make_unique<BackScreenNormal>();
        backScreen_->Initialize();
    }
}
