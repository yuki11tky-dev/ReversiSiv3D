#include "Feature/Common/AssetManager.h"
#include "Feature/Common/Math.h"
#include "Feature/Game/UI/Cursor/BoardCursor.h"

namespace ReversiSiv3D {

    BoardCursor::BoardCursor()
        : spriteCursor_{}
    {

    }

    BoardCursor::~BoardCursor()
    {

    }

    void BoardCursor::LoadTexture(const String& imagePath)
    {
        constexpr int horiSplitCount{ 1 };
        constexpr int vertSplitCount{ 2 };
        SpriteData spriteData{};
        spriteData.horizontalSplitCount_ = horiSplitCount;
        spriteData.verticalSplitCount_ = vertSplitCount;

        std::shared_ptr<ImageSprite> spriteCursor = std::make_shared<ImageSprite>();
        spriteCursor->LoadTexture(imagePath, spriteData);
        const double scale{ AssetManager::GetCurrentWindowScale() };
        spriteCursor->SetScale(scale);
        spriteCursor->SetColor(DefaultColor);

        // 所有権は基底クラスであるUIBaseが持つ
        // 本クラスでは弱参照として保持する
        spriteCursor_ = spriteCursor;
        AddDrawable(std::move(spriteCursor));
    }

    void BoardCursor::SetCenterPos(Vec2 pos)
    {
        std::shared_ptr<ImageSprite> spriteCursor = spriteCursor_.lock();
        if (!spriteCursor)
        {
            return;
        }
        spriteCursor->SetCenterPos(pos);
    }

    void BoardCursor::UpdateDrawIndex()
    {
        std::shared_ptr<ImageSprite> spriteCursor = spriteCursor_.lock();
        if (!spriteCursor)
        {
            return;
        }
        // インデックスを0と1で交互に切り替える
        int index{ spriteCursor->GetDrawIndex() };
        index = 1 - index;
        spriteCursor->SetDrawIndex(index);
    }


    void BoardCursor::Visible()
    {
        std::shared_ptr<ImageSprite> spriteCursor = spriteCursor_.lock();
        if (!spriteCursor)
        {
            return;
        }
        constexpr double maxAlpha{ 1.0 };
        spriteCursor->SetAlpha(maxAlpha);
    }

    void BoardCursor::Hide()
    {
        std::shared_ptr<ImageSprite> spriteCursor = spriteCursor_.lock();
        if (!spriteCursor)
        {
            return;
        }
        constexpr double minAlpha{ 0.0 };
        spriteCursor->SetAlpha(minAlpha);
    }
}
