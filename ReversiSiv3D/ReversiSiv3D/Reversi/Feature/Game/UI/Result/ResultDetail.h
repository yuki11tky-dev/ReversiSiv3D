#pragma once

#include "Feature/Game/UI/Image.h"
#include "Feature/Game/UI/Text.h"
#include "Feature/Game/UI/UIBase.h"
#include <memory>

namespace ReversiSiv3D {

    class ResultDetail : public UIBase
    {
    public:

        ResultDetail();
        ~ResultDetail();

        virtual void LoadResultIcon() = 0;
        virtual void Start() = 0;

        void Initialize();

        void LoadStoneIcon(const String&);
        void SetStoneIconCenterPos(Vec2);
        void SetResultIconCenterPos(Vec2);
        void SetStoneIconScale(double);
        void SetResultIconScale(double);
        void SetResultIconAngleForRadian(double);
        void SetResultIconAngleForDegree(double);

        void SetPlayerNameCenterPos(Vec2);
        void SetStoneCountCenterPos(Vec2);
        void SetPlayerNameText(const String&);
        void SetStoneCountText(const String&);
        void SetPlayerNameColor(ColorF);
        void SetStoneCountColor(ColorF);

    protected:

        void Visible();
        void Hide();
        double GetResultIconScale() const;
        double GetResultIconAngleForRadian() const;

    private:

        void SetStoneIconAlpha(double);
        void SetResultIconAlpha(double);
        void SetPlayerNameAlpha(double);
        void SetStoneCountAlpha(double);

    protected:

        std::weak_ptr<Image> stoneIcon_;
        std::weak_ptr<Image> resultIcon_;
        std::weak_ptr<Text> playerName_;
        std::weak_ptr<Text> stoneCount_;
    };
}
