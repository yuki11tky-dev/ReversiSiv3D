#include "Feature/Common/AssetManager.h"
#include "Feature/Game/UI/HUD/ScoreBoard.h"

namespace ReversiSiv3D {

    ScoreBoard::ScoreBoard()
        : HUDBase()
        , backBoard_{}
        , stoneIcon_{}
        , playerName_{}
        , stoneCount_{}
    {

    }

    ScoreBoard::~ScoreBoard()
    {

    }

    void ScoreBoard::Initialize(const String& boardFilePath, const String& stoneFilePath)
    {
        std::shared_ptr<Image> board = std::make_shared<Image>();
        board->LoadTexture(boardFilePath);
        const double scale{ AssetManager::GetCurrentWindowScale() };
        board->SetScale(scale);

        // 所有権は基底クラスであるUIBaseが持つ
        // 本クラスでは弱参照として保持する
        backBoard_ = board;
        AddDrawable(std::move(board));

        std::shared_ptr<Image> stoneIcon = std::make_shared<Image>();
        stoneIcon->LoadTexture(stoneFilePath);
        stoneIcon->SetScale(scale);

        // 所有権は基底クラスであるUIBaseが持つ
        // 本クラスでは弱参照として保持する
        stoneIcon_ = stoneIcon;
        AddDrawable(std::move(stoneIcon));

        // 所有権は基底クラスであるUIBaseが持つ
        // 本クラスでは弱参照として保持する
        std::shared_ptr<Text> playerName = std::make_shared<Text>();
        playerName_ = playerName;
        AddDrawable(std::move(playerName));

        // 所有権は基底クラスであるUIBaseが持つ
        // 本クラスでは弱参照として保持する
        std::shared_ptr<Text> stoneCount = std::make_shared<Text>();
        stoneCount_ = stoneCount;
        AddDrawable(std::move(stoneCount));
    }

    void ScoreBoard::SetPosition(Vec2 pos, bool isWide)
    {
        std::shared_ptr<Image> backBoard = backBoard_.lock();
        std::shared_ptr<Image> stoneIcon = stoneIcon_.lock();
        std::shared_ptr<Text> playerName = playerName_.lock();
        std::shared_ptr<Text> stoneCount = stoneCount_.lock();
        if (backBoard)
        {
            backBoard->SetCenterPos(pos);
        }

        // 画面比率がフルHD(16:9)とスタンダード(4:3)の2種類の関係で
        // ボードの表示サイズをスタンダードの時のみ0.75倍している。
        // 引数のisWideがワイド表示(フルHD)かどうかを表し、isWide = false
        // つまりスタンダードの時は位置調整値に対して再度0.75倍している
        if (stoneIcon)
        {
            Vec2 adjustPos{ AssetManager::AdjustCurrentWindowScale(StonePosOffset) };
            if (isWide)
            {
                stoneIcon->SetCenterPos(pos + adjustPos);
            }
            else
            {
                adjustPos = pos + adjustPos * AspectRatioAdjustValue;
                stoneIcon->SetCenterPos(adjustPos);
            }
        }

        if (playerName)
        {
            Vec2 adjustPos{ AssetManager::AdjustCurrentWindowScale(PlayerNamePosOffset) };
            if (isWide)
            {
                playerName->SetCenterPos(pos + adjustPos);
            }
            else
            {
                adjustPos = pos + adjustPos * AspectRatioAdjustValue;
                playerName->SetCenterPos(adjustPos);
            }
        }

        if (stoneCount)
        {
            Vec2 adjustPos{ AssetManager::AdjustCurrentWindowScale(StoneCountPosOffset) };
            if (isWide)
            {
                stoneCount->SetCenterPos(pos + adjustPos);
            }
            else
            {
                adjustPos = pos + adjustPos * AspectRatioAdjustValue;
                stoneCount->SetCenterPos(adjustPos);
            }
        }
    }

    void ScoreBoard::SetScale(double scale)
    {
        std::shared_ptr<Image> backBoard = backBoard_.lock();
        std::shared_ptr<Image> stoneIcon = stoneIcon_.lock();
        if (backBoard)
        {
            backBoard->SetScale(scale);
        }

        if (stoneIcon)
        {
            stoneIcon->SetScale(scale);
        }
    }

    void ScoreBoard::SetImageColor(ColorF color)
    {
        std::shared_ptr<Image> backBoard = backBoard_.lock();
        std::shared_ptr<Image> stoneIcon = stoneIcon_.lock();
        if (backBoard)
        {
            backBoard->SetColor(color);
        }

        if (stoneIcon)
        {
            stoneIcon->SetColor(color);
        }
    }

    void ScoreBoard::SetPlayerName(const String& playerNameText)
    {
        std::shared_ptr<Text> playerName = playerName_.lock();
        if (!playerName)
        {
            return;
        }
        playerName->SetText(playerNameText);
    }

    void ScoreBoard::SetStoneCount(const String& stoneCountText)
    {
        std::shared_ptr<Text> stoneCount = stoneCount_.lock();
        if (!stoneCount)
        {
            return;
        }
        stoneCount->SetText(stoneCountText);
    }

    void ScoreBoard::SetPlayerNameFont(FontMethod method, int32 size, Typeface typeFace)
    {
        std::shared_ptr<Text> playerName = playerName_.lock();
        if (!playerName)
        {
            return;
        }
        playerName->SetFont(method, size, typeFace);
    }

    void ScoreBoard::SetStoneCountFont(FontMethod method, int32 size, Typeface typeFace)
    {
        std::shared_ptr<Text> stoneCount = stoneCount_.lock();
        if (!stoneCount)
        {
            return;
        }
        stoneCount->SetFont(method, size, typeFace);
    }

    void ScoreBoard::SetPlayerNameColor(ColorF color)
    {
        std::shared_ptr<Text> playerName = playerName_.lock();
        if (!playerName)
        {
            return;
        }
        playerName->SetColor(color);
    }

    void ScoreBoard::SetStoneCountColor(ColorF color)
    {
        std::shared_ptr<Text> stoneCount = stoneCount_.lock();
        if (!stoneCount)
        {
            return;
        }
        stoneCount->SetColor(color);
    }

    Vec2 ScoreBoard::GetCenterPos() const
    {
        std::shared_ptr<Image> backBoard = backBoard_.lock();
        if (!backBoard)
        {
            return Vec2::Zero();
        }
        return backBoard->GetCenterPos();
    }
}
