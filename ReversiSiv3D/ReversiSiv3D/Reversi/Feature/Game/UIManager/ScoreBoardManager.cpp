#include "Feature/Common/AssetManager.h"
#include "Feature/Common/Math.h"
#include "Feature/Game/ResourceProvider.h"
#include "Feature/Game/UIManager/ScoreBoardManager.h"

namespace ReversiSiv3D {

    ScoreBoardManager::ScoreBoardManager()
        : Task(TaskId::ScoreBoard)
        , whiteStoneBoard_{}
        , blackStoneBoard_{}
        , isInitialized_{}
        , stateObserver_{}
    {

    }

    ScoreBoardManager::~ScoreBoardManager()
    {

    }

    void ScoreBoardManager::Update(double)
    {
        if (!isInitialized_)
        {
            return;
        }

        std::shared_ptr<GameState> gameState;
        const bool isSuccess = TryGetGameState(gameState);
        if (!isSuccess)
        {
            return;
        }
        const bool isUpdate = gameState->IsUpdateStoneCount();
        if (!isUpdate)
        {
            return;
        }

        gameState->ResetUpdateStoneCount();
        const int blackStoneCount{ gameState->GetBlackStoneCount() };
        const int whiteStoneCount{ gameState->GetWhiteStoneCount() };
        blackStoneBoard_->SetStoneCount(ToString(blackStoneCount));
        whiteStoneBoard_->SetStoneCount(ToString(whiteStoneCount));
    }

    void ScoreBoardManager::Render()
    {
        if (!isInitialized_)
        {
            return;
        }

        whiteStoneBoard_->Render();
        blackStoneBoard_->Render();
    }

    void ScoreBoardManager::SetStateObserver(const std::shared_ptr<GameState>& gameState)
    {
        stateObserver_ = gameState;
    }

    void ScoreBoardManager::CreateBoard()
    {
        const double windowScale{ AssetManager::GetCurrentWindowScale() };
        double scale{ windowScale };
        bool isWide{ true };
        int32 playerNameFontSize{ Math::RoundForInt(AssetManager::AdjustCurrentWindowScale(PlayerNameSize)) };
        int32 stoneCountFontSize{ Math::RoundForInt(AssetManager::AdjustCurrentWindowScale(StoneCountSize)) };

        // 画面比率がフルHD(16:9)とスタンダード(4:3)の2種類の関係で
        // ボードの表示サイズをスタンダードの時のみ0.75倍する。
        // それに合わせてフォントサイズも0.75倍する。
        if (windowScale < FullHDWindowScale)
        {
            scale *= ScoreBoard::AspectRatioAdjustValue;
            playerNameFontSize = Math::Multiply(playerNameFontSize, ScoreBoard::AspectRatioAdjustValue);
            stoneCountFontSize = Math::Multiply(stoneCountFontSize, ScoreBoard::AspectRatioAdjustValue);
            isWide = false;
        }

        const String defaultStoneCount{ U"0" };
        whiteStoneBoard_ = std::make_unique<ScoreBoard>();
        whiteStoneBoard_->Initialize(U"image/wood_frame_01.png", U"image/stone_white.png");
        whiteStoneBoard_->SetPosition(DefaultPos, isWide);
        whiteStoneBoard_->SetScale(scale);
        whiteStoneBoard_->SetImageColor(ImageDefaultColor);
        whiteStoneBoard_->SetPlayerNameColor(WhiteTextColor);
        whiteStoneBoard_->SetStoneCountColor(WhiteTextColor);
        whiteStoneBoard_->SetPlayerNameFont(FontMethod::MSDF, playerNameFontSize, Typeface::Bold);
        whiteStoneBoard_->SetStoneCountFont(FontMethod::MSDF, stoneCountFontSize, Typeface::Bold);
        whiteStoneBoard_->SetStoneCount(defaultStoneCount);

        blackStoneBoard_ = std::make_unique<ScoreBoard>();
        blackStoneBoard_->Initialize(U"image/wood_frame_02.png", U"image/stone_black.png");
        blackStoneBoard_->SetPosition(DefaultPos, isWide);
        blackStoneBoard_->SetScale(scale);
        blackStoneBoard_->SetImageColor(ImageDefaultColor);
        blackStoneBoard_->SetPlayerNameColor(BlackTextColor);
        blackStoneBoard_->SetStoneCountColor(BlackTextColor);
        blackStoneBoard_->SetPlayerNameFont(FontMethod::MSDF, playerNameFontSize, Typeface::Bold);
        blackStoneBoard_->SetStoneCountFont(FontMethod::MSDF, stoneCountFontSize, Typeface::Bold);
        blackStoneBoard_->SetStoneCount(defaultStoneCount);

        // ボード生成時には描画しないように設定しておく
        isInitialized_ = false;
    }

