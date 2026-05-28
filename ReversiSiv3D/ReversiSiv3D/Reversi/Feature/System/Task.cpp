#include "Feature/System/Task.h"

namespace ReversiSiv3D {

    Task::Task(TaskId taskId)
        : taskId_{ taskId }
        , isReleased_{ false }
    {

    }

    Task::~Task()
    {

    }

    TaskId Task::GetTaskId() const
    {
        return taskId_;
    }

    bool Task::IsRelease() const
    {
        return isReleased_;
    }

    void Task::SetReleased()
    {
        isReleased_ = true;
    }
}
