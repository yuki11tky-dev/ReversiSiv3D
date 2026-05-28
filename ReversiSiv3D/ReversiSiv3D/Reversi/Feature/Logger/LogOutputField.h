#pragma once

#include <string>

namespace ReversiSiv3D {

    class LogOutputField
    {
    public:

        LogOutputField();
        ~LogOutputField();

        static void WriteLine(const std::wstring&);
    };
}
