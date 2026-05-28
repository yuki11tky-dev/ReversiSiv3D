#pragma once

#include "Feature/Game/GameState.h"
#include "Feature/Game/UI/HUD/ScoreBoard.h"
#include "Feature/System/Task.h"
#include <memory>
#include <unordered_map>
#include <vector>

namespace ReversiSiv3D {

    class ScoreBoardManager : public Task
    {
    public:

        ScoreBoardManager();
        ~ScoreBoardManager();

        void Update(double) override;
        void Render() override;
        void CreateBoard();

        void SetStateObserver(const std::shared_ptr<GameState>&);
        void Assigned(const PlayerData& usedBlack, const PlayerData& usedWhite);
        Vec2 GetScoreBoardCenterPos(StoneType) const;

    private:

        void CompletedInitialized();
        bool TryGetGameState(std::shared_ptr<GameState>&) const;

    private:

        static constexpr Vec2 DefaultPos{ 0.0, 0.0 };
        static constexpr Vec2 PlayerPosOffset{ 625.0, 220.0 };
        static constexpr Vec2 EnemyPosOffset{ -625.0, -220.0 };
        static constexpr Vec2 PlayerPosReOffset{ -55.0, 55.0 };
        static constexpr Vec2 EnemyPosReOffset{ 55.0, -55.0 };
        static constexpr ColorF ImageDefaultColor{ 1.0, 1.0, 1.0, 1.0 };
        static constexpr ColorF WhiteTextColor{ 1.0, 1.0, 1.0, 1.0 };
        static constexpr ColorF BlackTextColor{ 0.0, 0.0, 0.0, 1.0 };
        static constexpr int32 PlayerNameSize{ 80 };
        static constexpr int32 StoneCountSize{ 120 };
        static constexpr double FullHDWindowScale{ 1.0 };

    private:

        std::unique_ptr<ScoreBoard> whiteStoneBoard_;
        std::unique_ptr<ScoreBoard> blackStoneBoard_;
        bool isInitialized_;
        std::weak_ptr<GameState> stateObserver_;
    };
}
