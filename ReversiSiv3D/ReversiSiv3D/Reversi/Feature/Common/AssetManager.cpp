#include "Feature/Common/AssetManager.h"
#include "Feature/Common/Math.h"
#include "Feature/Game/GameInfo.h"
#include "Feature/System/GameConfig.h"

namespace ReversiSiv3D {

    void AssetManager::CalculateWindowScale()
    {
        const GameConfig& gameConfig = GameInfo::GetGameConfig();
        const GameWindowSettings& windowSettings = gameConfig.GetWindowSettings();
        const double dblWindowWidth = static_cast<double>(windowSettings.width_);
        const double dblWindowHeight = static_cast<double>(windowSettings.height_);

        windowScale_ = Max(Math::Divide(dblWindowWidth, static_cast<double>(GameConfig::DefaultWindowWidth)),
                           Math::Divide(dblWindowHeight, static_cast<double>(GameConfig::DefaultWindowHeight)));
    }

    double AssetManager::GetCurrentWindowScale()
    {
        return windowScale_;
    }

    double AssetManager::AdjustCurrentWindowScale(double originValue)
    {
        return Math::Multiply(static_cast<double>(originValue), windowScale_);
    }

    Size AssetManager::AdjustCurrentWindowScale(Size originPoint)
    {
        Size resize{};
        resize.x = Math::RoundForInt(Math::Multiply(static_cast<double>(originPoint.x), windowScale_));
        resize.y = Math::RoundForInt(Math::Multiply(static_cast<double>(originPoint.y), windowScale_));
        return resize;
    }

    Vec2 AssetManager::AdjustCurrentWindowScale(Vec2 originVec)
    {
        Vec2 resize{};
        resize.x = Math::Round(Math::Multiply(static_cast<double>(originVec.x), windowScale_));
        resize.y = Math::Round(Math::Multiply(static_cast<double>(originVec.y), windowScale_));
        return resize;
    }
}
