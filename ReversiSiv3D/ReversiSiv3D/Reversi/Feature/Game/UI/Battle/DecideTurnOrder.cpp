#include "Feature/Common/AssetManager.h"
#include "Feature/Common/Math.h"
#include "Feature/Game/UI/Battle/DecideTurnOrder.h"
#include <cassert>

namespace ReversiSiv3D {

    DecideTurnOrder::DecideTurnOrder()
        : UIBase{}
        , rectBoard_{}
        , rectBoardShadow_{}
        , guideText_{}
    {

    }

    DecideTurnOrder::~DecideTurnOrder()
    {

    }

    void DecideTurnOrder::Initialize()
    {
        const Vec2 windowCenter{ Scene::CenterF() };
        const Size boardResize{ AssetManager::AdjustCurrentWindowScale(BackBoardSize) };
        const Point boardCenterPos{ windowCenter.asPoint() };
        constexpr int roundingDigitsZero{ 0 };

        std::shared_ptr<Rectangle> rectBoard = std::make_shared<Rectangle>();
        rectBoard->SetSize(boardResize);
        rectBoard->SetCenterPos(boardCenterPos);
        rectBoard->SetColor(BackBoardColor);

        // 所有権は基底クラスであるUIBaseが持つ
        // 本クラスでは弱参照として保持する
        rectBoard_ = rectBoard;
        AddDrawable(std::move(rectBoard));

        std::shared_ptr<Rectangle> rectBoardShadow = std::make_shared<Rectangle>();

        rectBoardShadow->SetSize(boardResize);
        rectBoardShadow->SetCenterPos(boardCenterPos);
        rectBoardShadow->SetColor(ShadowColor);
        rectBoardShadow->SetOffset(ShadowOffset);

        // 所有権は基底クラスであるUIBaseが持つ
        // 本クラスでは弱参照として保持する
        rectBoardShadow_ = rectBoardShadow;
        AddDrawable(std::move(rectBoardShadow));

        std::shared_ptr<Text> guideText = std::make_shared<Text>();
        guideText->SetText(U"Select Turn Order");
        guideText->SetColor(GuideTextColor);

        const int32 guideTextResize{ Math::RoundForInt(AssetManager::AdjustCurrentWindowScale(GuideTextSize)) };
        guideText->SetFont(FontMethod::MSDF, guideTextResize, Typeface::Bold);
        const Vec2 textPos{ windowCenter + Math::RoundForVec2(AssetManager::AdjustCurrentWindowScale(GuideTextOffsetPos), roundingDigitsZero) };
        guideText->SetCenterPos(textPos);

        // 所有権は基底クラスであるUIBaseが持つ
        // 本クラスでは弱参照として保持する
        guideText_ = guideText;
        AddDrawable(std::move(guideText));
    }

    void DecideTurnOrder::Visible()
    {
        std::shared_ptr<Rectangle> rectBoard = rectBoard_.lock();
        std::shared_ptr<Rectangle> rectBoardShadow = rectBoardShadow_.lock();
        std::shared_ptr<Text> guideText = guideText_.lock();
        if (rectBoard)
        {
            rectBoard->SetAlpha(BackBoardColor.a);
        }

        if (rectBoardShadow)
        {
            rectBoardShadow->SetAlpha(ShadowColor.a);
        }

        if (guideText)
        {
            guideText->SetAlpha(GuideTextColor.a);
        }
    }

    void DecideTurnOrder::Hide()
    {
        std::shared_ptr<Rectangle> rectBoard = rectBoard_.lock();
        std::shared_ptr<Rectangle> rectBoardShadow = rectBoardShadow_.lock();
        std::shared_ptr<Text> guideText = guideText_.lock();
        if (rectBoard)
        {
            rectBoard->SetAlpha(AlphaMinValue);
        }

        if (rectBoardShadow)
        {
            rectBoardShadow->SetAlpha(AlphaMinValue);
        }

        if (guideText)
        {
            guideText->SetAlpha(AlphaMinValue);
        }
    }
}
