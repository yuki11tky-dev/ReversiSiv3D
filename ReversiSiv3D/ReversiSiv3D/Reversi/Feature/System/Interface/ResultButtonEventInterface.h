#pragma once

namespace ReversiSiv3D {

    class ResultButtonEventInterface {
    public:

        ResultButtonEventInterface()
        {

        }

        ~ResultButtonEventInterface()
        {

        }

        virtual void OnStartGame() = 0;
        virtual void OnReturnTitle() = 0;
    };
}
