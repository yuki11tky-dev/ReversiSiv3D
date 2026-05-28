#include "Feature/Common/Math.h"
#include "Feature/Game/ReversiLogic.h"
#include "Feature/Game/UIManager/BattleBoard.h"
#include "Feature/Logger/Logger.h"
#include <cassert>
#include <format>
#include <queue>
#include <ranges>

namespace ReversiSiv3D {

    ReversiLogic::ReversiLogic()
    {

    }

    ReversiLogic::~ReversiLogic()
    {

    }

    bool ReversiLogic::IsMouseCursorOnTheBoard(const BoardData& boardData, Vec2 originCoord)
    {
        Point bottomRightPos{};
        bottomRightPos.x = boardData.topLeftPos_.x + BattleBoard::BoardColCellCount * boardData.pieceSize_.x;
        bottomRightPos.y = boardData.topLeftPos_.y + BattleBoard::BoardRowCellCount * boardData.pieceSize_.y;

        if ((originCoord.x >= boardData.topLeftPos_.x && originCoord.x < bottomRightPos.x) &&
            (originCoord.y >= boardData.topLeftPos_.y && originCoord.y < bottomRightPos.y))
        {
            return true;
        }

        return false;
    }

    BoardCell ReversiLogic::ConvertCoordinatesToGridCell(const BoardData& boardData, Vec2 originCoord)
    {
        BoardCell cell{};
        const Vec2 boardCoord{ originCoord - boardData.topLeftPos_ };
        cell.col_ = Math::FloorForInt(Math::Divide(boardCoord.x, static_cast<double>(boardData.pieceSize_.x)));
        cell.row_ = Math::FloorForInt(Math::Divide(boardCoord.y, static_cast<double>(boardData.pieceSize_.y)));
        return cell;
    }

    Vec2 ReversiLogic::ConvertGridCellToCenterCoord(const BoardData& boardData, const BoardCell& cell)
    {
        Vec2 coord{};
        coord.x = static_cast<double>(boardData.topLeftPos_.x) + static_cast<double>(boardData.pieceSize_.x) / 2;
        coord.x += cell.col_ * boardData.pieceSize_.x;
        coord.y = static_cast<double>(boardData.topLeftPos_.y) + static_cast<double>(boardData.pieceSize_.y) / 2;
        coord.y += cell.row_ * boardData.pieceSize_.y;
        return Math::RoundForVec2(coord, 2);
    }

    Vec2 ReversiLogic::ConvertCoordPosToCenterCoord(const BoardData& boardData, Vec2 originCoord)
    {
        const BoardCell cell{ ConvertCoordinatesToGridCell(boardData, originCoord) };
        return ConvertGridCellToCenterCoord(boardData, cell);
    }

    TurnResult ReversiLogic::CheckAvailablePlaceCell(GameState& gameState, StoneType checkStoneType)
    {
        gameState.ClearPlaceableCells();

        for (int row = 0; row < BattleBoard::BoardRowCellCount; ++row)
        {
            for (int col = 0; col < BattleBoard::BoardColCellCount; ++col)
            {
                const BoardCell checkCell{ col , row };
                const std::optional<Stone>& stone = gameState.GetStone(checkCell);
                if (stone.has_value())
                {
                    continue;
                }

                const std::vector<BoardCell>& cellOfFlipOverList = TryGetCellOfFlipOverStone(gameState, checkStoneType, checkCell);
                if (cellOfFlipOverList.empty())
                {
                    continue;
                }
                gameState.TryAddPlaceableCell(checkCell);
                gameState.AddFlipStone(checkCell, cellOfFlipOverList);
            }
        }

        if (gameState.IsAnyPlaceableCell())
        {
            // 配置可能なマスがある場合
            // ・プレイヤーを切り替えてゲーム継続
            gameState.ResetSkipCount();
            return TurnResult::Continue;
        }
        else
        {
            // 配置可能なマスが無い場合
            // ・全てのマスに石が配置された場合はゲーム終了
            // ・スキップ回数が1回目ならプレイヤー切り替えをスキップ
            // ・スキップ回数が2回連続、つまり両者配置できない場合はゲーム終了
            if (gameState.IsPlacedAllStone())
            {
                gameState.ResetSkipCount();
                return TurnResult::GameEnd;
            }

            gameState.AddSkipCount();
            if (gameState.IsSkippedTwice())
            {
                return TurnResult::GameEnd;
            }
            else
            {
                return TurnResult::Skip;
            }
        }
    }

