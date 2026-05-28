#include "Feature/Common/AssetManager.h"
#include "Feature/Game/UI/Board/BoardFrameDrawer.h"
#include <cassert>

namespace ReversiSiv3D {

    BoardFrameDrawer::BoardFrameDrawer()
        : UIBase{}
        , spriteBoardFrame_{}
    {

    }

    BoardFrameDrawer::~BoardFrameDrawer()
    {

    }

    void BoardFrameDrawer::Initialize()
    {
        std::shared_ptr<ImageSprite> spriteBoardFrame = std::make_shared<ImageSprite>();

        constexpr int horiSplitCount{ 1 };
        constexpr int vertSplitCount{ 2 };
        SpriteData spriteData{};
        spriteData.horizontalSplitCount_ = horiSplitCount;
        spriteData.verticalSplitCount_ = vertSplitCount;
        spriteBoardFrame->LoadTexture(U"image/frame.png", spriteData);

        const double scale{ AssetManager::GetCurrentWindowScale() };
        spriteBoardFrame->SetColor(DefaultColor);
        spriteBoardFrame->SetScale(scale);

        // 所有権は基底クラスであるUIBaseと本クラスが保持する
        // 本クラスでの設定更新処理が頻繁に実施されるため、
        // 本クラスにも所有権を保持する
        spriteBoardFrame_ = spriteBoardFrame;
        AddDrawable(spriteBoardFrame);
    }

    void BoardFrameDrawer::SetCenterPos(Vec2 pos)
    {
        assert(spriteBoardFrame_);
        spriteBoardFrame_->SetCenterPos(pos);
    }

    void BoardFrameDrawer::SetAngle(double angle)
    {
        assert(spriteBoardFrame_);
        spriteBoardFrame_->SetAngle(angle);
    }

    void BoardFrameDrawer::SetDrawIndex(int index)
    {
        assert(spriteBoardFrame_);
        spriteBoardFrame_->SetDrawIndex(index);
    }

    Size BoardFrameDrawer::GetOriginImageSize() const
    {
        assert(spriteBoardFrame_);
        return spriteBoardFrame_->GetOriginSize();
    }

    Size BoardFrameDrawer::GetPieceSize() const
    {
        assert(spriteBoardFrame_);
        return spriteBoardFrame_->GetPieceSize();
    }
}
