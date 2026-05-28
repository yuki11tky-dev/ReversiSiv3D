#include "Feature/Common/AssetManager.h"
#include "Feature/Game/GameInfo.h"
#include "Feature/Logger/Logger.h"
#include "Feature/System/GameConfigLoader.h"
#include "Feature/System/SoundManager.h"
#include <cassert>

namespace ReversiSiv3D {

    GameConfigLoader::GameConfigLoader()
    {

    }

    GameConfigLoader::~GameConfigLoader()
    {

    }

    bool GameConfigLoader::LoadGameConfig()
    {
        GameConfig gameConfig{};
        // ウィンドウサイズの以下3種類のみ対応
        // 1. 1920 : 1080
        // 2. 1200 :  900
        // 3.  800 :  600
        const GameWindowSettings defaultWindowSettings{ GameConfig::DefaultWindowWidth, GameConfig::DefaultWindowHeight };
        gameConfig.SetGameWindowSettings(defaultWindowSettings);
        // サウンドボリュームは 0.0(最小)～1.0(最大) の間で設定すること
        gameConfig.SetSoundVolume(GameConfig::DefaultSoundVolume);
        gameConfig.SetStartupLevel(LevelType::BootLevel);

        // JSON型はSiv3D内で用意されているJSON形式のデータを扱う型
        // Siv3Dのクラスでは正しく処理されたかどうかの確認に関数 isEmpty が用意されている
        // isEmpty は異常時にあらゆる例外を投げず false を返す仕様で設計されている
        JSON configJson = JSON::Load(ConfigJsonPath);
        if (configJson.isEmpty())
        {
            // 読み取ろうとするファイルが存在しない、
            // 読み取ったデータがJSON形式ではないなどの異常がある場合は
            // デフォルトデータでコンフィグファイルを出力する
            SaveConfig(gameConfig);
        }
        else
        {
            // CheckLoadData ではJSON形式が正しいかどうか
            // 設定されている値の型が正しいかなど形式に関する確認を実施する
            // 不正な形式が含まれる場合は正しい形式の情報に置き換えて返却される
            const bool isLoadSuccess = CheckLoadData(configJson);

            GameWindowSettings loadWindowSettings{};
            loadWindowSettings.width_ = configJson[U"WindowSize"][U"width"].get<int32>();
            loadWindowSettings.height_ = configJson[U"WindowSize"][U"height"].get<int32>();
            double loadSoundVolmue = configJson[U"Sound"][U"volume"].get<double>();
            LevelType loadLevelType = static_cast<LevelType>(configJson[U"Debug"][U"StartLevel"].get<int32>());

            gameConfig.SetGameWindowSettings(loadWindowSettings);
            gameConfig.SetSoundVolume(loadSoundVolmue);
            gameConfig.SetStartupLevel(loadLevelType);

            // CheckConfigData では本ゲームで使用する正しい設定値が
            // 設定されているかどうかをチェックする
            // 不正な形式が含まれる場合は正しい形式の情報に置き換えて返却される
            const bool isConfigSuccess = CheckConfigData(gameConfig);
            if (!isLoadSuccess || !isConfigSuccess)
            {
                SaveConfig(gameConfig);
            }
        }

        GameInfo::SetGameConfig(gameConfig);
        AssetManager::CalculateWindowScale();
        SoundManager::SetupSoundPathList();

#ifndef NDEBUG

        Logger::Used();
        std::vector<LogType> logTypeList{ LogType::Debug };
        std::vector<LogDetail> logDetailList{ LogDetail::Unspecified };
        Logger::SetOutputLogType(logTypeList);
        Logger::SetOutputLogDetail(logDetailList);
#endif
        return true;
    }

