#include "Feature/Common/AssetManager.h"
#include "Feature/Common/Math.h"
#include "Feature/Game/UI/Result/ResultDetail.h"
#include <cassert>

namespace ReversiSiv3D {

    ResultDetail::ResultDetail()
        : UIBase{}
        , stoneIcon_{}
        , resultIcon_{}
        , playerName_{}
        , stoneCount_{}
    {

    }

    ResultDetail::~ResultDetail()
    {

    }

    void ResultDetail::Initialize()
    {
        constexpr ColorF defaultImageColor{ 1.0, 1.0, 1.0, 1.0 };
        constexpr ColorF defaultTextColor{ 0.0, 0.0, 0.0, 1.0 };
        const double scale{ AssetManager::GetCurrentWindowScale() };

        std::shared_ptr<Image> stoneIcon = std::make_shared<Image>();
        stoneIcon->SetColor(defaultImageColor);
        stoneIcon->SetScale(scale);

        // 所有権は基底クラスであるUIBaseが持つ
        // 本クラスでは弱参照として保持する
        stoneIcon_ = stoneIcon;
        AddDrawable(std::move(stoneIcon));

        std::shared_ptr<Image> resultIcon = std::make_shared<Image>();
        resultIcon->SetColor(defaultImageColor);
        resultIcon->SetScale(scale);

        // 所有権は基底クラスであるUIBaseが持つ
        // 本クラスでは弱参照として保持する
        resultIcon_ = resultIcon;
        AddDrawable(std::move(resultIcon));

        constexpr int32 textSize{ 80 };
        const int32 textResize{ Math::RoundForInt(AssetManager::AdjustCurrentWindowScale(textSize)) };
        std::shared_ptr<Text> playerName = std::make_shared<Text>();
        playerName->SetFont(FontMethod::MSDF, textResize, Typeface::Bold);
        playerName->SetColor(defaultTextColor);

        // 所有権は基底クラスであるUIBaseが持つ
        // 本クラスでは弱参照として保持する
        playerName_ = playerName;
        AddDrawable(std::move(playerName));

        std::shared_ptr<Text> stoneCount = std::make_shared<Text>();
        stoneCount->SetFont(FontMethod::MSDF, textResize, Typeface::Bold);
        stoneCount->SetColor(defaultTextColor);

        // 所有権は基底クラスであるUIBaseが持つ
        // 本クラスでは弱参照として保持する
        stoneCount_ = stoneCount;
        AddDrawable(std::move(stoneCount));

        Hide();
    }

    void ResultDetail::Visible()
    {
        constexpr double MaxAlphaValue{ 1.0 };
        SetStoneIconAlpha(MaxAlphaValue);
        SetResultIconAlpha(MaxAlphaValue);
        SetPlayerNameAlpha(MaxAlphaValue);
        SetStoneCountAlpha(MaxAlphaValue);
    }

    void ResultDetail::Hide()
    {
        constexpr double MinAlphaValue{ 0.0 };
        SetStoneIconAlpha(MinAlphaValue);
        SetResultIconAlpha(MinAlphaValue);
        SetPlayerNameAlpha(MinAlphaValue);
        SetStoneCountAlpha(MinAlphaValue);
    }

    double ResultDetail::GetResultIconScale() const
    {
        std::shared_ptr<Image> resultIcon = resultIcon_.lock();
        if (!resultIcon)
        {
            return 0.0;
        }
        return resultIcon->GetScale();
    }

    double ResultDetail::GetResultIconAngleForRadian() const
    {
        std::shared_ptr<Image> resultIcon = resultIcon_.lock();
        if (!resultIcon)
        {
            return 0.0;
        }
        return resultIcon->GetAngle();
    }

    void ResultDetail::LoadStoneIcon(const String& filePath)
    {
        std::shared_ptr<Image> stoneIcon = stoneIcon_.lock();
        if (!stoneIcon)
        {
            return;
        }
        stoneIcon->LoadTexture(filePath);
    }

