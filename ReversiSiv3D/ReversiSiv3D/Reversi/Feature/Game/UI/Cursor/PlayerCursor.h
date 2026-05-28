#pragma once

#include "Feature/Game/UI/Cursor/BoardCursor.h"

namespace ReversiSiv3D {

    class PlayerCursor : public BoardCursor
    {
    public:

        PlayerCursor();
        ~PlayerCursor();

        void Initialize();
        void Update(double) override;

    private:

        static constexpr double ResetTime{ 0.0 };
        static constexpr double SwitchImageTime{ 0.5 };

    private:

        double elapsedTime_;
    };
}
