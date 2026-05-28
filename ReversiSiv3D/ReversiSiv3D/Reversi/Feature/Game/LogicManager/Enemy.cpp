#include "Feature/Game/LogicManager/Enemy.h"
#include "Feature/Game/ReversiLogic.h"
#include "Feature/Logger/Logger.h"
#include <format>

namespace ReversiSiv3D {

    Enemy::Enemy()
        : Task(TaskId::Enemy)
        , currentPhase_{ ThinkingPhase::Idle }
        , stateObserver_{}
        , placedCell_{}
        , elapsedTime_{}
        , checkIndex_{}
        , evaluationResultList_{}
    {

    }

    Enemy::~Enemy()
    {

    }

    void Enemy::Update(double deltaTimeSec)
    {
        switch (currentPhase_)
        {
        case ThinkingPhase::Start:
        {
            checkIndex_ = ResetIndex;
            evaluationResultList_.clear();
            ChangeThinkingPhase(ThinkingPhase::Calculate);
            break;
        }
        case ThinkingPhase::Calculate:
        {
            const bool isFinish = ComputeScore();
            if (!isFinish)
            {
                break;
            }
            ChangeThinkingPhase(ThinkingPhase::Evaluate);
            break;
        }
        case ThinkingPhase::Evaluate:
        {
            placedCell_ = EvaluateScore();

            elapsedTime_ = ResetTime;
            ChangeThinkingPhase(ThinkingPhase::WaitFinish);
            break;
        }
        case ThinkingPhase::WaitFinish:
        {
            elapsedTime_ += deltaTimeSec;
            if (elapsedTime_ < WaitTime)
            {
                break;
            }
            elapsedTime_ = ResetTime;

            ChangeThinkingPhase(ThinkingPhase::Finish);
            break;
        }
        case ThinkingPhase::Finish:
        {

            break;
        }
        }
    }

    void Enemy::ResetThinking()
    {
        currentPhase_ = ThinkingPhase::Idle;
    }

    void Enemy::StartThinking()
    {
        currentPhase_ = ThinkingPhase::Start;
    }

    bool Enemy::IsFinishThinking() const
    {
        return currentPhase_ == ThinkingPhase::Finish;
    }

    void Enemy::SetStateObserver(const std::shared_ptr<GameState>& gameState)
    {
        stateObserver_ = gameState;
    }

    const BoardCell& Enemy::GetPlacedCell() const
    {
        return placedCell_;
    }

    bool Enemy::ComputeScore()
    {
        std::shared_ptr<GameState> gameState;
        const bool isSuccess = TryGetGameState(gameState);
        if (!isSuccess)
        {
            return true;
        }

        // 配置可能な場所に対して評価を行う
        std::optional<const BoardCell> checkCell = gameState->TryGetPlaceableCellByIndex(checkIndex_);
        if (!checkCell.has_value())
        {
            return true;
        }

        const BoardCell& placeableCell = checkCell.value();
        const EvaluateResult& result = ReversiLogic::CalculateEvaluationData(*gameState, placeableCell);
        evaluationResultList_.emplace(placeableCell, result);

        ++checkIndex_;

        return false;
    }

    BoardCell Enemy::EvaluateScore()
    {
        BoardCell returnCell{};

        int evaluationMaxValue{ EvaMaxInitValue };
        for (const auto& [placedCell, evaResult] : evaluationResultList_)
        {
            // 変数   : 型(クラス)
            // placedCell : BoardCell
            // evaResult  : EvaluateResult

            // 盤の評価と確定石の差分からトータルの評価値を算出する
            const int total_value{ EvaBoardCoeffient * evaResult.value_ + ConfirmStoneCoeffient * evaResult.confirmCount_ };

            // 評価値を出力
            std::wstring message{ std::format(L"評価値  設置箇所(列:{},行:{}) : {}", placedCell.col_, placedCell.row_, total_value) };
            Logger::DebugWriteLine(message);

            // 評価値の最大値と比較して今回取得した評価値の方が大きい場合、最大値を置き換える
            if (total_value > evaluationMaxValue) {
                evaluationMaxValue = total_value;
                returnCell = placedCell;
            }
        }

        // 評価値を出力
        std::wstring message{ std::format(L"☆ 評価値最大  設置箇所(列:{},行:{}) : {}", returnCell.col_, returnCell.row_, evaluationMaxValue) };
        Logger::DebugWriteLine(message);

        return returnCell;
    }

    void Enemy::ChangeThinkingPhase(ThinkingPhase phase)
    {
        currentPhase_ = phase;
    }

    bool Enemy::TryGetGameState(std::shared_ptr<GameState>& outGameState) const
    {
        outGameState = stateObserver_.lock();
        return static_cast<bool>(outGameState);
    }
}
