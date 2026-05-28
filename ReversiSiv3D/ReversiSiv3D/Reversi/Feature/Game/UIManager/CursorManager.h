#pragma once

#include "Feature/Game/SharedEnum.h"
#include "Feature/Game/UI/Cursor/PlayerCursor.h"
#include "Feature/System/Task.h"

namespace ReversiSiv3D {

    class CursorManager : public Task
    {
    public:

        CursorManager();
        ~CursorManager();

        void Update(double) override;
        void Render() override;
        void Release() override;

        void CreatePlayerCursor();
        void CreateEnemyCursor();

        void SetCurrentCursorPos(Vec2);
        void VisibleCursor(CursorType);
        void HideAllCursor();

    private:

        std::unordered_map<CursorType, std::unique_ptr<BoardCursor>> boardCursorList_;
    };
}
