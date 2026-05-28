#pragma once

#include "Feature/Game/SharedEnum.h"
#include "Feature/System/TaskId.h"
#include <unordered_map>
#include <vector>

namespace ReversiSiv3D {

    struct GameWindowSettings
    {
    public:

        int32 width_;
        int32 height_;

    public:

        constexpr GameWindowSettings(int32 width = 0, int32 height = 0)
            : width_{ width }
            , height_{ height }
        {

        };
    };

    struct PlayerData
    {
        PlayerType playerType_;
        int32 stoneCount_;
        ResultLayoutSide resultLayoutSide_;

        constexpr PlayerData(PlayerType playerType = PlayerType::None,
                             int32 count = 0,
                             ResultLayoutSide layoutSide = ResultLayoutSide::Left)
        {
            playerType_ = playerType;
            stoneCount_ = count;
            resultLayoutSide_ = layoutSide;
        }
    };

    class GameConfig
    {
    public:

        GameConfig();
        ~GameConfig();

        void SetGameWindowSettings(const GameWindowSettings&);
        void SetSoundVolume(double);
        void SetStartupLevel(LevelType);
        void AddPlayData(StoneType, const PlayerData&);
        void UpdateStoneCount(StoneType, int32);

        const GameWindowSettings& GetWindowSettings() const;
        const PlayerData& GetPlayerData(StoneType) const;
        const double GetSoundVolume() const;
        LevelType GetStartupLevel() const;

    public:

        bool IsExistsPlayerData(StoneType) const;

    public:

        static constexpr int32 DefaultWindowWidth{ 1920 };
        static constexpr int32 DefaultWindowHeight{ 1080 };
        static constexpr double DefaultSoundVolume{ 0.5 };

    private:

        GameWindowSettings windowSettings_;
        std::unordered_map<StoneType, PlayerData> playerDataList_;
        double soundVolume_;

    private:

        LevelType startupLevel_;
    };
}
