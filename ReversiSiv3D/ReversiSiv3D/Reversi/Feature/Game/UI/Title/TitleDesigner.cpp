#include "Feature/Common/AssetManager.h"
#include "Feature/Common/Math.h"
#include "Feature/Game/UI/Title/TitleDesigner.h"

namespace ReversiSiv3D {

    TitleDesigner::TitleDesigner()
        : UIBase{}
        , titleBoard_{}
        , titleText_{}
        , guideText_{}
        , rectTextBackBoard_{}
        , guideTextDrawTimer_{}
    {

    }

    TitleDesigner::~TitleDesigner()
    {

    }

    void TitleDesigner::Initialize()
    {
        const Vec2 windowCenter{ Scene::CenterF() };
        constexpr int roundingDigitsZero{ 0 };
        const double scale{ AssetManager::GetCurrentWindowScale() };

        // タイトルボードの準備
        std::shared_ptr<Image> titleBoard = std::make_shared<Image>();
        titleBoard->LoadTexture(U"Image/title_backscreen_design.png");

        constexpr int roundingDigits{ 3 };
        Vec2 boardPos{ windowCenter - AssetManager::AdjustCurrentWindowScale(BoardAdjustPos) };
        boardPos = Math::RoundForVec2(boardPos, roundingDigits);
        titleBoard->SetCenterPos(boardPos);
        titleBoard->SetScale(scale);
        titleBoard->SetColor(ImageColor);

        // 所有権は基底クラスであるUIBaseが持つ
        // 本クラスでは弱参照として保持する
        titleBoard_ = titleBoard;
        AddDrawable(std::move(titleBoard));

        // 文字表記用背景板の準備
        std::shared_ptr<Rectangle> rectTextBackBoard = std::make_shared<Rectangle>();

        SizeF backRectSize{ AssetManager::AdjustCurrentWindowScale(BackBoardSize) };
        backRectSize = Math::RoundForVec2(backRectSize, roundingDigitsZero);
        rectTextBackBoard->Create(backRectSize.asPoint());

        Vec2 backBoardPos{ windowCenter - AssetManager::AdjustCurrentWindowScale(TextBackBoardAdjustPos) };
        backBoardPos = Math::RoundForVec2(backBoardPos, roundingDigitsZero);
        rectTextBackBoard->SetCenterPos(backBoardPos.asPoint());

        rectTextBackBoard->SetColor(TextBackColor);

        // 所有権は基底クラスであるUIBaseが持つ
        // 本クラスでは弱参照として保持する
        rectTextBackBoard_ = rectTextBackBoard;
        AddDrawable(std::move(rectTextBackBoard));

        // タイトル文字の準備
        std::shared_ptr<Text> titleText = std::make_shared<Text>();
        titleText->SetText(U"Reversi");
        titleText->SetColor(TextColor);

        const int32 titleTextResize{ Math::RoundForInt(AssetManager::AdjustCurrentWindowScale(TitleTextSize)) };
        titleText->SetFont(FontMethod::MSDF, titleTextResize, Typeface::Bold);

        const Vec2 titleTextPos{ windowCenter - AssetManager::AdjustCurrentWindowScale(TitleTextAdjustPos) };
        titleText->SetCenterPos(titleTextPos);

        // 所有権は基底クラスであるUIBaseが持つ
        // 本クラスでは弱参照として保持する
        titleText_ = titleText;
        AddDrawable(std::move(titleText));

        // ガイド文字の準備
        std::shared_ptr<Text> guideText = std::make_shared<Text>();
        guideText->SetText(U"Please select button!");
        guideText->SetColor(TextColor);

        const int32 guideTextResize{ Math::RoundForInt(AssetManager::AdjustCurrentWindowScale(GuideTextSize)) };
        guideText->SetFont(FontMethod::MSDF, guideTextResize, Typeface::Bold);

        const Vec2 guideTextPos{ windowCenter - AssetManager::AdjustCurrentWindowScale(GuideTextAdjustPos) };
        guideText->SetCenterPos(guideTextPos);

        // 所有権は基底クラスであるUIBaseが持つ
        // 本クラスでは弱参照として保持する
        guideText_ = guideText;
        AddDrawable(std::move(guideText));
    }

    void TitleDesigner::Update(double deltaTime)
    {
        guideTextDrawTimer_ += deltaTime;
        if (guideTextDrawTimer_ < GuideTextDrawInterval)
        {
            return;
        }
        guideTextDrawTimer_ = ResetTime;

        std::shared_ptr<Text> guideText = guideText_.lock();
        if (guideText)
        {
            double alpha = guideText->GetAlpha();
            alpha = 1.0 - alpha;
            guideText->SetAlpha(alpha);
        }
    }
}
