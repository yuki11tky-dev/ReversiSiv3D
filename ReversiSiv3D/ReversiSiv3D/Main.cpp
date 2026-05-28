
#include <Siv3D.hpp> // Siv3D v0.6.16
#include <Windows.h>

#include "Feature/Common/AssetManager.h"
#include "Feature/Game/GameInfo.h"
#include "Feature/System/Fade.h"
#include "Feature/System/FadeManager.h"
#include "Feature/System/GameConfigLoader.h"
#include "Feature/System/LevelChanger.h"
#include "Feature/System/TaskManager.h"


bool StartupSettings()
{
    using namespace ReversiSiv3D;

    const bool isSuccess = GameConfigLoader::LoadGameConfig();
    if (!isSuccess)
    {
        return false;
    }

    const GameConfig& gameConfig = GameInfo::GetGameConfig();
    const GameWindowSettings& gameWindowSettings = gameConfig.GetWindowSettings();
    const Size windowSize{ gameWindowSettings.width_, gameWindowSettings.height_ };

    Window::Resize(windowSize);
    Scene::Resize(windowSize);

    return true;
}

void Main()
{

#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    using namespace ReversiSiv3D;

    // ウィンドウサイズの変更を実施
    StartupSettings();

    // taskManager : タスクマネージャークラス
    // ゲーム中のレベルやアセットなどをタスクとして扱い毎フレーム実施する
    // ロジック処理と描画処理をメインループで処理する
    // タスクマネージャにタスクを積む作業は各レベル上で実施する
    std::shared_ptr<TaskManager> taskManager = std::make_shared<TaskManager>();

    // levelChanger : レベルチェンジャー
    // ゲームで扱う各画面をレベルと呼び、各レベルの切り替えを処理する
    // レベルには起動時画面(BootLevel)、タイトル画面(TitleLevel)、
    // ゲーム画面(BattleLevel)、リザルト画面(ResultLevel)の4種類が存在する
    std::shared_ptr<LevelChanger> levelChanger = std::make_shared<LevelChanger>(taskManager);

    // fadeManager : フェードマネージャー
    // レベル切り替え時の暗転処理(フェードインとフェードアウト)を実施する
    std::shared_ptr<FadeManager> fadeManager = std::make_shared<FadeManager>();

    bool isSuccess = taskManager->AddTask(levelChanger);
    if (!isSuccess)
    {
        return;
    }

    // Fadeクラス(Facade)を利用して各レベルからフェードインとフェードアウトを開始する
    // 暗転するための毎フレームの更新処理はタスクマネージャ上で実施する
    fadeManager->Initialize();
    Fade::Initialize(fadeManager);

    isSuccess = taskManager->AddTask(fadeManager);
    if (!isSuccess)
    {
        taskManager->ReleaseTask(TaskId::LevelChanger);
        return;
    }

    RenderManager::CreateRenderLayerList();

    // メインループ処理
    bool isGameFinished{ false };
    while (true)
    {

        if (!isGameFinished)
        {
            // System::Update()ではウィンドウの閉じるボタン押下など
            // アプリケーション終了トリガーが発生した場合または、
            // 回復不能な内部エラーが発生した場合に false を返します。
            if (!System::Update())
            {
                isGameFinished = true;
                levelChanger->SetFinishRequest();
            }
            else
            {
                // System::Update()が true を返し続ける間、
                // つまり正常稼働中はゲーム内の操作でゲーム終了したかどうかを確認する
                if (levelChanger->IsReleaseCompleted())
                {
                    break;
                }
            }
        }
        else
        {
            // System::Update()で false を返された場合、
            // ゲームは稼働中のため終了処理が完了するまで待機する
            if (levelChanger->IsReleaseCompleted())
            {
                break;
            }
        }

        const double deltaTime = Scene::DeltaTime();

        // ゲームで使用するクラスのロジック処理実施
        taskManager->Update(deltaTime);
        // ゲームで使用するアセットの描画処理実施
        taskManager->Render();
    }

    // 後片付け
    taskManager->ReleaseTask(TaskId::FadeManager);
    taskManager->ReleaseTask(TaskId::LevelChanger);
}

//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要な場合があります。
//
