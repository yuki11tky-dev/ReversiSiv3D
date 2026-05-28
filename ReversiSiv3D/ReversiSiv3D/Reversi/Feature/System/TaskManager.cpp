#include "Feature/System/TaskManager.h"


namespace ReversiSiv3D {

    TaskManager::TaskManager()
        : taskList_{}
        , addTaskList_{}
        , releaseTaskIdList_{}
        , isUpdating_{ false }
    {

    }

    TaskManager::~TaskManager()
    {

    }

    bool TaskManager::AddTask(std::shared_ptr<Task> task)
    {
        if (!task)
        {
            return false;
        }

        const TaskId taskId = task->GetTaskId();
        if (taskList_.contains(taskId))
        {
            return false;
        }

        if (isUpdating_)
        {
            // 更新処理中は taskList_ を使用しているためリストの要素数の変更は実施しない
            // 一度 addTaskList_ に追加しておき、更新処理後にリスト追加する
            addTaskList_.push_back(std::move(task));
        }
        else
        {
            taskList_[taskId] = std::move(task);
        }

        return true;
    }

    void TaskManager::ReleaseTask(TaskId taskId)
    {
        if (!taskList_.contains(taskId))
        {
            return;
        }

        const std::shared_ptr<Task>& task = taskList_.at(taskId);
        if (!task)
        {
            return;
        }

        task->SetReleased();

        if (isUpdating_)
        {
            // 更新処理中は taskList_ を使用しているためリストの要素数の変更は実施しない
            // 一度 releaseTaskIdList_ に追加しておき、更新処理後にリストから削除する
            releaseTaskIdList_.push_back(taskId);
        }
        else
        {
            taskList_.erase(taskId);
        }
    }

    void TaskManager::Update(double deltaTime)
    {
        isUpdating_ = true;

        for (const auto& [taskId, task] : taskList_)
        {
            // 変数   : 型(クラス)
            // taskId : TaskId
            // task   : std::shared_ptr<Task>
            if (!task)
            {
                continue;
            }
            if (task->IsRelease())
            {
                continue;
            }
            task->Update(deltaTime);
        }

        isUpdating_ = false;

        for (const std::shared_ptr<Task>& task : addTaskList_)
        {
            if (!task)
            {
                continue;
            }
            const TaskId taskId = task->GetTaskId();
            taskList_[taskId] = task;
        }
        addTaskList_.clear();

        for (const TaskId taskId : releaseTaskIdList_)
        {
            if (!taskList_.contains(taskId))
            {
                continue;
            }
            const std::shared_ptr<Task>& task = taskList_.at(taskId);
            if (task)
            {
                task->Release();
            }
            taskList_.erase(taskId);
        }
        releaseTaskIdList_.clear();
    }

    void TaskManager::Render()
    {
        const std::vector<TaskId> renderOrder = RenderManager::GetRenderOrder();
        for (const TaskId taskId : renderOrder)
        {
            if (!taskList_.contains(taskId))
            {
                continue;
            }
            const std::shared_ptr<Task>& task = taskList_.at(taskId);
            if (!task)
            {
                continue;
            }
            if (task->IsRelease())
            {
                continue;
            }
            task->Render();
        }
    }
}
