#define DEBUG_MODULE "LEDMatrixDeck"
#include "debug.h"
#include "deck.h"
#include "i2cdev.h"

static bool send(uint8_t address, uint8_t data) {
	return i2cdevWriteByte(I2C1_DEV, 0x75, address, data);
}

static void LEDMatrixInit() {
	DEBUG_PRINT("Beginning LED matrix initialization\n");
	i2cdevInit(I2C1_DEV);

	send(0xfd, 0xb);  // Select function register
	send(0x0a, 0);    // Shutdown mode
	send(0xfd, 0xb);
	send(0x0a, 1);    // Normal mode

	send(0xfd, 0x0);  // Select page one

	// Set brightness
	for (uint8_t i = 0x24; i < 0xb3; i++) {
		send(i, 0x80);
	}

	uint8_t pattern[7][11] = { { 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0 },
			                   { 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0 },
							   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
							   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
							   { 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0 },
			                   { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 },
							   { 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0 }, };
	for (int x = 0; x < 11; x++) {
		int page;
		if (x <= 4) {
			page = 9 - (x * 2);
		} else {
			page = 10 - ((x - 5) * 2);
		}

		uint8_t col = 0;
		for (int y = 7; y >= 0; y--) {
			col <<= 1;
			col |= pattern[y][x];
		}

		send(page, col);
	}

	DEBUG_PRINT("LED matrix initialization complete!\n");
}

static const DeckDriver LEDMatrixDriver = { .name = "LEDMatrix",
		                                    .init = LEDMatrixInit };

DECK_DRIVER(LEDMatrixDriver);
