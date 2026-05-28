#include "Feature/Common/AssetManager.h"
#include "Feature/Common/Math.h"
#include "Feature/Game/UI/Button/Button.h"
#include <cassert>

namespace ReversiSiv3D {

    Button::Button()
        : buttonRect_{}
        , shadowRect_{}
        , buttonText_{}
        , buttonColor_{}
        , isHovered_{}
        , isPressLeft_{}
        , isPressRight_{}
        , isDecided_{}
        , isEnabled_{}
        , isShadow_{}
    {

    }

    Button::~Button()
    {

    }

    void Button::Initialize()
    {
        // ボタンの影
        std::shared_ptr<Rectangle> shadowRect = std::make_shared<Rectangle>();
        const Size buttonResize = AssetManager::AdjustCurrentWindowScale(ButtonRectSize);
        shadowRect->Create(buttonResize);
        shadowRect->SetColor(ShadowIntensity);

        const Size adjustShadowOffset = AssetManager::AdjustCurrentWindowScale(ShadowOffset);
        shadowRect->SetOffset(adjustShadowOffset);

        // 所有権は基底クラスであるUIBaseと本クラスが保持する
        // 本クラスでの設定更新処理が頻繁に実施されるため、
        // 本クラスにも所有権を保持する
        shadowRect_ = shadowRect;
        AddDrawable(shadowRect);

        // ボタンの外観
        std::shared_ptr<MouseInteractableRect> buttonRect = std::make_shared<MouseInteractableRect>();
        buttonRect->Create(buttonResize);
        buttonRect->SetColor(DefaultButtonColor);

        // 所有権は基底クラスであるUIBaseと本クラスが保持する
        // 本クラスでの設定更新処理が頻繁に実施されるため、
        // 本クラスにも所有権を保持する
        buttonRect_ = buttonRect;
        AddDrawable(buttonRect);

        // ボタンテキスト
        std::shared_ptr<Text> buttonText = std::make_shared<Text>();
        buttonText->SetColor(DefaultTextColor);

        const int32 buttonTextResize{ Math::RoundForInt(AssetManager::AdjustCurrentWindowScale(ButtonFontSize)) };
        buttonText->SetFont(FontMethod::MSDF, buttonTextResize, Typeface::Regular);

        // 所有権は基底クラスであるUIBaseと本クラスが保持する
        // 本クラスでの設定更新処理が頻繁に実施されるため、
        // 本クラスにも所有権を保持する
        buttonText_ = buttonText;
        AddDrawable(buttonText);

        Enabled();
    }

    void Button::Update(double deltaTime)
    {
        assert(buttonRect_);

        if (buttonRect_->OnHover())
        {
            isHovered_ = true;
        }
        else
        {
            isHovered_ = false;
        }

        if (!isEnabled_)
        {
            return;
        }

        if (buttonRect_->OnLeftClicked())
        {
            OnLeftClicked();
        }
        else if (buttonRect_->OnRightClicked())
        {
            OnRightClicked();
        }

        if (buttonRect_->OnLeftPressed())
        {
            isPressLeft_ = true;

            OnLeftPressed();
        }
        else
        {
            isPressLeft_ = false;

            if (buttonRect_->OnRightPressed())
            {
                isPressRight_ = true;
                OnRightPressed();
            }
            else
            {
                isPressRight_ = false;
            }
        }


        if (buttonRect_->OnLeftReleased())
        {
            OnLeftReleased();
        }
        else if (buttonRect_->OnRightReleased())
        {
            OnRightReleased();
        }

        UpdateButton(deltaTime);
    }

    void Button::ApplyPressedOffset(Point offset)
    {
        assert(buttonRect_ && buttonText_);
        buttonRect_->SetOffset(offset);

        const Point buttonCenterPos{ buttonRect_->GetCenterPos() };
        const Point adjustTextPos{ buttonCenterPos + offset };
        buttonText_->SetCenterPos(adjustTextPos);
    }

    void Button::SetDefaultColor(ColorF color)
    {
        assert(buttonRect_);
        buttonColor_ = color;
    }

    void Button::SetColor(ColorF color)
    {
        assert(buttonRect_);
        buttonRect_->SetColor(color);
    }

    void Button::SetSize(Size size)
    {
        assert(buttonRect_);
        const Size resize{ AssetManager::AdjustCurrentWindowScale(size) };
        buttonRect_->SetSize(resize);
    }

    void Button::SetText(const String& text)
    {
        assert(buttonText_);
        buttonText_->SetText(text);
    }

    void Button::SetFont(FontMethod method, int32 size, Typeface typeFace)
    {
        assert(buttonText_);
        const int32 resize{ Math::RoundForInt(AssetManager::AdjustCurrentWindowScale(size)) };
        buttonText_->SetFont(method, resize, typeFace);
    }

    void Button::SetTextColor(ColorF color)
    {
        assert(buttonText_);
        buttonText_->SetColor(color);
    }

    void Button::SetCenterPos(Point pos)
    {
        assert(buttonRect_ && shadowRect_ && buttonText_);
        buttonRect_->SetCenterPos(pos);
        shadowRect_->SetCenterPos(pos);
        buttonText_->SetCenterPos(pos);
    }

    bool Button::IsDecided() const
    {
        return isDecided_;
    }

    void Button::Disabled()
    {
        isEnabled_ = false;
    }

    void Button::Enabled()
    {
        isEnabled_ = true;
    }

    void Button::ResetDecided()
    {
        isDecided_ = false;
    }

    void Button::EnabledShadow()
    {
        isShadow_ = true;
    }

    void Button::Visible()
    {
        assert(buttonRect_ && shadowRect_ && buttonText_);
        constexpr double maxAlphaValue{ 1.0 };
        buttonRect_->SetAlpha(maxAlphaValue);
        shadowRect_->SetAlpha(ShadowIntensity.a);
        buttonText_->SetAlpha(maxAlphaValue);
    }

    void Button::Hide()
    {
        assert(buttonRect_ && shadowRect_ && buttonText_);
        constexpr double minAlphaValue{ 0.0 };
        buttonRect_->SetAlpha(minAlphaValue);
        shadowRect_->SetAlpha(minAlphaValue);
        buttonText_->SetAlpha(minAlphaValue);
    }
}
