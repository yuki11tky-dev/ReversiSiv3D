#pragma once

#include "Feature/System/RenderManager.h"
#include "Feature/System/Task.h"
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>


namespace ReversiSiv3D {

    class TaskManager
    {
    public:

        TaskManager();
        ~TaskManager();

        bool AddTask(std::shared_ptr<Task>);
        void ReleaseTask(TaskId);
        void Update(double);
        void Render();

    private:

        std::unordered_map<TaskId, std::shared_ptr<Task>> taskList_;
        std::vector<std::shared_ptr<Task>> addTaskList_;
        std::vector<TaskId> releaseTaskIdList_;
        bool isUpdating_;
    };
}
