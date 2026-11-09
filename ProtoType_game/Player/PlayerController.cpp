#include "PlayerController.h"


PlayerController::PlayerController(Player& player) : player(player)
{
}

void PlayerController::handleAction(PlayerAction action)
{
    switch (action)
    {
    case PlayerAction::MoveLeft:
        player.moveLeft();
        break;

    case PlayerAction::MoveRight:
        player.moveRight();
        break;

    case PlayerAction::Jump:
        player.jump();
        break;

    case PlayerAction::CrouchStart:
        player.startCrouch();
        break;

    case PlayerAction::CrouchEnd:
        player.stopCrouch();
        break;
    }
}