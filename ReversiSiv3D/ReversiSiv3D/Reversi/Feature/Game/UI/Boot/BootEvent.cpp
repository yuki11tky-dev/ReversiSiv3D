#include "Feature/Common/AssetManager.h"
#include "Feature/Common/Math.h"
#include "Feature/Game/UI/Boot/BootEvent.h"
#include "Feature/Logger/Logger.h"
#include <cassert>
#include <cmath>

namespace ReversiSiv3D {

    BootEvent::BootEvent()
        : UIBase{}
        , spriteStone_{}
        , bootText_{}
        , periodTextList_{}
        , currentState_{}
        , elapsedTimeForPeriod_{}
        , elapsedTimeForImage_{}
        , velocity_{}
        , bounceIndex_{}
        , bounceHeightList_{}
        , bounceGroundPosY_{}
        , appearedPeriodIndex_{}
    {

    }

    BootEvent::~BootEvent()
    {

    }

    void BootEvent::Update(double deltaTimeSec)
    {

        UpdateStoneSpinAnimation(deltaTimeSec);

        switch (currentState_)
        {
        case EventState::Start:
        {
            elapsedTimeForPeriod_ = ResetTime;
            elapsedTimeForImage_ = ResetTime;
            ChangeEventState(EventState::AddPeriod);
            break;
        }
        case EventState::AddPeriod:
        {
            const bool isFinished = UpdatePeriodText(deltaTimeSec);
            if (!isFinished)
            {
                break;
            }
            elapsedTimeForPeriod_ = ResetTime;
            ChangeEventState(EventState::Animation);
            break;
        }
        case EventState::Animation:
        {
            const bool isFinished = UpdateStoneJumpAnimation(deltaTimeSec);
            if (!isFinished)
            {
                break;
            }
            ChangeEventState(EventState::Finish);
            break;
        }
        case EventState::Finish:
        {
            ChangeEventState(EventState::Finished);
            break;
        }
        default:
        {
            break;
        }
        }
    }

    void BootEvent::Initialize()
    {
        const Vec2 windowCenter{ Scene::CenterF() };
        constexpr int roundingDigitsZero{ 0 };

        const String bootTitleText = U"Boot";
        const String bootPeriodText = U".";

        // Bootテキスト準備
        std::shared_ptr<Text> bootText = std::make_shared<Text>();
        bootText->SetText(bootTitleText);

        Vec2 bootTextPos{ windowCenter - AssetManager::AdjustCurrentWindowScale(BootTextAdjustPos) };
        bootTextPos = Math::RoundForVec2(bootTextPos, roundingDigitsZero);
        bootText->SetCenterPos(bootTextPos);
        bootText->SetColor(TextColor);

        const int32 bootTextResize{ Math::RoundForInt(AssetManager::AdjustCurrentWindowScale(BootTextSize)) };
        bootText->SetFont(FontMethod::MSDF, bootTextResize, Typeface::Bold);

        // 所有権は基底クラスであるUIBaseが持つ
        // 本クラスでは弱参照として保持する
        bootText_ = bootText;
        AddDrawable(std::move(bootText));

        std::shared_ptr<Text> periodText = std::make_shared<Text>();
        periodText->SetText(bootPeriodText);
        periodText->SetColor(TextColor);
        periodText->SetFont(FontMethod::MSDF, bootTextResize, Typeface::Bold);

        constexpr double alphaMinValue{ 0.0 };
        periodText->SetAlpha(alphaMinValue);

        Vec2 periodTextPos = windowCenter + AssetManager::AdjustCurrentWindowScale(PeriodTextAdjustFirstPos);
        periodText->SetCenterPos(periodTextPos);

        const Vec2 displayPosInterval{ AssetManager::AdjustCurrentWindowScale(PeriodTextDisplayInterval) };
        std::shared_ptr<Text> periodSecondText = std::make_shared<Text>(*periodText);
        periodTextPos += displayPosInterval;
        periodSecondText->SetCenterPos(periodTextPos);

        std::shared_ptr<Text> periodThirdText = std::make_shared<Text>(*periodText);
        periodTextPos += displayPosInterval;
        periodThirdText->SetCenterPos(periodTextPos);

        // 所有権は基底クラスであるUIBaseが持つ
        // 本クラスでは弱参照として保持する
        periodTextList_.push_back(periodText);
        periodTextList_.push_back(periodSecondText);
        periodTextList_.push_back(periodThirdText);
        AddDrawable(std::move(periodText));
        AddDrawable(std::move(periodSecondText));
        AddDrawable(std::move(periodThirdText));


        // スピンアニメーション用リバーシの石スプライトの準備

        std::shared_ptr<ImageSprite> spriteStone = std::make_shared<ImageSprite>();
        constexpr int horiSplitCount{ 2 };
        constexpr int vertSplitCount{ 7 };
        SpriteData spriteData{};
        spriteData.horizontalSplitCount_ = horiSplitCount;
        spriteData.verticalSplitCount_ = vertSplitCount;
        spriteStone->LoadTexture(U"image/boot_icon.png", spriteData);
        spriteStone->SetColor(SpinStoneColor);

        const double scale{ AssetManager::GetCurrentWindowScale() };
        spriteStone->SetScale(scale);

        const Vec2 stonePos{ windowCenter + AssetManager::AdjustCurrentWindowScale(StoneAdjustPos) };
        spriteStone->SetCenterPos(stonePos);

        // 所有権は基底クラスであるUIBaseが持つ
        // 本クラスでは弱参照として保持する
        spriteStone_ = spriteStone;
        AddDrawable(std::move(spriteStone));

        // ジャンプアニメーションの準備
        constexpr int initIndex{ 0 };
        appearedPeriodIndex_ = initIndex;

        for (const double heightValue : BounceHeights)
        {
            bounceHeightList_.push_back(AssetManager::AdjustCurrentWindowScale(heightValue));
        }

        velocity_ = ResetVelocity();
        bounceGroundPosY_ = AssetManager::AdjustCurrentWindowScale(GroundY);
    }

