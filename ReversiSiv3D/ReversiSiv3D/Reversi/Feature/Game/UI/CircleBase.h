#pragma once

#include "Feature/Game/UI/Drawer.h"

namespace ReversiSiv3D {

    class CircleBase : public Drawer
    {
    public:

        CircleBase();
        ~CircleBase();

        void Render() override;

        void SetRadius(int32);
        void SetCenterPos(Vec2);
        void SetColor(ColorF);
        void SetAlpha(double);
        void SetScale(double);

        Vec2 GetCenterPos() const;
        double GetAlpha() const;

    protected:

        Circle circle_;

    private:

        ColorF color_;
        double scale_;
    };
}
