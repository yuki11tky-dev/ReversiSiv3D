#pragma once

namespace ReversiSiv3D {

    class TitleButtonEventInterface {
    public:

        TitleButtonEventInterface()
        {

        }

        ~TitleButtonEventInterface()
        {

        }

        virtual void OnStartGame() = 0;
        virtual void OnEndGame() = 0;
    };
}
