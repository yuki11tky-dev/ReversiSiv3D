#include "Feature/Game/GameState.h"
#include "Feature/Game/ReversiLogic.h"
#include "Feature/Game/UIManager/BattleBoard.h"
#include <cassert>
#include <format>
#include <string>
#include <Windows.h>

namespace ReversiSiv3D {

    GameState::GameState()
        : placeableCellList_{}
        , placedStoneStack_{}
        , placedStoneMap_{}
        , flipStoneList_{}
        , playerDataList_{}
        , boardData_{}
        , mousePos_{}
        , isPressedLeftButton_{}
        , checkRange_{}
        , skipCount_{}
        , isUpdatedStoneCount_{}
        , currentTurn_{ StoneType::Black }
    {

    }

    GameState::~GameState()
    {

    }

    void GameState::Initialize()
    {
        constexpr int boardRowCount{ BattleBoard::BoardRowCellCount };
        constexpr int boardColCount{ BattleBoard::BoardColCellCount };

        placedStoneStack_.reserve(boardRowCount * boardColCount);
        placedStoneMap_.reserve(boardRowCount);
        const std::vector<std::vector<std::optional<Stone>>> board(boardRowCount, std::vector<std::optional<Stone>>(boardColCount));
        placedStoneMap_ = board;

        // ひっくり返すことができる石の探索に使用
        // 配置した石の外周マスまでの距離(マス数)を表すため
        // 初期化時の値は 0 ではなく 1 として徐々に増加させていく
        checkRange_ = ResetRange;
    }

    void GameState::SetBoardTopLeftPos(Point pos)
    {
        boardData_.topLeftPos_ = pos;
    }

    void GameState::SetBoardPieceSize(Size size)
    {
        boardData_.pieceSize_ = size;
    }

    void GameState::SetFrameTopLeftPos(Point pos)
    {
        boardData_.frameTopLeftPos_ = pos;
    }

    void GameState::SetFramePieceSize(Size size)
    {
        boardData_.framePieceSize_ = size;
    }

    void GameState::SetMouseCursorPos(Vec2 pos)
    {
        mousePos_ = pos;
    }

    void GameState::SetPressLeftButton(bool isPress)
    {
        isPressedLeftButton_ = isPress;
    }

    BoardData GameState::GetBoardData() const
    {
        return boardData_;
    }

    Vec2 GameState::GetMouseCursorPos() const
    {
        return mousePos_;
    }

    bool GameState::GetPressLeftButton() const
    {
        return isPressedLeftButton_;
    }

    std::optional<const PlacedStoneInfo> GameState::GetPlacedStone()
    {
        if (placedStoneStack_.empty())
        {
            return std::nullopt;
        }
        return placedStoneStack_.back();
    }

    const std::vector<PlacedStoneInfo>& GameState::GetPlacedStoneList() const
    {
        return placedStoneStack_;
    }

    std::vector<PlacedStoneInfo>& GameState::GetPlacedStoneList()
    {
        return placedStoneStack_;
    }

    std::optional<std::span<const BoardCell>> GameState::TryGetFlipStoneList(const BoardCell& putStoneCell) const
    {
        if (flipStoneList_.contains(putStoneCell))
        {
            return std::span<const BoardCell>(flipStoneList_.at(putStoneCell));
        }
        return std::nullopt;
    }

    void GameState::ExpandCheckRange()
    {
        ++checkRange_;
    }

    const int GameState::GetCheckRange() const
    {
        return checkRange_;
    }

    void GameState::AddSkipCount()
    {
        ++skipCount_;
    }

    void GameState::ResetSkipCount()
    {
        skipCount_ = ResetCount;
    }

    bool GameState::IsSkippedTwice() const
    {
        return skipCount_ >= GameFinishedCount;
    }

    void GameState::SetTurnOrder(PlayerType blackStoneUser, PlayerType whiteStoneUser)
    {
        // 黒色が先攻
        currentTurn_ = StoneType::Black;

        ResultLayoutSide placeTypeBlack{ ResultLayoutSide::Left };
        ResultLayoutSide placeTypeWhite{ ResultLayoutSide::Right };
        if (blackStoneUser == PlayerType::Player1)
        {
            placeTypeBlack = ResultLayoutSide::Left;
            placeTypeWhite = ResultLayoutSide::Right;
        }
        else if (whiteStoneUser == PlayerType::Player1)
        {
            placeTypeBlack = ResultLayoutSide::Right;
            placeTypeWhite = ResultLayoutSide::Left;
        }

        constexpr int playerCount{ 2 };
        playerDataList_.reserve(playerCount);
        constexpr int32 emptyStoneCount{ 0 };
        PlayerData useBlackData{ blackStoneUser, emptyStoneCount, placeTypeBlack };
        playerDataList_.emplace(StoneType::Black, useBlackData);
        PlayerData useWhiteData{ whiteStoneUser, emptyStoneCount, placeTypeWhite };
        playerDataList_.emplace(StoneType::White, useWhiteData);

        GameConfig& gameConfig = GameInfo::GetGameConfig();
        gameConfig.AddPlayData(StoneType::Black, useBlackData);
        gameConfig.AddPlayData(StoneType::White, useWhiteData);
    }

    PlayerData GameState::GetCurrentPlayData() const
    {
        assert(playerDataList_.contains(currentTurn_));
        return playerDataList_.at(currentTurn_);
    }

    StoneType GameState::GetCurrentTurn() const
    {
        return currentTurn_;
    }

