#include "Feature/Common/AssetManager.h"
#include "Feature/Game/UI/Indicator/PlaceableCellIndicator.h"

namespace ReversiSiv3D {

    PlaceableCellIndicator::PlaceableCellIndicator()
        : UIBase{}
        , indicator_{}
    {

    }

    PlaceableCellIndicator::~PlaceableCellIndicator()
    {

    }

    void PlaceableCellIndicator::Initialize(int32 radius, ColorF color)
    {
        std::shared_ptr<CircleBase> indicator = std::make_shared<CircleBase>();
        indicator->SetRadius(radius);
        indicator->SetColor(color);
        const double scale = AssetManager::GetCurrentWindowScale();
        indicator->SetScale(scale);

        // 所有権は基底クラスであるUIBaseが持つ
        // 本クラスでは弱参照として保持する
        indicator_ = indicator;
        AddDrawable(std::move(indicator));
    }

    void PlaceableCellIndicator::SetCenterPos(Vec2 pos)
    {
        std::shared_ptr<CircleBase> indicator = indicator_.lock();
        if (!indicator)
        {
            return;
        }
        indicator->SetCenterPos(pos);
    }

    void PlaceableCellIndicator::SetColor(ColorF color)
    {
        std::shared_ptr<CircleBase> indicator = indicator_.lock();
        if (!indicator)
        {
            return;
        }
        indicator->SetColor(color);
    }

    void PlaceableCellIndicator::Visible(double alphaValue)
    {
        std::shared_ptr<CircleBase> indicator = indicator_.lock();
        if (!indicator)
        {
            return;
        }
        indicator->SetAlpha(alphaValue);
    }

    void PlaceableCellIndicator::Hide()
    {
        std::shared_ptr<CircleBase> indicator = indicator_.lock();
        if (!indicator)
        {
            return;
        }
        constexpr double alphaMinValue{ 0.0 };
        indicator->SetAlpha(alphaMinValue);
    }
}
