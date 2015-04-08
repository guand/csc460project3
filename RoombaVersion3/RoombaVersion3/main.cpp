// #include "sonar/sonar.h"
#include "main.h"

SERVICE* radio_receive_service;
SERVICE* ir_receive_service;

// bluetooth driver
Bluetooth *blue = new Bluetooth("RoombaBluetooth");

int16_t velocity_x = 0;
int16_t velocity_y = 0;

// everyone hard-codes this number when they begin the game
uint8_t roomba_num = 1;
uint8_t ir_count = 0;
radiopacket_t tx_packet;

ROOMBA_STATE roomba;
uint8_t autonomy_counter;
uint8_t zombie_song = 0;
uint8_t human_song = 0;

typedef enum autonomy {
	USER,
	TURN_LEFT,
	TURN_RIGHT
} AUTONOMY;

AUTONOMY autonomy_state = USER;

// Just wake up the task that is waiting for a radio packet
void radio_rxhandler(uint8_t pipenumber) {
	LED_RX_STATE_INDICATOR();
	Service_Publish(radio_receive_service,0);
}

void ir_rxhandler() {
	int16_t value = IR_getLast();
	for(int i = 0; i < 4; ++i) {
		if(value == PLAYER_IDS[i]) {
			if(value != PLAYER_IDS[roomba_num]) {
				IR_STATE_INDICATOR();
				Service_Publish(ir_receive_service, value);
			}
		}
	}
}

void handleRoombaInput()
{
// 	int16_t x_value = (((game->velocity_x*5)/256) - 2)*40;
// 	int16_t y_value = (((game->velocity_y*5)/256) - 2)*250;
	int16_t x_value = velocity_x;
	int16_t y_value = velocity_y;
	// reset into safe mode.
	Roomba_Send_Byte(SAFE);
	Roomba_Drive(y_value, x_value);
// 	if(game->game_team == ZOMBIE && game->game_state == (uint8_t)STUNNED) {
// 		Roomba_Drive(15,0x8000);
// 		
// 	} else if(autonomy_state == USER) {
// 		autonomy_counter = 0;
// 		if( x_value == 0 && y_value == 0) {
// 			Roomba_Drive(0,0x8000);
// 		} else if( x_value == 0) {
// 			y_value = -y_value;
// 			Roomba_Drive(y_value,0x8000);
// 		} else if( y_value == 0) {
// 			// turn in place
// 			uint16_t deg = 1;
// 			if( x_value < 0 ){
// 				deg = -1;
// 			}
// 			Roomba_Drive(250,deg);
// 		} else {
// 			x_value = x_value;
// 			y_value = -y_value;
// 			Roomba_Drive(y_value,x_value);
// 		}
// 	} else if(autonomy_state = TURN_LEFT) {
// 		if(autonomy_counter > 3) {
// 			autonomy_state = USER;
// 		}
// 		autonomy_counter++;
// 		Roomba_Drive(250,1);
// 	} else if(autonomy_state = TURN_RIGHT) {
// 		if(autonomy_counter > 3) {
// 			autonomy_state = USER;
// 		}
// 		autonomy_counter++;
// 		Roomba_Drive(250,-1);
// 		
// 	}

	// fire every 5th packet
	ir_count+= 1;
	if(ir_count == 5){
		IR_transmit(roomba.player_id);
		ir_count = 0;
	}
}

