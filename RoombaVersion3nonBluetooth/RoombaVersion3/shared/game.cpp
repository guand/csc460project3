/*
 * cops_and_robbers.h
 *
 * Created: 2015-01-26 17:09:37
 *  Author: Daniel
 */

#include "game.h"
#include "uart/uart.h"
#include "roomba/roomba_sci.h"
#include "avr/io.h"

uint8_t ROOMBA_ADDRESSES[4][5] = {
	{0x4A,0x4A,0x4A,0x4A,0x4A},
	{0x4B,0x4B,0x4B,0x4B,0x4B},
	{0x4C,0x4C,0x4C,0x4C,0x4C},
	{0x4D,0x4D,0x4D,0x4D,0x4D}
};
uint8_t ROOMBA_FREQUENCIES [4] = {104, 106, 108, 110};
uint8_t PLAYER_IDS[4] = {0x4A,0x4B,0x4C,0x4D};

uint8_t base_station_address[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

// each roomba on a differnent freq
// base station

void Game_send_command_to_roomba(roomba_command_t* cmd){
    if (cmd->opcode == START ||
        cmd->opcode == BAUD ||
        cmd->opcode == SAFE ||
        cmd->opcode == FULL ||
        cmd->opcode == SENSORS)
    {
        return;
    }

    //Pass the command to the Roomba.
    Roomba_Send_Byte(cmd->opcode);
    int i;
    for (i = 0; i < cmd->num_args; i++){
        Roomba_Send_Byte(cmd->args[i]);
    }
}


