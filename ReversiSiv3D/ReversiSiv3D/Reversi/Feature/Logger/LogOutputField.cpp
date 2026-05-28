#include "Feature/Logger/LogOutputField.h"
#include <Windows.h>

namespace ReversiSiv3D {

    LogOutputField::LogOutputField()
    {

    }

    LogOutputField::~LogOutputField()
    {

    }

    void LogOutputField::WriteLine(const std::wstring& message)
    {
        OutputDebugStringW(message.c_str());
        OutputDebugStringW(L"\n");
    }
}
