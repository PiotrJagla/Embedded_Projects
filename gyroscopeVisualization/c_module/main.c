#include <stdio.h>
#include <raylib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdint.h>


int main() {
  int fileDescriptor, len;
  int8_t text[255];
  struct termios options;

  fileDescriptor = open("/dev/ttyACM0", O_RDWR | O_NDELAY | O_NOCTTY);
  if(fileDescriptor < 0) {
    perror("Error opening serial port");
    return -1;
  }

  //tcgetattr(fileDescriptor, &options);

  options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
  options.c_iflag = IGNPAR;
  options.c_oflag = 0;
  options.c_lflag = 0;

  tcflush(fileDescriptor, TCIFLUSH);
  tcsetattr(fileDescriptor, TCSANOW, &options);

  while(true) {
    memset(text, 0, 255);
    len = read(fileDescriptor, text, 6);
    printf("1: %d, 2: %d, 3: %d\n", text[1] << 8 | text[0],  text[3] << 8 | text[2],  text[5] << 8 | text[4]);
    usleep(100 * 1000);

  }

  

  close(fileDescriptor);

  return 0;
}
