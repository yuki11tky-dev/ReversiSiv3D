#include "Feature/Game/ReversiLogic.h"
#include "Feature/Game/UIManager/BattleBoard.h"
#include "Feature/Game/UIManager/ResultStoneAnimator.h"
#include <cassert>

namespace ReversiSiv3D {

    ResultStoneAnimator::ResultStoneAnimator()
        : Task{ TaskId::StoneAnimator }
        , stateObserver_ {}
        , currentPhase_ {}
        , elapsedTime_{}
        , placedCount_{}
    {

    }

    ResultStoneAnimator::~ResultStoneAnimator()
    {

    }

    void ResultStoneAnimator::Update(double deltaTimeSec)
    {
        switch (currentPhase_)
        {
        case AnimationPhase::Start:
        {
            constexpr int resetCount{ 0 };
            placedCount_ = resetCount;

#ifndef NDEBUG
            GameConfig& gameConfig = GameInfo::GetGameConfig();
            [[maybe_unused]] const PlayerData& playerUsedBlack = gameConfig.GetPlayerData(StoneType::Black);
            [[maybe_unused]] const PlayerData& playerUsedWhite = gameConfig.GetPlayerData(StoneType::White);
            assert(playerUsedBlack.stoneCount_ >= 0 && playerUsedWhite.stoneCount_ >= 0);
            assert(playerUsedBlack.stoneCount_ + playerUsedWhite.stoneCount_ <= BattleBoard::BoardRowCellCount * BattleBoard::BoardColCellCount);
#endif
            ChangeAnimationPhase(AnimationPhase::ShowResult);
            break;
        }
        case AnimationPhase::ShowResult:
        {
            elapsedTime_ += deltaTimeSec;
            if (elapsedTime_ < PlaceStoneInterval)
            {
                break;
            }
            elapsedTime_ = ResetTime;

            const bool isFinished = ShowResult();
            if (!isFinished)
            {
                break;
            }
            ChangeAnimationPhase(AnimationPhase::WaitOperate);
            break;
        }
        case AnimationPhase::WaitOperate:
        {
            ChangeAnimationPhase(AnimationPhase::Finish);
            break;
        }
        case AnimationPhase::Finish:
        {
            ChangeAnimationPhase(AnimationPhase::Finished);
            break;
        }
        case AnimationPhase::Finished:
        {
            break;
        }
        }
    }

    void ResultStoneAnimator::SetStateObserver(const std::shared_ptr<GameState>& gameState)
    {
        stateObserver_ = gameState;
    }

    void ResultStoneAnimator::Start()
    {
        currentPhase_ = AnimationPhase::Start;
    }

    bool ResultStoneAnimator::IsFinished() const
    {
        return currentPhase_ == AnimationPhase::Finished;
    }

    bool ResultStoneAnimator::ShowResult()
    {
        std::shared_ptr<GameState> gameState;
        bool isSuccess = TryGetGameState(gameState);
        if (!isSuccess)
        {
            return true;
        }

        GameConfig& gameConfig = GameInfo::GetGameConfig();
        const PlayerData& playerUsedBlack = gameConfig.GetPlayerData(StoneType::Black);
        const PlayerData& playerUsedWhite = gameConfig.GetPlayerData(StoneType::White);
        const bool isCompletedBlack{ placedCount_ >= playerUsedBlack.stoneCount_ };
        const bool isCompletedWhite{ placedCount_ >= playerUsedWhite.stoneCount_ };
        if (!isCompletedBlack)
        {
            const BoardCell placedCell{ PlaceNext(playerUsedBlack.resultLayoutSide_) };
            ReversiLogic::PlacedStone(*gameState, StoneType::Black, placedCell);
        }

        if (!isCompletedWhite)
        {
            const BoardCell placedCell{ PlaceNext(playerUsedWhite.resultLayoutSide_) };
            ReversiLogic::PlacedStone(*gameState, StoneType::White, placedCell);
        }

        if (isCompletedBlack && isCompletedWhite)
        {
            return true;
        }
        else
        {
            ++placedCount_;
            assert(placedCount_ <= BattleBoard::BoardRowCellCount * BattleBoard::BoardColCellCount);
            return false;
        }
    }

    BoardCell ResultStoneAnimator::PlaceNext(ResultLayoutSide layoutSide)
    {
        const int col{ placedCount_ / BattleBoard::BoardRowCellCount };
        const int row{ placedCount_ % BattleBoard::BoardColCellCount };

        // 開始位置が左側の場合は右方向へ列数加算
        // 開始位置が右側の場合は左方向へ列数減算
        BoardCell nextCell{};
        switch (layoutSide)
        {
        case ResultLayoutSide::Left:
        {
            // 盤の左上の隅から配置する
            nextCell = StartCellLeftSide;
            nextCell.col_ += col;

            // 偶数列に配置する場合は盤の下から上へ順番に配置
            // 奇数列に配置する場合は盤の上から下へ順番に配置
            if (nextCell.col_ % 2 == 1)
            {
                const int maxRow = BattleBoard::BoardRowCellCount - 1;
                nextCell.row_ = maxRow - row;
            }
            else
            {
                nextCell.row_ = row;
            }
            break;
        }
        case ResultLayoutSide::Right:
        {
            // 盤の右上の隅から配置する
            nextCell = StartCellRightSide;
            nextCell.col_ -= col;

            // 偶数列に配置する場合は盤の下から上へ順番に配置
            // 奇数列に配置する場合は盤の上から下へ順番に配置
            if (nextCell.col_ % 2 == 0)
            {
                const int maxRow = BattleBoard::BoardRowCellCount - 1;
                nextCell.row_ = maxRow - row;
            }
            else
            {
                nextCell.row_ = row;
            }
            break;
        }
        }

        return nextCell;
    }

    void ResultStoneAnimator::ChangeAnimationPhase(AnimationPhase phase)
    {
        currentPhase_ = phase;
    }

    bool ResultStoneAnimator::TryGetGameState(std::shared_ptr<GameState>& outGameState) const
    {
        outGameState = stateObserver_.lock();
        return static_cast<bool>(outGameState);
    }
}
