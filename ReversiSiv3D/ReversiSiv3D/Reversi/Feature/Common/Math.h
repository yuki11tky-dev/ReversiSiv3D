#pragma once

namespace ReversiSiv3D {

    class Math
    {
    public:

        static double Multiply(double, double);
        static double Divide(double, double);
        static double Multiply(int, int);
        static double Divide(int, int);

        static Point Multiply(Point, int);
        static Point Multiply(Point, double);
        static Vec2 Multiply(Vec2, double);
        static Vec2 Divide(Vec2, int);

        static int Multiply(int, double);

        static double Round(double);
        static int RoundForInt(double);
        static Vec2 RoundForVec2(Vec2, int);

        static double Floor(double);
        static int FloorForInt(double);

        static bool AlmostEqual(double, double);

        static int AbsDiff(int, int);

        static double ToRadians(double);

    private:

        static constexpr double Epsilon{ 0.00000001 };      // Allowable error range
    };
}
