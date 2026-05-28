#include "Feature/Common/AssetManager.h"
#include "Feature/Common/Math.h"
#include "Feature/Game/UIManager/Button/TitleButtonManager.h"
#include "Feature/System/SoundManager.h"

namespace ReversiSiv3D {

    TitleButtonManager::TitleButtonManager(TitleButtonEventInterface& eventInterface)
        : NormalButtonManager{}
        , Task{ TaskId::TitleButton }
        , eventInterface_{ eventInterface }
    {

    }

    TitleButtonManager::~TitleButtonManager()
    {

    }

    void TitleButtonManager::Initialize()
    {
        const Point windowCenter{ Scene::Center() };
        constexpr int roundingDigitsZero{ 0 };

        std::unique_ptr<NormalButton> startButton = std::make_unique<NormalButton>(NormalButtonType::GameStart);
        startButton->Initialize();
        startButton->SetText(U"GameStart");
        startButton->SetDefaultColor(ButtonColor);
        startButton->SetTextColor(ButtonTextColor);
        const Vec2 buttonCenterAdjustPos{ Math::RoundForVec2(AssetManager::AdjustCurrentWindowScale(ButtonFirstAdjustPos), roundingDigitsZero) };
        const Point buttonGameStartPos{ windowCenter + buttonCenterAdjustPos.asPoint() };
        startButton->SetCenterPos(buttonGameStartPos);
        startButton->EnabledShadow();
        AddButtonList(std::move(startButton));

        std::unique_ptr<NormalButton> endButton = std::make_unique<NormalButton>(NormalButtonType::GameEnd);
        endButton->Initialize();
        endButton->SetText(U"GameEnd");
        endButton->SetDefaultColor(ButtonColor);
        endButton->SetTextColor(ButtonTextColor);
        const Vec2 buttonPosInterval{ Math::RoundForVec2(AssetManager::AdjustCurrentWindowScale(ButtonPosInterval), roundingDigitsZero) };
        const Point buttonGameEndPos{ buttonGameStartPos + buttonPosInterval.asPoint() };
        endButton->SetCenterPos(buttonGameEndPos);
        endButton->EnabledShadow();
        AddButtonList(std::move(endButton));
    }

    void TitleButtonManager::Update(double deltaTime)
    {
        UpdateButton(deltaTime);

        const NormalButtonType decidedButton{ TryGetDecidedButton() };
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
        case NormalButtonType::GameEnd:
        {
            eventInterface_.OnEndGame();
            ResetAndDisabledAllButton();
            break;
        }
        default:
        {
            assert(false && "Incorrect button placement.");
            break;
        }
        }
    }

    void TitleButtonManager::Render()
    {
        RenderButton();
    }
}
