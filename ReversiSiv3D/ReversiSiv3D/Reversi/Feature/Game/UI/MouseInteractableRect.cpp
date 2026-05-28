#include "Feature/Game/UI/MouseInteractableRect.h"

namespace ReversiSiv3D
{
    MouseInteractableRect::MouseInteractableRect()
    {

    }

    MouseInteractableRect::~MouseInteractableRect()
    {

    }

    bool MouseInteractableRect::OnHover()
    {
        return rect_.mouseOver();
    }

    bool MouseInteractableRect::OnLeftClicked()
    {
        return rect_.leftClicked();
    }

    bool MouseInteractableRect::OnRightClicked()
    {
        return rect_.rightClicked();
    }

    bool MouseInteractableRect::OnLeftPressed()
    {
        return rect_.leftPressed();
    }

    bool MouseInteractableRect::OnRightPressed()
    {
        return rect_.rightPressed();
    }

    bool MouseInteractableRect::OnLeftReleased()
    {
        return rect_.leftReleased();
    }

    bool MouseInteractableRect::OnRightReleased()
    {
        return rect_.rightReleased();
    }
}
