#include "Feature/Common/Math.h"
#include <cmath>
#include <limits>

namespace ReversiSiv3D {

    double Math::Multiply(double value, double scalar)
    {
        double result = value * scalar;
        if (std::abs(result) <= Epsilon)
        {
            result = 0.0;
        }
        return result;
    }

    double Math::Divide(double value, double divisor)
    {
        if (std::abs(divisor) <= Epsilon)
        {
            divisor = 0.0;
        }

        if (divisor == 0.0)
        {
            return 0.0;
        }

        double result = value / divisor;
        if (std::abs(result) <= Epsilon)
        {
            result = 0.0;
        }
        return result;
    }

    double Math::Multiply(int value, int scalar)
    {
        double result = static_cast<double>(value) * static_cast<double>(scalar);
        if (std::abs(result) <= Epsilon)
        {
            result = 0.0;
        }
        return result;
    }

    double Math::Divide(int value, int divisor)
    {
        const double dblValue = static_cast<double>(value);
        double dblDivisor = static_cast<double>(divisor);
        if (std::abs(dblDivisor) <= Epsilon)
        {
            dblDivisor = 0.0;
        }

        if (dblDivisor == 0.0)
        {
            return std::numeric_limits<double>::max();
        }

        double result = dblValue / dblDivisor;
        if (std::abs(result) <= Epsilon)
        {
            result = 0.0;
        }
        return result;
    }

    Point Math::Multiply(Point values, int scalar)
    {
        const double dblValueX = static_cast<double>(values.x) * scalar;
        const double dblValueY = static_cast<double>(values.y) * scalar;
        return Point{ RoundForInt(dblValueX), RoundForInt(dblValueY) };
    }

    Point Math::Multiply(Point values, double scalar)
    {
        const double dblValueX = static_cast<double>(values.x) * scalar;
        const double dblValueY = static_cast<double>(values.y) * scalar;
        return Point{ RoundForInt(dblValueX), RoundForInt(dblValueY) };
    }

    Vec2 Math::Multiply(Vec2 vec, double scalar)
    {
        Vec2 resultVec{};
        resultVec.x = vec.x * scalar;
        resultVec.y = vec.y * scalar;
        return resultVec;
    }

    Vec2 Math::Divide(Vec2 vec, int divisor)
    {
        double dblDivisor = static_cast<double>(divisor);
        if (std::abs(dblDivisor) <= Epsilon)
        {
            dblDivisor = 0.0;
        }

        if (dblDivisor == 0.0)
        {
            return Vec2{ std::numeric_limits<double>::max(), std::numeric_limits<double>::max() };
        }

        Vec2 returnVec{};
        returnVec.x = vec.x / dblDivisor;
        returnVec.y = vec.y / dblDivisor;
        return returnVec;
    }

    int Math::Multiply(int intValue, double scalar)
    {
        const double dblValue = static_cast<double>(intValue);
        return RoundForInt(dblValue * scalar);
    }

    double Math::Round(double value)
    {
        return std::round(value / Epsilon) * Epsilon;
    }

    int Math::RoundForInt(double value)
    {
        return static_cast<int>(std::round(value));
    }

    Vec2 Math::RoundForVec2(Vec2 values, int digitsOfNum)
    {
        const double scale = std::pow(10.0, digitsOfNum);
        Vec2 resultValues = Vec2{ values };
        resultValues.x = std::round(resultValues.x * scale) / scale;
        resultValues.y = std::round(resultValues.y * scale) / scale;
        return resultValues;
    }

    double Math::Floor(double value)
    {
        return std::floor(value);
    }

    int Math::FloorForInt(double value)
    {
        return static_cast<int>(std::floor(value));
    }

    bool Math::AlmostEqual(double valueX, double valueY)
    {
        return std::abs(valueX - valueY) < Epsilon;
    }

    int Math::AbsDiff(int valueX, int valueY)
    {
        return (valueX > valueY) ? (valueX - valueY) : (valueY - valueX);
    }

    double Math::ToRadians(double digree)
    {
        return s3d::Math::ToRadians(digree);
    }
}
