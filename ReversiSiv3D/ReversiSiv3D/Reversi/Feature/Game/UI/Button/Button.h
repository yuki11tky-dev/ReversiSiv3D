#pragma once

#include "Feature/Game/UI/Image.h"
#include "Feature/Game/UI/MouseInteractableRect.h"
#include "Feature/Game/UI/Rectangle.h"
#include "Feature/Game/UI/Text.h"
#include "Feature/Game/UI/UIBase.h"
#include <memory>

namespace ReversiSiv3D {

    class Button : public UIBase
    {
    public:

        Button();
        virtual ~Button();

        void Update(double);

        bool IsDecided() const;
        void Disabled();
        void Enabled();
        void ResetDecided();
        void EnabledShadow();
        void Visible();
        void Hide();

        void SetDefaultColor(ColorF);
        void SetColor(ColorF);
        void SetSize(Size);
        void SetText(const String&);
        void SetFont(FontMethod, int32, Typeface);
        void SetTextColor(ColorF);

        void SetCenterPos(Point);

    protected:

        void Initialize();

        void ApplyPressedOffset(Point);

        virtual void OnMouseOver() {};
        virtual void OnLeftClicked() {};
        virtual void OnRightClicked() {};
        virtual void OnLeftPressed() {};
        virtual void OnRightPressed() {};
        virtual void OnLeftReleased() {};
        virtual void OnRightReleased() {};

        virtual void UpdateButton(double) = 0;

    protected:

        static constexpr Size ButtonRectSize{ 300, 75 };
        static constexpr Point ShadowOffset{ 5, 5 };
        static constexpr ColorF DefaultTextColor{ 1.0, 1.0, 1.0, 1.0 };
        static constexpr int32 ButtonFontSize{ 40 };
        static constexpr ColorF ShadowIntensity{ 0.0, 0.0, 0.0, 0.3 };
        static constexpr Point ResetOffset{ 0, 0 };
        static constexpr Point PressOffset{ 3, 3 };
        static constexpr ColorF DefaultButtonColor{ 0.0, 0.0, 0.0 , 1.0 };
        static constexpr ColorF SelectedAddColor{ 0.3, 0.3, 0.3, 0.0 };

    protected:

        std::shared_ptr<MouseInteractableRect> buttonRect_;
        std::shared_ptr<Rectangle> shadowRect_;
        std::shared_ptr<Text> buttonText_;

        ColorF buttonColor_;

        bool isHovered_;
        bool isPressLeft_;
        bool isPressRight_;
        bool isDecided_;
        bool isEnabled_;
        bool isShadow_;
    };
}
