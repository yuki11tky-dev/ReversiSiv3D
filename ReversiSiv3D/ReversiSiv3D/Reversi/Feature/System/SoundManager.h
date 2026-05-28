#pragma once

#include <optional>
#include <unordered_map>

namespace ReversiSiv3D {

    enum class SoundRole
    {
        None,
        SE,
        BGM
    };

    enum class SoundType
    {
        Empty,
        TitleLevelBgm,
        BattleLevelBgm,
        ResultLevelBgm,
        ButtonClickSe,
        PlacedStoneSe,
        ResultWinSe,
        ResultLoseSe,
        ResultDrawSe,
    };

    struct SoundData
    {
        SoundRole soundRole_;
        Audio audio_;

        SoundData(SoundRole role = SoundRole::None, Audio audio = Audio{})
            : soundRole_{ role }
            , audio_{ audio }
        {

        }
    };

    class SoundManager
    {
    public:

        static void SetupSoundPathList();
        static void Clear();
        static void PlayOneShot(SoundType);
        static void PlayLoop(SoundType, Duration startTime = 0s, Duration loopStartTime = 0s, Duration loopEndTime = 0s);
        static void Pause(SoundType);
        static void FadeVolume(SoundType, double, Duration);

    private:

        static std::optional<Audio> GenerateSound(SoundType, Duration startTime = 0s, Duration endTime = 0s);

    private:

        inline static std::unordered_map<SoundType, String> soundPathList_{};
        inline static std::unordered_map<SoundType, SoundData> soundDataList_{};
    };
}
