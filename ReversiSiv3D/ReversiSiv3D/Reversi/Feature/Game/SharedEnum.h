#pragma once

namespace ReversiSiv3D {

    enum class StoneType : int
    {
        Empty = 0,
        Black = 1,
        White = 2
    };

    enum class PlayerType
    {
        None,
        Player1,
        Player2,
        Cpu
    };

    enum class CursorType
    {
        None,
        Player1,
        Player2,
    };

    enum class ResultLayoutSide
    {
        Left,
        Right
    };
}
