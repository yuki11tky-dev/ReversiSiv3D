#pragma once

#include "Feature/Game/GameState.h"

namespace ReversiSiv3D {

    enum class TurnResult
    {
        Continue, // 配置可能な場所がある ⇒ そのままゲーム進行
        Skip,     // 配置可能な場所がない ⇒ 次のプレイヤーはスキップ対象
        GameEnd,  // 2回連続でスキップ    ⇒ ゲーム終了
        Error     // エラー
    };

    class ReversiLogic
    {
    public:

        ReversiLogic();
        ~ReversiLogic();

        static bool IsMouseCursorOnTheBoard(const BoardData&, Vec2);
        static BoardCell ConvertCoordinatesToGridCell(const BoardData&, Vec2);
        static Vec2 ConvertGridCellToCenterCoord(const BoardData&, const BoardCell&);
        static Vec2 ConvertCoordPosToCenterCoord(const BoardData&, Vec2);

        static TurnResult CheckAvailablePlaceCell(GameState&, StoneType);
        static std::vector<BoardCell> TryGetCellOfFlipOverStone(GameState&, StoneType, const BoardCell&);
        static bool FlipOverStone(GameState&, StoneType);
        static void CheckStoneCountAfterFlipOver(GameState&);
        static void PlacedStone(GameState&, StoneType, const BoardCell&);
        static EvaluateResult CalculateEvaluationData(GameState&, const BoardCell&);

    private:

        static int CalculateBoardScore(const std::vector<std::vector<int>>&,
                                       const std::vector<std::vector<StoneType>>&,
                                       const StoneType&);
        static std::pair<int, int> CountConfirmationStone(const std::vector<std::vector<StoneType>>&);
        static std::vector<BoardCell> FindStableStonesFromCorners(
                                           const std::vector<std::vector<StoneType>>&,
                                           const BoardCell&,
                                           const BoardCell&);

    private:

        static void DebugCheckCurrentStoneMap(const std::vector<std::vector<StoneType>>&);

    private:

        enum class Direction
        {
            Up,
            UpperRight,
            Right,
            LowerRight,
            Down,
            LowerLeft,
            Left,
            UpperLeft,
        };

        enum class CornerName
        {
            TopLeft,
            TopRight,
            BottomLeft,
            BottomRight
        };

    private:

        inline static std::vector<std::vector<int>> FieldWeight =
        {
            { 45, -11,  4, -1, -1,  4, -11,  45},
            {-11, -16, -1, -3, -3,  2, -16, -11},
            {  4,  -1,  2, -1, -1,  2,  -1,   4},
            { -1,  -3, -1,  0,  0, -1,  -3,  -1},
            { -1,  -3, -1,  0,  0, -1,  -3,  -1},
            {  4,  -1,  2, -1, -1,  2,  -1,   4},
            {-11, -16, -1, -3, -3, -1, -16, -11},
            { 45, -11,  4, -1, -1,  4, -11,  45}
        };
    };
}
