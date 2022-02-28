# BrillianceLight

此项目采用 WS2812 全彩 RGB 灯珠作为像素点；

将相机 ISO 降低，光圈缩小，曝光时间增大至数秒，挥动光辉帮即可显示相应图片或图案；



### 方案V1.0

* 最小系统板
  * 采用STM32F401CCU6单片机
  * 电源管理：外挂电池，可充电，带有充电指示灯
  * 供电方式：电池供电 3.7V (灯带亮度无法达到最亮，但足够正常使用)，TYPE-C供电 5V
  * 采用 SD 卡存储图片，TYPE-C 仅用做供电
  * 采用三个按键进行人机交互，通过 0.96 寸 OLED 屏幕 (IIC) 进行显示 ( 排针连接 )



> By HCl 
>
> Supported by NJUPT-SAST-E

