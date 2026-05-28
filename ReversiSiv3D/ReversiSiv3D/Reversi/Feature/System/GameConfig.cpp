#include "Feature/System/GameConfig.h"
#include <cassert>

namespace ReversiSiv3D {

    GameConfig::GameConfig()
        : windowSettings_{}
        , playerDataList_{}
        , soundVolume_{}
        , startupLevel_{ LevelType::BootLevel }
    {

    }

    GameConfig::~GameConfig()
    {

    }

    void GameConfig::SetGameWindowSettings(const GameWindowSettings& windowSettings)
    {
        windowSettings_ = windowSettings;
    }

    void GameConfig::SetSoundVolume(double volume)
    {
        soundVolume_ = volume;
    }

    void GameConfig::SetStartupLevel(LevelType levelType)
    {
        startupLevel_ = levelType;
    }

    void GameConfig::AddPlayData(StoneType stoneType, const PlayerData& playerData)
    {
        playerDataList_.emplace(stoneType, playerData);
    }

    void GameConfig::UpdateStoneCount(StoneType stoneType, int32 count)
    {
        assert(playerDataList_.contains(stoneType));
        PlayerData& playerData = playerDataList_.at(stoneType);
        playerData.stoneCount_ = count;
    }

    const GameWindowSettings& GameConfig::GetWindowSettings() const
    {
        return windowSettings_;
    }

    const PlayerData& GameConfig::GetPlayerData(StoneType stoneType) const
    {
        assert(playerDataList_.contains(stoneType));
        return playerDataList_.at(stoneType);
    }

    const double GameConfig::GetSoundVolume() const
    {
        return soundVolume_;
    }

    LevelType GameConfig::GetStartupLevel() const
    {
        return startupLevel_;
    }

    bool GameConfig::IsExistsPlayerData(StoneType stoneType) const
    {
        return playerDataList_.contains(stoneType);
    }
}