void load_death_music()
{
	roomba_music_song_t death_song;
	death_song.len = 0;
	death_song.song_num = 1;
	if(roomba.team == ZOMBIE){
		Roomba_Music_add_note(&death_song, 83, 9);
		Roomba_Music_add_note(&death_song, 90, 18);
		Roomba_Music_add_note(&death_song, 89, 9);
		Roomba_Music_add_note(&death_song, 89, 9);
		Roomba_Music_add_note(&death_song, 88, 9);
		Roomba_Music_add_note(&death_song, 86, 9);
		Roomba_Music_add_note(&death_song, 84, 9);
		Roomba_Music_add_note(&death_song, 88, 9);
		Roomba_Music_add_note(&death_song, 88, 9);
		Roomba_Music_add_note(&death_song, 84, 9);
	} else {
		Roomba_Music_add_note(&death_song, 89, 9);
		Roomba_Music_add_note(&death_song, 81, 9);
		Roomba_Music_add_note(&death_song, 83, 9);
		Roomba_Music_add_note(&death_song, 89, 9);
		Roomba_Music_add_note(&death_song, 81, 9);
		Roomba_Music_add_note(&death_song, 83, 9);
		Roomba_Music_add_note(&death_song, 89, 9);
		Roomba_Music_add_note(&death_song, 81, 9);
		Roomba_Music_add_note(&death_song, 83, 9);
		Roomba_Music_add_note(&death_song, 84, 9);
		Roomba_Music_add_note(&death_song, 88, 9);
		Roomba_Music_add_note(&death_song, 83, 9);
		Roomba_Music_add_note(&death_song, 84, 9);
		Roomba_Music_add_note(&death_song, 83, 9);
		Roomba_Music_add_note(&death_song, 91, 9);
		Roomba_Music_add_note(&death_song, 88, 9);
	}
	Roomba_Music_load_song(&death_song);
}

void handleStateInput(pf_game_t* game){
	roomba.old_team = roomba.team;
	roomba.old_state = roomba.state;
	roomba.team = game->game_team;
	roomba.state = game->game_state;
	
	if(roomba.team != roomba.old_team) {
		TEAM_HUMAN_PIN_OFF();
		TEAM_ZOMBIE_PIN_ON();
		zombie_song = 0;
	} else {
		switch(roomba.team) {
			case ZOMBIE:
			{

				if(zombie_song == 0) {
					load_death_music();
					Roomba_Music_play_song(1);
					zombie_song = 1;
				}
				TEAM_HUMAN_PIN_OFF();
				TEAM_ZOMBIE_PIN_ON();
				if(roomba.state == NORMAL) {
					TEAM_ZOMBIE_STUN_ON();
				} else if(roomba.state == STUNNED) {
					TEAM_ZOMBIE_STUN_OFF();
				}
				break;
			}
			case HUMAN:
			{
				if(human_song == 0) {
					load_death_music();
					Roomba_Music_play_song(1);
					human_song = 1;
				}
				zombie_song = 0;
				TEAM_ZOMBIE_PIN_OFF();
				TEAM_HUMAN_PIN_ON();
				if(roomba.state == SHIELDED) {
					TEAM_HUMAN_SHIELD_ON();
				} else if(roomba.state == NORMAL) {
					TEAM_HUMAN_SHIELD_OFF();
				}
				break;
			}
			default:
				break;
		}
 	}
}


// Return a packet back to the base station reporting
// if we have been hit, and by whom
void send_back_packet()
{
	LED_TX_STATE_INDICATOR();

	tx_packet.type = GAME;
	for(int i = 0;i < 5; ++i){
		tx_packet.payload.game.sender_address[i] = ROOMBA_ADDRESSES[roomba_num][i];
	}

	tx_packet.payload.game.game_player_id = roomba.player_id;
	tx_packet.payload.game.game_team = roomba.team;
	tx_packet.payload.game.game_state = roomba.state;
	tx_packet.payload.game.game_hit_flag = (roomba.last_ir_code != 0) ? 1: 0;
	tx_packet.payload.game.game_enemy_id = roomba.last_ir_code;

	// reset the stuff.
	roomba.last_ir_code = 0;

	Radio_Transmit(&tx_packet, RADIO_RETURN_ON_TX);
}

void current_radio_state() {
	//Start the Roomba for the first time.
	int16_t value;
	RADIO_RX_STATUS result;
	radiopacket_t packet;

	for(;;) {
		Service_Subscribe(radio_receive_service,&value);
		
		do {
			result = Radio_Receive(&packet);

			if(result == RADIO_RX_SUCCESS || result == RADIO_RX_MORE_PACKETS) {
				if( packet.type == GAME)
				{
					handleStateInput(&packet.payload.game);
				}
			}

		} while (result == RADIO_RX_MORE_PACKETS);

		send_back_packet();
	}
}

