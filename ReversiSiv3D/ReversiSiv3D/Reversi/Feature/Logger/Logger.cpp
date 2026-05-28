#include "Feature/Logger/Logger.h"

namespace ReversiSiv3D {

    void Logger::Used()
    {
        isUsed_ = true;
    }

    void Logger::SetOutputLogType(const std::vector<LogType>& logTypeList)
    {
        isSpecifiedLogType_ = true;
        outputLogTypeList_ = logTypeList;
    }

    void Logger::SetOutputLogDetail(const std::vector<LogDetail>& logDetailList)
    {
        isSpecifiedLogDetail_ = true;
        outputLogDetailList_ = logDetailList;
    }

    void Logger::InfoWriteLine(const std::wstring& message, LogDetail logDetail)
    {
        OutputFieldWriteLine(message, LogType::Info, logDetail);
    }

    void Logger::DebugWriteLine(const std::wstring& message, LogDetail logDetail)
    {
        OutputFieldWriteLine(message, LogType::Debug, logDetail);
    }

    void Logger::WarningWriteLine(const std::wstring& message, LogDetail logDetail)
    {
        OutputFieldWriteLine(message, LogType::Warning, logDetail);
    }

    void Logger::ErrorWriteLine(const std::wstring& message, LogDetail logDetail)
    {
        OutputFieldWriteLine(message, LogType::Error, logDetail);
    }

    void Logger::OutputFieldWriteLine(const std::wstring& message, LogType logType, LogDetail logDetail)
    {
        const bool isOutput = IsOutput(logType, logDetail);
        if (!isOutput)
        {
            return;
        }

        const LogOutputField& logOutputField{ GetLogOutputFieldInstance() };
        std::wstring logMessage{ std::format(L"[{}] : {}", GetLogTypeName(logType), message) };
        logOutputField.WriteLine(logMessage);
    }

    bool Logger::IsOutput(LogType logType, LogDetail logDetail)
    {
        if (!isUsed_)
        {
            return false;
        }

        if (isSpecifiedLogType_)
        {
            const bool isContain = std::ranges::find(outputLogTypeList_, logType) != outputLogTypeList_.end();
            if (!isContain)
            {
                return false;
            }
        }

        if (isSpecifiedLogDetail_)
        {
            const bool isContain = std::ranges::find(outputLogDetailList_, logDetail) != outputLogDetailList_.end();
            if (!isContain)
            {
                return false;
            }
        }

        return true;
    }

    const LogOutputField& Logger::GetLogOutputFieldInstance()
    {
        if (!outputField_)
        {
            outputField_ = std::make_unique<LogOutputField>();
        }
        return *outputField_.get();
    }

    std::wstring Logger::GetLogTypeName(LogType logType)
    {
        switch (logType)
        {
        case LogType::Info:
        {
            return L"Info";
        }
        case LogType::Debug:
        {
            return L"Debug";
        }
        case LogType::Warning:
        {
            return L"Warning";
        }
        case LogType::Error:
        {
            return L"Error";
        }
        default:
        {
            return L"Unknown";
        }
        }
    }
}
