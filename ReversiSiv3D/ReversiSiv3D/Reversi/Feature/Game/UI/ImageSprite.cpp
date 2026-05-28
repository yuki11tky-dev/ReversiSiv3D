#include "Feature/Common/Math.h"
#include "Feature/Game/UI/ImageSprite.h"
#include <cassert>
#include <limits>

namespace ReversiSiv3D {

    ImageSprite::ImageSprite()
        : Drawer()
        , imageList_{}
        , centerPos_{}
        , color_{}
        , scale_{}
        , angle_{}
        , drawIndex_{}
        , originSize_{}
    {

    }

    ImageSprite::~ImageSprite()
    {

    }

    void ImageSprite::Render()
    {
        assert(!imageList_.empty());
        assert(drawIndex_ < imageList_.size());
        imageList_.at(drawIndex_).scaled(scale_).rotated(angle_).drawAt(centerPos_, color_);
    }

    void ImageSprite::LoadTexture(const String& filePath, const SpriteData& spriteData)
    {
        // ファイルパスをロードして画像データを生成
        // 画像分割後の幅と高さを算出した後に画像を分割して取得する
        const Texture image = Texture{ filePath };
        originSize_ = image.size();
        const double splitedWidth{ static_cast<double>(originSize_.x) / spriteData.horizontalSplitCount_ };
        const double splitedHeight{ static_cast<double>(originSize_.y) / spriteData.verticalSplitCount_ };
        const int totalSplitCount{ spriteData.horizontalSplitCount_ * spriteData.verticalSplitCount_ };
        imageList_.reserve(totalSplitCount);

        double splitStartPosWidth{};
        double splitStartPosHeight{};
        for (int vertSplitIndex = 0; vertSplitIndex < spriteData.verticalSplitCount_; ++vertSplitIndex)
        {
            splitStartPosHeight = vertSplitIndex * splitedHeight;

            for (int horiSplitIndex = 0; horiSplitIndex < spriteData.horizontalSplitCount_; ++horiSplitIndex)
            {
                splitStartPosWidth = horiSplitIndex * splitedWidth;
                imageList_.push_back(image(splitStartPosWidth, splitStartPosHeight, splitedWidth, splitedHeight));
            }
        }
    }

    void ImageSprite::SetCenterPos(Vec2 centerPos)
    {
        centerPos_ = centerPos;
    }

    void ImageSprite::SetColor(ColorF color)
    {
        color_ = color;
    }

    void ImageSprite::SetScale(double scale)
    {
        scale_ = scale;
    }

    void ImageSprite::SetAlpha(double alpha)
    {
        color_.a = alpha;
    }

    void ImageSprite::SetAngle(double angle)
    {
        angle_ = angle;
    }

    void ImageSprite::SetDrawIndex(int index)
    {
        drawIndex_ = index;
    }

    Vec2 ImageSprite::GetCenterPos() const
    {
        return centerPos_;
    }

    double ImageSprite::GetAlpha() const
    {
        return color_.a;
    }

    int ImageSprite::GetDrawIndex() const
    {
        return drawIndex_;
    }

    int ImageSprite::GetMaxListSize() const
    {
        assert(imageList_.size() <= static_cast<size_t>(std::numeric_limits<int32_t>::max()));
        return static_cast<int>(imageList_.size());
    }

    Size ImageSprite::GetOriginSize() const
    {
        return originSize_;
    }

    Size ImageSprite::GetPieceSize() const
    {
        assert(!imageList_.empty());
        return imageList_.at(0).size.asPoint();
    }
}
