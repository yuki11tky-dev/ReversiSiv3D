#pragma once

namespace ReversiSiv3D {

    class AssetManager {
    public:

        static void CalculateWindowScale();

        static double GetCurrentWindowScale();

        static double AdjustCurrentWindowScale(double);
        static Size AdjustCurrentWindowScale(Size);
        static Vec2 AdjustCurrentWindowScale(Vec2);

    private:

        static constexpr double DefaultScale{ 1.0 };

    private:

        inline static double windowScale_{ DefaultScale };
    };
}
