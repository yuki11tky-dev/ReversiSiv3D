#include "Feature/Game/UI/Text.h"

namespace ReversiSiv3D {

    Text::Text()
        : Drawer()
        , text_{}
        , textFont_{}
        , color_{}
        , centerPos_{}
    {

    }

    Text::~Text()
    {

    }

    void Text::Render()
    {
        textFont_(text_).drawAt(centerPos_, color_);
    }

    void Text::SetText(const String& text)
    {
        text_ = text;
    }

    void Text::SetFont(FontMethod method, int32 size, Typeface typeFace)
    {
        textFont_ = Font{ method, size, typeFace };
    }

    void Text::SetColor(ColorF color)
    {
        color_ = color;
    }

    void Text::SetCenterPos(Vec2 centerPos)
    {
        centerPos_ = centerPos;
    }

    void Text::SetAlpha(double alpha)
    {
        color_.a = alpha;
    }

    double Text::GetAlpha() const
    {
        return color_.a;
    }
}