    std::vector<BoardCell> ReversiLogic::TryGetCellOfFlipOverStone(GameState& gameState, StoneType checkStoneType, const BoardCell& checkCell)
    {
        std::queue<BoardCell> turnOverPos{};
        std::vector<BoardCell> turnOverAllPos{};

        std::unordered_map<Direction, BoardCell> directionList;
        directionList.reserve(8);
        directionList.emplace(Direction::Up, BoardCell{ 0, -1 });
        directionList.emplace(Direction::UpperRight, BoardCell{ 1, -1 });
        directionList.emplace(Direction::Right, BoardCell{ 1, 0 });
        directionList.emplace(Direction::LowerRight, BoardCell{ 1, 1 });
        directionList.emplace(Direction::Down, BoardCell{ 0, 1 });
        directionList.emplace(Direction::LowerLeft, BoardCell{ -1, 1 });
        directionList.emplace(Direction::Left, BoardCell{ -1, 0 });
        directionList.emplace(Direction::UpperLeft, BoardCell{ -1, -1 });

        for (const auto& [direction, directionOffset] : directionList)
        {
            // 変数   : 型(クラス)
            // direction       : Direction
            // directionOffset : BoardCell

            BoardCell checkPos{ checkCell };
            turnOverPos = std::queue<BoardCell>{};

            while (true)
            {
                checkPos.col_ += directionOffset.col_;
                checkPos.row_ += directionOffset.row_;

                if ((checkPos.col_ < 0 || checkPos.col_ >= BattleBoard::BoardColCellCount) ||
                    (checkPos.row_ < 0 || checkPos.row_ >= BattleBoard::BoardRowCellCount))
                {
                    break;
                }

                const std::optional<Stone>& stone = gameState.GetStone(checkPos);
                if (!stone.has_value())
                {
                    break;
                }
                const StoneType stone_type{ stone->GetStoneType() };
                if (stone_type != checkStoneType)
                {
                    turnOverPos.push(checkPos);
                }
                else
                {
                    while (!turnOverPos.empty())
                    {
                        turnOverAllPos.push_back(turnOverPos.front());
                        turnOverPos.pop();
                    }
                    break;
                }
            }
        }
        return turnOverAllPos;
    }

