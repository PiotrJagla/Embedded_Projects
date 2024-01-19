#include <stdio.h>
#include <pthread.h>
#include <raylib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdint.h>
#include <raymath.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "my_strings.h"


#define SEGMENTS_NUMBER 6
#define TEXT_SIZE 100

int16_t gyro_x;
int16_t gyro_y;
int16_t gyro_z;

int16_t acc_x;
int16_t acc_y;
int16_t acc_z;

void* dataCollectionRoutine(void* args) {
  int fileDescriptor = (int)args;
  char text[TEXT_SIZE];
  while(true){
    strcpy(text, ".");
    int len = strlen(text);
    len = write(fileDescriptor, text, len);

    usleep(20 * 1000);

    memset(text, 0, TEXT_SIZE);
    len = read(fileDescriptor, text, TEXT_SIZE);

    printf("received %s\n", text);
    char** segments = split(text, strlen(text), '|');
    int numberStartIndex = 3;
    for(int i = 0 ; i < SEGMENTS_NUMBER; ++i) {
      if(segments[i] == NULL) {
        break;
      }
      char* strInt = substring(segments[i], numberStartIndex, strlen(segments[i]));
      int measurement = atoi(strInt);
      printf("segment %d: %s, value: %d\n", i+1, segments[i], measurement);
      if(measurement != 0) {
        if(segments[i][0] == 'g' && segments[i][1] == 'x') {
          gyro_x = measurement;
        } else if(segments[i][0] == 'g' && segments[i][1] == 'y') {
          gyro_y = measurement;
        } else if(segments[i][0] == 'g' && segments[i][1] == 'z') {
          gyro_z = measurement;
        } else if(segments[i][0] == 'a' && segments[i][1] == 'x') {
          acc_x = measurement;
        } else if(segments[i][0] == 'a' && segments[i][1] == 'y') {
          acc_y = measurement;
        } else if(segments[i][0] == 'a' && segments[i][1] == 'z') {
          acc_z = measurement;
        }
      }
    }
    for(int i = 0 ; segments[i] != NULL; ++i) {
      free(segments[i]);
    }
    free(segments);
  }
  return NULL;
}

int main() {

  int fileDescriptor;
  struct termios options;

  fileDescriptor = open("/dev/ttyACM0", O_RDWR | O_NDELAY | O_NOCTTY);
  if(fileDescriptor < 0) {
    perror("Error opening serial port");
    return -1;
  }

  options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
  options.c_iflag = IGNPAR;
  options.c_oflag = 0;
  options.c_lflag = 0;

  tcflush(fileDescriptor, TCIFLUSH);
  tcsetattr(fileDescriptor, TCSANOW, &options);
  

  //pthread init
  pthread_t tid;
  int ret = pthread_create(&tid, NULL, dataCollectionRoutine, fileDescriptor);


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

  while(!WindowShouldClose()) {
    //printf("X: %d, y:%d, z:%d\n", acc_x, acc_y, acc_z);
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
