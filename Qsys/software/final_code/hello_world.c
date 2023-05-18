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
	else if(duty_num1==0) dir1 = 3;
	else dir1 = 2; //10 正轉

	if(duty_num2<0) dir2 = 1; //01 反轉
	else if(duty_num2==0) dir2 = 3;
	else dir2= 2; //10 正轉

	IOWR(MOTORA_DUTY_BASE, 0, abs(duty_num1));
	IOWR(MOTORB_DUTY_BASE, 0, abs(duty_num2));
	IOWR(MOTORA_DIR_BASE, 0, dir1);
	IOWR(MOTORB_DIR_BASE, 0, dir2);

}

int main()
{
  int go_speed = 180;
  int right_speed_p = 200;
  int right_speed_n = -170;
  int left_speed_p = 180;
  int left_speed_n = -180;
  int min_speed_left = 170;
  int min_speed_right = 170;

  printf("Hello from Nios II!\n");

  int sensor[5] = {IORD(INFRA_SENSOR_0_BASE, 0), IORD(INFRA_SENSOR_1_BASE, 0), IORD(INFRA_SENSOR_2_BASE, 0), IORD(INFRA_SENSOR_3_BASE, 0), IORD(INFRA_SENSOR_4_BASE, 0)};
  int U_Turn_Left = 0, U_Turn_Right = 0, Left = 0, Right = 0, Min_Right = 0, Min_Left = 0, Go = 1;
  while(1){
	  //紅外線感測器讀值
	  sensor[0] = IORD(INFRA_SENSOR_0_BASE, 0);
	  sensor[1] = IORD(INFRA_SENSOR_1_BASE, 0);
	  sensor[2] = IORD(INFRA_SENSOR_2_BASE, 0);
	  sensor[3] = IORD(INFRA_SENSOR_3_BASE, 0);
	  sensor[4] = IORD(INFRA_SENSOR_4_BASE, 0);

	  if(Go){
		  sensor[2] = IORD(INFRA_SENSOR_2_BASE, 0);
		  motor(go_speed, go_speed);
		  if((sensor[0] && sensor[1])||(sensor[3] && sensor[4])){ 				//中間感測器在白色區域
			  if(sensor[1]){			//左轉
				  Go = 0;
				  Left = 1;
			  } else if(sensor[3]){		//右轉
				  Go = 0;
				  Right = 1;
			  }
		  } else{						//中間感測器在黑色區域
			  if(sensor[0]){			//向左大轉彎
				  U_Turn_Left = 1;
				  Go = 0;
			  } else if(sensor[4]){	 	//向右大轉彎
				  U_Turn_Right = 1;
				  Go = 0;
			  }

			  while(sensor[1]){			//向左微調
				  motor(-min_speed_left, min_speed_left);

				  sensor[0] = IORD(INFRA_SENSOR_0_BASE, 0);
				  sensor[1] = IORD(INFRA_SENSOR_1_BASE, 0);
				  sensor[4] = IORD(INFRA_SENSOR_4_BASE, 0);

				  if(sensor[0]){			//向左大轉彎
					  U_Turn_Left = 1;
					  Go = 0;
				  } else if(sensor[4]){	 	//向右大轉彎
					  U_Turn_Right = 1;
					  Go = 0;
				  }
				  usleep(1000);
			  }
			  while(sensor[3]){			//向右微調
				  motor(min_speed_right, -min_speed_right);

				  sensor[0] = IORD(INFRA_SENSOR_0_BASE, 0);
				  sensor[3] = IORD(INFRA_SENSOR_3_BASE, 0);
				  sensor[4] = IORD(INFRA_SENSOR_4_BASE, 0);

				  if(sensor[0]){			//向左大轉彎
					  U_Turn_Left = 1;
					  Go = 0;
				  } else if(sensor[4]){	 	//向右大轉彎
					  U_Turn_Right = 1;
					  Go = 0;
				  }
				  usleep(1000);
			  }
		  }
	  }else if(U_Turn_Left){			//向左大轉彎
		  sensor[2] = IORD(INFRA_SENSOR_2_BASE, 0);
		  while(sensor[2]){				//先直走直到中間感測器偵測到白色區域
			  sensor[2] = IORD(INFRA_SENSOR_2_BASE, 0);
			  motor(go_speed, go_speed);
			  usleep(1000);
		  }

		  while(!sensor[2]){			//往左轉
			  sensor[2] = IORD(INFRA_SENSOR_2_BASE, 0);
			  motor(left_speed_n, left_speed_p);
			  usleep(1000);
		  }
		  Go = 1;
		  U_Turn_Left = 0;
	  } else if(U_Turn_Right){			//向右大轉彎
  		  sensor[2] = IORD(INFRA_SENSOR_2_BASE, 0);
  		  while(sensor[2]){				//先直走直到中間感測器偵測到白色區域
  			  sensor[2] = IORD(INFRA_SENSOR_2_BASE, 0);
  			  motor(go_speed, go_speed);
  			  usleep(1000);
  		  }
  		  while(!sensor[2]){			//往右轉
  			  sensor[2] = IORD(INFRA_SENSOR_2_BASE, 0);
  			  motor(right_speed_p, right_speed_n);
  			  usleep(1000);
  		  }
  		  Go = 1;
  		  U_Turn_Right = 0;
  	  }else if(Right){					//往右轉
		  sensor[2] = IORD(INFRA_SENSOR_2_BASE, 0);
		  while(!sensor[2]){
			  sensor[2] = IORD(INFRA_SENSOR_2_BASE, 0);
			  motor(right_speed_p, right_speed_n);
			  usleep(1000);
		  }
		  Go = 1;
		  Right = 0;
  	  }else if(Left){					//往左轉
		  sensor[2] = IORD(INFRA_SENSOR_2_BASE, 0);
		  while(!sensor[2]){
			  sensor[2] = IORD(INFRA_SENSOR_2_BASE, 0);
			  motor(left_speed_n, left_speed_p);
			  usleep(1000);
		  }
		  Go = 1;
		  Left = 0;
	  }

	  usleep(1000);
  }

  return 0;
}
