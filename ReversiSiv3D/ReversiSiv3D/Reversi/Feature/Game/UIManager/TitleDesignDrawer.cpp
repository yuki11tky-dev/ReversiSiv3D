#include "Feature/Game/UIManager/TitleDesignDrawer.h"
#include <cassert>

namespace ReversiSiv3D {

    TitleDesignDrawer::TitleDesignDrawer()
        : Task{ TaskId::TitleDesigner }
        , titleDesigner_{}
    {

    }

    TitleDesignDrawer::~TitleDesignDrawer()
    {

    }

    void TitleDesignDrawer::Update(double deltaTimeSec)
    {
        assert(titleDesigner_);
        titleDesigner_->Update(deltaTimeSec);
    }

    void TitleDesignDrawer::Render()
    {
        assert(titleDesigner_);
        titleDesigner_->Render();
    }

    void TitleDesignDrawer::Initialize()
    {
        titleDesigner_ = std::make_unique<TitleDesigner>();
        titleDesigner_->Initialize();
    }
}
