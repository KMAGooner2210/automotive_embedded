
# Automative_Embedded
## Bài 1: GPIO (General Purpose Input Output)
![Image](https://github.com/user-attachments/assets/f275f738-034e-41e5-849a-892cb47e31d6)

#### **Cấp Clock cho ngoại vi**

* **Module RCC** cung cấp các hàm để cấu hình xung clock

`RCC_APB1PeriphClockCmd`

`RCC_APB2PeriphClockCmd` 

`RCC_AHBPeriphClockCmd`
 
* **Nhận 2 tham số**:

 ◦ Ngoại vi Clock

 ◦ ENABLE/DISABLE

#### **Cấu hình ngoại vi**


* **GPIO_Pin:** Chân cần được cấu hình 

`GPIO_Pin_<chân cần được cấu hình>`


 
* **GPIO_Mode:** Chế độ muốn cấu hình

    `typedef enum {`

        GPIO_Mode_AIN = 0x00,            //Analog Input
        GPIO_Mode_IN_FLOATING = 0x04,    //Input bình thường
        GPIO_Mode_IPD = 0x28,            //Input có điện trở kéo xuống    
        GPIO_Mode_IPU = 0x48,            //Input có điện trở kéo lên
        GPIO_Mode_Out_OD = 0x14,         //Output dạng open-drain
        GPIO_Mode_Out_PP = 0x10,         //Output dạng push-pull
        GPIO_Mode_AF_OD = 0x1C,          //Chế độ ngoại vi khác dạng open-drain
        GPIO_Mode_AF_PP = 0x18           //Chế độ ngoại vi khác dạng push-pull
    `} GPIOMode_TypeDef;`

* **GPIO_Speed:** Tốc độ đáp ứng của chân

`GPIO_Speed_<tốc độ muốn cấu hình>`    

#### **Sử dụng ngoại vi**


* **Để gắn các giá trị muốn cấu hình vào các thanh ghi** thì ta sử dụng hàm "GPIO_Init" có 2 tham số cung cấp các hàm để cấu hình xung clock


  ◦Tham số đầu là tên ngoại vi muốn cấu hình

  ◦Tham số thứ hai là con trỏ đến struct **"GPIO_InitTypeDef"**

```c
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;

    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
    GPIO_Init(GPIOC, &GPIO_InitStruct);
```

* **Các hàm thông dụng để điều khiển GPIO** 

```c
    uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);            \\Đọc giá trị 1 bit trong cổng GPIO được cấu hình là INPUT (IDR), có thể đọc nhiều pin nhờ toán tử OR
    uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);                                 \\Đọc giá trị nguyên cổng GPIO được cấu hình là INPUT (IDR)
    uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);           \\Đọc giá trị 1 bit trong cổng GPIO được cấu hình là OUTPUT (ODR), có thể đọc nhiều pin nhờ toán tử OR
    uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);                                \\Đọc giá trị nguyên cổng GPIO được cấu hình là OUTPUT (ODR)
    void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);                        \\Cho giá trị 1 bit trong cổng GPIO = 1, có thể ghi nhiều pin nhờ toán tử OR
    void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);                      \\Cho giá trị 1 bit trong cổng GPIO = 0, có thể ghi nhiều pin nhờ toán tử OR
    void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);     \\Ghi giá trị "BitVal" vào 1 bit trong cổng GPIO, có thể ghi nhiều pin nhờ toán tử OR
    void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);                           \\Ghi giá trị "PortVal" vào nguyên cổng GPIO
```









#### **Pull-Up vs Pull-Down ???**


![Image](https://github.com/user-attachments/assets/2e06645d-579f-4b64-970c-f09a46cf949f)



* **Pull-up:**

     ◦ Được kết nối giữa chân **đầu vào** với **nguồn VCC**

     => Đảm bảo rằng khi **không** có tín hiệu hoặc thiết bị nào **tác động** vào chân nó sẽ **luôn ở mức cao**


     ◦ **Không** có tín hiệu vào:
 
    `GPIO-> HIGH (Pull up kéo lên Vcc)` 

     ◦ **Có**  tín hiệu vào:
 
    `GPIO->  HIGH => LOW `

     ◦ Thường được ứng dụng trong nút nhấn


* **Pull-down:**

    ◦ Được kết nối giữa chân **đầu vào** với **GND**

    => Kéo chân về mức **thấp** khi **không** có tín hiệu hoặc thiết bị nào **tác động** 


    ◦ **Không** có tín hiệu vào:
 
    `GPIO-> LOW (Trở kéo về GND)` 

    ◦ **Có**  tín hiệu vào:
 
    `GPIO->  LOW => HIGH `

    ◦ Thường được dùng để xác định trạng thái khi công tắc hoặc thiết bị đầu vào tắt hoặc không hoạt động




#### **Các chế độ input khác**

* **Input-Floating:**

    ◦ **Mục đích:** Đọc tín hiệu số từ bên ngoài mà không có điện trở kéo lên hoặc kéo xuống bên trong

    ◦ **Cấu hình điện:** Không có trở kéo lên hoặc xuống bên trong vi điều khiển.Trạng thái logic phụ thuộc vào chân bên ngoài

    ◦ **Sử dụng:** Đọc tín hiệu số từ cảm biến bên ngoài đã có trở kéo lên/xuống

    ◦ **Lưu ý:** Dễ bị nhiễu


* **Analog Input:**

    ◦ **Mục đích:** Sử dụng chân GPIO làm đầu vào cho **ADC** tích hợp

    ◦ **Cấu hình điện:** Ngắt kết nối  mạch số cuả GPIO để cho phép tín hiệu analog đi trực tiếp vào **ADC**

    ◦ **Sử dụng:** Đọc điện áp analog từ cảm biến analog (cảm biến nhiệt độ,ánh sáng,biến trở)

    ◦ **Lưu ý:** Dễ bị nhiễu
    

#### **Các chế độ output**

* **Output Push-Pull:**

    ◦ **Mục đích:**  Xuất tín hiệu số kéo VDD (HIGH) và kéo xuống GND (LOW) 1 cách chủ động


    ◦ **Sử dụng:** Điều khiển relay,led,...



* **Output Open-Drain (Đầu ra hở mạch):**

    ◦ **Mục đích:**  Xuất tín hiệu số kéo chỉ có khả năng kéo GND (LOW) 1 cách chủ động.Kéo lên HIGH cần có điện trở kéo bên ngoài

    ◦ **Cấu hình điện:** Khi xuất mức HIGH ,transistor tắt,GPIO thả nổi và cần điện trở kéo lên bên ngoài

    ◦ **Sử dụng:** I2C,...   



#### **Các chế độ Alternate Function**


* **AF-Push Pull:**

    ◦ **Mục đích:** Sử dụng chân GPIO để xuất tín hiệu từ các peripheral

    ◦ **Sử dụng:** Xuất tín hiệu PWM từ Timer, clock từ SPI,...   


* **AF-Open Drain:**

    ◦ **Mục đích:** Sử dụng cho các giao thức yêu cầu đường truyền 2 chiều

    ◦ **Sử dụng:** GPIO cho giao tiếp I2C (SCL,SDA) ,UART (truyền nhận)


* **AF-Input:**

    ◦ **Mục đích:** Sử dụng chân GPIO để làm đầu vào cho 1 số peripheral

    ◦ **Sử dụng:** ADC , DMA(Scan Mode),...
