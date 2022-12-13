// It is owned by the company Dverg Verksted.


#include "Game/SaveLoad/RSSaveGame.h"


FPlayerSaveData* URSSaveGame::GetPlayerData()
{
    if (SavedPlayer.PlayerID != "None")
        return &SavedPlayer;

    return nullptr;
}

void URSSaveGame::ClearData()
{
    SavedPlayer = {};
    SavedActors.Empty();
}
