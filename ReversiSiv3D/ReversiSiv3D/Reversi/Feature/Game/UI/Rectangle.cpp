#include "Feature/Game/UI/Rectangle.h"

namespace ReversiSiv3D {

    Rectangle::Rectangle()
        : Drawer()
        , rect_{}
        , color_{}
        , offset_{}
    {

    }

    Rectangle::~Rectangle()
    {

    }

    void Rectangle::Render()
    {
        rect_.movedBy(offset_).draw(color_);
    }

    void Rectangle::Create(Size size)
    {
        rect_ = Rect{ size };
    }

    void Rectangle::Create(Rect rect)
    {
        rect_ = Rect{ rect };
    }

    void Rectangle::SetCenterPos(Point centerPos)
    {
        rect_.setCenter(centerPos);
    }

    void Rectangle::SetColor(ColorF color)
    {
        color_ = color;
    }

    void Rectangle::SetAlpha(double alpha)
    {
        color_.a = alpha;
    }

    void Rectangle::SetSize(Size size)
    {
        rect_.setSize(size);
    }

    void Rectangle::SetOffset(Point offset)
    {
        offset_ = offset;
    }

    Point Rectangle::GetCenterPos() const
    {
        return rect_.center().asPoint();
    }

    double Rectangle::GetAlpha() const
    {
        return color_.a;
    }
}
