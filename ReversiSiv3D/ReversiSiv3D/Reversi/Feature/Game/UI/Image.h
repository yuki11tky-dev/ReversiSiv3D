#pragma once

#include "Feature/Game/UI/Drawer.h"

namespace ReversiSiv3D {

    // ロードした画像をそのまま使用するクラス
    class Image : public Drawer
    {
    public:

        Image();
        ~Image();

        void Render() override;

        void LoadTexture(const String&);
        void SetCenterPos(Vec2);
        void SetColor(ColorF);
        void SetScale(double);
        void SetAlpha(double);
        void SetAngle(double);

        Vec2 GetCenterPos() const;
        double GetAlpha() const;
        Size GetSize() const;
        double GetScale() const;
        double GetAngle() const;

    private:

        Texture image_;
        Vec2 centerPos_;
        ColorF color_;
        double scale_;
        double angle_;
    };
}
