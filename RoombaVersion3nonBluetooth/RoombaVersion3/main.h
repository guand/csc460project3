/*
 * main.h
 *
 * Created: 4/4/2015 4:36:58 PM
 *  Author: 
 */ 

#ifndef MAIN_H_
#define MAIN_H_

#include "shared/rtos_n/os.h"
#include "shared/rtos_n/timer.h"
#include "shared/radio/radio.h"
#include "shared/roomba/roomba.h"
#include "shared/roomba/roomba_sci.h"
#include "shared/roomba/roomba_led_sci.h"
#include "shared/roomba/roomba_bump_detection.h"
#include "shared/uart/uart.h"
#include "shared/ir/ir.h"
#include "shared/game.h"
#include "shared/profiler.h"
#include "shared/trace_uart/trace_uart.h"
#include "shared/trace/trace.h"
#include "shared/music_stream.h"
#include "shared/roomba/roomba_music.h"

#define LED_PORT      PORTB
#define LED_DDR       DDRB
#define LED_RX_PIN    PB4 // digital pin 10
#define LED_TX_PIN    PB5 // digital pin 11
#define LED_IR_PIN    PB6 // digital pin 12

#define LED_RX_STATE_INDICATOR() LED_PORT ^= (1 << LED_RX_PIN)
#define LED_TX_STATE_INDICATOR() LED_PORT ^= (1 << LED_TX_PIN)
#define IR_STATE_INDICATOR() LED_PORT ^= (1 << LED_IR_PIN)

// TODO: Change this to a proper pin number
#define TEAM_DDR            DDRC
#define TEAM_PORT           PORTC
#define TEAM_HUMAN_PIN      PC5 // digital pin 32
#define TEAM_ZOMBIE_PIN     PC4 // digital pin 33
#define EFFECT_INDICATOR	PC3 // digital pin 34

#define EFFECT_INDICATOR_ON()	TEAM_PORT |= (1 << EFFECT_INDICATOR)
#define EFFECT_INDICATOR_OFF()	TEAM_PORT &= ~(1 << EFFECT_INDICATOR)

#define TEAM_HUMAN_SHIELD_ON()	EFFECT_INDICATOR_ON()
#define TEAM_HUMAN_SHIELD_OFF() EFFECT_INDICATOR_OFF()

#define TEAM_HUMAN_PIN_ON()		TEAM_PORT |= (1 << TEAM_HUMAN_PIN)
#define TEAM_HUMAN_PIN_OFF()	TEAM_PORT &= ~(1 << TEAM_HUMAN_PIN)

#define TEAM_ZOMBIE_STUN_ON()	EFFECT_INDICATOR_ON()
#define TEAM_ZOMBIE_STUN_OFF()	EFFECT_INDICATOR_OFF()

#define TEAM_ZOMBIE_PIN_ON()	TEAM_PORT |= (1 << TEAM_ZOMBIE_PIN)
#define TEAM_ZOMBIE_PIN_OFF()	TEAM_PORT &= ~(1 << TEAM_ZOMBIE_PIN)
 

typedef struct roomba_state {
	uint8_t player_id; // 0x4A, 0x4B, 0x4C, 0x4D
	uint8_t team;   // ZOMBIE = 0, HUMAN = 1
	uint8_t state; // NORMAL/STUNNED or SHIELDED/SHIELDLESS
	uint8_t last_ir_code; // 0 if no code
	uint8_t old_team;
	uint8_t old_state;
	int16_t old_vx;
	int16_t old_vy;
	int16_t old_button;
	int16_t vx;
	int16_t vy;
	uint8_t button;
} ROOMBA_STATE;

#endif /* MAIN_H_ */