#pragma once

#include "Feature/Game/UI/Drawer.h"
#include <memory>
#include <vector>

namespace ReversiSiv3D {

    class UIBase
    {
    public:

        UIBase();
        virtual ~UIBase();

        // ロジック処理は派生先クラスで必要に応じて実装する
        // 描画処理はDrawerクラスを使用してこのクラス上で実施する
        virtual void Update(double) {};
        void Render();

    protected:

        void AddDrawable(std::shared_ptr<Drawer>);

    private:

        std::vector<std::shared_ptr<Drawer>> drawerList_;
    };
}
