#include "Feature/Game/ResourceProvider.h"

namespace ReversiSiv3D {

    String ResourceProvider::GetStoneIconFilePath(StoneType stoneType)
    {
        String filePath{};
        switch (stoneType)
        {
        case StoneType::Black:
        {
            filePath = U"Image/stone_black.png";
            break;
        }
        case StoneType::White:
        {
            filePath = U"Image/stone_white.png";
            break;
        }
        default:
        {
            break;
        }
        }

        return filePath;
    }

    String ResourceProvider::GetPlayerNameText(PlayerType playerType)
    {
        String playerName{};
        switch (playerType)
        {
        case PlayerType::Player1:
        {
            playerName = U"YOU";
            break;
        }
        case PlayerType::Player2:
        {
            playerName = U"OPNT";
            break;
        }
        case PlayerType::Cpu:
        {
            playerName = U"CPU";
            break;
        }
        default:
        {
            break;
        }
        }

        return playerName;
    }
}