    bool ReversiLogic::FlipOverStone(GameState& gameState, StoneType changeStoneType)
    {
        std::optional<const PlacedStoneInfo> placedStoneInfo = gameState.GetPlacedStone();
        if (!placedStoneInfo.has_value())
        {
            return true;
        }

        std::optional<std::span<const BoardCell>> flipStoneList = gameState.TryGetFlipStoneList(placedStoneInfo->placedCell_);
        if (!flipStoneList.has_value())
        {
            return true;
        }

        std::unordered_map<Direction, BoardCell> directionList;
        directionList.reserve(8);
        directionList.emplace(Direction::Up, BoardCell{ 0, -1 });
        directionList.emplace(Direction::UpperRight, BoardCell{ 1, -1 });
        directionList.emplace(Direction::Right, BoardCell{ 1, 0 });
        directionList.emplace(Direction::LowerRight, BoardCell{ 1, 1 });
        directionList.emplace(Direction::Down, BoardCell{ 0, 1 });
        directionList.emplace(Direction::LowerLeft, BoardCell{ -1, 1 });
        directionList.emplace(Direction::Left, BoardCell{ -1, 0 });
        directionList.emplace(Direction::UpperLeft, BoardCell{ -1, -1 });

        bool isExistsFlipStone{ false };
        BoardCell checkCell{ placedStoneInfo->placedCell_ };
        const BoardCell placedCell{ placedStoneInfo->placedCell_ };
        const int flipStoneIndex{ gameState.GetCheckRange() };

        // 配置した石の一番近い周囲マスから順番にひっくり返す石があるか調べる
        // 一度の処理で全てを調査するのではなく、一定時間置きに実施して、
        // 配置した位置から放射上に調査範囲を伸ばしていく
        // 時間管理は呼び出し元で実施し、ここでは調査だけ実施する
        for (const auto& [direction, directionOffset] : directionList)
        {
            // 変数   : 型(クラス)
            // direction       : Direction
            // directionOffset : BoardCell

            checkCell.col_ = placedCell.col_ + flipStoneIndex * directionOffset.col_;
            checkCell.row_ = placedCell.row_ + flipStoneIndex * directionOffset.row_;

            const bool canBeFlip = gameState.CanBeFlippedOver(placedCell, checkCell);
            if (!canBeFlip)
            {
                continue;
            }

            std::optional<Stone>& stone = gameState.GetStone(checkCell);
            if (!stone.has_value())
            {
                continue;
            }

            const StoneType stoneType{ stone->GetStoneType() };
            if (stoneType != changeStoneType)
            {
                isExistsFlipStone = true;
                stone->ChangeStoneType();
                stone->StartFlipping();
            }
        }

        // 調査後、ひっくり返す石があったなら調査量をインクリメントする
        // 調査後、ひっくり返す石が見つからなかったら調査量とひっくり返す石リストをクリアする
        if (isExistsFlipStone)
        {
            gameState.ExpandCheckRange();
            return false;
        }
        else
        {
            gameState.ResetFlipStone();
            return true;
        }
    }

