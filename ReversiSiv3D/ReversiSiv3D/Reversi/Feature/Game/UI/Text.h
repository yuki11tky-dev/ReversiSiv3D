#pragma once

#include "Feature/Game/UI/Drawer.h"

namespace ReversiSiv3D {

    class Text : public Drawer
    {
    public:

        Text();
        ~Text();

        void Render() override;

        void SetText(const String&);
        void SetFont(FontMethod, int32, Typeface);
        void SetColor(ColorF);
        void SetCenterPos(Vec2);
        void SetAlpha(double);
        double GetAlpha() const;

    private:

        String text_;
        Font textFont_;
        ColorF color_;
        Vec2 centerPos_;
    };
}
