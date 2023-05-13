#include <io.h> 	/* 系統I/O函式庫*/
#include <stdio.h> 	/* 系統標準輸出入函式庫 */
#include <system.h> /* 系統函式庫 */
#include <unistd.h> /* 宣告usleep函數 */
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

void motor(int duty_num1, int duty_num2)
{
	int dir1, dir2;
	if(duty_num1<0) dir1 = 1; //01 反轉
	else dir1 = 2; //10 正轉
	if(duty_num2<0) dir2 = 1; //01 反轉
	else dir2= 2; //10 正轉

	IOWR(MOTORA_DUTY_BASE, 0, abs(duty_num1));
	IOWR(MOTORA_DIR_BASE, 0, dir1);
	IOWR(MOTORB_DUTY_BASE, 0, abs(duty_num2));
	IOWR(MOTORB_DIR_BASE, 0, dir2);

}

int main()
{
  printf("Hello from Nios II!\n");

  int sensor[5] = {IORD(INFRA_SENSOR_0_BASE, 0), IORD(INFRA_SENSOR_1_BASE, 0), IORD(INFRA_SENSOR_2_BASE, 0), IORD(INFRA_SENSOR_3_BASE, 0), IORD(INFRA_SENSOR_4_BASE, 0)};
  int U_Turn_Left = 0, U_Turn_Right = 0, Left = 0, Right = 0, Go = 1;
  while(1){
	  if(Go){
		  //紅外線感測器讀值
		  sensor[0] = IORD(INFRA_SENSOR_0_BASE, 0);
		  sensor[1] = IORD(INFRA_SENSOR_1_BASE, 0);
		  sensor[2] = IORD(INFRA_SENSOR_2_BASE, 0);
		  sensor[3] = IORD(INFRA_SENSOR_3_BASE, 0);
		  sensor[4] = IORD(INFRA_SENSOR_4_BASE, 0);

		  //前進
		  motor(200, 200);

		  //判斷哪一顆感測器讀到值
		  if(sensor[0]){
			  U_Turn_Left = 1;
		  	  Go = 0;
		  } else if(sensor[1]){
			  Left = 1;
			  Go = 0;
		  } else if(sensor[4]){
			  U_Turn_Right = 1;
		  	  Go = 0;
		  } else if(sensor[3]){
			  Right = 1;
			  Go = 0;
		  }
	  } else if(U_Turn_Left){
		  sensor[2] = IORD(INFRA_SENSOR_2_BASE, 0);
		  while(sensor[2]){
			  sensor[2] = IORD(INFRA_SENSOR_2_BASE, 0);
			  motor(200, 200);
		  }
		  motor(0, 0);
		  usleep(50000);
		  while(!sensor[2]){
			  sensor[2] = IORD(INFRA_SENSOR_2_BASE, 0);
			  motor(-160, 180);
		  }
		  Go = 1;
		  U_Turn_Left = 0;
	  } else if(Left){
		  sensor[2] = IORD(INFRA_SENSOR_2_BASE, 0);
		  while(!sensor[2]){
			  sensor[2] = IORD(INFRA_SENSOR_2_BASE, 0);
			  sensor[3] = IORD(INFRA_SENSOR_3_BASE, 0);
			  if(sensor[3])
				  break;
			  motor(-160, 180);
		  }
		  if(sensor[3]){
			  Left = 0;
			  Right = 1;
			  Go = 0;
		  } else{
			  Go = 1;
			  Left = 0;
		  }
	  } else if(U_Turn_Right){
		  sensor[2] = IORD(INFRA_SENSOR_2_BASE, 0);
		  while(sensor[2]){
			  sensor[2] = IORD(INFRA_SENSOR_2_BASE, 0);
			  motor(200, 200);
		  }
		  motor(0, 0);
		  usleep(50000);
		  while(!sensor[2]){
			  sensor[2] = IORD(INFRA_SENSOR_2_BASE, 0);
			  motor(180, -160);
		  }
		  Go = 1;
		  U_Turn_Right = 0;
	  } else if(Right){
		  sensor[2] = IORD(INFRA_SENSOR_2_BASE, 0);
		  while(!sensor[2]){
			  sensor[2] = IORD(INFRA_SENSOR_2_BASE, 0);
			  sensor[1] = IORD(INFRA_SENSOR_1_BASE, 0);
			  if(sensor[1])
				  break;
			  motor(180, -160);
		  }
		  if(sensor[1]){
			  Right = 0;
			  Left = 1;
		  } else{
			  Go = 1;
			  Right = 0;
		  }
	  }
  }

  return 0;
}
