# BrillianceLight

此项目采用 WS2812 全彩 RGB 灯珠作为像素点；

将相机 ISO 降低，光圈缩小，曝光时间增大至数秒，挥动光辉帮即可显示相应图片或图案；

### 方案V0.8

* 最小系统板
  * 采用STM32F401CCU6单片机
  * 电源管理：外挂电池，可充电，带有充电指示灯
  * 供电方式：电池供电 3.7V (灯带亮度无法达到最亮，但足够正常使用)，TYPE-C供电 5V
  * 采用 SD 卡存储图片，TYPE-C 仅用做供电
  * 采用三个按键进行人机交互，通过 0.96 寸 OLED 屏幕 (IIC) 进行显示 ( 排针连接 )

<u> PS：在工程版最后做电池测试时，芯片烧毁，项目被迫暂停！</u> 2022.3.12

---

### 方案V1.0

* 采用STM32F401CCU6单片机
* 供电：
  * Type-C 5V 供电
  * 外接 7.4V 电池供电 (Sony NP-FW50 相机电池)
* SD 卡存储 BMP 格式照片
  * 照片宽度为 60 像素
  * BMP 为24 位深度
  * SD 卡为 SPI 驱动
* 采用 Fatfs 文件系统读取图片
* IIC 0.96 寸 OLED 屏幕显示
  * 引脚为 (3V3 GND SCL SDA)
* 灯带为 60 个 WS2812C 灯珠

> 目前已知问题：
> 
> * 灯珠无法超过 60 颗，会显示乱码
> * 图片的第一个像素不能为黑色，否则无法显示

## 附录：

移植Fatfs文件系统：[基于STM32完成FATFS文件系统移植与运用--这是完全免费开源的FAT文件系统 - 云+社区 - 腾讯云 (tencent.com)](https://cloud.tencent.com/developer/article/1938091)

文件扫描：[(31条消息) FatFs-目录下文件扫描_EmbededCoder的博客-CSDN博客_fatfs文件扫描](https://blog.csdn.net/u012308586/article/details/115903193?utm_medium=distribute.pc_aggpage_search_result.none-task-blog-2~aggregatepage~first_rank_ecpm_v1~rank_v31_ecpm-2-115903193.pc_agg_new_rank&utm_term=FATFS获取路径下文件列表&spm=1000.2123.3001.4430)

> By HCl 
> 
> Supported by NJUPT-SAST-E
