#ifndef MCP3202_H
#define MCP3202_H

#include <Arduino.h>
#include <SPI.h>

#define MCP3202_CONFIG_START 1
#define MCP3202_CONFIG_MSBF 1

#define MCP3202_FREQUENCY 1200000
#define MCP3202_BIT_ORDER MSBFIRST
#define MCP3202_SPI_MODE SPI_MODE0
#define MCP3203_SELECT_PIN 15

class MCP3202 {
  public:
    const static uint8_t CH0 = 0b10100000;
    const static uint8_t CH1 = 0b11100000;

    MCP3202() {}
    void begin();
    uint16_t sample(uint8_t channel);
    
  private:
    SPISettings _settings = SPISettings(MCP3202_FREQUENCY, MCP3202_BIT_ORDER, MCP3202_SPI_MODE);

  	void select();
	  void deselect();
};

#endif // End MCP3202_H