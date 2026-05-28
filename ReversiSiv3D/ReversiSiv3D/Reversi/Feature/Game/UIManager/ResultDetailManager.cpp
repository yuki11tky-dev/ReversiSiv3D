#include "Feature/Common/AssetManager.h"
#include "Feature/Common/Math.h"
#include "Feature/Game/ResourceProvider.h"
#include "Feature/Game/UIManager/ResultDetailManager.h"
#include "Feature/System/SoundManager.h"
#include <cassert>

namespace ReversiSiv3D {

    ResultDetailManager::ResultDetailManager()
        : Task{ TaskId::ResultDetail }
        , resultDetailList_{}
        , resultSeType_{}
    {

    }

    ResultDetailManager::~ResultDetailManager()
    {

    }

    void ResultDetailManager::Update(double deltaTimeSec)
    {
        for (const std::unique_ptr<ResultDetail>& detail : resultDetailList_)
        {
            assert(detail);
            detail->Update(deltaTimeSec);
        }
    }

    void ResultDetailManager::Render()
    {
        for (const std::unique_ptr<ResultDetail>& detail : resultDetailList_)
        {
            assert(detail);
            detail->Render();
        }
    }

    void ResultDetailManager::Initialize()
    {
        const GameConfig& gameConfig = GameInfo::GetGameConfig();
        const PlayerData& useBlackData = gameConfig.GetPlayerData(StoneType::Black);
        const PlayerData& useWhiteData = gameConfig.GetPlayerData(StoneType::White);

        ResultType blackResult = CheckResultOfUseBlackStone(useBlackData.stoneCount_, useWhiteData.stoneCount_);
        switch (blackResult)
        {
        case ResultType::Win:
        {
            resultDetailList_.push_back(std::move(CreateResultDetail<Win>(StoneType::Black, useBlackData)));
            resultDetailList_.push_back(std::move(CreateResultDetail<Lose>(StoneType::White, useWhiteData)));
            break;
        }
        case ResultType::Lose:
        {
            resultDetailList_.push_back(std::move(CreateResultDetail<Lose>(StoneType::Black, useBlackData)));
            resultDetailList_.push_back(std::move(CreateResultDetail<Win>(StoneType::White, useWhiteData)));
            break;
        }
        case ResultType::Draw:
        {
            resultDetailList_.push_back(std::move(CreateResultDetail<Draw>(StoneType::Black, useBlackData)));
            resultDetailList_.push_back(std::move(CreateResultDetail<Draw>(StoneType::White, useWhiteData)));
            break;
        }
        }

        SettingSound(useBlackData, useWhiteData);
    }

