#pragma once

#include "Feature/Logger/LogOutputField.h"
#include <memory>
#include <string>

namespace ReversiSiv3D {

    enum class LogType
    {
        Info,
        Debug,
        Warning,
        Error
    };

    enum class LogDetail
    {
        Unspecified
    };

    class Logger
    {
    public:

        static void Used();
        static void SetOutputLogType(const std::vector<LogType>&);
        static void SetOutputLogDetail(const std::vector<LogDetail>&);

        static void InfoWriteLine(const std::wstring&, LogDetail logDetail = LogDetail::Unspecified);
        static void DebugWriteLine(const std::wstring&, LogDetail logDetail = LogDetail::Unspecified);
        static void WarningWriteLine(const std::wstring&, LogDetail logDetail = LogDetail::Unspecified);
        static void ErrorWriteLine(const std::wstring&, LogDetail logDetail = LogDetail::Unspecified);

    private:

        static void OutputFieldWriteLine(const std::wstring&, LogType, LogDetail);
        static bool IsOutput(LogType, LogDetail);
        static const LogOutputField& GetLogOutputFieldInstance();
        static std::wstring GetLogTypeName(LogType);

    private:

        inline static std::vector<LogType> outputLogTypeList_{};
        inline static std::vector<LogDetail> outputLogDetailList_{};

        inline static bool isUsed_ = false;
        inline static bool isSpecifiedLogType_ = false;
        inline static bool isSpecifiedLogDetail_ = false;
        inline static std::unique_ptr<LogOutputField> outputField_{};
    };
}
