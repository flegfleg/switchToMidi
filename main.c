#include "pressanykey/pressanykey.h"
#include "midi.h"


#define LED_PORT 0
#define LED_PIN 7
#define KEY_PORT 2
#define KEY_PIN 2

bool oldSensor = false;

void main(void) {
	MIDI_Init(NULL);
	GPIO_SetDir(LED_PORT, LED_PIN, GPIO_Output);
	GPIO_SetDir(KEY_PORT, KEY_PIN, GPIO_Input);
	//GPIO_SETPULL(KEY_PORT, KEY_PIN, IOCON_IO_PULL_UP);
	GPIO_SETHYSTERESIS (KEY_PORT, KEY_PIN, IOCON_IO_HYSTERESIS_ON);
	SYSCON_StartSystick(71999);
}

int fingerDown = 0;

void systick(void) {
	bool sensor = GPIO_ReadInput(KEY_PORT, KEY_PIN);
	GPIO_WriteOutput(LED_PORT, LED_PIN, !sensor);
	if (!sensor && oldSensor) {
		uint8_t command [] = {0b10110001,50,127};
		MIDI_WriteCommand (command,3);
		fingerDown = 50;
	}
	oldSensor = sensor;
	if (fingerDown > 0) {
		fingerDown--;
		if (fingerDown < 1) {
			uint8_t command [] = {0b10110001,50,0};
			MIDI_WriteCommand (command,3);
		}
	}
}
