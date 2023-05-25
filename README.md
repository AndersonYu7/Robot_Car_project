# Robot_Car_project

## 紀錄進度

### 5/8 :

* 確認機構
* 紅外線規劃(使用三個感測器)
* 馬達測試
* 紅外線感測器測試


### 5/11 :

* 車子組裝完畢
* 感測器在車上測試
* 馬達運轉測試
* 簡單跑循線
* 遇到問題：L298N 只能正轉 不能反轉

https://github.com/AndersonYu7/Robot_Car_project/assets/95768254/fbe707a0-541e-4394-98ac-dddb3c9f1208

### 5/15 :

* 更改為五個感測器(讓銳角是使用最外面的感測器做偵測 -> 預防110 011的方向問題)
* 110 011 的情況改成先往一個方向轉 假如方向錯 會再轉回去

https://github.com/AndersonYu7/Robot_Car_project/assets/95768254/ba05477a-bc22-4481-a021-06c273242789


### 5/16 : 
* 遇到問題:GPIO 無法給值

### 5/17 : 
* 解決GPIO問題
* 更改感測器位置
* 重新測試車子

### 5/22 : 
* 最旁邊兩側感測器會有偵測不到的情況導致大轉彎轉彎不順利

https://github.com/AndersonYu7/Robot_Car_project/assets/95768254/53e20605-5549-4d3f-b4c5-fa045188e179

### 5/25 : 
* 驗收 : 最終成績 53.35秒

https://github.com/AndersonYu7/Robot_Car_project/assets/95768254/98c2480d-6055-4eae-9cd2-609fc6d3bc1c



### 問題 :

* 當感測器為110 or 011時 會發生兩種反向的情況 

  例如 : 可能在直角轉彎時會遇到110 而這時是要右轉 但是在銳角轉彎時也會遇到110 而這時是要左轉 會起衝突
  
* GPIO 無法給值

  重新創建Quartus II project 可暫時解決問題 但重開機 問題一樣
  
  解決 : 發現是L298N IN的線接觸不良 且GPIO0_D[3] 沒有輸出

* 拔除連接線 程式在某個地方會卡住 

  原因 : printf太多 導致buffer空間不足 

  解決 : printf全部刪除或註解

* L298N 只能正轉 不能反轉

  可能原因 : 電壓不足
