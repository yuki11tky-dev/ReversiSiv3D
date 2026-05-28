#include "Feature/Common/Math.h"
#include "Feature/Game/LogicManager/MouseController.h"
#include <Windows.h>

namespace ReversiSiv3D {

    MouseController::MouseController()
        : Task(TaskId::MouseController)
        , mousePos_{}
        , mousePosOld_{}
        , isPressedLeftButton_{ false }
    {

    }

    MouseController::~MouseController()
    {

    }

    void MouseController::Update(double)
    {
        // マウスカーソルの座標と操作(左クリック押下)を取得する

        // ゲームウィンドウ上のマウスカーソルの座標
        // ゲームウィンドウ左上隅が原点を表す
        mousePos_ = Math::RoundForVec2(Cursor::PosF(), 1);
        mousePosOld_ = Math::RoundForVec2(Cursor::PreviousPosF(), 1);

        isPressedLeftButton_ = MouseL.down();
    }

    Vec2 MouseController::GetCurrentMouseCursorPos() const
    {
        return mousePos_;
    }

    Vec2 MouseController::GetPreviousMouseCursorPos() const
    {
        return mousePosOld_;
    }

    bool MouseController::IsPressLeftButton() const
    {
        return isPressedLeftButton_;
    }
}
