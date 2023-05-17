#include <io.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <system.h>
#include <unistd.h>

void motor_ctrl(int ma_duty, int mb_duty);
void sensor_ctrl(int sensor_num);

int main() {
  int i, sw;
  printf("hello nios II\n");
  while (1) {
    printf("sensor: %d\n", IORD(SENSOR_BASE, 0));
    // motor_ctrl(0, 255);
    // usleep(1000000);
    // motor_ctrl(255, 0);
    // usleep(1000000);
    // motor_ctrl(-128, -255);
    // usleep(1000000);
    // motor_ctrl(128, -50);
    // usleep(1000000);
  }
  return 0;
}

void motor_ctrl(int ma_duty, int mb_duty) {
  int ma_dir, mb_dir;
  // duty cycle range: -255 ~ 255

  // motor a ctrl
  if (ma_duty == 0)
    ma_dir = 0x03;
  else
    ma_dir = (ma_duty < 0) ? 0x01 : 0x02;
  IOWR(MOTORA_DIR_BASE, 0, ma_dir);
  IOWR(MOTORA_DUTY_BASE, 0, abs(ma_duty));

  // motor b ctrl
  if (mb_duty == 0)
    mb_dir = 0x03;
  else
    mb_dir = (mb_duty < 0) ? 0x01 : 0x02;
  IOWR(MOTORB_DIR_BASE, 0, mb_dir);
  IOWR(MOTORB_DUTY_BASE, 0, abs(mb_duty));
}

void sensor_ctrl(int sensor_num) {
  int sensor = IORD(SENSOR_BASE, 0);
  printf("sensor: %d\n", sensor);
  // control motor
  switch (sensor) {
  case 0x00:
    // motor ctrl
    break;
  case 0x01:
    // motor ctrl
    break;
  case 0x02:
    // motor ctrl
    break;
  case 0x03:
    // motor ctrl
    break;
  default:
    // motor ctrl
    break;
  }
}
