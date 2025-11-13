#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MLX90640.h>

// I²C 配置
#define I2C_SCL 22
#define I2C_SDA 21
#define I2C_FREQ 1000000  // MLX90640 支持最高 1MHz[^20^]

Adafruit_MLX90640 mlx;

// 温度数据存储（32列 × 24行 = 768个像素点）
float frame[32 * 24];

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  delay(1000);  // 等待串口稳定

  Serial.println("ESP32-MLX90640 热成像初始化...");
  
  // 初始化 I²C 总线
  Wire.begin(I2C_SDA, I2C_SCL, I2C_FREQ);
  
  // 初始化 MLX90640
  if (!mlx.begin(MLX90640_I2CADDR_DEFAULT, &Wire)) {
    Serial.println("错误：未检测到 MLX90640 传感器！请检查接线");
    while (1) delay(10);
  }
  Serial.println("成功：MLX90640 已连接");

  // 配置传感器参数
  mlx.setMode(MLX90640_CHESS);  // 棋盘模式，噪声更低[^20^]
  mlx.setResolution(MLX90640_ADC_18BIT);  // 18位分辨率[^20^]
  mlx.setRefreshRate(MLX90640_32_HZ);  // 32Hz刷新率[^20^]
  
  Serial.println("配置完成，开始读取温度数据...");
}

void loop() {
  // 获取一帧温度数据
  if (mlx.getFrame(frame) != 0) {
    Serial.println("读取失败，重试...");
    delay(100);
    return;
  }

  // 打印中心点温度（第12列，第12行）
  float centerTemp = frame[12 * 32 + 12];
  Serial.print("中心温度: ");
  Serial.print(centerTemp, 2);
  Serial.println(" °C");

  // 可选：打印所有像素温度
  // for (int i = 0; i < 768; i++) {
  //   Serial.print(frame[i], 2);
  //   Serial.print(", ");
  // }

  delay(100);  // 延时100ms
}