    void ResultDetailManager::SetupResultDetail(ResultDetail& result,
                                                StoneType stoneType,
                                                const PlayerData& playerData)
    {
        const String stoneFilePath{ ResourceProvider::GetStoneIconFilePath(stoneType) };
        const String playerName{ ResourceProvider::GetPlayerNameText(playerData.playerType_) };
        result.Initialize();
        result.LoadResultIcon();
        result.LoadStoneIcon(stoneFilePath);
        result.SetPlayerNameText(playerName);
        result.SetStoneCountText(ToString(playerData.stoneCount_));

        const Vec2 windowCenter{ Scene::CenterF() };
        Vec2 playerNamePos{ windowCenter };
        Vec2 stoneIconPos{ windowCenter };
        Vec2 stoneCountPos{ windowCenter };
        Vec2 resultIconPos{ windowCenter };

        const double windowScale{ AssetManager::GetCurrentWindowScale() };
        switch (playerData.resultLayoutSide_)
        {
        case ResultLayoutSide::Left:
        {
            if (windowScale < FullHDWindowScale)
            {
                playerNamePos += AssetManager::AdjustCurrentWindowScale(PlayerNameOffsetPos + ScaleOffsetPos) * ReverseXAxis;
                stoneIconPos += AssetManager::AdjustCurrentWindowScale(StoneIconOffsetPos + ScaleOffsetPos) * ReverseXAxis;
                stoneCountPos += AssetManager::AdjustCurrentWindowScale(StoneCountOffsetPos + ScaleOffsetPos) * ReverseXAxis;
                resultIconPos += AssetManager::AdjustCurrentWindowScale(ResultIconOffsetPos + ScaleOffsetPos) * ReverseXAxis;
            }
            else
            {
                playerNamePos += AssetManager::AdjustCurrentWindowScale(PlayerNameOffsetPos) * ReverseXAxis;
                stoneIconPos += AssetManager::AdjustCurrentWindowScale(StoneIconOffsetPos) * ReverseXAxis;
                stoneCountPos += AssetManager::AdjustCurrentWindowScale(StoneCountOffsetPos) * ReverseXAxis;
                resultIconPos += AssetManager::AdjustCurrentWindowScale(ResultIconOffsetPos) * ReverseXAxis;
            }
            break;
        }
        case ResultLayoutSide::Right:
        {
            if (windowScale < FullHDWindowScale)
            {
                playerNamePos += AssetManager::AdjustCurrentWindowScale(PlayerNameOffsetPos + ScaleOffsetPos);
                stoneIconPos += AssetManager::AdjustCurrentWindowScale(StoneIconOffsetPos + ScaleOffsetPos);
                stoneCountPos += AssetManager::AdjustCurrentWindowScale(StoneCountOffsetPos + ScaleOffsetPos);
                resultIconPos += AssetManager::AdjustCurrentWindowScale(ResultIconOffsetPos + ScaleOffsetPos);
            }
            else
            {
                playerNamePos += AssetManager::AdjustCurrentWindowScale(PlayerNameOffsetPos);
                stoneIconPos += AssetManager::AdjustCurrentWindowScale(StoneIconOffsetPos);
                stoneCountPos += AssetManager::AdjustCurrentWindowScale(StoneCountOffsetPos);
                resultIconPos += AssetManager::AdjustCurrentWindowScale(ResultIconOffsetPos);
            }
            break;
        }
        }

        constexpr int roundingDigitsZero{ 0 };
        result.SetPlayerNameCenterPos(Math::RoundForVec2(playerNamePos, roundingDigitsZero));
        result.SetStoneIconCenterPos(Math::RoundForVec2(stoneIconPos, roundingDigitsZero));
        result.SetStoneCountCenterPos(Math::RoundForVec2(stoneCountPos, roundingDigitsZero));
        result.SetResultIconCenterPos(Math::RoundForVec2(resultIconPos, roundingDigitsZero));
    }

    void ResultDetailManager::Show()
    {
        for (const std::unique_ptr<ResultDetail>& resultDetail : resultDetailList_)
        {
            resultDetail->Start();
            SoundManager::PlayOneShot(resultSeType_);
        }
    }

    ResultType ResultDetailManager::CheckResultOfUseBlackStone(int blackCount, int whiteCount) const
    {
        std::unique_ptr<ResultDetail> result;
        if (blackCount > whiteCount)
        {
            return ResultType::Win;
        }
        else if (blackCount < whiteCount)
        {
            return ResultType::Lose;
        }
        else
        {
            return ResultType::Draw;
        }
    }

    void ResultDetailManager::SettingSound(const PlayerData& useBlackData, const PlayerData& useWhiteData)
    {
        ResultType blackResult = CheckResultOfUseBlackStone(useBlackData.stoneCount_, useWhiteData.stoneCount_);
        if (useBlackData.playerType_ == PlayerType::Player1)
        {
            switch (blackResult)
            {
            case ResultType::Win:
            {
                resultSeType_ = SoundType::ResultWinSe;
                break;
            }
            case ResultType::Lose:
            {
                resultSeType_ = SoundType::ResultLoseSe;
                break;
            }
            case ResultType::Draw:
            {
                resultSeType_ = SoundType::ResultDrawSe;
                break;
            }
            }
        }
        else if (useWhiteData.playerType_ == PlayerType::Player1)
        {
            switch (blackResult)
            {
            case ResultType::Win:
            {
                resultSeType_ = SoundType::ResultLoseSe;
                break;
            }
            case ResultType::Lose:
            {
                resultSeType_ = SoundType::ResultWinSe;
                break;
            }
            case ResultType::Draw:
            {
                resultSeType_ = SoundType::ResultDrawSe;
                break;
            }
            }
        }
        else
        {
            resultSeType_ = SoundType::ResultDrawSe;
        }
    }
}