    void BootEvent::Start()
    {
        ChangeEventState(EventState::Start);
    }

    bool BootEvent::IsFinished() const
    {
        return currentState_ == EventState::Finished;
    }

    bool BootEvent::UpdatePeriodText(double deltaTimeSec)
    {
        if (appearedPeriodIndex_ >= periodTextList_.size())
        {
            return true;
        }

        elapsedTimeForPeriod_ += deltaTimeSec;
        if (elapsedTimeForPeriod_ >= PeriodAddTime)
        {
            elapsedTimeForPeriod_ = ResetTime;

            assert(appearedPeriodIndex_ >= 0 && appearedPeriodIndex_ < periodTextList_.size());
            std::shared_ptr<Text> periodText = periodTextList_.at(appearedPeriodIndex_).lock();
            if (periodText)
            {
                constexpr double maxAlpha{ 1.0 };
                periodText->SetAlpha(maxAlpha);
            }
            ++appearedPeriodIndex_;
        }
        return false;
    }

    bool BootEvent::UpdateStoneJumpAnimation(double deltaTimeSec)
    {
        std::shared_ptr<ImageSprite> spriteStone = spriteStone_.lock();
        if (!spriteStone)
        {
            return true;
        }

        bool isFinishJumpAnim{ false };
        // 地面に3回衝突した後にジャンプアニメーションが終了します。
        velocity_ -= Math::Multiply(Gravity, deltaTimeSec);
        Vec2 imagePos{ spriteStone->GetCenterPos() };
        imagePos.y -= Math::Multiply(velocity_, deltaTimeSec);
        if (imagePos.y >= bounceGroundPosY_)
        {
            // 地面の下に位置している場合、接地していることにする
            imagePos.y = bounceGroundPosY_;
            ++bounceIndex_;
            if (bounceIndex_ < bounceHeightList_.size())
            {
                velocity_ = ResetVelocity();
            }
            else
            {
                constexpr double resetVelocity{ 0.0 };
                velocity_ = resetVelocity;
                isFinishJumpAnim = true;
            }
        }

        spriteStone->SetCenterPos(imagePos);
        return isFinishJumpAnim;
    }

    void BootEvent::UpdateStoneSpinAnimation(double deltaTimeSec)
    {
        elapsedTimeForImage_ += deltaTimeSec;
        if (elapsedTimeForImage_ >= SwitchImageTime)
        {
            elapsedTimeForImage_ = ResetTime;

            std::shared_ptr<ImageSprite> spriteStone = spriteStone_.lock();
            if (spriteStone)
            {
                int drawIndex{ spriteStone->GetDrawIndex() };
                const int maxSize{ spriteStone->GetMaxListSize() };
                ++drawIndex;
                if (drawIndex >= maxSize)
                {
                    constexpr int resetIndex{ 0 };
                    drawIndex = resetIndex;
                }
                spriteStone->SetDrawIndex(drawIndex);
            }
        }
    }

    double BootEvent::ResetVelocity()
    {
        const double targetHeight{ bounceHeightList_.at(bounceIndex_) };
        return Math::Round(std::sqrt(CoefficientValue * Gravity * targetHeight));
    }

    void BootEvent::ChangeEventState(EventState state)
    {
        currentState_ = state;
    }
}
