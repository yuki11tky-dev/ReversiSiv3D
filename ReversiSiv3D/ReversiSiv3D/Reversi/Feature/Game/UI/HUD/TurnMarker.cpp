#include "Feature/Common/AssetManager.h"
#include "Feature/Common/Math.h"
#include "Feature/Game/UI/HUD/ScoreBoard.h"
#include "Feature/Game/UI/HUD/TurnMarker.h"
#include <cassert>

namespace ReversiSiv3D {

    TurnMarker::TurnMarker()
        : HUDBase()
        , rectTurnMarker_{}
    {

    }

    TurnMarker::~TurnMarker()
    {

    }

    void TurnMarker::Initialize()
    {
        const double windowScale{ AssetManager::GetCurrentWindowScale() };
        Size resize{ AssetManager::AdjustCurrentWindowScale(MarkerSize) };
        if (windowScale < FullHDWindowScale)
        {
            resize = Math::Multiply(resize, ScoreBoard::AspectRatioAdjustValue);
        }

        std::shared_ptr<Rectangle> rectTurnMarker = std::make_shared<Rectangle>();
        rectTurnMarker->SetSize(resize);
        rectTurnMarker->SetColor(Red);

        // 所有権は基底クラスであるUIBaseと本クラスが保持する
        // 本クラスでの設定更新処理が頻繁に実施されるため、
        // 本クラスにも所有権を保持する
        rectTurnMarker_ = rectTurnMarker;
        AddDrawable(rectTurnMarker);
    }

    void TurnMarker::SetCenterPos(Point pos)
    {
        assert(rectTurnMarker_);
        rectTurnMarker_->SetCenterPos(pos);
    }

    void TurnMarker::SetSize(Size size)
    {
        assert(rectTurnMarker_);
        rectTurnMarker_->SetSize(size);
    }

    void TurnMarker::SetImageColor(ColorF color)
    {
        assert(rectTurnMarker_);
        rectTurnMarker_->SetColor(color);
    }

    void TurnMarker::SetAlphaValue(double alpha)
    {
        assert(rectTurnMarker_);
        rectTurnMarker_->SetAlpha(alpha);
    }

    double TurnMarker::GetAlphaValue() const
    {
        assert(rectTurnMarker_);
        return rectTurnMarker_->GetAlpha();
    }
}
