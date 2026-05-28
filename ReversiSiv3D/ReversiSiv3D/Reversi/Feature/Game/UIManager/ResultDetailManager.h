#pragma once

#include "Feature/Game/GameInfo.h"
#include "Feature/Game/SharedEnum.h"
#include "Feature/Game/UI/Result/Draw.h"
#include "Feature/Game/UI/Result/Lose.h"
#include "Feature/Game/UI/Result/Win.h"
#include "Feature/System/SoundManager.h"
#include "Feature/System/Task.h"
#include <memory>
#include <type_traits>
#include <vector>

namespace ReversiSiv3D {

    enum class ResultType : int
    {
        Win = 0,
        Lose = 1,
        Draw = 2
    };

    class ResultDetailManager : public Task
    {
    public:

        ResultDetailManager();
        ~ResultDetailManager();

        void Update(double) override;
        void Render();

        void Initialize();
        void Show();

    private:

        template <typename TResultDetail>
        std::unique_ptr<TResultDetail> CreateResultDetail(
            StoneType stoneType, const PlayerData& playerData)
        {
            static_assert(std::is_base_of_v<ResultDetail, TResultDetail>);
            std::unique_ptr<TResultDetail> result = std::make_unique<TResultDetail>();
            SetupResultDetail(*result, stoneType, playerData);
            return result;
        }

        void SetupResultDetail(ResultDetail&, StoneType, const PlayerData&);
        ResultType CheckResultOfUseBlackStone(int, int) const;
        void SettingSound(const PlayerData&, const PlayerData&);

    private:

        static constexpr Vec2 StoneIconOffsetPos{ 700.0, -250.0 };
        static constexpr Vec2 PlayerNameOffsetPos{ 700.0, -125.0 };
        static constexpr Vec2 StoneCountOffsetPos{ 700.0, -25.0 };
        static constexpr Vec2 ResultIconOffsetPos{ 700.0, 175.0 };
        static constexpr Vec2 ReverseXAxis{ -1.0, 1.0 };
        static constexpr Vec2 ScaleOffsetPos{ -135.0, 0.0 };
        static constexpr double FullHDWindowScale{ 1.0 };

    private:

        std::vector<std::unique_ptr<ResultDetail>> resultDetailList_;
        SoundType resultSeType_;
    };
}
