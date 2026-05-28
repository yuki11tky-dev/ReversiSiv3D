#include "Feature/Game/ReversiLogic.h"
#include "Feature/Game/UIManager/CursorManager.h"
#include <cassert>

namespace ReversiSiv3D {

    CursorManager::CursorManager()
        : Task{ TaskId::Cursor }
        , boardCursorList_{}
    {

    }

    CursorManager::~CursorManager()
    {

    }

    void CursorManager::Update(double deltaTime)
    {
        for (const auto& [type, cursor] : boardCursorList_)
        {
            // 変数   : 型(クラス)
            // type   : CursorType
            // cursor : const std::unique_ptr<BoardCursor>&
            assert(cursor);
            cursor->Update(deltaTime);
        }
    }

    void CursorManager::Render()
    {
        for (const auto& [type, cursor] : boardCursorList_)
        {
            // 変数   : 型(クラス)
            // type   : CursorType
            // cursor : const std::unique_ptr<BoardCursor>&
            assert(cursor);
            cursor->Render();
        }
    }

    void CursorManager::Release()
    {
        boardCursorList_.clear();
    }

    void CursorManager::CreatePlayerCursor()
    {
        std::unique_ptr<PlayerCursor> playerCursor = std::make_unique<PlayerCursor>();
        playerCursor->LoadTexture(U"Image/player_cursor.png");
        playerCursor->Initialize();
        boardCursorList_.emplace(CursorType::Player1, std::move(playerCursor));
    }

    void CursorManager::CreateEnemyCursor()
    {
        std::unique_ptr<PlayerCursor> playerCursor = std::make_unique<PlayerCursor>();
        playerCursor->LoadTexture(U"Image/enemy_cursor.png");
        playerCursor->Initialize();
        boardCursorList_.emplace(CursorType::Player2, std::move(playerCursor));
    }

    void CursorManager::SetCurrentCursorPos(Vec2 cursorCenterPos)
    {
        // カーソルを描画する位置がボード内であることはこの関数を実行前にチェックする必要あり

        for (const auto& [type, cursor] : boardCursorList_)
        {
            // 変数   : 型(クラス)
            // type   : CursorType
            // cursor : const std::unique_ptr<BoardCursor>&
            assert(cursor);
            cursor->SetCenterPos(cursorCenterPos);
        }
    }

    void CursorManager::VisibleCursor(CursorType switchedCursorType)
    {
        for (const auto& [type, cursor] : boardCursorList_)
        {
            // 変数   : 型(クラス)
            // type   : CursorType
            // cursor : const std::unique_ptr<BoardCursor>&
            assert(cursor);
            if (switchedCursorType == type)
            {
                cursor->Visible();
            }
            else
            {
                cursor->Hide();
            }
        }
    }

    void CursorManager::HideAllCursor()
    {
        for (const auto& [type, cursor] : boardCursorList_)
        {
            // 変数   : 型(クラス)
            // type   : CursorType
            // cursor : std::unique_ptr<BoardCursor>
            assert(cursor);
            cursor->Hide();
        }
    }
}