    void ResultDetail::SetStoneIconCenterPos(Vec2 pos)
    {
        std::shared_ptr<Image> stoneIcon = stoneIcon_.lock();
        if (!stoneIcon)
        {
            return;
        }
        stoneIcon->SetCenterPos(pos);
    }

    void ResultDetail::SetResultIconCenterPos(Vec2 pos)
    {
        std::shared_ptr<Image> resultIcon = resultIcon_.lock();
        if (!resultIcon)
        {
            return;
        }
        resultIcon->SetCenterPos(pos);
    }

    void ResultDetail::SetStoneIconScale(double scale)
    {
        std::shared_ptr<Image> stoneIcon = stoneIcon_.lock();
        if (!stoneIcon)
        {
            return;
        }
        stoneIcon->SetScale(scale);
    }

    void ResultDetail::SetResultIconScale(double scale)
    {
        std::shared_ptr<Image> resultIcon = resultIcon_.lock();
        if (!resultIcon)
        {
            return;
        }
        resultIcon->SetScale(scale);
    }

    void ResultDetail::SetResultIconAngleForRadian(double radianValue)
    {
        std::shared_ptr<Image> resultIcon = resultIcon_.lock();
        if (!resultIcon)
        {
            return;
        }
        resultIcon->SetAngle(radianValue);
    }

    void ResultDetail::SetResultIconAngleForDegree(double degreeValue)
    {
        std::shared_ptr<Image> resultIcon = resultIcon_.lock();
        if (!resultIcon)
        {
            return;
        }
        double radianValue = Math::Round(Math::ToRadians(degreeValue));
        resultIcon->SetAngle(radianValue);
    }

    void ResultDetail::SetStoneIconAlpha(double alpha)
    {
        std::shared_ptr<Image> stoneIcon = stoneIcon_.lock();
        if (!stoneIcon)
        {
            return;
        }
        stoneIcon->SetAlpha(alpha);
    }

    void ResultDetail::SetResultIconAlpha(double alpha)
    {
        std::shared_ptr<Image> resultIcon = resultIcon_.lock();
        if (!resultIcon)
        {
            return;
        }
        resultIcon->SetAlpha(alpha);
    }

    void ResultDetail::SetPlayerNameCenterPos(Vec2 pos)
    {
        std::shared_ptr<Text> playerName = playerName_.lock();
        if (!playerName)
        {
            return;
        }
        playerName->SetCenterPos(pos);
    }

    void ResultDetail::SetStoneCountCenterPos(Vec2 pos)
    {
        std::shared_ptr<Text> stoneCount = stoneCount_.lock();
        if (!stoneCount)
        {
            return;
        }
        stoneCount->SetCenterPos(pos);
    }

    void ResultDetail::SetPlayerNameText(const String& playerNameText)
    {
        std::shared_ptr<Text> playerName = playerName_.lock();
        if (!playerName)
        {
            return;
        }
        playerName->SetText(playerNameText);
    }

    void ResultDetail::SetStoneCountText(const String& stoneCountText)
    {
        std::shared_ptr<Text> stoneCount = stoneCount_.lock();
        if (!stoneCount)
        {
            return;
        }
        stoneCount->SetText(stoneCountText);
    }

    void ResultDetail::SetPlayerNameColor(ColorF color)
    {
        std::shared_ptr<Text> playerName = playerName_.lock();
        if (!playerName)
        {
            return;
        }
        playerName->SetColor(color);
    }

    void ResultDetail::SetStoneCountColor(ColorF color)
    {
        std::shared_ptr<Text> stoneCount = stoneCount_.lock();
        if (!stoneCount)
        {
            return;
        }
        stoneCount->SetColor(color);
    }

    void ResultDetail::SetPlayerNameAlpha(double alpha)
    {
        std::shared_ptr<Text> playerName = playerName_.lock();
        if (!playerName)
        {
            return;
        }
        playerName->SetAlpha(alpha);
    }

    void ResultDetail::SetStoneCountAlpha(double alpha)
    {
        std::shared_ptr<Text> stoneCount = stoneCount_.lock();
        if (!stoneCount)
        {
            return;
        }
        stoneCount->SetAlpha(alpha);
    }
}
