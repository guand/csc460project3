/*
 * roomba_bump_detection.cpp
 *
 * Created: 4/5/2015 11:34:12 PM
 *  Author: d
 */ 
#include "roomba_bump_detection.h"

uint8_t get_roomba_bump_sensors() {
	roomba_sensor_data_t sensor_packet;
	Roomba_UpdateSensorPacket(EXTERNAL, &sensor_packet);
	return sensor_packet.bumps_wheeldrops;
}

uint8_t sensor_detect() {
	uint8_t roomba_bump_sensors = get_roomba_bump_sensors();
	if (roomba_bump_sensors && BUMP_LEFT) {
		return 1;
	} else if (roomba_bump_sensors && BUMP_RIGHT) {
		return 2;
	}
	return 0;
}