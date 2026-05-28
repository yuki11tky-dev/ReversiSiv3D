#include "Feature/Common/AssetManager.h"
#include "Feature/Game/UI/BackScreen/BackScreenNormal.h"

namespace ReversiSiv3D {

    BackScreenNormal::BackScreenNormal()
        : UIBase{}
    {

    }

    BackScreenNormal::~BackScreenNormal()
    {

    }

    void BackScreenNormal::Initialize()
    {
        std::shared_ptr<Image> backScreen = std::make_shared<Image>();
        backScreen->LoadTexture(U"Image/backscreen.png");
        const double scale{ AssetManager::GetCurrentWindowScale() };
        backScreen->SetCenterPos(Scene::CenterF());
        backScreen->SetScale(scale);
        backScreen->SetColor(DefaultColor);

        // 所有権は基底クラスであるUIBaseが持つ
        // 本クラスでは弱参照として保持する
        backScreen_ = backScreen;
        AddDrawable(std::move(backScreen));
    }
}
