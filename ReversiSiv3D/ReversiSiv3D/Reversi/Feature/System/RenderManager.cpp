#include "Feature/System/RenderManager.h"

namespace ReversiSiv3D {

    RenderManager::RenderManager()
    {

    }

    RenderManager::~RenderManager()
    {

    }

    void RenderManager::CreateRenderLayerList()
    {
        renderOrderTaskTypeList_[RenderLayer::TaskType::BackGround] = TaskId::BackGround;
        renderOrderTaskTypeList_[RenderLayer::TaskType::BootLevel] = TaskId::BootLevel;
        renderOrderTaskTypeList_[RenderLayer::TaskType::TitleLevel] = TaskId::TitleLevel;
        renderOrderTaskTypeList_[RenderLayer::TaskType::BattleLevel] = TaskId::BattleLevel;
        renderOrderTaskTypeList_[RenderLayer::TaskType::ResultLevel] = TaskId::ResultLevel;
        renderOrderTaskTypeList_[RenderLayer::TaskType::BootEvent] = TaskId::BootEvent;
        renderOrderTaskTypeList_[RenderLayer::TaskType::BattleBoard] = TaskId::BattleBorad;
        renderOrderTaskTypeList_[RenderLayer::TaskType::Cursor] = TaskId::Cursor;
        renderOrderTaskTypeList_[RenderLayer::TaskType::Stone] = TaskId::Stone;
        renderOrderTaskTypeList_[RenderLayer::TaskType::Indicator] = TaskId::Indicator;
        renderOrderTaskTypeList_[RenderLayer::TaskType::GameMaster] = TaskId::GameMaster;

        renderOrderHudList_[RenderLayer::HUDType::TurnMarker] = TaskId::TurnMarker;
        renderOrderHudList_[RenderLayer::HUDType::ScoreBoard] = TaskId::ScoreBoard;

        renderOrderUiList_[RenderLayer::UIType::TitleDesigner] = TaskId::TitleDesigner;
        renderOrderUiList_[RenderLayer::UIType::TitleButton] = TaskId::TitleButton;
        renderOrderUiList_[RenderLayer::UIType::PanelUI] = TaskId::PanelUI;
        renderOrderUiList_[RenderLayer::UIType::TurnOrder] = TaskId::TurnOrder;
        renderOrderUiList_[RenderLayer::UIType::ResultDetail] = TaskId::ResultDetail;
        renderOrderUiList_[RenderLayer::UIType::ResultButton] = TaskId::ResultButton;

        renderOrderSystemUiList_[RenderLayer::SystemUIType::Fade] = TaskId::FadeManager;
    }

    std::vector<TaskId> RenderManager::GetRenderOrder()
    {
        // 描画の実行順序を関数内で作成し、
        // 作成したリストを呼び出し元へ返す
        std::vector<TaskId> renderOrder;

        // 今後、ここで描画順序を取得して設定する
        std::vector<RenderLayer::RenderType> renderTypeList;
        constexpr int renderLayerCount{ 4 };
        renderTypeList.reserve(renderLayerCount);
        renderTypeList.push_back(RenderLayer::RenderType::Task);
        renderTypeList.push_back(RenderLayer::RenderType::HUD);
        renderTypeList.push_back(RenderLayer::RenderType::UI);
        renderTypeList.push_back(RenderLayer::RenderType::SystemUI);

        for (const RenderLayer::RenderType renderType : renderTypeList)
        {
            switch (renderType)
            {
            case RenderLayer::RenderType::Task:
            {
                for (const auto& [taskType, taskId] : renderOrderTaskTypeList_)
                {
                    // 変数   : 型(クラス)
                    // taskType : RenderLayer::TaskType
                    // taskId   : TaskId
                    renderOrder.push_back(taskId);
                }
                break;
            }
            case RenderLayer::RenderType::HUD:
            {
                for (const auto& [hudType, taskId] : renderOrderHudList_)
                {
                    // 変数   : 型(クラス)
                    // hudType : RenderLayer::HUDType
                    // taskId  : TaskId
                    renderOrder.push_back(taskId);
                }
                break;
            }
            case RenderLayer::RenderType::UI:
            {
                for (const auto& [uiType, taskId] : renderOrderUiList_)
                {
                    // 変数   : 型(クラス)
                    // uiType : RenderLayer::UIType
                    // taskId : TaskId
                    renderOrder.push_back(taskId);
                }
                break;
            }
            case RenderLayer::RenderType::SystemUI:
            {
                for (const auto& [systemUiType, taskId] : renderOrderSystemUiList_)
                {
                    // 変数   : 型(クラス)
                    // systemUiType : RenderLayer::SystemUIType
                    // taskId       : TaskId
                    renderOrder.push_back(taskId);
                }
                break;
            }
            default:
            {
                break;
            }
            }
        }
        return renderOrder;
    }
}
