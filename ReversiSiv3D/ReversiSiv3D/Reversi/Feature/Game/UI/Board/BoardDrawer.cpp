#include "Feature/Common/AssetManager.h"
#include "Feature/Game/UI/Board/BoardDrawer.h"
#include <cassert>

namespace ReversiSiv3D {

    BoardDrawer::BoardDrawer()
        : UIBase{}
        , board_{}
    {

    }

    BoardDrawer::~BoardDrawer()
    {

    }

    void BoardDrawer::Initialize()
    {
        std::shared_ptr<Image> board = std::make_shared<Image>();

        board->LoadTexture(U"image/board.png");

        const double scale{ AssetManager::GetCurrentWindowScale() };
        board->SetColor(DefaultColor);
        board->SetScale(scale);

        // 所有権は基底クラスであるUIBaseと本クラスが保持する
        // 本クラスでの設定更新処理が頻繁に実施されるため、
        // 本クラスにも所有権を保持する
        board_ = board;
        AddDrawable(board);
    }

    void BoardDrawer::SetCenterPos(Vec2 centerPos)
    {
        assert(board_);
        board_->SetCenterPos(centerPos);
    }

    Size BoardDrawer::GetImageSize() const
    {
        assert(board_);
        return board_->GetSize();
    }
}
