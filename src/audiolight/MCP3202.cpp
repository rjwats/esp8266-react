#include <audiolight/MCP3202.h>

void MCP3202::begin() {
  pinMode(MCP3203_SELECT_PIN, OUTPUT);
	SPI.begin();
  deselect();
}

void MCP3202::select() {
	SPI.beginTransaction(_settings);
	digitalWrite(MCP3203_SELECT_PIN, LOW);
}

void MCP3202::deselect() {
  SPI.endTransaction();
	digitalWrite(MCP3203_SELECT_PIN, HIGH);
}

uint16_t MCP3202::sample(uint8_t channelSelect){
  uint8_t bh, bl;

	select();
  SPI.transfer(1);
	bh = SPI.transfer(channelSelect);
	bl = SPI.transfer(0);
	deselect();

  // mask off four most significant bits from bh
  // when forming our 12-bit value
	return ((uint16_t) bh & 0x0f) << 8 | bl;
}