    void GameState::SwitchOperationPlayer()
    {
        switch (currentTurn_)
        {
        case StoneType::Black:
        {
            currentTurn_ = StoneType::White;
            break;
        }
        case StoneType::White:
        {
            currentTurn_ = StoneType::Black;
            break;
        }
        }
    }

    void GameState::PlacedStone(const PlacedStoneInfo& stoneInfo)
    {
        placedStoneStack_.push_back(stoneInfo);
    }

    void GameState::UpdateStoneMap(const BoardCell& cell, const Stone& stone)
    {
        placedStoneMap_.at(cell.row_).at(cell.col_) = stone;
    }

    std::optional<Stone>& GameState::GetStone(const BoardCell& cell)
    {
        return placedStoneMap_.at(cell.row_).at(cell.col_);
    }

    void GameState::ClearPlaceableCells()
    {
        placeableCellList_.clear();
    }

    bool GameState::IsAnyPlaceableCell() const
    {
        return !placeableCellList_.empty();
    }

    bool GameState::IsCellPlaceable(const BoardCell& cell) const
    {
        return std::ranges::find(placeableCellList_, cell) != placeableCellList_.end();
    }

    void GameState::TryAddPlaceableCell(const BoardCell& cell)
    {
        // ここで関数IsCellPlaceableを使用しているのは配置可能かチェックしているのではなく、
        // 対象リストに既に追加済みの値かどうかをチェックしている
        // 追加済みの場合は、新たに追加する必要性はないためスキップする
        const bool isExists = IsCellPlaceable(cell);
        if (isExists)
        {
            return;
        }
        placeableCellList_.push_back(cell);
    }

    bool GameState::CanBeFlippedOver(const BoardCell& putStoneCell, const BoardCell& checkCell)
    {
        // 配置可能なマスに対して配置されたことが保証された場所で使用されるため
        // flipStoneList_ に 引数 putStoneCell は必ず含まれる
        assert(flipStoneList_.contains(putStoneCell));
        const std::vector<BoardCell>& flipStoneList = flipStoneList_[putStoneCell];
        return std::ranges::find(flipStoneList, checkCell) != flipStoneList.end();
    }

    void GameState::SetStoneCount(StoneType stoneType, int32 count)
    {
        assert(playerDataList_.contains(stoneType));
        isUpdatedStoneCount_ = true;
        playerDataList_.at(stoneType).stoneCount_ = count;
    }

    int32 GameState::GetBlackStoneCount() const
    {
        assert(playerDataList_.contains(StoneType::Black));
        return playerDataList_.at(StoneType::Black).stoneCount_;
    }
    int32 GameState::GetWhiteStoneCount() const
    {
        assert(playerDataList_.contains(StoneType::White));
        return playerDataList_.at(StoneType::White).stoneCount_;
    }

    bool GameState::IsPlacedAllStone()
    {
        constexpr int32 placeableStoneMaxCount = BattleBoard::BoardRowCellCount * BattleBoard::BoardColCellCount;
        if (placedStoneStack_.size() >= placeableStoneMaxCount)
        {
            return true;
        }

        if (placedStoneStack_.size() > 0)
        {
            int blackStoneCount{ 0 };
            int whiteStoneCount{ 0 };
            for (const PlacedStoneInfo& stoneInfo : placedStoneStack_)
            {
                const std::optional<const Stone>& stone = GetStone(stoneInfo.placedCell_);
                if (!stone.has_value())
                {
                    continue;
                }
                StoneType stoneType = stone.value().GetStoneType();
                switch (stoneType)
                {
                case StoneType::Black:
                {
                    ++blackStoneCount;
                    break;
                }
                case StoneType::White:
                {
                    ++whiteStoneCount;
                    break;
                }
                }
            }

            constexpr int NotExists = 0;
            if (blackStoneCount == NotExists || whiteStoneCount == NotExists)
            {
                return true;
            }
        }

        return false;
    }

    bool GameState::IsUpdateStoneCount() const
    {
        return isUpdatedStoneCount_;
    }

    void GameState::ResetUpdateStoneCount()
    {
        isUpdatedStoneCount_ = false;
    }

    std::optional<const BoardCell> GameState::TryGetPlaceableCellByIndex(int index) const
    {
        assert(index >= 0);
        if (index >= placeableCellList_.size())
        {
            return std::nullopt;
        }
        return placeableCellList_.at(index);
    }
    std::vector<Vec2> GameState::GetPlaceableCellPosList()
    {
        std::vector<Vec2> placeableCellPosList;
        placeableCellPosList.reserve(placeableCellList_.size());
        for (const BoardCell& cell : placeableCellList_)
        {
            const Vec2 cellCenterPos{ ReversiLogic::ConvertGridCellToCenterCoord(boardData_, cell) };
            placeableCellPosList.push_back(cellCenterPos);
        }
        return placeableCellPosList;
    }

    void GameState::ResetFlipStone()
    {
        checkRange_ = ResetRange;
        flipStoneList_.clear();
    }

    void GameState::AddFlipStone(const BoardCell& cell, const std::vector<BoardCell>& flipStones)
    {
        flipStoneList_.emplace(cell, flipStones);
    }

    void GameState::ReleaseData()
    {
        placeableCellList_.clear();
        placedStoneStack_.clear();
        placedStoneMap_.clear();
        flipStoneList_.clear();
        playerDataList_.clear();
    }
}
