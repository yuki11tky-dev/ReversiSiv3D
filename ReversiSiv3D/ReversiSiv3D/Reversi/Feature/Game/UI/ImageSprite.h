#pragma once

#include "Feature/Game/UI/Drawer.h"
#include <vector>

namespace ReversiSiv3D {

    struct SpriteData
    {
    public:

        int horizontalSplitCount_;
        int verticalSplitCount_;

        constexpr SpriteData(int hSplitCount = 0, int vSplitCount = 0)
            : horizontalSplitCount_{ hSplitCount }
            , verticalSplitCount_{ vSplitCount }
        {

        }
    };

    // ロードした画像を分割する場合に使用するクラス
    class ImageSprite : public Drawer
    {
    public:

        ImageSprite();
        ~ImageSprite();

        void Render() override;

        void LoadTexture(const String& filePath, const SpriteData&);
        void SetCenterPos(Vec2);
        void SetColor(ColorF);
        void SetScale(double);
        void SetAlpha(double);
        void SetAngle(double);
        void SetDrawIndex(int);

        Vec2 GetCenterPos() const;
        double GetAlpha() const;
        int GetDrawIndex() const;
        int GetMaxListSize() const;
        Size GetOriginSize() const;
        Size GetPieceSize() const;

    private:

        std::vector<TextureRegion> imageList_;
        Vec2 centerPos_;
        ColorF color_;
        double scale_;
        double angle_;
        int drawIndex_;
        Size originSize_;
    };
}
