#include "Feature/Common/AssetManager.h"
#include "Feature/Game/GameInfo.h"
#include "Feature/Game/LogicManager/GameResultManager.h"
#include <cassert>
#include <memory>

namespace ReversiSiv3D {

    GameResultManager::GameResultManager()
        : Task{ TaskId::GameResult }
        , stateObserver_{}
        , stoneAnimator_{}
        , resultDetailManager_{}
        , currentPhase_{}
    {

    }

    GameResultManager::~GameResultManager()
    {

    }

    void GameResultManager::Update(double)
    {
        switch (currentPhase_)
        {
        case ResultPhase::Start:
        {
            assert(stoneAnimator_);
            stoneAnimator_->Start();
            ChangeResultPhase(ResultPhase::StoneAnimation);
            break;
        }
        case ResultPhase::StoneAnimation:
        {
            assert(stoneAnimator_);
            const bool isFinished = stoneAnimator_->IsFinished();
            if (!isFinished)
            {
                break;
            }

            ChangeResultPhase(ResultPhase::ShowResultDetail);
            break;
        }
        case ResultPhase::ShowResultDetail:
        {
            assert(resultDetailManager_);
            resultDetailManager_->Show();
            ChangeResultPhase(ResultPhase::Finish);
            break;
        }
        case ResultPhase::Finish:
        {

            ChangeResultPhase(ResultPhase::Finished);
            break;
        }
        case ResultPhase::Finished:
        {
            break;
        }
        }
    }

    void GameResultManager::SetStateObserver(const std::shared_ptr<GameState>& gameState)
    {
        stateObserver_ = gameState;
    }
    void GameResultManager::SetStoneAnimator(std::shared_ptr<ResultStoneAnimator> stoneAnimator)
    {
        stoneAnimator_ = stoneAnimator;
    }

    void GameResultManager::SetResultDetailManager(std::shared_ptr<ResultDetailManager> detailManager)
    {
        resultDetailManager_ = detailManager;
    }

    void GameResultManager::Start()
    {
        currentPhase_ = ResultPhase::Start;
    }

    bool GameResultManager::IsFinished() const
    {
        return currentPhase_ == ResultPhase::Finished;
    }

    void GameResultManager::ChangeResultPhase(ResultPhase phase)
    {
        currentPhase_ = phase;
    }
}
