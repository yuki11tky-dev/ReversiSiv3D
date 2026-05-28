#pragma once

#include "Feature/Game/SharedEnum.h"
#include <vector>

namespace ReversiSiv3D {

    struct RenderStoneInfo
    {
        bool isHide_;
        int drawIndex_;
        Vec2 placeCenterPos_;

        constexpr RenderStoneInfo(bool isHide = false, int index = 0, Vec2 pos = Vec2{})
            : isHide_{ isHide }
            , drawIndex_{ index }
            , placeCenterPos_{ pos }
        {

        }
    };

    // 盤上の石の状態や描画に必要な情報を扱うクラス
    // 実際の描画は StoneDrawer クラスが担う
    class Stone
    {
    public:

        Stone();
        ~Stone();

        void Update(double);

        RenderStoneInfo GetRenderInfo();

        void SetPos(Vec2);
        void SetStoneType(StoneType);
        void ChangeStoneType();
        StoneType GetStoneType() const;
        void Hide();
        void Visible();
        void StartFlipping();

    private:

        enum class PhaseType
        {
            Wait,
            Hide,
            Placed,
            StartFlipping,
            Flipping,
            EndFlipping
        };

    private:

        bool FlipOverAnimation();
        void ChangeCurrentPhase(PhaseType);

    private:

        static constexpr int BlackStoneIndex{ 0 };
        static constexpr int WhiteStoneIndex{ 7 };

        static constexpr double ResetTime{ 0.0 };
        static constexpr double SwitchAnimation{ 0.05 };

    private:

        Vec2 imagePos_;
        int drawIndex_;
        double elapsedTime_;
        PhaseType currentPhase_;
        StoneType stoneType_;
    };
}
