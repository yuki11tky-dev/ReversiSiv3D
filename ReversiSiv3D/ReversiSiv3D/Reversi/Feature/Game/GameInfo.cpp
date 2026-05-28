#include "Feature/Game/GameInfo.h"

namespace ReversiSiv3D {

    GameConfig GameInfo::gameConfig_{};

    GameInfo::GameInfo()
    {

    }

    GameInfo::~GameInfo()
    {

    }

    void GameInfo::SetGameConfig(const GameConfig& config)
    {
        gameConfig_ = config;
    }

    GameConfig& GameInfo::GetGameConfig()
    {
        return gameConfig_;
    }
}
