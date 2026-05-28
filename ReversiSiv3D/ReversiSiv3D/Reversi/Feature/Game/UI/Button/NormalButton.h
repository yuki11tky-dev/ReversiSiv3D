#pragma once

#include "Feature/Game/UI/Button/Button.h"

namespace ReversiSiv3D {

    enum class NormalButtonType
    {
        None,
        GameStart,
        GameEnd,
        ReturnTitle,
        DecideFirstPlayer,
        DecideSecondPlayer
    };

    class NormalButton : public Button
    {
    public:

        NormalButton(NormalButtonType);
        ~NormalButton();

        void Initialize();

        NormalButtonType GetButtonType() const;

    private:

        void OnMouseOver() override;
        void OnLeftClicked() override;
        void OnRightClicked() override;
        void OnLeftPressed() override;
        void OnRightPressed() override;
        void OnLeftReleased() override;
        void OnRightReleased() override;

        void UpdateButton(double) override;

    private:

        NormalButtonType buttonType_;
    };
}