    void ReversiLogic::CheckStoneCountAfterFlipOver(GameState& gameState)
    {
        std::optional<const PlacedStoneInfo> placedStoneInfo = gameState.GetPlacedStone();
        if (!placedStoneInfo.has_value())
        {
            return;
        }

        std::optional<std::span<const BoardCell>> flipStoneList = gameState.TryGetFlipStoneList(placedStoneInfo->placedCell_);
        if (!flipStoneList.has_value())
        {
            return;
        }

        // 黒と白の石の数を数える
        int whiteStoneCount{ 0 };
        int blackStoneCount{ 0 };
        for (int row = 0; row < BattleBoard::BoardRowCellCount; ++row)
        {
            for (int col = 0; col < BattleBoard::BoardColCellCount; ++col)
            {
                const BoardCell checkCell{ col , row };
                const std::optional<Stone>& stone = gameState.GetStone(checkCell);
                if (!stone.has_value())
                {
                    continue;
                }

                const StoneType currentStoneType{ stone.value().GetStoneType() };
                const auto& end = flipStoneList->end();

                // ひっくり返る石に含まれる = 現在の色とは反対の色として加算
                // ひっくり返る石に含まれない = 現在の色で加算
                const bool isFlipped = std::ranges::find(flipStoneList.value(), checkCell) != end;
                StoneType finalStoneType{ currentStoneType };
                if (isFlipped)
                {
                    finalStoneType = currentStoneType == StoneType::Black ? StoneType::White : StoneType::Black;
                }
                switch (finalStoneType)
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
        }

        gameState.SetStoneCount(StoneType::Black, blackStoneCount);
        gameState.SetStoneCount(StoneType::White, whiteStoneCount);
    }

    void ReversiLogic::PlacedStone(GameState& gameState, StoneType stoneType, const BoardCell& cell)
    {
        const BoardData& boardData = gameState.GetBoardData();
        const Vec2 stonePos = ConvertGridCellToCenterCoord(boardData, cell);

        // リバーシの盤上の配置された石のマップを更新する
        Stone stone{};
        stone.SetStoneType(stoneType);
        stone.SetPos(stonePos);
        stone.Visible();
        gameState.UpdateStoneMap(cell, stone);

        // 配置した石情報をスタックに格納
        PlacedStoneInfo stoneInfo{};
        stoneInfo.placedCell_ = cell;
        stoneInfo.stoneTypeWhenPlaced_ = stoneType;
        gameState.PlacedStone(stoneInfo);
    }

    EvaluateResult ReversiLogic::CalculateEvaluationData(GameState& gameState, const BoardCell& placeableCell)
    {
        EvaluateResult returnResult{};

        constexpr int boardRowCount{ BattleBoard::BoardRowCellCount };
        constexpr int boardColCount{ BattleBoard::BoardColCellCount };

        std::vector<std::vector<StoneType>> currentBoard{};
        currentBoard.reserve(boardRowCount);
        const std::vector<std::vector<StoneType>> Initboard(boardRowCount, std::vector<StoneType>(boardColCount));
        currentBoard = Initboard;

        for (int row = 0; row < boardRowCount; ++row)
        {
            for (int col = 0; col < boardColCount; ++col)
            {
                const BoardCell checkCell{ col , row };
                const std::optional<const Stone> stone = gameState.GetStone(checkCell);
                if (!stone.has_value())
                {
                    continue;
                }
                const StoneType stoneType{ stone.value().GetStoneType() };
                currentBoard.at(row).at(col) = stoneType;
            }
        }

        const StoneType currentStoneType{ gameState.GetCurrentTurn() };
        // 計算用のフィールド情報を用意
        std::vector<std::vector<StoneType>> checkBoard;

        // フィールド情報を初期化
        checkBoard = currentBoard;

        DebugCheckCurrentStoneMap(checkBoard);

        // 石を配置して盤面をひっくり返した後の状態にする
        checkBoard.at(placeableCell.row_).at(placeableCell.col_) = currentStoneType;
        std::wstring message{ std::format(L"配置箇所 (列:{},行:{})", placeableCell.col_, placeableCell.row_) };
        Logger::DebugWriteLine(message);
        const std::optional<std::span<const BoardCell>> flipStoneList = gameState.TryGetFlipStoneList(placeableCell);
        assert(flipStoneList.has_value());
        for (const BoardCell& flipStoneCell : flipStoneList.value())
        {
            checkBoard.at(flipStoneCell.row_).at(flipStoneCell.col_) = currentStoneType;
            message = std::format(L"反転箇所(列:{}, 行:{})", flipStoneCell.col_, flipStoneCell.row_);
            Logger::DebugWriteLine(message);
        }
        DebugCheckCurrentStoneMap(checkBoard);

        //---------------------------------
        //評価処理
        //---------------------------------
        returnResult.value_ = CalculateBoardScore(FieldWeight, checkBoard, currentStoneType);

        //---------------------------------
        //確定石調査 ※外周の辺のみ調査
        //---------------------------------
        const std::pair<int, int> confirmData{ CountConfirmationStone(checkBoard) };
        const int confirmBlack{ confirmData.first };
        const int confirmWhite{ confirmData.second };

        message = std::format(L"配置箇所 (列:{},行:{}) , 確定石  黒:{}, 白:{}", placeableCell.col_, placeableCell.row_, confirmBlack, confirmWhite);
        Logger::DebugWriteLine(message);

        // 確定石の差分を取得
        switch (currentStoneType) {
        case StoneType::Black: {
            returnResult.confirmCount_ = confirmBlack - confirmWhite;
            break;
        }
        case StoneType::White: {
            returnResult.confirmCount_ = confirmWhite - confirmBlack;
            break;
        }
        }
        return returnResult;
    }

    int ReversiLogic::CalculateBoardScore(const std::vector<std::vector<int>>& fieldWeight,
                                          const std::vector<std::vector<StoneType>>& checkBoard,
                                          const StoneType& currentStoneType)
    {
        constexpr int defaultValue{ 0 };
        int evaluationValue{ defaultValue };
        for (int row = 0; row < BattleBoard::BoardRowCellCount; ++row)
        {
            for (int col = 0; col < BattleBoard::BoardColCellCount; ++col)
            {
                // 正負の符号
                int evaluationSign{ 0 };
                const StoneType& stoneType = checkBoard.at(row).at(col);

                // 指定座標が置いた石と同じ種類なら符号は「プラス」にする
                // 指定座標が置いた石と異なる種類なら符号は「マイナス」にする
                const int signPlus{ +1 };
                const int signMinus{ -1 };
                if (stoneType == StoneType::Empty)
                {
                    continue;
                }
                else if (stoneType == currentStoneType)
                {
                    evaluationSign = signPlus;
                }
                else
                {
                    evaluationSign = signMinus;
                }

                // 各マスの重みを考慮して評価値に加算していく
                const int wightValue{ fieldWeight.at(row).at(col) };
                evaluationValue += evaluationSign * wightValue;
            }
        }

        return evaluationValue;
    }

    std::pair<int, int> ReversiLogic::CountConfirmationStone(const std::vector<std::vector<StoneType>>& checkBoard)
    {
        // 確定石格納配列
        std::vector<BoardCell> confirmStoneList;
        // 四隅
        std::unordered_map<CornerName, BoardCell> cornerList;
        cornerList.emplace(CornerName::TopLeft, BoardCell{ 0, 0 });   // 盤の左上
        cornerList.emplace(CornerName::TopRight, BoardCell{ 7, 0 });   // 盤の右上
        cornerList.emplace(CornerName::BottomRight, BoardCell{ 7, 7 });   // 盤の右下
        cornerList.emplace(CornerName::BottomLeft, BoardCell{ 0, 7 });   // 盤の左下

        // 四隅から盤上の外周の石を一辺ずつ調査し、確定石が存在するか確認する
        for (const auto& [cornerName, checkCell] : cornerList)
        {
            // 変数   : 型(クラス)
            // cornerName : CornerName
            // checkCell  : BoardCell

            // 隅のマスに石がなければ処理終了
            if (checkBoard.at(checkCell.row_).at(checkCell.col_) == StoneType::Empty)
            {
                continue;
            }

            std::vector<BoardCell> directionList{};

            switch (cornerName)
            {
            case CornerName::TopLeft:
            {
                directionList.emplace_back(1, 0);
                directionList.emplace_back(0, 1);
                break;
            }
            case CornerName::TopRight:
            {
                directionList.emplace_back(-1, 0 );
                directionList.emplace_back(0, 1);
                break;
            }
            case CornerName::BottomLeft:
            {
                directionList.emplace_back(1, 0);
                directionList.emplace_back(0, -1);
                break;
            }
            case CornerName::BottomRight:
            {
                directionList.emplace_back( -1, 0);
                directionList.emplace_back(0, -1);
                break;
            }
            }

            for (const BoardCell& direction : directionList)
            {
                std::vector<BoardCell> stoneCellList = FindStableStonesFromCorners(checkBoard, checkCell, direction);
                const int listSize{ static_cast<int>(stoneCellList.size()) };
                static_assert(BattleBoard::BoardColCellCount == BattleBoard::BoardRowCellCount);
                constexpr int maxIndex{ BattleBoard::BoardColCellCount - 1 };
                if (listSize < maxIndex)
                {
                    // 外周の一辺に対して石が埋まり切っていない場合は、
                    // リストに格納した石の種類をひとつずつ遡る
                    // 未確定の石が含まれる場合は破棄する
                    bool isExistsUnconfirmed{ false };
                    std::vector<BoardCell> pendingCellList{};
                    StoneType prevStoneType{};
                    while (!stoneCellList.empty())
                    {
                        const BoardCell& cell = stoneCellList.back();
                        const StoneType stoneType{ checkBoard.at(cell.row_).at(cell.col_) };
                        if (prevStoneType != StoneType::Empty)
                        {
                            if (stoneType != prevStoneType)
                            {
                                isExistsUnconfirmed = true;
                                pendingCellList.clear();
                                break;
                            }
                        }
                        stoneCellList.pop_back();
                        prevStoneType = stoneType;
                        pendingCellList.push_back(cell);
                    }

                    if (!isExistsUnconfirmed)
                    {
                        stoneCellList = std::move(pendingCellList);
                    }
                }

                for (const BoardCell& cell : stoneCellList)
                {
                    // 既に確定石として登録済みか確認し、未登録なら登録する
                    const bool isContains = std::ranges::find(confirmStoneList, cell) != confirmStoneList.end();
                    if (isContains)
                    {
                        continue;
                    }
                    confirmStoneList.push_back(cell);
                }
            }
        }

        int blackStoneCount{};
        int whiteStoneCount{};
        for (const BoardCell& cell : confirmStoneList)
        {
            const StoneType stoneType{ checkBoard.at(cell.row_).at(cell.col_) };
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

        return std::make_pair(blackStoneCount, whiteStoneCount);
    }

    std::vector<BoardCell> ReversiLogic::FindStableStonesFromCorners(const std::vector<std::vector<StoneType>>& checkBoard,
                                                  const BoardCell& originCell,
                                                  const BoardCell& direction)
    {
        // 座標を一旦格納しておく配列
        std::vector<BoardCell> tempStoneList;
        tempStoneList.push_back(originCell);
        BoardCell checkCell{ originCell };

        while (true)
        {
            // マスを指定方向へ進める
            // 0:上方向(0, -1), 1:右方向(1, 0), 2:下方向(0, 1), 3:左方向(-1, 0)
            // 石が存在しない場合はループを抜ける
            // 石が存在する場合は石の種類によらずリストに格納する

            checkCell.col_ += direction.col_;
            checkCell.row_ += direction.row_;

            if (checkBoard.at(checkCell.row_).at(checkCell.col_) == StoneType::Empty)
            {
                break;
            }
            else
            {
                tempStoneList.push_back(checkCell);
            }

            BoardCell amountInvestigated{};
            amountInvestigated.col_ = Math::AbsDiff(checkCell.col_, originCell.col_);
            amountInvestigated.row_ = Math::AbsDiff(checkCell.row_, originCell.row_);

            // 盤の端まで調査し終えたら処理終了
            constexpr int maxColIndex{ BattleBoard::BoardColCellCount - 1 };
            constexpr int maxRowIndex{ BattleBoard::BoardRowCellCount - 1 };
            if (amountInvestigated.col_ >= maxColIndex ||
                amountInvestigated.row_ >= maxRowIndex)
            {
                break;
            }
        }

        return tempStoneList;
    }

    void ReversiLogic::DebugCheckCurrentStoneMap(const std::vector<std::vector<StoneType>>& checkMap)
    {
        std::wstring message{};
        message = L"-----------------------------------\n";
        Logger::DebugWriteLine(message);

        for (int row = 0; row < BattleBoard::BoardRowCellCount; ++row)
        {
            std::wstring wstrPlacedInfo{};
            for (int col = 0; col < BattleBoard::BoardColCellCount; ++col)
            {
                StoneType stoneType = checkMap.at(row).at(col);
                wstrPlacedInfo += std::format(L"{} ", static_cast<int>(stoneType));
            }
            message = L"PlacedPos   " + wstrPlacedInfo;
            Logger::DebugWriteLine(message);
        }
    }
}