    bool GameConfigLoader::CheckConfigData(GameConfig& gameConfig)
    {
        bool isSuccess{ true };
        const GameWindowSettings& windowSettings = gameConfig.GetWindowSettings();
        if (!CheckWindowSettings(windowSettings))
        {
            isSuccess = false;
            GameWindowSettings defaultSettings{ GameConfig::DefaultWindowWidth, GameConfig::DefaultWindowHeight };
            gameConfig.SetGameWindowSettings(defaultSettings);
        }

        const double& soundVolume = gameConfig.GetSoundVolume();
        if (!CheckSoundVolume(soundVolume))
        {
            isSuccess = false;
            gameConfig.SetSoundVolume(GameConfig::DefaultSoundVolume);
        }

        const LevelType& levelType = gameConfig.GetStartupLevel();
        if (!CheckStartupLevel(levelType))
        {
            isSuccess = false;
            gameConfig.SetStartupLevel(LevelType::BootLevel);
        }
        return isSuccess;
    }

    bool GameConfigLoader::CheckWindowSettings(GameWindowSettings windowSettings)
    {
        if (!((windowSettings.width_ == 1920 && windowSettings.height_ == 1080) ||
              (windowSettings.width_ == 1200 && windowSettings.height_ == 900) ||
              (windowSettings.width_ == 800 && windowSettings.height_ == 600)))
        {
            return false;
        }
        return true;
    }

    bool GameConfigLoader::CheckSoundVolume(double volume)
    {
        if (volume < 0.0 || volume > 1.0)
        {
            return false;
        }
        return true;
    }

    bool GameConfigLoader::CheckStartupLevel(LevelType levelType)
    {
        if (!(levelType == LevelType::BootLevel || levelType == LevelType::TitleLevel ||
            levelType == LevelType::BattleLevel || levelType == LevelType::ResultLevel))
        {
            return false;
        }
        return true;
    }

    bool GameConfigLoader::CheckLoadData(JSON& inConfigJson)
    {
        bool isSuccess{ true };
        if (!inConfigJson.contains(U"WindowSize"))
        {
            isSuccess = false;
            inConfigJson[U"WindowSize"][U"width"] = GameConfig::DefaultWindowWidth;
            inConfigJson[U"WindowSize"][U"height"] = GameConfig::DefaultWindowHeight;
        }
        else
        {
            const Optional<int32> widthOpt = inConfigJson[U"WindowSize"][U"width"].getOpt<int32>();
            const Optional<int32> heightOpt = inConfigJson[U"WindowSize"][U"height"].getOpt<int32>();
            if (!widthOpt || !heightOpt)
            {
                isSuccess = false;
                inConfigJson[U"WindowSize"][U"width"] = GameConfig::DefaultWindowWidth;
                inConfigJson[U"WindowSize"][U"height"] = GameConfig::DefaultWindowHeight;
            }
        }

        if (!inConfigJson.contains(U"Sound"))
        {
            isSuccess = false;
            inConfigJson[U"Sound"][U"volume"] = GameConfig::DefaultSoundVolume;
        }
        else
        {
            const Optional<double> volumeOpt = inConfigJson[U"Sound"][U"volume"].getOpt<double>();
            if (!volumeOpt)
            {
                isSuccess = false;
                inConfigJson[U"Sound"][U"volume"] = GameConfig::DefaultSoundVolume;
            }
        }

        if (!inConfigJson.contains(U"Debug"))
        {
            isSuccess = false;
            inConfigJson[U"Debug"][U"StartLevel"] = static_cast<int32>(LevelType::BootLevel);
        }
        else
        {
            const Optional<int32> intLevelTypeOpt = inConfigJson[U"Debug"][U"StartLevel"].getOpt<int32>();
            if (!intLevelTypeOpt)
            {
                isSuccess = false;
                inConfigJson[U"Debug"][U"StartLevel"] = static_cast<int32>(LevelType::BootLevel);
            }
        }

        return isSuccess;
    }

    JSON GameConfigLoader::SaveConfig(const GameConfig& gameConfig)
    {
        JSON newConfigJson{};
        newConfigJson[U"WindowSize"][U"width"] = gameConfig.GetWindowSettings().width_;
        newConfigJson[U"WindowSize"][U"height"] = gameConfig.GetWindowSettings().height_;
        newConfigJson[U"Sound"][U"volume"] = gameConfig.GetSoundVolume();
        newConfigJson[U"Debug"][U"StartLevel"] = static_cast<int>(gameConfig.GetStartupLevel());
        newConfigJson.save(ConfigJsonPath);

        return newConfigJson;
    }
}