    void ScoreBoardManager::Assigned(const PlayerData& usedBlack, const PlayerData& usedWhite)
    {
        const double windowScale{ AssetManager::GetCurrentWindowScale() };
        bool isWide{ true };
        Vec2 playerPosOffset{};
        Vec2 enemyPosOffset{};
        if (windowScale < FullHDWindowScale)
        {
            isWide = false;
            playerPosOffset = AssetManager::AdjustCurrentWindowScale(PlayerPosOffset + PlayerPosReOffset);
            enemyPosOffset = AssetManager::AdjustCurrentWindowScale(EnemyPosOffset + EnemyPosReOffset);
        }
        else
        {
            playerPosOffset = AssetManager::AdjustCurrentWindowScale(PlayerPosOffset);
            enemyPosOffset = AssetManager::AdjustCurrentWindowScale(EnemyPosOffset);
        }
        const Vec2 centerPos{ Scene::CenterF() };
        const Vec2 playerBoardPos{ centerPos + playerPosOffset };
        const Vec2 enemyBoardPos{ centerPos + enemyPosOffset };

        String useBlackPlayerName = ResourceProvider::GetPlayerNameText(usedBlack.playerType_);
        String useWhitePlayerName = ResourceProvider::GetPlayerNameText(usedWhite.playerType_);

        if (usedBlack.playerType_ == PlayerType::Player1)
        {
            whiteStoneBoard_->SetPosition(enemyBoardPos, isWide);
            whiteStoneBoard_->SetPlayerName(useWhitePlayerName);

            blackStoneBoard_->SetPosition(playerBoardPos, isWide);
            blackStoneBoard_->SetPlayerName(useBlackPlayerName);
        }
        else
        {
            whiteStoneBoard_->SetPosition(playerBoardPos, isWide);
            whiteStoneBoard_->SetPlayerName(useWhitePlayerName);

            blackStoneBoard_->SetPosition(enemyBoardPos, isWide);
            blackStoneBoard_->SetPlayerName(useBlackPlayerName);
        }

        // 石が割り当てられてたら描画を開始する
        CompletedInitialized();
    }

    Vec2 ScoreBoardManager::GetScoreBoardCenterPos(StoneType stoneType) const
    {
        switch (stoneType)
        {
        case StoneType::Black:
        {
            assert(blackStoneBoard_);
            return blackStoneBoard_->GetCenterPos();
        }
        case StoneType::White:
        {
            assert(whiteStoneBoard_);
            return whiteStoneBoard_->GetCenterPos();
        }
        default:
        {
            assert(false && "StoneType isn't match.");
            return Vec2::Zero();
        }
        }
    }

    void ScoreBoardManager::CompletedInitialized()
    {
        isInitialized_ = true;
    }

    bool ScoreBoardManager::TryGetGameState(std::shared_ptr<GameState>& outGameState) const
    {
        outGameState = stateObserver_.lock();
        return static_cast<bool>(outGameState);
    }
}
