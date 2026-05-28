#pragma once

namespace ReversiSiv3D {

    class Drawer
    {
    public:

        Drawer();
        virtual ~Drawer();

        virtual void Render() = 0;
    };
}
