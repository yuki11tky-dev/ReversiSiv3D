#include "Feature/Common/AssetManager.h"
#include "Feature/Game/UI/Board/StoneDrawer.h"
#include <cassert>

namespace ReversiSiv3D {

    StoneDrawer::StoneDrawer()
        : UIBase{}
        , spriteStone_{}
    {

    }

    StoneDrawer::~StoneDrawer()
    {

    }

    void StoneDrawer::Initialize()
    {
        std::shared_ptr<ImageSprite> spriteStone = std::make_shared<ImageSprite>();

        constexpr int horiSplitCount{ 2 };
        constexpr int vertSplitCount{ 4 };
        SpriteData spriteData{};
        spriteData.horizontalSplitCount_ = horiSplitCount;
        spriteData.verticalSplitCount_ = vertSplitCount;
        spriteStone->LoadTexture(U"image/stone.png", spriteData);

        const double scale{ AssetManager::GetCurrentWindowScale() };
        spriteStone->SetColor(DefaultColor);
        spriteStone->SetScale(scale);

        // 所有権は基底クラスであるUIBaseと本クラスが保持する
        // 本クラスでの設定更新処理が頻繁に実施されるため、
        // 本クラスにも所有権を保持する
        spriteStone_ = spriteStone;
        AddDrawable(spriteStone);
    }

    void StoneDrawer::SetCenterPos(Vec2 pos)
    {
        assert(spriteStone_);
        spriteStone_->SetCenterPos(pos);
    }

    void StoneDrawer::SetDrawIndex(int index)
    {
        assert(spriteStone_);
        spriteStone_->SetDrawIndex(index);
    }
}
