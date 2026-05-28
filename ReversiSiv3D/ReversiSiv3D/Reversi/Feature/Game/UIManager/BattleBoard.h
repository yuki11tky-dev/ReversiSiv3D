#pragma once

#include "Feature/Game/GameState.h"
#include "Feature/Game/UI/Board/BoardDrawer.h"
#include "Feature/Game/UI/Board/BoardFrameDrawer.h"
#include "Feature/System/Task.h"
#include <memory>
#include <vector>

namespace ReversiSiv3D {

    class BattleBoard : public Task
    {
    public:

        BattleBoard();
        ~BattleBoard();

        void Initialize(const std::shared_ptr<GameState>&);
        void Render() override;

    public:

        static constexpr int BoardRowCellCount{ 8 };
        static constexpr int BoardColCellCount{ 8 };

    private:

        static constexpr int BoardRowFrameCount{ 10 };
        static constexpr int BoardColFrameCount{ 10 };
        static constexpr int FrameStartIndex{ 0 };
        static constexpr int FrameEndIndex{ 9 };
        static constexpr double DefaultScale{ 1.0 };

    private:

        enum FrameType : int
        {
            Outer = 0,
            Conner = 1,
        };

        struct RenderFrameInfo
        {
            Vec2 centerPos_;
            int drawIndex_;
            double angle_;

            RenderFrameInfo(Vec2 pos = Vec2{}, int drawIndex = 0, double angle = 0.0)
                : centerPos_{ pos }
                , drawIndex_{ drawIndex }
                , angle_{ angle }
            {

            }
        };

    private:

        std::unique_ptr<BoardDrawer> boardDrawer_;
        std::unique_ptr<BoardFrameDrawer> boardFrameDrawer_;

        std::vector<Vec2> boardDrawInfoList_;
        std::vector<RenderFrameInfo> frameDrawInfoList_;
    };
}
