#pragma once

#include "Feature/System/RenderLayer.h"
#include "Feature/System/TaskId.h"
#include <map>
#include <vector>

namespace ReversiSiv3D {

    class RenderManager
    {
    public:

        RenderManager();
        ~RenderManager();

        static void CreateRenderLayerList();
        static std::vector<TaskId> GetRenderOrder();

    private:

        inline static std::map<RenderLayer::TaskType, TaskId> renderOrderTaskTypeList_{};
        inline static std::map<RenderLayer::HUDType, TaskId> renderOrderHudList_{};
        inline static std::map<RenderLayer::UIType, TaskId> renderOrderUiList_{};
        inline static std::map<RenderLayer::SystemUIType, TaskId> renderOrderSystemUiList_{};
    };
}
