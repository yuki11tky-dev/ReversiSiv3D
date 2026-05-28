#include "Feature/Common/Math.h"
#include "Feature/Game/UI/Image.h"
#include <cassert>
#include <limits>

namespace ReversiSiv3D {

    Image::Image()
        : Drawer()
        , image_{}
        , centerPos_{}
        , color_{}
        , scale_{}
        , angle_{}
    {

    }

    Image::~Image()
    {

    }

    void Image::Render()
    {
        assert(image_);
        image_.scaled(scale_).rotated(angle_).drawAt(centerPos_, color_);
    }

    void Image::LoadTexture(const String& filePath)
    {
        image_ = Texture{ filePath };
    }

    void Image::SetCenterPos(Vec2 centerPos)
    {
        centerPos_ = centerPos;
    }

    void Image::SetColor(ColorF color)
    {
        color_ = color;
    }

    void Image::SetScale(double scale)
    {
        scale_ = scale;
    }

    void Image::SetAlpha(double alpha)
    {
        color_.a = alpha;
    }

    void Image::SetAngle(double angle)
    {
        angle_ = angle;
    }

    Vec2 Image::GetCenterPos() const
    {
        return centerPos_;
    }

    double Image::GetAlpha() const
    {
        return color_.a;
    }

    Size Image::GetSize() const
    {
        assert(image_);
        return image_.size();
    }

    double Image::GetScale() const
    {
        return scale_;
    }

    double Image::GetAngle() const
    {
        return angle_;
    }
}
