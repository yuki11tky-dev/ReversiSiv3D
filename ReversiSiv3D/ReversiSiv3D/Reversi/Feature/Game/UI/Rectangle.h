#pragma once

#include "Feature/Game/UI/Drawer.h"

namespace ReversiSiv3D {

    class Rectangle : public Drawer
    {
    public:

        Rectangle();
        ~Rectangle();

        void Render() override;

        void Create(Size);
        void Create(Rect);
        void SetCenterPos(Point);
        void SetColor(ColorF);
        void SetAlpha(double);
        void SetSize(Size);
        void SetOffset(Point);

        Point GetCenterPos() const;
        double GetAlpha() const;

    protected:

        Rect rect_;

    private:

        ColorF color_;
        Point offset_;
    };
}
