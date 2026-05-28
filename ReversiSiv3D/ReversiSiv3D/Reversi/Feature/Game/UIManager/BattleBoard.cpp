#include "Feature/Common/AssetManager.h"
#include "Feature/Common/Math.h"
#include "Feature/Game/UIManager/BattleBoard.h"
#include <cassert>

namespace ReversiSiv3D {

    BattleBoard::BattleBoard()
        : Task{ TaskId::BattleBorad }
        , boardDrawer_{}
        , boardFrameDrawer_{}
        , boardDrawInfoList_{}
        , frameDrawInfoList_{}
    {

    }

    BattleBoard::~BattleBoard()
    {

    }

    void BattleBoard::Initialize(const std::shared_ptr<GameState>& gameState)
    {
        boardDrawer_ = std::make_unique<BoardDrawer>();
        boardDrawer_->Initialize();

        boardFrameDrawer_ = std::make_unique<BoardFrameDrawer>();
        boardFrameDrawer_->Initialize();

        const Point windowCenter{ Scene::Center() };
        const Size boardPieceResize{ AssetManager::AdjustCurrentWindowScale(boardDrawer_->GetImageSize()) };

        constexpr int rowCellHalfCount{ BoardRowCellCount / 2 };
        constexpr int colCellHalfCount{ BoardColCellCount / 2 };
        Point boardTopLeftPos{};
        boardTopLeftPos.x = windowCenter.x - boardPieceResize.x * colCellHalfCount;
        boardTopLeftPos.y = windowCenter.y - boardPieceResize.y * rowCellHalfCount;

        const Size framePieceSize{ boardFrameDrawer_->GetPieceSize() };
        const Size framePieceResize{ AssetManager::AdjustCurrentWindowScale(framePieceSize) };
        Point frameTopLeft{ boardTopLeftPos - framePieceResize };

        gameState->SetBoardTopLeftPos(boardTopLeftPos);
        gameState->SetBoardPieceSize(boardPieceResize);
        gameState->SetFrameTopLeftPos(frameTopLeft);
        gameState->SetFramePieceSize(framePieceResize);

        // 描画位置の算出
        double angle{ 0_deg };
        FrameType frameType{ FrameType::Conner };
        Vec2 boardPos{};
        RenderFrameInfo renderFrameInfo{};
        for (int row = 0; row < BoardRowFrameCount; ++row)
        {
            boardPos.y = frameTopLeft.y + row * framePieceResize.y;
            boardPos.y += Math::Divide(framePieceResize.y, 2);

            for (int col = 0; col < BoardColFrameCount; ++col)
            {
                // リバーシ盤面に関する描画情報を扱う
                if ((row != FrameStartIndex && row != FrameEndIndex) &&
                    (col != FrameStartIndex && col != FrameEndIndex))
                {
                    // 描画位置をリストに格納
                    boardPos.x = frameTopLeft.x + col * boardPieceResize.x;
                    boardPos.x += Math::Divide(boardPieceResize.x, 2);

                    boardDrawInfoList_.push_back(boardPos);
                    continue;
                }

                // リバーシ盤の外フレームに関する描画情報を扱う
                if (row == FrameStartIndex && col == FrameStartIndex)
                {
                    frameType = FrameType::Conner;
                    angle = 0_deg;
                }
                else if (row == FrameStartIndex && col == FrameEndIndex)
                {
                    frameType = FrameType::Conner;
                    angle = 90_deg;
                }
                else if (row == FrameEndIndex && col == FrameEndIndex)
                {
                    frameType = FrameType::Conner;
                    angle = 180_deg;
                }
                else if (row == FrameEndIndex && col == FrameStartIndex)
                {
                    frameType = FrameType::Conner;
                    angle = 270_deg;
                }
                else
                {
                    frameType = FrameType::Outer;
                    if (row == FrameStartIndex)
                    {
                        angle = 0_deg;
                    }
                    else if (row == FrameEndIndex)
                    {
                        angle = 180_deg;
                    }
                    else if (col == FrameStartIndex)
                    {
                        angle = 270_deg;
                    }
                    else if (col == FrameEndIndex)
                    {
                        angle = 90_deg;
                    }
                }

                // 描画情報をリストに格納
                boardPos.x = frameTopLeft.x + col * framePieceResize.x;
                boardPos.x += Math::Divide(framePieceResize.x, 2);

                renderFrameInfo.centerPos_ = boardPos;
                renderFrameInfo.drawIndex_ = frameType;
                renderFrameInfo.angle_ = angle;
                frameDrawInfoList_.push_back(renderFrameInfo);
            }
        }
    }

    void BattleBoard::Render()
    {
        assert(boardDrawer_ && boardFrameDrawer_);

        for (const Vec2& drawPos : boardDrawInfoList_)
        {
            boardDrawer_->SetCenterPos(drawPos);
            boardDrawer_->Render();
        }

        for (const RenderFrameInfo& renderFrameInfo : frameDrawInfoList_)
        {
            boardFrameDrawer_->SetCenterPos(renderFrameInfo.centerPos_);
            boardFrameDrawer_->SetDrawIndex(renderFrameInfo.drawIndex_);
            boardFrameDrawer_->SetAngle(renderFrameInfo.angle_);
            boardFrameDrawer_->Render();
        }
    }
}
