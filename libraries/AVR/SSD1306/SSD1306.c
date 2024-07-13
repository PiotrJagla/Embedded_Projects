#include <SSD1306.h>
#include <SPI.h>

void SSD1306_send_command(uint8_t data) {
  PORTB &= ~(1<<DC);
  PORTB &= ~(1<<CS);
  SPI_MasterTransmit(data);
  PORTB |= (1<<CS);
}

void SSD1306_init_display() {
  PORTB |= (1<<RST);
  _delay_ms(1);
  PORTB &= ~(1<<RST);
  _delay_ms(10);
  PORTB |= (1<<RST);

  SSD1360_send_command(0xDB);
  SSD1360_send_command(0x40);
  SSD1360_send_command(SSD1306_DISPLAYOFF);
  SSD1360_send_command(SSD1306_SETDISPLAYALLON);
  SSD1360_send_command(SSD1306_DISPLAYON);
}
