#pragma once

#include "Feature/System/TaskId.h"

namespace ReversiSiv3D {

    class Task
    {
    public:

        Task(TaskId);
        virtual ~Task();

        virtual void Update(double) {};
        virtual void Render() {};
        virtual void Release() {};

        TaskId GetTaskId() const;
        bool IsRelease() const;
        void SetReleased();

    private:

        TaskId taskId_;
        bool isReleased_;
    };
}
