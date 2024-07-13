#include <SPI.h>

void SPI_MasterInit(void) {
  DDR_SPI = (1<<MOSI) | (1<<SCK) | (1<<DC) | (1<<RST) | (1<<CS);
  SPCR = (1<<SPE) | (1<<MSTR) | (1<<DORD);
}

void SPI_MasterTransmit(uint8_t data) {
  SPDR = cData;

  while(!(SPSR & (1<<SPIF)));
}
