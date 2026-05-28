#include "Feature/Common/AssetManager.h"
#include "Feature/Common/Math.h"
#include "Feature/Game/UIManager/Button/TurnOrderButtonManager.h"
#include "Feature/System/SoundManager.h"

namespace ReversiSiv3D {

    TurnOrderButtonManager::TurnOrderButtonManager(BattleButtonEventInterface& eventInterface)
        : NormalButtonManager{}
        , eventInterface_{ eventInterface }
    {

    }

    TurnOrderButtonManager::~TurnOrderButtonManager()
    {

    }

    void TurnOrderButtonManager::Initialize()
    {
        const Point windowCenter{ Scene::Center() };
        constexpr int roundingDigitsZero{ 0 };

        std::unique_ptr<NormalButton> firstButton = std::make_unique<NormalButton>(NormalButtonType::DecideFirstPlayer);
        firstButton->Initialize();
        firstButton->SetDefaultColor(ButtonColor);
        firstButton->SetText(U"First Attack");
        firstButton->SetTextColor(ButtonTextColor);
        const Vec2 buttonCenterAdjustPos{ Math::RoundForVec2(AssetManager::AdjustCurrentWindowScale(ButtonFirstAdjustPos), roundingDigitsZero) };
        const Point buttonGameStartPos{ windowCenter + buttonCenterAdjustPos.asPoint() };
        firstButton->SetCenterPos(buttonGameStartPos);
        firstButton->EnabledShadow();
        AddButtonList(std::move(firstButton));

        std::unique_ptr<NormalButton> secondButton = std::make_unique<NormalButton>(NormalButtonType::DecideSecondPlayer);
        secondButton->Initialize();
        secondButton->SetDefaultColor(ButtonColor);
        secondButton->SetText(U"Second Attack");
        secondButton->SetTextColor(ButtonTextColor);
        const Vec2 buttonPosInterval{ Math::RoundForVec2(AssetManager::AdjustCurrentWindowScale(ButtonPosInterval), roundingDigitsZero) };
        const Point buttonGameEndPos{ buttonGameStartPos + buttonPosInterval.asPoint() };
        secondButton->SetCenterPos(buttonGameEndPos);
        secondButton->EnabledShadow();
        AddButtonList(std::move(secondButton));
    }

    void TurnOrderButtonManager::Update(double deltaTime)
    {
        UpdateButton(deltaTime);

        const NormalButtonType decidedButton{ TryGetDecidedButton() };
        switch (decidedButton)
        {
        case NormalButtonType::None:
        {
            break;
        }
        case NormalButtonType::DecideFirstPlayer:
        {
            eventInterface_.OnDecideFirstPlayer();
            ResetAndDisabledAllButton();
            SoundManager::PlayOneShot(SoundType::ButtonClickSe);
            break;
        }
        case NormalButtonType::DecideSecondPlayer:
        {
            eventInterface_.OnDecideSecondPlayer();
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

    void TurnOrderButtonManager::Render()
    {
        RenderButton();
    }
}
