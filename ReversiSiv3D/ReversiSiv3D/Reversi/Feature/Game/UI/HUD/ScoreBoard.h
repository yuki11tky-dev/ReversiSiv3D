#pragma once

#include "Feature/Game/UI/HUD/HUDBase.h"
#include "Feature/Game/UI/Image.h"
#include "Feature/Game/UI/Text.h"

namespace ReversiSiv3D {

    class ScoreBoard : public HUDBase
    {
    public:

        ScoreBoard();
        ~ScoreBoard();

        void Initialize(const String&, const String&);
        void SetPosition(Vec2, bool);
        void SetScale(double);
        void SetImageColor(ColorF);
        void SetPlayerName(const String&);
        void SetStoneCount(const String&);
        void SetPlayerNameFont(FontMethod, int32, Typeface);
        void SetStoneCountFont(FontMethod, int32, Typeface);
        void SetPlayerNameColor(ColorF);
        void SetStoneCountColor(ColorF);

        Vec2 GetCenterPos() const;

    public:

        static constexpr double AspectRatioAdjustValue{ 0.75 };

    private:

        static constexpr Vec2 StonePosOffset{ -70.0, 50.0 };
        static constexpr Vec2 PlayerNamePosOffset{ 0.0, -75.0 };
        static constexpr Vec2 StoneCountPosOffset{ 70.0, 50.0 };

    private:

        std::weak_ptr<Image> backBoard_;
        std::weak_ptr<Image> stoneIcon_;
        std::weak_ptr<Text> playerName_;
        std::weak_ptr<Text> stoneCount_;
    };
}
