#pragma once

#include "Feature/System/Task.h"

namespace ReversiSiv3D {

    class MouseController : public Task
    {
    public:

        MouseController();
        ~MouseController();

        void Update(double) override;

        Vec2 GetCurrentMouseCursorPos() const;
        Vec2 GetPreviousMouseCursorPos() const;
        bool IsPressLeftButton() const;

    private:

        Vec2 mousePos_;
        Vec2 mousePosOld_;

        bool isPressedLeftButton_;
    };
}
