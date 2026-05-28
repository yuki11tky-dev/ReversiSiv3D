#include "Feature/Game/GameInfo.h"
#include "Feature/System/SoundManager.h"

namespace ReversiSiv3D {

    void SoundManager::SetupSoundPathList()
    {
        soundPathList_.emplace(SoundType::TitleLevelBgm, U"Audio/carnelian.mp3");
        soundPathList_.emplace(SoundType::BattleLevelBgm, U"Audio/larimar.mp3");
        soundPathList_.emplace(SoundType::ResultLevelBgm, U"Audio/hello_world.mp3");

        soundPathList_.emplace(SoundType::ButtonClickSe, U"Audio/game_start.mp3");
        soundPathList_.emplace(SoundType::PlacedStoneSe, U"Audio/placedstone.mp3");
        soundPathList_.emplace(SoundType::ResultWinSe, U"Audio/winner.mp3");
        soundPathList_.emplace(SoundType::ResultLoseSe, U"Audio/loser.mp3");
        soundPathList_.emplace(SoundType::ResultDrawSe, U"Audio/hand_clap.mp3");
    }

    void SoundManager::Clear()
    {
        for (const auto& [type, audioData] : soundDataList_)
        {
            // type  : SoundType
            // audio : SoundData
            if (audioData.audio_.isPlaying())
            {
                audioData.audio_.pause();
            }
        }
        soundDataList_.clear();
    }

    void SoundManager::PlayOneShot(SoundType soundType)
    {
        // サウンドエフェクト用の実行関数

        if (!soundDataList_.contains(soundType))
        {
            std::optional<Audio> audio = GenerateSound(soundType);
            if (!audio.has_value())
            {
                return;
            }
            SoundData soundData{};
            soundData.soundRole_ = SoundRole::SE;
            soundData.audio_ = audio.value();
            soundDataList_.emplace(soundType, soundData);
        }
        const GameConfig& gameConfig = GameInfo::GetGameConfig();
        soundDataList_.at(soundType).audio_.playOneShot(gameConfig.GetSoundVolume());
    }

    void SoundManager::PlayLoop(SoundType soundType, Duration startTime, Duration loopStartTime, Duration loopEndTime)
    {
        // バックグラウンドミュージック用の実行関数

        if (!soundDataList_.contains(soundType))
        {
            std::optional<Audio> audio = GenerateSound(soundType, loopStartTime, loopEndTime);
            if (!audio.has_value())
            {
                return;
            }
            const GameConfig& gameConfig = GameInfo::GetGameConfig();
            const Audio& bgmAudio = audio.value().setVolume(gameConfig.GetSoundVolume());
            bgmAudio.setLoop(true);

            SoundData soundData{};
            soundData.soundRole_ = SoundRole::BGM;
            soundData.audio_ = bgmAudio;
            soundDataList_.emplace(soundType, soundData);
        }
        soundDataList_.at(soundType).audio_.seekTime(startTime);
        soundDataList_.at(soundType).audio_.play();
    }

    void SoundManager::Pause(SoundType soundType)
    {
        if (!soundDataList_.contains(soundType))
        {
            return;
        }
        const SoundData& audioData = soundDataList_.at(soundType);
        if (!audioData.audio_.isPlaying())
        {
            return;
        }
        audioData.audio_.pause();
    }

    void SoundManager::FadeVolume(SoundType soundType, double volume, Duration fadeTime)
    {
        if (!soundDataList_.contains(soundType))
        {
            return;
        }
        const SoundData& audioData = soundDataList_.at(soundType);
        audioData.audio_.fadeVolume(volume, fadeTime);
    }

    std::optional<Audio> SoundManager::GenerateSound(SoundType soundType, Duration startTime, Duration endTime)
    {
        // サウンドは必ず1種類につき1つの生成を上限とする

        Audio audio;
        if (!soundPathList_.contains(soundType))
        {
            return std::nullopt;
        }
        if (startTime == 0s && endTime == 0s)
        {
            audio = Audio{ soundPathList_.at(soundType) };
        }
        else if (endTime == 0s)
        {
            audio = Audio{ soundPathList_.at(soundType), startTime };
        }
        else
        {
            audio = Audio{ soundPathList_.at(soundType), startTime, endTime };
        }

        if (audio.isEmpty())
        {
            return std::nullopt;
        }
        return audio;
    }
}
