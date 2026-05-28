#include "Feature/Common/AssetManager.h"
#include "Feature/Game/UI/Button/NormalButton.h"
#include <cassert>

namespace ReversiSiv3D {

    NormalButton::NormalButton(NormalButtonType buttonType)
        : buttonType_{ buttonType }
    {

    }

    NormalButton::~NormalButton()
    {

    }

    void NormalButton::Initialize()
    {
        Button::Initialize();
    }

    NormalButtonType NormalButton::GetButtonType() const
    {
        return buttonType_;
    }

    void NormalButton::OnMouseOver()
    {
        // 使用予定がないため未実装
    }

    void NormalButton::OnLeftClicked()
    {
        // 使用予定がないため未実装
    }

    void NormalButton::OnRightClicked()
    {
        // 使用予定がないため未実装
    }

    void NormalButton::OnLeftPressed()
    {
        // 使用予定がないため未実装
    }

    void NormalButton::OnRightPressed()
    {
        // 使用予定がないため未実装
    }

    void NormalButton::OnLeftReleased()
    {
        isDecided_ = true;
    }

    void NormalButton::OnRightReleased()
    {
        // 使用予定がないため未実装
    }

    void NormalButton::UpdateButton(double)
    {
        Point buttonOffset{ ResetOffset };
        ColorF drawButtonColor{ buttonColor_ };
        if (isHovered_)
        {
            drawButtonColor.r +=SelectedAddColor.r;
            drawButtonColor.g += SelectedAddColor.g;
            drawButtonColor.b += SelectedAddColor.b;
            drawButtonColor.a += SelectedAddColor.a;

            if (isPressLeft_)
            {
                buttonOffset = AssetManager::AdjustCurrentWindowScale(PressOffset);
            }
            else
            {
                buttonOffset = ResetOffset;
            }
        }

        SetColor(drawButtonColor);
        ApplyPressedOffset(buttonOffset);
    }
}
