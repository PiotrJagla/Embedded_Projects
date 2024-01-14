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

//char* substring(char* text, int startI, int endI) {
//  char* buf = (char*)malloc((endI - startI) * sizeof(char));
//
//  for(int i = startI ; i <= endI ; ++i) {
//    buf[i = startI] = text[startI];
//  }
//  return buf;
//}
//
//char** split(char* text, char splitter, int len) {
//  char** splittedSegments = (char**)malloc(5*sizeof(char*));
//  int segmentsCount = 0;
//  int previosSplitterIndex = 0;
//
//  for(int i = 0 ; i < len ; ++i){
//    if(text[i] == splitter) {
//      splittedSegments[segmentsCount++] = substring(text, previosSplitterIndex, i - 1);
//      previosSplitterIndex = i;
//    }
//  }
//  return splittedSegments;
//}

#define TEXT_SIZE 100


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
    gyro_x = text[1] << 8 | text[0];
    gyro_y = text[3] << 8 | text[2];
    gyro_z = text[5] << 8 | text[4];
    //printf("x: %d, y: %d, z: %d\n",gyro_x, gyro_y , gyro_z );
    //char** gyroData = split(text, '|', 29);
    //for(int i = 0 ; i < 5 ; ++i) {
    //  printf("%d segment: %s\n", i, gyroData[i]);
    //}
    
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

      int measurement = atoi(strInt);

      //printf("Str in int: %s\n", strInt); 
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
    


    acc_x = (int)(((int)acc_x)* 3.9);
    acc_y = (int)(((int)acc_y)* 3.9);
    acc_z = (int)(((int)acc_z)* 3.9);

    pitch = 180.0f * atan((-acc_x)/sqrt(acc_y*acc_y + acc_z*acc_z))/M_PI;
    roll = 180.0f * atan(acc_y/sqrt(acc_x*acc_x + acc_z*acc_z))/M_PI;
    

    //model.transform = MatrixRotateXYZ((Vector3){DEG2RAD*pitch, 0.0f, DEG2RAD*roll});
    model.transform = MatrixRotateXYZ((Vector3){0.0f ,DEG2RAD*pitch, DEG2RAD*roll});
    //model.transform = MatrixRotateXYZ((Vector3){DEG2RAD*gyro_x, DEG2RAD*gyro_y, DEG2RAD*gyro_z});

    //RAYLIB DRAWING
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode3D(camera);

    DrawModel(model, (Vector3){0.0f, -1.0f, 0.0f}, 1.0f, WHITE);
    DrawModelWires(model, (Vector3){0.0f, -1.0f, 0.0f}, 1.0f, BLACK);
    //DrawGrid(10, 1.0f);

    EndMode3D();


    DrawFPS(10,1.0f);

    EndDrawing();

  }

  CloseWindow();

  close(fileDescriptor);

  return 0;
}
