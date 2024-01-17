#include <stdio.h>
#include <raylib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdint.h>
#include <raymath.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>


#define TEXT_SIZE 100

struct rect {
  int x;
  int y;
  int width;
  int height;
};


int main() {

  int fileDescriptor;
  char text[TEXT_SIZE];
  struct termios options;

  fileDescriptor = open("/dev/ttyACM0", O_RDWR | O_NDELAY | O_NOCTTY);
  if(fileDescriptor < 0) {
    perror("Error opening serial port");
    return -1;
  }

  strcpy(text, "Hello\n\r");
  int len = strlen(text);
  len = write(fileDescriptor, text, len);
  printf("Wrote data: %d\n", len);
  
    


  options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
  options.c_iflag = IGNPAR;
  options.c_oflag = 0;
  options.c_lflag = 0;

  tcflush(fileDescriptor, TCIFLUSH);
  tcsetattr(fileDescriptor, TCSANOW, &options);
  


  //INIT RAYLIB
  InitWindow(800,600, "basic window");
  Camera camera = {0};
  camera.position = (Vector3){0.0f,10.0f, 10.0f};
  camera.target = (Vector3){0.0f,0.0f, 0.0f};
  camera.up = (Vector3){0.0f,1.0f, 0.0f};
  camera.fovy = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  Model model = LoadModelFromMesh(GenMeshCube(1.0f,3.0f,1.0f));

  float pitch = 0.0f;
  float roll = 0.0f;
  float yaw = 0.0f;

  int16_t gyro_x;
  int16_t gyro_y;
  int16_t gyro_z;

  int16_t acc_x;
  int16_t acc_y;
  int16_t acc_z;



  while(!WindowShouldClose()) {
    strcpy(text, ".");
    int len = strlen(text);
    len = write(fileDescriptor, text, len);

    usleep(20 * 1000);

    memset(text, 0, TEXT_SIZE);
    read(fileDescriptor, text, TEXT_SIZE);
    
    printf("received %s\n", text);
    const char delimiter[] = "|";
    char* token = strtok(text, delimiter);

    while(token != (void*)0) {
      printf("Token: %s\n", token); 

      //Process token
      int startNumberIndex = 3;
      char strInt[strlen(token) - startNumberIndex];
      for(int i = startNumberIndex; token[i] != '\0' ; ++i) {
        strInt[i-startNumberIndex] = token[i];
      }
      struct rect r = {5,5,5,5};

      int measurement = atoi(strInt);

      printf("int value: %d\n", measurement); 

      if(measurement != 0) {
        if(token[0] == 'g' && token[1] == 'x') {
          gyro_x = measurement;
        } else if(token[0] == 'g' && token[1] == 'y') {
          gyro_y = measurement;
        } else if(token[0] == 'g' && token[1] == 'z') {
          gyro_z = measurement;
        } else if(token[0] == 'a' && token[1] == 'x') {
          acc_x = measurement;
        } else if(token[0] == 'a' && token[1] == 'y') {
          acc_y = measurement;
        } else if(token[0] == 'a' && token[1] == 'z') {
          acc_z = measurement;
        }
      }
      token = strtok((void*)0, delimiter);
    }
    printf("___________\n");
    



    pitch = 180.0f * atan((-acc_x)/sqrt(acc_y*acc_y + acc_z*acc_z))/M_PI;
    roll = 180.0f * atan(acc_y/sqrt(acc_x*acc_x + acc_z*acc_z))/M_PI;
    

    model.transform = MatrixRotateXYZ((Vector3){DEG2RAD*pitch, 0.0f, DEG2RAD*roll});

    //RAYLIB DRAWING
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode3D(camera);

    DrawModel(model, (Vector3){0.0f, 1.0f, 0.0f}, 1.0f, WHITE);
    DrawModelWires(model, (Vector3){0.0f, 1.0f, 0.0f}, 1.0f, BLACK);

    EndMode3D();


    DrawFPS(10,1.0f);

    EndDrawing();

  }

  CloseWindow();

  close(fileDescriptor);

  return 0;
}
