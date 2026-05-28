#pragma once

namespace ReversiSiv3D {

    // TaskManagerクラス上で管理するタスクのID
    // タスク生成時に割り当てる

    enum class TaskId : int
    {
        Empty = 0,
        LevelChanger = 1,
        FadeManager = 10,
        BootLevel = 20,
        TitleLevel = 30,
        BattleLevel = 40,
        ResultLevel = 50,
        MouseController = 90,
        BackGround = 100,
        BootEvent = 110,
        TitleDesigner = 200,
        TitleButton = 210,
        BattleBorad = 300,
        Cursor = 310,
        Stone = 320,
        Indicator = 330,
        GameMaster = 340,
        PanelUI = 350,
        TurnMarker = 360,
        ScoreBoard = 370,
        Enemy = 380,
        TurnOrder = 390,
        GameResult = 500,
        StoneAnimator = 510,
        ResultDetail = 520,
        ResultButton = 530,
    };

    enum class LevelType
    {
        BootLevel,
        TitleLevel,
        BattleLevel,
        ResultLevel
    };
}
