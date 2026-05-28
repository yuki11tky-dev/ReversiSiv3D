#include "Feature/Common/AssetManager.h"
#include "Feature/Game/UIManager/Button/ResultButtonManager.h"
#include "Feature/System/SoundManager.h"

namespace ReversiSiv3D {

    ResultButtonManager::ResultButtonManager(ResultButtonEventInterface& eventInterface)
        : NormalButtonManager{}
        , Task{ TaskId::ResultButton }
        , eventInterface_{ eventInterface }
    {

    }

    ResultButtonManager::~ResultButtonManager()
    {

    }

    void ResultButtonManager::Initialize()
    {
        const Point windowCenter{ Scene::Center() };

        std::unique_ptr<NormalButton> retryButton = std::make_unique<NormalButton>(NormalButtonType::GameStart);
        retryButton->Initialize();
        retryButton->SetText(U"Play Again");
        retryButton->SetDefaultColor(ButtonColor);
        retryButton->SetTextColor(ButtonTextColor);
        constexpr Point adjustRetryButtonPos{ ReverseXAxis * ButtonOffsetPos };
        const Point buttonPlayAgainPos = windowCenter + AssetManager::AdjustCurrentWindowScale(adjustRetryButtonPos);
        retryButton->SetCenterPos(buttonPlayAgainPos);
        retryButton->EnabledShadow();
        retryButton->Disabled();
        retryButton->Hide();
        AddButtonList(std::move(retryButton));

        std::unique_ptr<NormalButton> returnButton = std::make_unique<NormalButton>(NormalButtonType::ReturnTitle);
        returnButton->Initialize();
        returnButton->SetText(U"Return Title");
        returnButton->SetDefaultColor(ButtonColor);
        returnButton->SetTextColor(ButtonTextColor);
        const Point buttonReturnTitlePos = windowCenter + AssetManager::AdjustCurrentWindowScale(ButtonOffsetPos);
        returnButton->SetCenterPos(buttonReturnTitlePos);
        returnButton->EnabledShadow();
        returnButton->Disabled();
        returnButton->Hide();
        AddButtonList(std::move(returnButton));
    }

    void ResultButtonManager::Update(double deltaTime)
    {
        UpdateButton(deltaTime);

        const NormalButtonType decidedButton = TryGetDecidedButton();
        switch (decidedButton)
        {
        case NormalButtonType::None:
        {
            break;
        }
        case NormalButtonType::GameStart:
        {
            eventInterface_.OnStartGame();
            ResetAndDisabledAllButton();
            SoundManager::PlayOneShot(SoundType::ButtonClickSe);
            break;
        }
        case NormalButtonType::ReturnTitle:
        {
            eventInterface_.OnReturnTitle();
            ResetAndDisabledAllButton();
            SoundManager::PlayOneShot(SoundType::ButtonClickSe);
            break;
        }
        default:
        {
            assert(false && "Incorrect button placement.");
            break;
        }
        }
    }

    void ResultButtonManager::Render()
    {
        RenderButton();
    }
}