void current_ir_state()
{
	int16_t value;
	for(;;){
		Service_Subscribe(ir_receive_service,&value);
		roomba.last_ir_code = value;
	}
}

void power_cycle_radio()
{
	//Turn off radio power.
	DDRL |= (1 << PL2);
	PORTL &= ~(1<< PL2);
	_delay_ms(500);
	PORTL |= (1<<PL2);
	_delay_ms(500);
}




void check_bump_sensor() {
	for (;;)
	{
		if(sensor_detect() == 1) {
			autonomy_state = TURN_LEFT;
		} else if(sensor_detect() == 2) {
			autonomy_state = TURN_RIGHT;
		}
		Task_Next();
	}
}




void setup_roomba()
{
	Roomba_Init();
	Radio_Set_Tx_Addr(base_station_address);
	zombie_song = 0;
	human_song = 0;
 	LED_DDR |= (1<<LED_RX_PIN);
 	LED_DDR |= (1<<LED_TX_PIN);
	LED_DDR |= (1<<LED_IR_PIN);
 	LED_PORT &= ~(1<<LED_RX_PIN);
 	LED_PORT &= ~(1<<LED_TX_PIN);
	LED_PORT &= ~(1<<LED_IR_PIN);

	TEAM_DDR |= (1<<TEAM_HUMAN_PIN);
	TEAM_DDR |= (1<<TEAM_ZOMBIE_PIN);
	TEAM_DDR |= (1<<EFFECT_INDICATOR);
	TEAM_PORT &= ~(1<<TEAM_HUMAN_PIN);
	TEAM_PORT &= ~(1<<TEAM_ZOMBIE_PIN);
	TEAM_PORT &= ~(1<<EFFECT_INDICATOR);
}

void init_roomba_state(ROOMBA_STATE* roomba)
{
	roomba->player_id = PLAYER_IDS[roomba_num];
	roomba->team = ZOMBIE;
	roomba->state = NORMAL;
	roomba->last_ir_code= 0;
	roomba->old_team = 0;
	roomba->old_state = 0;
	roomba->old_vx = 0;
	roomba->old_vy = 0;
	roomba->old_button = 0;
	roomba->vx = 0;
	roomba->vy = 0;
	roomba->button = 0;
}

// void rr_task_hello_world(void) {
// 	while (1) {
// 		_delay_ms(5000);
// 		blue->Send("Hello World!");
// 	}
// }

bool alternating_velocity = true;
void current_bluetooth_state() {
	for(;;) {
		while (blue->btSerial->available()) {
			char received_value = blue->btSerial->read();
			if (alternating_velocity) {
				alternating_velocity = !alternating_velocity;
				velocity_y = (((int)received_value)-75)*50;
				digitalWrite(13, HIGH);
				_delay_ms(100);
			} else {
				alternating_velocity = !alternating_velocity;
				velocity_x = (((int)received_value)-75)*200;
			}
			digitalWrite(13, LOW);
			_delay_ms(100);
			
			handleRoombaInput();
		}
	}
}

int r_main(void)
{
	//Initialize arduino
	init();
	
		pinMode(13, OUTPUT);
	
  	//Setup bluetooth
  	blue->setupBluetooth();
	
	power_cycle_radio();
	init_roomba_state(&roomba);
	
	//Initialize radio
	Radio_Init();
	IR_init();
	Radio_Configure_Rx(RADIO_PIPE_0, ROOMBA_ADDRESSES[roomba_num], ENABLE);
	Radio_Configure(RADIO_1MBPS, RADIO_HIGHEST_POWER);
 
	//Create the services
	radio_receive_service = Service_Init();
	ir_receive_service = Service_Init();

	//Create the tasks
	Task_Create_System(setup_roomba, 0);
	Task_Create_RR(current_bluetooth_state,0);
 	Task_Create_RR(current_radio_state,0);
 	Task_Create_RR(current_ir_state,0);
	//Task_Create_RR(rr_task_hello_world,0);
	//Task_Create_Periodic(check_bump_sensor, 0, 80, 60, 1000);
 
	Task_Terminate();
	return 0 ;
}