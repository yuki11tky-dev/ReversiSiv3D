#pragma once

namespace ReversiSiv3D {

    class RenderLayer {
    public :

        enum RenderType : int
        {
            Task = 0,
            HUD = 1000,
            UI = 2000,
            SystemUI = 3000,
        };

        enum TaskType : int
        {
            BackGround = 0,
            BootLevel = 10,
            TitleLevel = 20,
            BattleLevel = 30,
            ResultLevel = 40,
            BootEvent = 50,
            BattleBoard = 300,
            Cursor = 310,
            Stone = 320,
            Indicator = 330,
            GameMaster = 340,
        };

        enum HUDType : int
        {
            TurnMarker = 300,
            ScoreBoard = 310,
        };

        enum UIType : int
        {
            TitleDesigner = 100,
            TitleButton = 110,
            PanelUI = 200,
            TurnOrder= 300,
            ResultDetail = 400,
            ResultButton = 410,
        };

        enum SystemUIType : int
        {
            Fade = 999,
        };
    };
}
