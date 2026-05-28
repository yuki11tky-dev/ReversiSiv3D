#include "Feature/Game/UI/CircleBase.h"

namespace ReversiSiv3D {

    CircleBase::CircleBase()
        : Drawer()
        , circle_{}
        , color_{}
        , scale_{}
    {

    }

    CircleBase::~CircleBase()
    {

    }

    void CircleBase::Render()
    {
        circle_.scaled(scale_).draw(color_);
    }

    void CircleBase::SetRadius(int32 radius)
    {
        circle_.r = radius;
    }

    void CircleBase::SetCenterPos(Vec2 centerPos)
    {
        circle_.setCenter(centerPos);
    }

    void CircleBase::SetColor(ColorF color)
    {
        color_ = color;
    }

    void CircleBase::SetAlpha(double alpha)
    {
        color_.a = alpha;
    }

    void CircleBase::SetScale(double scale)
    {
        scale_ = scale;
    }

    Vec2 CircleBase::GetCenterPos() const
    {
        return circle_.center;
    }

    double CircleBase::GetAlpha() const
    {
        return color_.a;
    }
}
