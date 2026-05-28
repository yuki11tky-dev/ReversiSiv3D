#pragma once

namespace ReversiSiv3D {

    class BattleButtonEventInterface {
    public:

        BattleButtonEventInterface()
        {

        }

        ~BattleButtonEventInterface()
        {

        }

        virtual void OnDecideFirstPlayer() = 0;
        virtual void OnDecideSecondPlayer() = 0;
    };
}
