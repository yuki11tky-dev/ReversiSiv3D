#pragma once

#include "Feature/Game/GameInfo.h"
#include "Feature/Game/SharedEnum.h"
#include "Feature/Game/Stone.h"
#include <optional>
#include <span>
#include <unordered_map>

namespace ReversiSiv3D {

    struct BoardCell
    {
        int col_;
        int row_;

        constexpr BoardCell(int col = 0, int row = 0)
            : col_(col)
            , row_(row)
        {

        }

        constexpr bool operator==(const BoardCell&) const = default;
    };
}

namespace std
{
    template <>
    struct hash<ReversiSiv3D::BoardCell>
    {
        size_t operator()(const ReversiSiv3D::BoardCell& cell) const noexcept
        {
            size_t h1 = std::hash<int>{}(cell.col_);
            size_t h2 = std::hash<int>{}(cell.row_);
            return h1 ^ (h2 << 1);
        }
    };
}

namespace ReversiSiv3D {

    struct BoardData
    {
        Point topLeftPos_{};
        Size pieceSize_{};
        Point frameTopLeftPos_{};
        Size framePieceSize_{};
    };

    struct PlacedStoneInfo
    {
        BoardCell placedCell_{};
        StoneType stoneTypeWhenPlaced_{};
    };

    struct EvaluateResult
    {
        int value_{};
        int confirmCount_{};
    };

    class GameState {
    public:

        GameState();
        ~GameState();

        void SetBoardTopLeftPos(Point);
        void SetBoardPieceSize(Size);
        void SetFrameTopLeftPos(Point);
        void SetFramePieceSize(Size);
        void SetMouseCursorPos(Vec2);
        void SetPressLeftButton(bool);

        BoardData GetBoardData() const;
        Vec2 GetMouseCursorPos() const;
        bool GetPressLeftButton() const;
        std::optional<const PlacedStoneInfo> GetPlacedStone();
        const std::vector<PlacedStoneInfo>& GetPlacedStoneList() const;
        std::vector<PlacedStoneInfo>& GetPlacedStoneList();
        std::optional<std::span<const BoardCell>> TryGetFlipStoneList(const BoardCell&) const;
        void ExpandCheckRange();
        const int GetCheckRange() const;
        void AddSkipCount();
        void ResetSkipCount();
        bool IsSkippedTwice() const;

        void SetTurnOrder(PlayerType, PlayerType);
        PlayerData GetCurrentPlayData() const;
        StoneType GetCurrentTurn() const;
        void SwitchOperationPlayer();

        void Initialize();

    public:

        void PlacedStone(const PlacedStoneInfo&);
        void UpdateStoneMap(const BoardCell&, const Stone&);
        std::optional<Stone>& GetStone(const BoardCell&);

        void ClearPlaceableCells();
        bool IsAnyPlaceableCell() const;
        bool IsCellPlaceable(const BoardCell&) const;
        std::optional<const BoardCell> TryGetPlaceableCellByIndex(int) const;
        std::vector<Vec2> GetPlaceableCellPosList();
        void TryAddPlaceableCell(const BoardCell&);

        bool CanBeFlippedOver(const BoardCell&, const BoardCell&);
        void SetStoneCount(StoneType, int32);
        int32 GetBlackStoneCount() const;
        int32 GetWhiteStoneCount() const;

        bool IsPlacedAllStone();
        bool IsUpdateStoneCount() const;
        void ResetUpdateStoneCount();

        void ResetFlipStone();
        void AddFlipStone(const BoardCell&, const std::vector<BoardCell>&);
        void ReleaseData();

    public:

        void DebugCheckPlacedStone();
        void DebugCheckFlipStoneCell();

    private:

        static constexpr int ResetRange{ 1 };
        static constexpr int ResetCount{ 0 };
        static constexpr int GameFinishedCount{ 2 };

    private:

        std::vector<BoardCell> placeableCellList_;
        std::vector<PlacedStoneInfo> placedStoneStack_;
        std::vector<std::vector<std::optional<Stone>>> placedStoneMap_;
        std::unordered_map<BoardCell, std::vector<BoardCell>> flipStoneList_;
        std::unordered_map<StoneType, PlayerData> playerDataList_;

        BoardData boardData_;
        Vec2 mousePos_;
        bool isPressedLeftButton_;
        int checkRange_;
        int skipCount_;
        bool isUpdatedStoneCount_;
        StoneType currentTurn_;
    };
}
