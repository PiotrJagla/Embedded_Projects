#include <stdio.h>
#include <raylib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdint.h>
#include <raymath.h>


int main() {
  int fileDescriptor;
  char text[255];
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


  //INIT RAYLIB
  InitWindow(800,600, "basic window");
  Camera camera = {0};
  camera.position = (Vector3){0.0f,10.0f, 10.0f};
  camera.target = (Vector3){0.0f,0.0f, 0.0f};
  camera.up = (Vector3){0.0f,1.0f, 0.0f};
  camera.fovy = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  Model model = LoadModelFromMesh(GenMeshCube(2.0f,2.0f,2.0f));

  float pitch = 0.0f;
  float roll = 0.0f;
  float yaw = 0.0f;


  while(!WindowShouldClose()) {
    memset(text, 0, 255);
    read(fileDescriptor, text, 30);
    //int16_t gyro_x = text[1] << 8 | text[0];
    //int16_t gyro_y = text[3] << 8 | text[2];
    //int16_t gyro_z = text[5] << 8 | text[4];
    //printf("x: %d, y: %d, z: %d\n",gyro_x, gyro_y , gyro_z );
    printf("Text is: %s\n", text);
    usleep(100 * 1000);

    if (IsKeyDown(KEY_DOWN)) pitch += 0.6f;
    else if (IsKeyDown(KEY_UP)) pitch -= 0.6f;
    else
    {
        if (pitch > 0.3f) pitch -= 0.3f;
        else if (pitch < -0.3f) pitch += 0.3f;
    }

    // Plane yaw (y-axis) controls
    if (IsKeyDown(KEY_S)) yaw -= 1.0f;
    else if (IsKeyDown(KEY_A)) yaw += 1.0f;
    else
    {
        if (yaw > 0.0f) yaw -= 0.5f;
        else if (yaw < 0.0f) yaw += 0.5f;
    }

    // Plane roll (z-axis) controls
    if (IsKeyDown(KEY_LEFT)) roll -= 1.0f;
    else if (IsKeyDown(KEY_RIGHT)) roll += 1.0f;
    else
    {
        if (roll > 0.0f) roll -= 0.5f;
        else if (roll < 0.0f) roll += 0.5f;
    }

    model.transform = MatrixRotateXYZ((Vector3){DEG2RAD*pitch, DEG2RAD*yaw, DEG2RAD*roll});
    //model.transform = MatrixRotateXYZ((Vector3){DEG2RAD*gyro_x, DEG2RAD*gyro_y, DEG2RAD*gyro_z});

    //RAYLIB DRAWING
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode3D(camera);

    DrawModel(model, (Vector3){0.0f, -8.0f, 0.0f}, 1.0f, WHITE);
    DrawGrid(10, 1.0f);

    EndMode3D();


    DrawFPS(10,10);

    EndDrawing();

  }

  CloseWindow();

  close(fileDescriptor);

  return 0;
}
