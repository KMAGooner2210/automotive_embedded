
	
# AUTOMOTIVE_EMBEDDED

<details>
	<summary><strong>BÀI 1: GPIO</strong></summary>
	
## BÀI 1: GPIO (General Purpose Input Output)
![Image](https://github.com/user-attachments/assets/f275f738-034e-41e5-849a-892cb47e31d6)

### **1.Quy trình làm việc với GPIO**

#### **1.1. Cấp xung clock (RCC Configuration)**

* **Trên STM32, các ngoại vi đều bị tắt clock mặc định để tiết kiệm năng lượng. Phải kích hoạt clock trước khi sử dụng.**

	`void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);`
	
	`void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);` 
	
	`void RCC_AHBPeriphClockCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState);`
 
* **Nhận 2 tham số**:

	 ◦ Ngoại vi Clock
	
	 ◦ ENABLE/DISABLE

* **Định nghĩa các Port GPIO**:

		#define RCC_APB2Periph_GPIOA              ((uint32_t)0x00000004)
		#define RCC_APB2Periph_GPIOB              ((uint32_t)0x00000008)
		#define RCC_APB2Periph_GPIOC              ((uint32_t)0x00000010)
		#define RCC_APB2Periph_GPIOD              ((uint32_t)0x00000020)
		#define RCC_APB2Periph_GPIOE              ((uint32_t)0x00000040)
		#define RCC_APB2Periph_GPIOF              ((uint32_t)0x00000080)
		#define RCC_APB2Periph_GPIOG              ((uint32_t)0x00000100)

*  **VD:**

  		// Kích hoạt clock cho GPIOA và GPIOC
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);

		// Vô hiệu hóa clock cho GPIOB
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, DISABLE);

#### **1.2.Cấu hình GPIO**

* **Structure cấu hình**

		  typedef struct {
		    uint16_t GPIO_Pin;             // Chân GPIO cần cấu hình
		    GPIOSpeed_TypeDef GPIO_Speed;  // Tốc độ đáp ứng
		    GPIOMode_TypeDef GPIO_Mode;    // Chế độ hoạt động
		} GPIO_InitTypeDef;
  
* **Các chế độ hoạt động (GPIOMode_TypeDef)**

		typedef enum {
		    GPIO_Mode_AIN = 0x0,           // Analog Input
		    GPIO_Mode_IN_FLOATING = 0x04,  // Floating Input
		    GPIO_Mode_IPD = 0x28,          // Input Pull-Down
		    GPIO_Mode_IPU = 0x48,          // Input Pull-Up
		    GPIO_Mode_Out_OD = 0x14,       // Output Open-Drain
		    GPIO_Mode_Out_PP = 0x10,       // Output Push-Pull
		    GPIO_Mode_AF_OD = 0x1C,        // Alternate Function Open-Drain
		    GPIO_Mode_AF_PP = 0x18         // Alternate Function Push-Pull
		} GPIOMode_TypeDef;

 
* **Các tốc độ đáp ứng (GPIOSpeed_TypeDef)**

		typedef enum {
		    GPIO_Speed_2MHz   = 0x02,      // Tốc độ thấp (2MHz)
		    GPIO_Speed_10MHz  = 0x01,      // Tốc độ trung bình (10MHz)
		    GPIO_Speed_50MHz  = 0x03       // Tốc độ cao (50MHz)
		} GPIOSpeed_TypeDef;

  
* **Ví dụ cấu hình:**

		GPIO_InitTypeDef GPIO_InitStructure;
		
		// Cấu hình chân PC13 làm Output Push-Pull
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		
		// Cấu hình chân PA0 làm Input với điện trở kéo lên
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

### **3. Các hàm điều khiển GPIO**

#### **3.1. Ghi dữ liệu xuống Output**

* **Các hàm cơ bản:**

		// Set bit (xuất mức logic 1)
		void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
		
		// Reset bit (xuất mức logic 0)
		void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
		
		// Ghi giá trị cụ thể cho một chân
		void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);
		
		// Ghi giá trị cho toàn bộ port
		void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);
 
* **Ví dụ sử dụng:**

		// Bật LED trên PC13 (giả sử LED nối VCC, chân GPIO xuất 0 để bật)
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		
		// Tắt LED trên PC13
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		
		// Sử dụng GPIO_WriteBit
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);    // Tắt LED
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);  // Bật LED
		
		// Toggle LED sử dụng GPIO_ReadOutputDataBit
		if (GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13) == Bit_SET) {
		    GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		} else {
		    GPIO_SetBits(GPIOC, GPIO_Pin_13);
		}

#### **3.2. Đọc dữ liệu từ Input**

* **Các hàm đọc dữ liệu:**

		// Đọc trạng thái một chân input
		uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
		
		// Đọc giá trị toàn bộ port input
		uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);
		
		// Đọc trạng thái một chân output (trạng thái thanh ghi ODR)
		uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
		
		// Đọc giá trị toàn bộ port output
		uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);
		 
* **Ví dụ sử dụng:**

		// Đọc trạng thái nút nhấn trên PA0 (Input Pull-Up)
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET) {
		    // Nút được nhấn (mức 0 do Pull-Up)
		    GPIO_ResetBits(GPIOC, GPIO_Pin_13); // Bật LED
		} else {
		    // Nút không được nhấn
		    GPIO_SetBits(GPIOC, GPIO_Pin_13);   // Tắt LED
		}
		
		// Đọc nhiều chân cùng lúc
		uint16_t input_status = GPIO_ReadInputData(GPIOA);
		if ((input_status & (GPIO_Pin_0 | GPIO_Pin_1)) == (GPIO_Pin_0 | GPIO_Pin_1)) {
		    // Cả PA0 và PA1 đều ở mức cao
		}

### **4.Kiến thức cần chú ý**

#### **4.1.Pull-Up vs Pull-Down ???**


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




#### **4.2.Các chế độ input khác**

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
    

#### **4.3.Các chế độ output**

* **Output Push-Pull:**

    ◦ **Mục đích:**  Xuất tín hiệu số kéo VDD (HIGH) và kéo xuống GND (LOW) 1 cách chủ động


    ◦ **Sử dụng:** Điều khiển relay,led,...



* **Output Open-Drain (Đầu ra hở mạch):**

    ◦ **Mục đích:**  Xuất tín hiệu số kéo chỉ có khả năng kéo GND (LOW) 1 cách chủ động.Kéo lên HIGH cần có điện trở kéo bên ngoài

    ◦ **Cấu hình điện:** Khi xuất mức HIGH ,transistor tắt,GPIO thả nổi và cần điện trở kéo lên bên ngoài

    ◦ **Sử dụng:** I2C,...   



#### **4.4.Các chế độ Alternate Function**


* **AF-Push Pull:**

    ◦ **Mục đích:** Sử dụng chân GPIO để xuất tín hiệu từ các peripheral

    ◦ **Sử dụng:** Xuất tín hiệu PWM từ Timer, clock từ SPI,...   


* **AF-Open Drain:**

    ◦ **Mục đích:** Sử dụng cho các giao thức yêu cầu đường truyền 2 chiều

    ◦ **Sử dụng:** GPIO cho giao tiếp I2C (SCL,SDA) ,UART (truyền nhận)
</details>

<details>
	<summary><strong>BÀI 2: INTERRUPT </strong></summary>  

## BÀI 2: INTERRUPT 


### **1. Khái niệm**

![Image](https://github.com/user-attachments/assets/eb1762a6-057e-4212-91cd-7d216830df0d)


*  **Ngắt (Interrupt)** là sự kiện khẩn cấp yêu cầu vi điều khiển (MCU) tạm dừng chương trình chính để thực thi một đoạn chương trình đặc biệt gọi là **Interrupt Service Routine (ISR)**.

*  Ngắt giúp MCU phản ứng nhanh với các sự kiện từ phần cứng hoặc phần mềm mà không cần kiểm tra liên tục (polling).

#### **1.1. Các loại ngắt trên STM32F103**

*  **Ngắt ngoại vi:** EXTI, UART, Timer, ADC, SPI, I2C

*  **Ngắt nội bộ:**  Do CPU hoặc hệ thống tạo ra, như SysTick (ngắt định kỳ), PendSV (cho chuyển đổi tác vụ trong RTOS), Software Interrupt (SWI).

*  **Ngắt lỗi:** HardFault (lỗi phần cứng), MemManage (quản lý bộ nhớ), BusFault, UsageFault.

#### **1.2. Cơ chế hoạt động**

*  **1. Sự kiện ngắt xảy ra (ví dụ: nút nhấn trên GPIO).**

*  **2. NVIC (Nested Vectored Interrupt Controller) xác định số vector ngắt (vector number).**

*  **3. CPU tra cứu Vector Interrupt Table (VIT) để lấy địa chỉ ISR.**

*  **4. CPU lưu trạng thái hiện tại (stacking: đẩy thanh ghi vào stack).**

*  **5. Nhảy đến ISR để thực thi.**

*  **6. Sau khi hoàn thành, khôi phục trạng thái và quay lại chương trình chính.**
  

### **2. Interrupt Service Routine (Trình phục vụ ngắt)**

![Image](https://github.com/user-attachments/assets/90c8c28e-edea-4755-b2b3-8c84be788a61)

#### **2.1. Định nghĩa**

 * ISR là hàm xử lý ngắt, được gọi khi ngắt xảy ra. Mỗi ngắt có một ISR riêng, được xác định bởi địa chỉ trong **Vector Interrupt Table (VIT).**

#### **2.2. Khai báo ISR**

 * Trong STM32F103 (SPL), ISR được khai báo trong file startup (thường là `startup_stm32f10x_md.s` hoặc tương tự)

			void EXTI0_IRQHandler(void) {
			    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
			        // Xử lý ngắt (ví dụ: đảo trạng thái LED)
			        GPIO_ToggleBits(GPIOC, GPIO_Pin_13);
			        EXTI_ClearITPendingBit(EXTI_Line0);  // Xóa cờ ngắt
			    }
			}   		

#### **2.3. Lưu ý**

 * **Tối ưu tốc độ:** Tránh dùng delay hoặc vòng lặp dài.
   
 * **Xóa cờ ngắt:** Luôn xóa cờ ngắt (pending bit) trong ISR để tránh lặp ngắt vô hạn.
   
 * **Tránh lồng ngắt không cần thiết:** Chỉ bật ngắt lồng (nested interrupt) khi cần.

 * **Kiểm tra lỗi HardFault:** Nếu ISR gây lỗi (như truy cập bộ nhớ không hợp lệ), CPU nhảy đến HardFault.

			void EXTI0_IRQHandler(void) {
			    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
			        // Xử lý ngắt (ví dụ: đảo trạng thái LED)
			        GPIO_ToggleBits(GPIOC, GPIO_Pin_13);
			        EXTI_ClearITPendingBit(EXTI_Line0);  // Xóa cờ ngắt
			    }
			}
   
### **3.Vector Interrupt Table (Bảng vector ngắt)**

![Image](https://github.com/user-attachments/assets/038312e2-516d-4eb2-8125-438cf5885fc6)

#### **3.1. Khái Niệm**

 * VIT là bảng chứa danh sách các địa chỉ bộ nhớ của ISR, tương ứng với từng nguồn ngắt. Trên STM32F103:

	◦ **Vị trí:** Đặt ở đầu bộ nhớ Flash (mặc định: 0x00000000, có thể remap sang SRAM).
	
	◦ **Kích thước:** Mỗi mục là 4 byte (địa chỉ 32-bit). STM32F103 hỗ trợ ~80 vector (bao gồm Reset, HardFault, và ngắt ngoại vi).

 	◦ **Cấu trúc:** Bao gồm:

			Vector 0: Địa chỉ stack pointer ban đầu.
			Vector 1: Địa chỉ Reset handler.
			Vector 2-15: Ngắt hệ thống (NMI, HardFault, SysTick, v.v.).
			Vector 16+: Ngắt ngoại vi (EXTI, UART, Timer, v.v.).

#### **3.2. Ví Dụ Cấu Trúc VIT**



<img width="1126" height="714" alt="Image" src="https://github.com/user-attachments/assets/3cf7fd5e-6a5b-4d9b-bf51-c8bcc95f130e" />

<img width="1130" height="607" alt="Image" src="https://github.com/user-attachments/assets/2a6db406-ba68-4958-8847-83cae8c878bf" />



#### **3.3. Cách NVIC Sử Dụng VIT**


  1. Khi ngắt xảy ra, NVIC **xác định vector number** (ví dụ: EXTI0 là 6).
	
  2. **Tính địa chỉ ISR:** Base_Address + (vector_number * 4) (Base_Address thường là 0x00000000).
	
  3. CPU **lấy địa chỉ ISR từ VIT** và nhảy đến thực thi.
  	

### **4. NVIC (Nested Vectored Interrupt Controller)**

#### **4.1. Khái niệm**

* NVIC là bộ điều khiển ngắt lồng nhau của ARM Cortex-M3, quản lý tất cả ngắt trên STM32F103:

	◦ **Chức năng:** Bật/tắt ngắt, quản lý ưu tiên, hỗ trợ ngắt lồng (nested interrupt).

	◦ **Đặc điểm:** Hỗ trợ 16 mức ưu tiên (0-15, số nhỏ hơn có ưu tiên cao hơn).

#### **4.2. Quản lý ưu tiên**

* **NVIC sử dụng hai loại ưu tiên:**

	◦ **Preemption Priority:** Quyết định ngắt nào có thể chen ngang (preempt) ngắt khác. Số nhỏ hơn → ưu tiên cao hơn.

	◦ **SubPriority:** Quyết định thứ tự xử lý khi nhiều ngắt có cùng Preemption Priority xảy ra đồng thời. Số nhỏ hơn → xử lý trước.
  

* **Quản lý ưu tiên ngắt**

 
   ◦ Cho phép 16 mức ưu tiên (0-15)
   
   
   ◦ Mức ưu tiên thấp hơn (số nhỏ hơn) có độ ưu tiên cao hơn

#### **4.3. Priority Group**

* **Priority Group** xác định cách phân chia 4 bit ưu tiên (PRIGROUP) giữa Preemption và SubPriority.

* Hàm `NVIC_PriorityGroupConfig` được sử dụng:
    
![Image](https://github.com/user-attachments/assets/9ca9134e-b2e2-4de9-b352-74ad5a47331e)
 
#### **4.4. Cấu Hình NVIC**

* Sử dụng struct `NVIC_InitTypeDef`:

  ◦ **NVIC_IRQChannel:**  Kênh ngắt (ví dụ: EXTI0_IRQn).

  ◦ **NVIC_IRQChannelPreemptionPriority:**  Mức ưu tiên Preemption (0-15).

  ◦ **NVIC_IRQChannelSubPriority:** Mức ưu tiên SubPriority (0-15).

  ◦ **NVIC_IRQChannelCmd:** Bật (ENABLE) hoặc tắt (DISABLE) ngắt.


#### **4.5. VD**

* **Cấu hình ngắt cho EXTI0 (nút nhấn trên PA0):**
  
		#include "stm32f10x.h"
		
		void EXTI0_IRQHandler(void) {
		    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
		        GPIO_ToggleBits(GPIOC, GPIO_Pin_13);  // Đảo trạng thái LED
		        EXTI_ClearITPendingBit(EXTI_Line0);   // Xóa cờ ngắt
		    }
		}
		
		int main(void) {
		    // Bước 1: Kích hoạt clock
		    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
		
		    // Bước 2: Cấu hình GPIO (PA0 input pull-up, PC13 output)
		    GPIO_InitTypeDef GPIO_InitStruct;
		    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
		    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
		    GPIO_Init(GPIOA, &GPIO_InitStruct);
		    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
		    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
		    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		    GPIO_Init(GPIOC, &GPIO_InitStruct);
		
		    // Bước 3: Cấu hình EXTI
		    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
		    EXTI_InitTypeDef EXTI_InitStruct;
		    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
		    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
		    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;  // Kích hoạt khi nhấn nút (LOW)
		    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
		    EXTI_Init(&EXTI_InitStruct);
		
		    // Bước 4: Cấu hình NVIC
		    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // 4 mức Preemption, 4 mức SubPriority
		    NVIC_InitTypeDef NVIC_InitStruct;
		    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
		    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;
		    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
		    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		    NVIC_Init(&NVIC_InitStruct);
		
		    while(1);  // Vòng lặp chính
		}
  
</details>


<details>
	<summary><strong>BÀI 3: CÁC CHUẨN GIAO TIẾP CƠ BẢN </strong></summary> 


## **BÀI 3: CÁC CHUẨN GIAO TIẾP CƠ BẢN**

### **1. SPI (Serial Peripheral Interface)**

#### **1.1. Đặc điểm**

* **Loại:** Giao tiếp nối tiếp, đồng bộ, song công (full-duplex).

 	◦ **Nối Tiếp (Serial):** Dữ liệu được truyền đi từng bit một trên một đường dây (hoặc một cặp đường dây)

 	◦ **Đồng Bộ (Synchronous):** Quá trình truyền và nhận dữ liệu được đồng bộ hóa bởi một tín hiệu xung nhịp chung (SCK - Serial Clock) do Master tạo ra.Với mỗi xung nhịp (một cạnh lên hoặc cạnh xuống) do Master phát ra, một bit dữ liệu sẽ được dịch ra (trên MOSI) hoặc được dịch vào (trên MISO).

 	◦ **Song Công (Full-Duplex):** Dữ liệu có thể được truyền (TX) và nhận (RX) cùng một lúc.			

* **Số dây:** 4 dây (SCK, MISO, MOSI, SS/CS).
  	 
* **Chế độ:** Master (tạo clock) hoặc Slave (nhận clock).

#### **1.2. Sơ đồ chân**

![Image](https://github.com/user-attachments/assets/9e55733b-bf9f-4d37-912e-84e5faae3086)


* **SCK (Serial Clock)**: Thiết bị Master tạo xung tín hiệu SCK và cung cấp cho Slave

* **MISO (Master Input Slave Output)**: Tín hiệu tạo bởi thiết bị Slave và nhận bởi thiết bị Master.

* **MOSI (Master Output Slave Input)**: Tín hiệu tạo bởi thiết bị Master và nhận bởi thiết bị Slave.

* **SS (Slave Select) / CS (Chip Select)**: Chọn thiết bị Slave cụ thể để giao tiếp. Để chọn Slave giao tiếp thiết bị Master chủ động kéo đường SS tương ứng xuống mức 0 (Low).


#### **1.3.Quá trình truyền nhận**  

* **1.** Master kéo chân SS của chân Slave muốn giao tiếp xuống mức 0 (LOW) để báo hiệu muốn truyền nhận

* **2.** Master tạo xung clock trên SCK.

* **3.** Với mỗi xung clock:

	◦ Master gửi 1 bit qua MOSI.

	◦ Slave gửi 1 bit qua MISO

* **4.** Dữ liệu được dịch trong thanh ghi SPI_DR (Data Register).

* **5.** Lặp lại cho đến khi truyền xong 8/16 bit (tùy cấu hình).
  
* **6.** Master kéo SS lên mức 1 (HIGH) để kết thúc.

#### **1.4.Các chế độ hoạt động**

* SPI Có 4 chế độ hoạt động phụ thuộc **Clock Polarity (CPOL)** và **Clock Phase (CPHA)**.

* **CPOL:**

  ◦ **CPOL = 0:** Xung clock ban đầu ở mức 0.

  ◦ **CPOL = 1:** Xung clock ban đầu ở mức 1.

* **CPHA:**

  ◦ **CPHA = 0 (1Edge):**
  
         Dữ liệu được lấy mẫu (sampled) trên cạnh đầu tiên (leading edge) của chu kỳ đồng hồ và được thay đổi (shifted) trên cạnh thứ hai (trailing edge).
         Dữ liệu phải sẵn sàng trên đường truyền (MOSI/MISO) trước khi chu kỳ đồng hồ bắt đầu.
  
  ◦ **CPHA = 1 (2Edge):**
  
         Dữ liệu được thay đổi (shifted) trên cạnh đầu tiên (leading edge) và được lấy mẫu (sampled) trên cạnh thứ hai (trailing edge).
         Dữ liệu chỉ xuất hiện trên đường truyền sau khi chu kỳ đồng hồ bắt đầu.
  
  ![Image](https://github.com/user-attachments/assets/7035fcb0-fd0e-4d24-976b-0a56d80a1207)
  

* **CPOL = 0 (đồng hồ nghỉ ở mức thấp):**
  
  ◦  **1Edge (CPHA = 0):** Lấy mẫu trên rising edge (cạnh tăng), thay đổi trên falling edge (cạnh giảm).
  
  ◦  **2Edge (CPHA = 1):** Thay đổi trên rising edge, lấy mẫu trên falling edge.

* **CPOL = 1 (đồng hồ nghỉ ở mức cao):**
  
  ◦  **1Edge (CPHA = 0):** Lấy mẫu trên falling edge (cạnh giảm), thay đổi trên rising edge (cạnh tăng).
  
  ◦  **2Edge (CPHA = 1):** Thay đổi trên falling edge, lấy mẫu trên rising edge.

### **2. I2C (Inter-Integrated Circuit)**

#### **2.1. Đặc điểm**

* **Loại:** Giao tiếp nối tiếp, đồng bộ, bán song công (half-duplex).

 	◦ **Nối Tiếp (Serial):** Tương tự SPI, dữ liệu được truyền từng bit một trên một đường duy nhất (SDA), thay vì truyền song song nhiều bit.

 	◦ **Đồng Bộ (Synchronous):** Tương tự SPI, quá trình truyền được đồng bộ bởi một tín hiệu xung nhịp chung (SCL - Serial Clock) do Master duy nhất điều khiển. 

 	◦ **Bán song công (Half-Duplex):** Dữ liệu có thể được truyền và nhận, nhưng không thể thực hiện cùng lúc trên cùng một đường dây.Cả Master và Slave đều sử dụng chung một đường dữ liệu SDA. Vào một thời điểm, chỉ có một thiết bị được quyền "chiếm" đường SDA để gửi dữ liệu. Thiết bị còn lại phải ở chế độ nghe (listen).

* **Số dây:** 2 dây (SCL, SDA), đều là Open-Drain, cần điện trở kéo lên (Pull-up)
  	 
* **Chế độ:** Master (điều khiển bus) hoặc Slave (phản hồi Master)

#### **2.2. Sơ đồ chân**

![Image](https://github.com/user-attachments/assets/34dd609d-2f2e-421d-b416-0cabcbf9670a)


* **SCL (Serial Clock)**: Tạo xung tín hiệu để đồng bộ việc truyền/nhận dữ liệu với các Slave.

* **SDA (Serial Data)**: Chân chứa dữ liệu được truyền đi


#### **2.3.Quá trình truyền nhận**

* **1. Master gửi điều kiện Start (S):Chân SDA xuống mức 0 trước chân SCL.** 
  
* **2. Master gửi địa chỉ Slave (7 bit) + Bit R/W (1 bit)**

  ◦ **Bit R/W = 0:** Master muốn ghi (Write) dữ liệu vào Slave.

  ◦ **Bit R/W = 1:** Master muốn đọc (Read) dữ liệu từ Slave.

* **3. Slave phản hồi ACK (A):**

  ◦ Slave có địa chỉ khớp sẽ kéo đường SDA xuống LOW trong chu kỳ cao của xung clock thứ 9 để báo hiệu nó đã nhận được địa chỉ (ACK).

  ◦  Nếu không có Slave nào nhận ra địa chỉ, đường SDA sẽ được để ở mức HIGH (do điện trở kéo lên) trong xung clock thứ 9 (NACK). Master sẽ biết là không có Slave phản hồi.

* **4. Truyền Data Byte:**

  ◦ Master (khi ghi) hoặc Slave (khi đọc) sẽ gửi một byte dữ liệu (8 bit, MSB first).

  ◦ Sau mỗi byte, bên nhận phải gửi một bit ACK (kéo SDA xuống LOW) để xác nhận.

* **5. Lặp lại bước 4 cho đến khi truyền hết dữ liệu.**

* **6. Master gửi điều kiện Stop (P): SDA lên HIGH sau SCL.**

### **3. UART**

#### **3.1. Đặc điểm**

* **Loại:** Giao tiếp nối tiếp, không đồng bộ, song công (full-duplex).
  
 	◦ **Nối Tiếp (Serial):** Tương tự SPI và I2C, dữ liệu được truyền từng bit một trên một đường dây. 

 	◦ **Không đồng bộ (Asynchronous):** Không có đường tín hiệu xung nhịp (Clock) chung nào để đồng bộ hóa giữa hai thiết bị.
  
 	◦ **Song công (Full-Duplex):** ữ liệu có thể được truyền (TX) và nhận (RX) cùng một lúc vì chúng sử dụng hai đường dây hoàn toàn độc lập

* **Số dây:** 2 dây (Tx, Rx)
  	 
#### **3.2. Sơ đồ chân**

![Image](https://github.com/user-attachments/assets/5d657816-56e4-4871-8c40-7ce476b283c2)

**Tx (Transmit)**: Chân truyền dữ liệu. Dữ liệu từ thiết bị này phải được nối đến chân RX của thiết bị kia.

**Rx (Receive)**: Chân nhận dữ liệu. Dữ liệu từ thiết bị kia phải được nối đến chân TX của thiết bị này.

#### **3.3.Quá trình truyền nhận**

![Image](https://github.com/user-attachments/assets/e345c226-4ada-4991-963b-1dcb7e0ae24c)

* **Trạng thái Idle:** Đường TX được giữ ở mức logic HIGH (1) khi không truyền dữ liệu.
  
* **Start: 1 bit.** Luôn là mức logic LOW (0). Đây là tín hiệu báo cho Receiver biết một khung dữ liệu sắp được gửi đến. Nó đánh dấu sự kết thúc của trạng thái Idle và là điểm bắt đầu để Receiver đồng bộ clock nội của nó.

* **Bit dữ liệu:** 5 đến 9 bit.

* **Bit chẵn lẻ:**

    ◦ **Even Parity:** Bit parity được set/clear để tổng số bit 1 trong data bits + parity là một số **chẵn.**
  
    ◦ **Odd Parity:** Tổng số bit 1 là một số lẻ.
  
* **Stop Bit (1, 1.5, hoặc 2 bits):** Luôn là mức logic HIGH (1). Nó đánh dấu sự kết thúc của khung dữ liệu và đưa đường TX trở lại trạng thái Idle (HIGH) để sẵn sàng cho khung tiếp theo.
  
</details>


<details>
	<summary><strong>BÀI 4: GIAO TIẾP SPI</strong></summary> 

## **Bài 4: GIAO TIẾP SPI** 

## **4.1.SPI Software**

### **4.1.1.Xác định các chân GPIO**
* Định nghĩa 4 chân sử dụng SPI

  ```
  #define SPI_SCK_Pin    GPIO_Pin_0
  #define SPI_MISO_Pin   GPIO_Pin_1
  #define SPI_MOSI_Pin   GPIO_Pin_2
  #define SPI_CS_Pin     GPIO_Pin_3
  #define SPI_GPIO       GPIO_A
  ```
 

### **4.1.2.Cấu hình GPIO**


* **Master**:
  
◦ Các chân **SCK, CS, MOSI** để chế độ Out Push Pull

```
GPIO_InitStructure.GPIO_Pin = SPI_SCK_Pin | SPI_CS_Pin | SPI_MOSI_Pin;

GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
```

◦ Chân **MISO** để chế độ IN FLOATING

```
GPIO_InitStructure.GPIO_Pin = SPI_MISO_Pin;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
```

* **Slave**:

◦ Chân **MISO** để chế độ Out Push Pull
```
GPIO_InitStructure.GPIO_Pin = SPI_MISO_Pin;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
```
◦ Chân **MOSI,SCK,CS** để chế độ IN FLOATING
```
GPIO_InitStructure.GPIO_Pin = SPI_SCK_Pin | SPI_CS_Pin | SPI_MOSI_Pin;

GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
```

### **4.1.3.Tạo xung Clock**

```
void Clock(){
  GPIO_WriteBit(SPI_GPIO,SPI_SCK_Pin,Bit_SET);
  Delay_ms(4);
  GPIO_WriteBit(SPI_GPIO,SPI_SCK_Pin,Bit_RESET);
  Delay_ms(4);
}
```

### **4.1.4.Khởi tạo các chân cho SPI**

```
void SPI_Init(){
  GPIO_WriteBit(SPI_GPIO,SPI_SCK_Pin,Bit_RESET);
  GPIO_WriteBit(SPI_GPIO,SPI_CS_Pin,Bit_SET);
  GPIO_WriteBit(SPI_GPIO,SPI_MISO_Pin,Bit_RESET);
  GPIO_WriteBit(SPI_GPIO,SPI_MOSI_Pin,Bit_RESET);
}

```

### **4.1.5.Hàm truyền(Master)**

* Kéo **CS** xuống **0**

    ◦ Dịch 1 bit

    ◦ Truyền 1 bit

    ◦ Gửi clock() 

* Kéo **CS** lên **1**

```
void SPI_Master_Transmit(uint8_t u8Data){

uint8_t u8Mask = 0x80;
uint8_t tempData;

GPIO_WriteBit(SPI_GPIO,SPI_CS_Pin,Bit_RESET);
Delay_ms(1);

  for (int i=0; i<8; i++){
  tempData = u8Data & u8Mask
      if(tempData){
        GPIO_WriteBit(SPI_GPIO,SPI_MOSI_Pin,Bit_SET);
        Delay_ms(1);
      }
      else{
        GPIO_WriteBit(SPI_GPIO,SPI_MOSI_Pin,Bit_RESET);
        Delay_ms(1);
      }

  u8Data = u8Data << 1;
  Clock();
 }

GPIO_WriteBit(SPI_GPIO,SPI_CS_Pin,Bit_SET);
Delay_ms(1);
}
```
### **4.1.6.Hàm nhận (Slave)**

* Kiểm tra **CS** bằng **0** 

   ◦ Kiểm tra Clock = 1;

   ◦ Đọc data trên MOSI,ghi vào biến;

   ◦ Dịch 1 bit

* Kiểm tra **CS** bằng **1**

```
uint8_t Slave_Receive(void){
  uint8_t dataReceive = 0x00;
  uint8_t temp = 0x00;

  while(GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin)); --Chờ CS lên cao
  while(!GPIO_ReadInputDataBit(SPI_GPIO,SPI_CS_Pin)); --Chờ CS xuống thấp bắt đầu Truyền
  while(!GPIO_ReadInputDataBit(SPI_GPIO,SPI_SCK_Pin)); --Chờ SCK xuống thấp( ở trạng thái nghỉ);
    
    for(int i=0; i<8; i++){
  
      while(!GPIO_ReadInputDataBit(SPI_GPIO,SPI_SCK_Pin)); --Chờ SCK xuống thấp
      while(GPIO_ReadInputDataBit(SPI_GPIO,SPI_SCK_Pin)); --Chờ SCK lên cao bắt đầu Truyền

      temp = GPIO_ReadInputDataBit(SPI_GPIO,SPI_MOSI_Pin);
      dataReceive = dataReceive << 1;
      dataReceive = dataReceive | temp;

      while(GPIO_ReadInputDataBit(SPI_GPIO,SPI_SCK_Pin)); --Chờ SCK xuống thấp (kết thúc khung truyền)

    }
  while(GPIO_ReadInputDataBit(SPI_GPIO,SPI_CS_Pin)); --Chờ CS lên cao kết thúc khung Truyền
  return dataReceive;
}

```

### **4.1.7.Hàm main**

```
uint8_t DataTrans[] = {1,3,4,5,6,7,8,9};
int main (){
RCC_Config();
GPIO_Config();
TIM_Config();
SPI_Config();
while(1){
   for(int i=0; i<7;i++){
     SPI_Master_Transmit(DataTrans);
     Delay_ms(1);
   }
 }
}
```


## **4.2.SPI Hardware**

### **4.2.1.Xác định chân GPIO**

![Image](https://github.com/user-attachments/assets/6c977197-08fb-4993-9e0b-ce7e83ba6d6d)

```
#define SPI1_NSS     GPIO_Pin_4
#define SPI1_SCK     GPIO_Pin_5
#define SPI1_MISO    GPIO_Pin_6
#define SPI1_MOSI    GPIO_Pin_7
#define SPI1_GPIO    GPIOA

```

### **4.2.2.Cấu hình chân GPIO**

* NSS: **Input,Output,AF**

* MISO,MOSI,SCK: **AF**

* Phần cứng SPI đã được thiết kế để tự động xử lý giao tiếp theo chuẩn SPI nên chế độ không phải điều chỉnh nhiều

### **4.2.3.Cấu hình SPI**

* **SPI_Mode**: Quy định chế độ hoạt động SPI
* **SPI_Direction**: Quy định kiểu truyền của thiết bị
* **SPI_BaudRatePrescaler**: Hệ số chia clock cấp cho module SPI
* **SPI_CPOL**: Cấu hình cực tính của SCK
  
    ◦ **SPI_CPOL_LOW**: cực tính mức **0** khi SCK **không** truyền xung

    ◦ **SPI_CPOL_HIGH**: cực tính mức **1** khi SCK truyền xung

* **SPI_CPHA**: Cấu hình chế độ hoạt động của SCK

    ◦ **SPI_CPHA_1Edge**: tín hiệu truyền đi ở cạnh xung đầu tiên
    
    ◦ **SPI_CPHA_2Edge**: tín hiệu truyền đi ở cạnh xung thứ hai

* **SPI_DataSize**: Cấu hình số bit truyền (8/16 bit)
* **SPI_FirstBit**: Cấu hình chiều truyền đi là MSB hay LSB
* **SPI_CRCPolynominal** : Cấu hình số bit checksum cho SPI
* **SPI_NSS**: cấu hình chân SS điều khiển bằng phần cứng hay phần mềm

```
void SPI_Config(){
	SPI_InitTypeDef SPI_InitStructure;

	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_Direction =    SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);
}

```

### **4.2.4.Các hàm thông dụng**

* `SPI_I2S_SendData(SPI_TypeDef* SPIx,uint16_t Data)` nhận 2 tham số là bộ SPI sử dụng và data cần truyền

* `SPI_I2S_ReceiveData(SPI_TypeDef* SPIx)` trả về giá trị đọc được trên SPI

* `SPI_I2S_GetFlagStatus(SPI_TypeDef* SPIx,uint16_t SPI_I2S_FLAG)` trả về giá trị 1 cờ trong thanh ghi của SPI

     ◦ **SPI_I2S_FLAG_TXE** : Cờ báo truyền,cờ này sẽ set lên 1 khi truyền xong data trong buffer

     ◦ **SPI_I2S_FLAG_RXNE** : Cờ báo nhận,cờ nãy sẽ set lên 1 khi nhận xong data

     ◦ **SPI_I2S_FLAG_BSY**: Cờ báo bận,cờ này sẽ set lên 1 khi đang bận truyền nhận

### **4.2.5.Hàm truyền (Master)**

```
void SPI_Send1Byte(uint8_t data){
  GPIO_ResetBits(SPI1_GPIO,SPI_NSS);
  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET){}

  SPI_I2S_SendData(SPI1,data);

  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY)==SET){}
  GPIO_SetBits(SPI1_GPIO,SPI1_NSS);

}


```

### **4.2.6.Hàm nhận(Slave)**

```
uint8_t SPI_Receive1Byte(void){
while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY)==SET);

uint8_t temp=(uint8_t)SPI_I2S_ReceiveData(SPI1);

while(SPI_I2S_GetFlagStatus(SPI_I2S_FLAG_RXNE)==RESET);
return temp;
}


}

```

### **4.2.7.Hàm main**

```
uint8_t dataSend[]={1,2,3,4,5,6,7};
int main(){
  GPIO_Config();
  TIM_Config();
  SPI_Config();
  while(1){
    for(int i=0;i<7;i++){
      SPI_Send1Byte(dataSend);
      Delay_ms(1);
    }
  }
}


```
</details>


<details>
	<summary><strong>BÀI 5: GIAO TIẾP I2C </strong></summary> 


## **Bài 5: GIAO TIẾP I2C**

## **5.1.I2C Software**

### **5.1.1.Xác định các chân GPIO**
* Định nghĩa 2 chân sử dụng I2C: **SCL,SDA**

  ```
  #define I2C_SCL    GPIO_Pin_6
  #define I2C_SDA    GPIO_Pin_7

  ```
 

### **5.1.2.Cấu hình GPIO**



```
void GPIO_Config(){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = I2C_SDA| I2C_SCL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
}

```
### **5.1.3.Cấu hình I2C**

```
#define WRITE_SDA_0 	GPIO_ResetBits(I2C_GPIO, I2C_SDA)		// Kéo chân SDA xuống 0
#define WRITE_SDA_1 	GPIO_SetBits(I2C_GPIO, I2C_SDA)			// Kéo chân SDA lên 1
#define WRITE_SCL_0 	GPIO_ResetBits(I2C_GPIO, I2C_SCL)		// Kéo chân SCL xuống 0
#define WRITE_SCL_1 	GPIO_SetBits(I2C_GPIO, I2C_SCL)			// Kéo chân SDA lên 1
#define READ_SDA_VAL 	GPIO_ReadInputDataBit(I2C_GPIO, I2C_SDA)	// Đọc chân SDA


```

* **Khởi tạo I2C**

```
// Ban đầu kéo 2 chân SDA và SCL lên mức 1
void I2C_Config()
{
	WRITE_SDA_1;
	delay_us(1);
	WRITE_SCL_1;
	delay_us(1);
}
```

* **Hàm Start**
```
// Điều kiện Start: Chân SDA xuống mức 0 trước chân SCL
void I2C_Start(){	
	WRITE_SCL_1;  	
	delay_us(3);	
	WRITE_SDA_1;
	delay_us(3);

	WRITE_SDA_0;	
	delay_us(3);
	WRITE_SCL_0;
	delay_us(3);
}
```

* **Hàm Stop**
```
// Điều kiện Stop: Chân SCL lên mức 1 trước chân SDA
void I2C_Stop()
{
	WRITE_SDA_0;
	delay_us(3);

	WRITE_SCL_1; 	
	delay_us(3);
	WRITE_SDA_1;
	delay_us(3);
}
```
### **5.1.4.Hàm truyền**

* Hàm truyền sẽ truyền lần lượt 8 bit trong byte dữ liệu
  
  ◦ Truyền 1 bit

  ◦ Tạo clock

  ◦ Dịch 1 bit

```
status I2C_Write(uint8_t u8Data){	
	uint8_t i;
	status stRet;
	for (int i = 0; i < 8; i++){
		// Ghi dữ liệu vào chân SDA		
		if (u8Data & 0x80) {
			WRITE_SDA_1;
		} else {
			WRITE_SDA_0;
		}
		delay_us(3);

		// Tạo một tín hiệu xung
		WRITE_SCL_1;
		delay_us(5);
		WRITE_SCL_0;
		delay_us(2);

		u8Data <<= 1; // Dịch 1 bit
	}
	WRITE_SDA_1;					
	delay_us(3);
	WRITE_SCL_1;					
	delay_us(3);
	
	if (READ_SDA_VAL) {	
		stRet = NOT_OK;				
	} else {
		stRet = OK;					
	}

	delay_us(2);
	WRITE_SCL_0;
	delay_us(5);
	
	return stRet;
}
```
### **5.1.5.Hàm nhận**


* Kéo SDA lên 1

   ◦ Đọc data trên SDA, ghi vào biến.

   ◦ Dịch 1 bit

* Gửi lại 1 tín hiệu ACK ở xung thứ 9.


```
uint8_t I2C_Read(ACK_Bit _ACK)
{	
	uint8_t i;						
	uint8_t u8Ret = 0x00;
	WRITE_SDA_1;
	delay_us(3);	
	for (i = 0; i < 8; ++i) {
		u8Ret <<= 1;
		WRITE_SCL_1;
		delay_us(3);

		// Đọc dữ liệu từ chân SDA và ghi vào biến
		if (READ_SDA_VAL) {
			u8Ret |= 0x01;
		}
		delay_us(2);
		WRITE_SCL_0;
		delay_us(5);
	}

	if (_ACK) {	
		WRITE_SDA_0;
	} else {
		WRITE_SDA_1;
	}
	delay_us(3);
	
	WRITE_SCL_1;
	delay_us(5);
	WRITE_SCL_0;
	delay_us(5);

	return u8Ret;
}

```



## **5.2.I2C Hardware**

### **5.2.1.Xác định chân GPIO**

![Image](https://github.com/user-attachments/assets/b80e6194-eab7-4f07-bb89-a4e85871ee0f)

```
#define I2C1_SCL     GPIO_Pin_6
#define I2C1_SDA     GPIO_Pin_7
#define I2C1_GPIO    GPIOB

```

### **5.2.2.Cấu hình chân GPIO**

```
void GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = I2C1_SCL | I2C1_SDA; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
```

### **5.2.3.Cấu hình I2C**

* **I2C_Mode**: Quy định chế độ hoạt động I2C
   
    ◦ **I2C_Mode_I2C**: Chế độ I2C FM(Fast Mode)

    ◦ **I2C_Mode_SMBusDevice & I2C_Mode_SMBusHost** : Chế độ I2C SM(Slow Mode)

* **I2C_ClockSpeed**: Cấu hình clock cho I2C
    
    ◦ **100**khz với chế độ SM

    ◦ **400**khz với chế độ FM
    
* **I2C_DutyCycle**: Cấu hình chu kỳ nhiệm vụ của xung

    ◦ **I2C_DutyCycle_2** : Thời gian xung thấp / xung cao = 2

    ◦ **I2C_DutyCycle_16_9** : Thời gian xung thấp / xung cao = 16 / 9

    ![Image](https://github.com/user-attachments/assets/f4e38372-5f4d-47a1-8c77-4e5aafde213d)


* **I2C_OwnAddress1**: Cấu hình địa chỉ của thiết bị đang cấu hình

* **I2C_Ack**: Cấu hình ACK, có sử dụng ACK hay không

* **I2C_AcknowledgedAddress**: Cấu hình số bit địa chỉ (7/10 bit)


```
void I2C_Config(){
	I2C_InitTypeDef I2C_InitStructure;

    I2C_InitStructure.I2C_ClockSpeed = 400000;    
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2c_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x33;
	I2C_InitStructure.I2C_Ack = I2C_Ack_ENABLE;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit ;
	
	I2C_Init(I2C1, &I2C_InitStructure);
	I2C_Cmd(I2C1, ENABLE);
}

```

### **5.2.4.Các hàm  thông dụng**

* `I2C_Send7bitAddress(I2C_TypeDef* I2Cx,uint8_t Address,uint8_t Direction)` gửi đi 7 bit Address để xác định Slave cần giao tiếp.Hướng truyền đi được xác định I2C_Direction để  thêm bit RW

* `I2C_SendData(I2C_TypeDef* I2Cx,uint8_t Data)` gửi đi 8 bit data

* `I2C_ReceiveData(I2C_TypeDef* I2Cx)` trả về 8 bit data

* `I2C_AcknowledgeConfig(I2C_TypeDef* I2Cx, FunctionalState NewState)` kích hoạt việc master có gửi ack hay không

* `I2C_CheckEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)`trả về kết quả kiểm tra I2C_EVENT tương ứng:

    ◦ **I2C_EVENT_MASTER_MODE_SELECT**: Đợi bus I2C về chế độ rảnh,xác nhận phần cứng hoàn thành tín hiệu START

       > Liên quan tới cờ I2C_FLAG_SB(Start Bit): Cờ báo rằng tín hiệu START đã được tạo thành công trên bus
       > Nếu SB = 0 (START chưa hoàn tất) → I2C_CheckEvent trả về FALSE → !FALSE = TRUE → vòng lặp tiếp tục.
       > Nếu SB = 1 (START đã hoàn tất) → I2C_CheckEvent trả về TRUE → !TRUE = FALSE → vòng lặp dừng.

    ◦ **I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED**: Đợi xác nhận của slave với yêu cầu transmit của Master

       > Liên quan tới cờ I2C_FLAG_ADDR và I2C_FLAG_TXE
       > Nếu ADDR = 0 hoặc TXE = 0 (Gửi địa chỉ chưa hoàn tất hoặc chưa nhận ACK):I2C_CheckEvent trả về FALSE,!FALSE = TRUE,Vòng lặp tiếp tục chạy
       > Nếu ADDR = 1 và TXE = 1 (Địa chỉ đã gửi thành công, Slave ACK, thanh ghi trống):I2C_CheckEvent trả về TRUE,!TRUE = FALSE,Vòng lặp dừng
       


    ◦ **I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED**: Đợi xác nhận của slave với yêu cầu receive của Master

    ◦ **I2C_EVENT_MASTER_BYTE_TRANSMITTED**: Đợi truyền xong 1 byte data từ Master

    ◦ **I2C_EVENT_MASTER_BYTE_RECEIVED**: Đợi Master nhận đủ 1 byte data 

### **5.2.5.Hàm truyền**

* Bắt đầu truyền nhận ,bộ I2C sẽ tạo 1 tín hiệu START,đợi tín hiệu báo bus sẵn sàng

* Gửi 7 bit địa chỉ để xác định slave,đợi slave xác nhận

* Gửi/đọc các byte data,đợi truyền xong

* Sau đó kết thúc bằng tín hiệu STOP
```
void I2C_Write(uint8_t address,uint8_t * data,uint8_t length){
  
  //Tạo tín hiệu START
  I2C_GenerateSTART(I2C1,ENABLE);
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));

  //Gửi địa chỉ slave(7 bit address + Write Bit)
  I2C_Send7bitAddress(I2C1,address<<1,I2C_Direction_Transmitter);
  while(!(I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))

  //Gửi dữ liệu
  for(uint8_t i=0; i< lenght; i++){
    I2C_SendData(I2C1,data[i]);
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  }

  //Tạo tín hiệu STOP
  I2C_GenerateSTOP(I2C1,ENABLE);
  
}


```

### **5.2.6.Hàm nhận**


```
void I2C_Read(uint8_t address,uint8_t * data,uint8_t length){

  
  I2C_GenerateSTART(I2C1,ENABLE);
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));

  I2C_Send7bitAddress(I2C1,address <<1, I2C_Direction_Receiver);
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

  for(uint8_t i=0; i<length;i++){
    if(i==length - 1){
      I2C_AcknowledgeConfig(I2C1,DISABLE); //Tắt ACK cho byte cuối
    }
    while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED));
    data[i]=I2C_ReceiveData(I2C1);
  }
   I2C_GenerateSTOP(I2C1, ENABLE);
   I2C_AcknowledgeConfig(I2C1, ENABLE);
}
```

### **5.2.7.Hàm main**

```
int main(void){

I2C_Config();

uint8_t writeData[]={0x01,0x02,0x03};
uint8_t readData[3];
// Ghi 3 byte vào slave có địa chỉ 0x50
    I2C_Write(0x50, writeData, 3);
    
    // Đọc 3 byte từ slave có địa chỉ 0x50
    I2C_Read(0x50, readData, 3);
    
    while(1) {
        // Chương trình chính
    }
}
```
</details>


<details>
	<summary><strong>BÀI 6: GIAO TIẾP UART </strong></summary> 


## Bài 6: GIAO TIẾP UART 

## **6.1.UART Software**

### **6.1.1.Xác định các chân GPIO**
* Định nghĩa 2 chân sử dụng UART: **TX,RX**

  ```
  #define TX_Pin        GPIO_Pin_0
  #define RX_Pin        GPIO_Pin_1
  #define UART_GPIO     GPIOA
  
  ```
 
* **Baudrate:**

◦ Baudrate = Số bit truyền được / 1s

◦ Tốc độ baudrate thường dùng là 9600, ứng với mỗi bit là 105us.

`#define BaudrateTime  105`

### **6.1.2.Cấu hình GPIO**

* **TX:** Cấu hình Output Push-Pull, tốc độ cao (50 MHz).

* **RX:** Cấu hình Input Floating để đọc tín hiệu từ thiết bị gửi.

* TX cần điều khiển mức logic (0/1), RX cần đọc trạng thái mà không kéo lên/xuống.
```
void GPIO_Config(){
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = TX_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UART_GPIO, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = RX_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UART_GPIO, &GPIO_InitStructure);

}

```
### **6.1.3.Cấu hình UART**


* **Khởi tạo UART**

```
//Truyền dữ liệu nên kéo chân TX lên 1,vì mục đích giao tiếp 1 chiều nên không cần chân RX

void UART_Config()
{
	GPIO_SetBits(UART_GPIO,TX_Pin);
	Delay_us(100);
}
```


### **6.1.4.Hàm truyền**

* Hàm truyền sẽ truyền lần lượt 8 bit trong byte dữ liệu,sau khi tín hiệu start được gửi đi

* Tạo start,delay 1 period time

  ◦ Truyền 1 bit dữ liệu, mỗi bit truyền trong 1 period time

  ◦ Dịch 1 bit

* Tạo stop,delay tương ứng với số bit stop

![Image](https://github.com/user-attachments/assets/b96946d0-8eba-4382-b469-97e1eac2c14a)

```
	void UART_Transmitt(unsigned char c) {
	
	//Start Bit
	GPIO_ResetBits(UART_GPIO,TX_Pin);
	Delay_us(BaudrateTime);

	//Truyền các bit dữ liệu (LSB->MSB)
          for(int i=0;i<8;i++){
		if(c & (1 << i)){
                   GPIO_SetBits(UART_GPIO,TX_Pin);
		}else{
		   GPIO_ResetBits(UART_GPIO,TX_Pin);	
		}
		Delay_us(BaudrateTime);
	   }

	//Stop Bit
	GPIO_SetBits(UART_GPIO,TX_Pin);
        Delay_us(BaudrateTime);

}
```
### **6.1.5.Hàm nhận**


* Chờ tín hiệu START từ thiết bị gửi

* Delay 1,5 period time

   ◦ Đọc data trên RX,ghi vào biến

   ◦ Dịch 1 bit

   ◦ Delay 1 period time

* Delay 1 period time và đợi stop bit

![Image](https://github.com/user-attachments/assets/cce64408-ea97-4726-936f-0a3be009b5e8)


```
unsigned char stop_bit_error = 0;

unsigned char UART_Receive(){
  
  unsigned char c = 0;

  //Đợi START Bit
  while(GPIO_ReadInputDataBit(UART_GPIO,RX_Pin) == 1);

  //Chờ 1 nửa chu kỳ bit để lấy mẫu ở giữa bit
  Delay_us(BaudrateTime + BaudrateTime / 2);

  //Đọc các bit dữ liệu (LSB->MSB)
  for(int i=0;i<8;i++){
	  if(GPIO_ReadInputDataBit(UART_GPIO,RX_Pin)){
		  c |= (1 << i);
	  }
	  Delay_us(BaudrateTime);
  }
  Delay_us(BaudrateTime/2);

  stop_bit_error = 0;
  //Đợi STOP Bit
  if(GPIO_ReadInputDataBit(UART_GPIO,RX_Pin) != 1){
	   stop_bit_error = 1; 
  }

  return c;
}	
	
```

### **6.1.6.Parity**

* Bit chẵn/lẻ được thêm vào cuối Data

* Tùy vào cấu hình parity là chẵn hay lẻ mà thiết bị truyền có thể thêm bit parity là 0 hoặc 1.

* Phía nhận cấu hình parity giống như phía truyền, sau khi nhận đủ các bit sẽ kiểm tra parity có đúng hay không.

```
typedef enum{
	Parity_Mode_NONE,
	Parity_Mode_ODD,
	Parity_Mode_EVEN
}Parity_Mode;

```



## **6.2.UART Hardware**

### **6.2.1.Xác định chân GPIO**

![Image](https://github.com/user-attachments/assets/399d3f04-2761-45cc-ba06-308a1a452d8d)

```
#define TX_Pin        GPIO_Pin_9
#define RX_Pin        GPIO_Pin_10
#define USART1_GPIO   GPIOA

```

### **6.2.2.Cấu hình chân GPIO**

* **TX:** Alternate Function Push-Pull (AF_PP) để kết nối với UART phần cứng

* **RX:** Input Floating để nhận tín hiệu

```
void GPIO_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = TX_Pin; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART1_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = RX_Pin; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART1_GPIO, &GPIO_InitStructure);
}
```

### **6.2.3.Cấu hình UART**

* **USART_Mode**: Quy định chế độ hoạt động UART
   
    ◦ **USART_Mode_Tx**: Cấu hình truyền

    ◦ **USART_Mode_Rx**: Cấu hình nhận

    ◦ Cấu hình cả 2 cùng lúc (song công)

* **USART_BaudRate**: Cấu hình tốc độ baudrate cho UART
    
* **USART_HardwareFlowControl**: Cấu hình chế độ bắt tay cho UART

* **USART_WordLength**: Cấu hình số bit mỗi lần truyền

* **USART_StopBits**: Cấu hình số lượng stopbits

* **USART_Parity**: Cấu hình bit kiểm tra chẵn ,lẻ


```
void USART_Config(){
	USART_InitTypeDef USART_InitStructure;

    	USART_InitStructure.USART_BaudRate = 9600;    
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = 	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}

```



### **6.2.4.Các hàm thông dụng**

*  `USART_SendData(USART_TypeDef * USARTx,uint16_t Data)` truyền data từ UARTx.Data này đã được thêm bit chẵn/lẻ tùy cấu hình.

* `USART_ReceiveData(USART_TypeDef* USARTx)` nhận data từ UARTx

* Hàm `USART_GetFlagStatus(USART_TypeDef* USARTx,uint16_t USART_FLAG)` trả về trạng thái cờ USART_FLAG tương ứng

  ◦ **USART_FLAG_TXE** : Cờ báo thanh ghi chứa dữ liệu truyền đi (DR) **đang trống**
 
  ◦ **USART_FLAG_RXNE** : Cờ báo thanh ghi chứa dữ liệu nhận (DR) **đã có** dữ liệu 

  ◦ **USART_FLAG_IDLE** : Cờ báo đường truyền đang ở chế độ rảnh

  ◦ **USART_FLAG_PE**   : Cờ báo lỗi Parity

  ◦ **USART_FLAG_TC**   : Cờ báo đã hoàn thành quá trình truyền dữ liệu
    
### **6.2.5.Hàm truyền**

*   **Truyền byte**
  
    ◦ 1. Chờ cờ TXE

    ◦ 2. Gửi byte qua USART_SendData.

    ◦ 3. Chờ cờ TC (truyền hoàn tất).

```
void USART_TransmitByte(uint8_t byte){

// Chờ cho đến khi thanh ghi truyền dữ liệu trống (TXE)
while(USART_GetFlagStatus(USART_FLAG_TXE) == RESET);

// Truyền byte
USART_SendData(USART1,byte);

// Chờ cho đến khi việc truyền hoàn tất (TC)
while(USART_GetFlagStatus(USART_FLAG_TC) == RESET);
}

```

*   **Truyền chuỗi**
  
    ◦ Lặp qua từng ký tự, gọi hàm truyền byte.

```
void USART_TransmitString(const char *str){

  //Kiểm tra ký tự hiện tại (*str) có phải là ký tự kết thúc chuỗi (\0) hay không.
  while(*str != '\0'){

  // Gọi hàm truyền byte để truyền ký tự hiện tại
  USART_TransmitByte(*str)
  str++;
 }
}
```

### **6.2.6.Hàm nhận**

*   **Nhận byte**
 
    ◦ 1. Chờ cờ RXNE

    ◦ 2. Đọc dữ liệu từ USART_ReceiveData.

```
uint8_t USART_ReceiveByte(void){

uint8_t temp = 0x00;

// Chờ cho đến khi có dữ liệu trong thanh ghi nhận (RXNE)
while(USART_GetFlagStatus(USART_FLAG_RXNE)==RESET);

// Đọc dữ liệu nhận được
temp = USART_ReceiveData(USART1);
return temp;
}
```

### **6.2.7.Hàm main**

```
int main(void){

USART_Config();

const char *name = "KMAxIgnite!\r\n";
USART_TransmitString(name);

while(1){

	// Nhận một ký tự
        uint8_t received_char = USART_ReceiveByte();
    
	// Gửi lại ký tự vừa nhận (echo)
        USART_TransmitByte(received_char);

    USART_TransmitByte('\r');
    USART_TransmitByte('\n');
    
}

```

### **6.2.8. Các lỗi UART**

* **Parity Error (PE):** Parity nhận không khớp với cấu hình.
  
* **Framing Error (FE):** Stop bit không đúng (mức 0 thay vì 1).
  
* **Overrun Error (ORE):** Dữ liệu mới đến trước khi đọc dữ liệu cũ.
  
* **Noise Error (NE):** Nhiễu trên đường truyền.
  
* **Xử lý:** Kiểm tra cờ lỗi (PE, FE, ORE, NE) trong trình xử lý ngắt, gửi thông báo hoặc bỏ dữ liệu lỗi.

### **6.2.9.Hardware Flow Control (RTS/CTS)**

* **Mục đích:** Điều khiển luồng dữ liệu để tránh tràn bộ đệm.

* **Cấu hình:** Bật RTS/CTS qua USART_HardwareFlowControl_RTS_CTS.

* **Chân GPIO:** PA12 (RTS), PA11 (CTS) cho UART1.

*  **Hoạt động:**
  
    ◦ RTS: Thiết bị báo sẵn sàng nhận.

    ◦ CTS: Thiết bị đối tác báo sẵn sàng gửi.
</details>

<details>
	<summary><strong>BÀI 7: CÁC LOẠI NGẮT </strong></summary> 

## BÀI 7: CÁC LOẠI NGẮT

### **1. Ngắt ngoài (EXTI)**


* **Sơ đồ**

![Image](https://github.com/user-attachments/assets/b8531dc8-d8a1-4fea-b10b-90365810da53)


#### **1.1. Tổng quan**

* **Số lượng line ngắt:**

  ◦ STM32F103 hỗ trợ 16 EXTI line (Line 0 đến Line 15), tương ứng với các chân GPIO cùng số thứ tự (ví dụ: PA0, PB0, PC0 cho Line 0).

* **Kết nối với GPIO:** 

  ◦ Mỗi EXTI line có thể được liên kết với các chân GPIO có cùng số thứ tự

  ◦ Tuy nhiên, chỉ một chân duy nhất trong số các chân cùng thứ tự được chọn làm nguồn ngắt cho một EXTI line.Việc chọn được thực hiện qua thanh ghi AFIO_EXTICRx.

  
		Ví dụ: Nếu PB0 được chọn cho EXTI Line 0, các chân như PA0, PC0,... không thể được sử dụng đồng thời cho ngắt ngoài trên cùng Line 0.
  

* **Mục đích:**

  ◦ Ngắt ngoài được sử dụng để phát hiện các sự kiện từ bên ngoài (ví dụ: nhấn nút, tín hiệu từ cảm biến) và kích hoạt xử lý tức thời.

* **Thanh ghi chính:**

  ◦ **IMR (Interrupt Mask Register):** Bật/tắt ngắt cho mỗi line (bit 0-15: 1 = bật, 0 = tắt). Nếu bật, ngắt sẽ gọi ISR qua NVIC.
  
  ◦ **EMR (Event Mask Register):** Bật/tắt sự kiện (bit tương tự IMR). Sự kiện không gọi ISR mà chỉ kích hoạt các module khác (như DMA hoặc Wake-up từ Low Power mode).
  
  ◦ **RTSR (Rising Trigger Selection Register):** Chọn cạnh lên cho mỗi line (1 = bật rising trigger).

  ◦ **FTSR (Falling Trigger Selection Register):** Chọn cạnh xuống cho mỗi line (1 = bật falling trigger). Có thể bật cả hai để phát hiện thay đổi (change trigger).

  ◦ **PR (Pending Register):** Lưu trạng thái ngắt chờ xử lý (bit 1 = ngắt đang chờ). Phải xóa bit này trong ISR để tránh ngắt lặp.

#### **1.2.Cấu hình ngắt ngoài**

* **1.2.1. Bật clock**

  ◦ **Bật clock cho GPIO port chứa chân ngắt và AFIO (để cấu hình liên kết EXTI).**

  ◦ **Hàm:** `RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)`
    
    **Tham số:**

            RCC_APB2Periph: Chọn peripheral (ví dụ: RCC_APB2Periph_GPIOA cho port A, RCC_APB2Periph_AFIO cho AFIO).

            NewState: ENABLE hoặc DISABLE.
    
  ◦ VD:

		  void RCC_Config(){
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
		}
        
* **1.2.2. Cấu hình chân GPIO**

  ◦ Chân GPIO phải ở chế độ Input để nhận tín hiệu ngoài.

  ◦ Sử dụng pull-up/pull-down để tránh trạng thái floating (nhiễu).

  ◦ **Struct:** `GPIO_InitTypeDef`


        GPIO_Pin: Chọn pin (ví dụ: GPIO_Pin_0 cho pin 0).
        GPIO_Mode: Chế độ (ví dụ: GPIO_Mode_IPU cho Input Pull-Up, GPIO_Mode_IPD cho Input Pull-Down, GPIO_Mode_IN_FLOATING cho Input không pull).
        GPIO_Speed: Không cần thiết cho Input (có thể bỏ qua hoặc đặt GPIO_Speed_50MHz).

  ◦ **Hàm:** `GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)`

  ◦ VD:

        GPIO_InitTypeDef GPIOInitStructure;
        GPIOInitStructure.GPIO_Pin = GPIO_Pin_0;          // Chọn PB0
        GPIOInitStructure.GPIO_Mode = GPIO_Mode_IPU;      // Input với pull-up (tránh nhiễu khi không kết nối)
        GPIO_Init(GPIOB, &GPIOInitStructure);  

* **1.2.3. Liên kết chân GPIO với EXTI Line**

  ◦ **Hàm:** `GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)`
  
  ◦ **Tham số:**

    
        GPIO_PortSource: Port nguồn (ví dụ: GPIO_PortSourceGPIOB cho port B).
        GPIO_PinSource: Số thứ tự pin (ví dụ: GPIO_PinSource0 cho pin 0).
    
  ◦ VD:

        GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0); // Liên kết PB0 với EXTI Line 0.

* **1.2.4. Cấu hình EXTI**

  ◦ **Struct:** `EXTI_InitTypeDef`

        EXTI_Line: Line cụ thể (ví dụ: EXTI_Line0 đến EXTI_Line15).
        EXTI_Mode: EXTI_Mode_Interrupt (gọi ISR) hoặc EXTI_Mode_Event (không gọi ISR).
        EXTI_Trigger: EXTI_Trigger_Rising (cạnh lên), EXTI_Trigger_Falling (cạnh xuống), EXTI_Trigger_Rising_Falling (cả hai).
        EXTI_LineCmd: ENABLE hoặc DISABLE.

  ◦ **Hàm:** `EXTI_Init(EXTI_InitTypeDef* EXTI_InitStruct)`

        EXTI_InitTypeDef EXTI_InitStructure;
        EXTI_InitStructure.EXTI_Line = EXTI_Line0;
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  // Phát hiện cạnh xuống (nút bấm nhấn)
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure);


### **2. Ngắt Timer**

* **Sơ đồ**


![Image](https://github.com/user-attachments/assets/b0736d5c-3a49-41bf-95db-063762fdb254)


#### **2.1. Tổng quan**

* **Timer** trên STM32F103 được sử dụng để tạo các sự kiện định thời, chẳng hạn như tạo ngắt định kỳ, đo thời gian, hoặc điều khiển PWM.

* **Ngắt Timer:** Khi Timer đếm đến một giá trị xác định (gọi là Period), nó có thể kích hoạt ngắt để thực hiện các tác vụ được lập trình trong hàm xử lý ngắt.

* **Ngắt Timer thường được dùng để:** 

  ◦ Tạo độ trễ chính xác (thay thế cho các hàm delay thô sơ).

  ◦ Đếm thời gian hoặc số lần xảy ra sự kiện.

  ◦ Điều khiển các tác vụ định kỳ (ví dụ: gửi dữ liệu qua UART mỗi 1 giây).


#### **2.2. Cấu hình**

**Cấu hình Timer bao gồm bật clock, init timebase, bật ngắt, cấu hình NVIC, và viết ISR.**

* **2.2.1. Bật clock:** `RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); (TIM2-5 trên APB1, TIM1 trên APB2).`

* **2.2.2. Cấu hình TimeBase:**

    * **Struct:** `TIM_TimeBaseInitTypeDef`

        ◦ **TIM_Prescaler (uint16_t):** Chia tần số clock để giảm tốc độ đếm. Giá trị từ 0-65535.

            Công thức: Tick frequency = Timer clock / (TIM_Prescaler + 1).

            Clock hệ thống thường 72MHz (HSI/PLL). Prescaler làm chậm clock để dễ tính toán thời gian. Ví dụ: Prescaler = 71 → Tick freq = 72MHz / 72 = 1MHz (1 tick = 1μs).
            Lưu ý: Prescaler +1 vì đếm từ 0.


        ◦ **TIM_Period (uint16_t):** Giá trị counter đạt để reset và tạo ngắt (Update Event). Từ 0-65535 (cho 16-bit Timer).

            Công thức thời gian ngắt: Interval = (TIM_Period + 1) / Tick frequency.
            
            Counter đếm từ 0 đến Period, rồi reset. Ví dụ: Period = 999 với tick 1kHz → Interval = 1000 / 1000 = 1s.

            Lưu ý: Period +1 vì đếm bao gồm 0.

        ◦ **TIM_ClockDivision (uint16_t):** Chia clock thêm trước prescaler.

            TIM_CKD_DIV1 (không chia, mặc định)
            
            TIM_CKD_DIV2 (chia 2)

            TIM_CKD_DIV4 (chia 4)

        ◦ **TIM_CounterMode (uint16_t):** Chế độ đếm

            TIM_CounterMode_Up: Đếm lên từ 0 đến Period.
            
            TIM_CounterMode_Down: Đếm xuống từ Period đến 0.

            TIM_CounterMode_CenterAligned1/2/3: Cho PWM, ít dùng cho ngắt cơ bản.

    * **Hàm:** `TIM_TimeBaseInit(TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct)`

        ◦ **VD:** ngắt mỗi 1ms với clock 72MHz

            TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

            // Tick freq = 72MHz / (7199 + 1) = 10kHz (1 tick = 0.1ms)
            // Interval = (9 + 1) / 10kHz = 10 / 10000 = 1ms
            TIM_TimeBaseInitStruct.TIM_Prescaler = 7199;  // Chia clock thành 10kHz
            TIM_TimeBaseInitStruct.TIM_Period = 9;        // Đếm 10 ticks để ngắt
            TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;  // Không chia thêm
            TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  // Đếm lên

            TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);


* **2.2.3. Cấu hình TimeBase:**

    * **Kích hoạt ngắt:** `TIM_ITConfig(TIM_TypeDef* TIMx, uint16_t TIM_IT, FunctionalState NewState)`

        ◦ **Tham số:** 

            TIM_IT_Update cho Update Event, NewState = ENABLE.

        ◦ **VD:** 

            TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    * **Bật Timer:** `TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState)`

        ◦ **VD:** 

            TIM_Cmd(TIM2, ENABLE);

    * **Cấu hình NVIC:** 

        ◦ **Struct:** `NVIC_InitTypeDef`

            NVIC_IRQChannel: Kênh IRQ (ví dụ: TIM2_IRQn cho TIM2).
            NVIC_IRQChannelPreemptionPriority: Độ ưu tiên preempt (0-15, thấp hơn = ưu tiên cao hơn).
            NVIC_IRQChannelSubPriority: Độ ưu tiên sub (0-15).
            NVIC_IRQChannelCmd: ENABLE.

        ◦ **Hàm:** `NVIC_Init(NVIC_InitTypeDef* NVIC_InitStruct)`

            NVIC_InitTypeDef NVIC_InitStruct;
            NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
            NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;  // Ưu tiên cao
            NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStruct);

    * **Hàm xử lý ngắt Timer:** 

        ◦ **Tên cố định:** `TIMx_IRQHandler(void)` `(ví dụ: TIM2_IRQHandler).`

        ◦ **Quy trình:** 

            Kiểm tra cờ: TIM_GetITStatus(TIM_TypeDef* TIMx, uint16_t TIM_IT) → Trả về SET nếu ngắt từ Update.
      
            Thực thi tác vụ (ví dụ: tăng biến đếm).
      
            Xóa cờ: TIM_ClearITPendingBit(TIM_TypeDef* TIMx, uint16_t TIM_IT)

        ◦ **VD:** 

            volatile uint16_t count = 0;

            void TIM2_IRQHandler(void) {
                if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
                    count++;  // Tăng mỗi 1ms
                    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  // Phải xóa để tránh lặp ngắt
                }
            }

        ◦ **Lưu ý:**  Không xóa cờ → Ngắt lặp vô tận. Giữ ISR ngắn để tránh stack overflow.

    * **Hàm delay sử dụng Timer:** 


            void delay_ms(uint16_t time) {
                count = 0;  // Reset
                while (count < time);  // Chờ count tăng bởi ISR
            }

### **3. Ngắt truyền thông**

#### **3.1. Tổng quan**

![Image](https://github.com/user-attachments/assets/9affbbd5-a18d-40d2-9c22-588a47d195df)

#### **3.2. Cấu hình**

* **Kích hoạt ngắt UART:** Sau khi cấu hình UART, dùng `USART_ITConfig(USART_TypeDef* USARTx, uint16_t USART_IT, FunctionalState NewState)`

    * **Tham số:** 

        ◦ **USART_IT** 

            USART_IT_RXNE (RX not empty)

            USART_IT_TXE (TX empty)

            USART_IT_TC (Transmission Complete)

        ◦ **NewState** 

            ENABLE
            
* **Cấu hình NVIC:** Tương tự Timer, nhưng IRQ channel là `USART1_IRQn (cho USART1)`

    * **VD:** 

            NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;

#### **3.3. Hàm xử lý ngắt UART**

* **Tên:** `USARTx_IRQHandler(void)` (ví dụ: `USART1_IRQHandler`).

* **Quy trình:** 

    * **1. Kiểm tra ngắt:** 

        ◦ `USART_GetITStatus(USART_TypeDef* USARTx, uint16_t USART_IT)` → SET nếu có.

    * **2. Xử lý: Nhận dữ liệu** 

        ◦ `USART_ReceiveData(USART_TypeDef* USARTx)` (trả về uint16_t).

    * **3. Kiểm tra flag** 

        ◦ `USART_GetFlagStatus(USART_TypeDef* USARTx, uint16_t USART_FLAG)`

            USART_FLAG_RXNE (RX ready)

            USART_FLAG_TXE (TX ready)

            USART_FLAG_TC (Transmission Complete)


    * **4. Gửi dữ liệu** 

        ◦ `USART_SendData(USART_TypeDef* USARTx, uint16_t Data)`

    * **5. Xóa cờ** 

        ◦ `USART_ClearITPendingBit(USART_TypeDef* USARTx, uint16_t USART_IT)`

            void USART1_IRQHandler(void) {
                uint8_t data = 0x00;
                if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {  // Kiểm tra ngắt nhận
                    data = USART_ReceiveData(USART1);  // Nhận data
                    if (USART_GetFlagStatus(USART1, USART_FLAG_TXE) != RESET) {  // Kiểm tra TX ready
                        USART_SendData(USART1, data);  // Gửi lại
                        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);  // Chờ hoàn tất gửi
                    }
                    USART_ClearITPendingBit(USART1, USART_IT_RXNE);  // Xóa cờ RX
                }
            }            

</details>



<details>
	<summary><strong>BÀI 8: ADC </strong></summary> 


## Bài 8: ADC

## **8.1.Giới thiệu về ADC**

### **8.1.1.Định nghĩa**

* ADC (Analog-to-Digital Converter) là 1 mạch điện tử lấy điện áp tương tự làm đầu vào và chuyển đổi nó thành dữ liệu số (1 giá trị đại diện cho mức điện áp trong mã nhị phân).

![Image](https://github.com/user-attachments/assets/bb2f4a55-28b8-4af8-bd70-7f111aead2ec)
 


### **8.1.2.Độ phân giải**

* Số bit mà ADC dùng để xác định số lượng mức sẽ chia từ phạm vi điện áp tương tự

* Với STM32F103C8, ADC có độ phân giải 12 bit, tức là tín hiệu analog được chia thành 2^12 = 4096 mức

* Độ phân giải càng cao => càng nhiều mức sẽ cho được kết quả càng chính xác
```
VD: Với điện áp tham chiếu 3.3V, độ phân giải 12bit cho độ chính xác mỗi mức là 33/4096 = 0.805 mV
```


![Image](https://github.com/user-attachments/assets/893e3052-892a-42b7-bcc0-6fafbabe90ea)


### **8.1.3.Tần số lấy mẫu**


* Quy định tần suất mà tín hiệu tương tự được lấy mẫu

* Tần số lấy mẫu càng cao => lấy được nhiều mẫu => kết quả càng chính xác

* **Trong STM32:** Tần số lấy mẫu được xác định bởi thời gian lấy mẫu (`ADC_SampleTime`) và tần số clock của ADC

### **8.1.4. Các thông số khác**

* **Điện áp tham chiếu:** Phạm vi điện áp analog mà ADC có thể đo (thường là 0 đến 3.3V/5V trên STM32)

* **Thời gian chuyển đổi:** Thời gian cần để ADC hoàn thành một lần chuyển đổi, phụ thuộc vào tần số clock và thời gian lấy mẫu.

## **8.2.Sử dụng ADC trong STM32**

### **8.2.1.Đặc điểm**

* STM32F103C8 có 2 bộ ADC.Kết quả chuyển đổi được lưu trữ trong thanh ghi 16 bit

   ◦ **Độ phân giải:** 12 bit(4096 mức)

   ◦ **Thanh ghi dữ liệu:** Giá trị chuyển đổi được lưu trong thanh ghi 16 bit (căn lề trái hoặc phải).

   ◦ **Ngắt (Interrupt):** Hỗ trợ ngắt khi hoàn thành chuyển đổi (EOC), lỗi, hoặc các sự kiện khác.

   ◦ **Trigger ngoài:** Có thể kích hoạt chuyển đổi bằng tín hiệu từ timer, GPIO, hoặc nguồn khác.

   ◦ **DMA (Direct Memory Access):** Hỗ trợ truyền dữ liệu trực tiếp từ ADC đến bộ nhớ, giảm tải cho CPU.

   ◦ **Thời gian chuyển đổi nhanh:** Tùy thuộc vào cấu hình, thường từ vài chu kỳ clock.

### **8.2.2.Các chế độ của ADC**

  #### **Regular Conversion(Chuyển đổi tuần tự)**

  * **Single**: ADC chỉ đọc 1 kênh duy nhất và chỉ đọc khi nào được yêu cầu

  * **Single Continuous**: Chuyển đổi liên tục trên một kênh, tự động lặp lại sau mỗi lần hoàn thành (thường kết hợp với DMA để lưu dữ liệu vào bộ nhớ).

  * **Scan:Multi-Channels:** Quét qua và đọc dữ liệu nhiều kênh,nhưng chỉ đọc khi nào được yêu cầu

  * **Scan: Continuous Multi-Channel Repeat:** Quét qua và đọc dữ liệu nhiều kênh,nhưng đọc liên tiếp nhiều lần giống như Single Continous. 


  #### **Injected Conversion(Chuyển đổi ưu tiên)**
   
   * Trong trường hợp nhiều kênh hoạt động khi kênh có  mức độ ưu tiên cao hơn có thể tạo ra một **Injected Trigger**. 

   * Khi gặp **Injected Trigger** thì ngay lập tức kênh đang hoạt động bị ngưng lại để kênh được ưu tiên kia có thể hoạt động.


  
## **8.3.Cấu hình ADC**

### **8.3.1. Cấu hình Clock cho ADC**

####  **Cấu hình GPIO cho ADC**


* **Các bộ ADC được cấp xung từ RCC APB2**
```
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
```



![Image](https://github.com/user-attachments/assets/f59f57ea-5a7a-4fb7-811b-3ea2222a156e)


```
void GPIO_Config(){
	GPIO_InitTypeDef GPIO_InitStructure;  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

```
#### **8.3.2. Cấu hình ADC**

```
void ADC_Config(void) {
    ADC_InitTypeDef ADC_InitStruct;
    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent; // Chế độ độc lập
    ADC_InitStruct.ADC_NbrOfChannel = 1; // Số kênh (1 kênh cho PA0)
    ADC_InitStruct.ADC_ScanConvMode = DISABLE; // Không quét nhiều kênh
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE; // Chuyển đổi liên tục
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // Không dùng trigger ngoài
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right; // Căn phải
    ADC_Init(ADC1, &ADC_InitStruct);

    // Cấu hình kênh ADC
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);

    // Bật ADC
    ADC_Cmd(ADC1, ENABLE);

    // Hiệu chỉnh ADC
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1));
}
```

* **Các thông số:**

* **ADC_Mode:** 
    
    ◦ **Independent** cho ADC hoạt động độc lập, không đồng bộ với ADC khác.

    ◦ **DualMode** dùng khi ADC1 và ADC2 hoạt động đồng thời.

* **ADC_NbrOfChannel:** 

    ◦  Số kênh cần đọc (1 cho PA0).

* **ADC_ScanConvMode:** 

    ◦  Cấu hình chế độ quét ADC lần lượt từng kênh

* **ADC_ContinuousConvMode:** 

    ◦ Cấu hình bộ ADC có chuyển đổi liên tục hay không

* **ADC_ExternalTrigConv:** 

    ◦ Sử dụng tín hiệu Trigger

* **ADC_DataAlign:** 

    ◦ Cấu hình căn lề cho data

* **ADC_RegularChannelConfig:** 

    ◦ Cấu hình kênh cụ thể (ADC_Channel_?), thứ tự (Rank = ?), và thời gian lấy mẫu (? chu kỳ để đảm bảo độ chính xác).

#### **8.3.4. Kích hoạt và đọc ADC**

* **Kích hoạt chuyển đổi:**
```
ADC_SoftwareStartConvCmd(ADC1, ENABLE);
```
* **Đọc giá trị:**
```
while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)); // Chờ chuyển đổi hoàn tất
uint16_t adc_value = ADC_GetConversionValue(ADC1); // Đọc giá trị
```

#### **8.3.5. Thời gian lấy mẫu**

* Thời gian lấy mẫu **(ADC_SampleTime)** quyết định thời gian ADC thu thập tín hiệu analog trước khi chuyển đổi:

    ◦ Các giá trị: 1.5, 7.5, 13.5, 28.5, 41.5, 55.5, 71.5, 239.5 chu kỳ.

    ◦ Ví dụ: ADC_SampleTime_55Cycles5 cho thời gian lấy mẫu dài, tăng độ chính xác cho tín hiệu thay đổi chậm (như biến trở).

* Công thức thời gian chuyển đổi:

    ◦ Tconv = Tsample + 12.5 chu kỳ

## **8.4.Các hàm  thông dụng**

* **1.Cấu hình kênh ADC:** 
```
 ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel,uint8_t Rank, uint8_t ADC_SampleTime)
```
   **ADCx:** ADC1 hoặc ADC2

   **ADC_Channel:** Kênh ADC (ví dụ: ADC_Channel_0 cho PA0)

   **Rank:** Thứ tự kênh trong chế độ quét (1 nếu chỉ dùng 1 kênh).

   **ADC_SampleTime:** Thời gian lấy mẫu (ví dụ: ADC_SampleTime_55Cycles5).

* **2.Bắt đầu chuyển đổi:**   
```
ADC_SoftwareStartConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
```

* **3.Đọc giá trị ADC:**

```
uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx);
```
* **4.Đọc giá trị ở chế độ kép:**
```
uint32_t ADC_GetDualModeConversionValue(void);
```
* **5.Kiểm tra trạng thái:**
```
FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, uint8_t ADC_FLAG);
```
## **8.5.Bộ lọc Kalman**

### **8.5.1.Định nghĩa**

* Bộ lọc Kalman được sử dụng để ước lượng trạng thái của hệ thống từ các dữ liệu quan sát bị nhiễu

### **8.5.2.Đặc điểm**

* **Giai đoạn dự đoán(Prediction):** 

    ◦ Dự đoán trạng thái tiếp theo dựa trên mô hình hệ thống và trạng thái trước đó.

    ◦ Tính toán độ không chắc chắn (covariance) của dự đoán.

![Image](https://github.com/user-attachments/assets/f638e348-1525-4589-8793-7e51285f67b4)

* **Giai đoạn cập nhật(Correction):**

    ◦ Kết hợp dữ liệu đo mới (có thể chứa nhiễu) với dự đoán để cập nhật trạng thái.

    ◦  Tính toán Kalman Gain để cân bằng giữa dự đoán và đo lường thực tế.

![Image](https://github.com/user-attachments/assets/22643b2a-9310-4376-a93e-0ae6e8b2b3ee)

* Hàm thiết lập thông số ban đầu R,P,Q

```
void SimpleKalmanFilter(float mea_e, float est_e, float q){
	_err_measure = mea_e;
	_err_estimate = est_e;
	_q = q;
}
```

* Hàm tính giá trị qua bộ lọc Kalman

```
float updateEstimate(float mea){

	_kalman_gain = _err_estimate / (_err_estimate + _err_measure);
	_current_estimate = _last_estimate + _kalman_gain * (mea - _last_estimate);
	_err_estimate =  (1.0 - _kalman_gain)*_err_estimate + fabs(_last_estimate-_current_estimate)*_q;
    _last_estimate=_current_estimate;

 return _current_estimate;
}
```

* Tính toán giá trị đo được từ ADC

```
SimpleKalmanFilter(1, 2, 0.001);

while(1){
	val = ADC_GetConversionValue(ADC1);
	val_Update = (float)updateEstimate(float(val));
	Delay_ms(100);
}
```
</details>



<details>
	<summary><strong>BÀI 9: DMA và PWM </strong></summary> 

## Bài 9: DMA và PWM

## **9.1.Hoạt động của MCU**

![Image](https://github.com/user-attachments/assets/248243ef-2eac-4d56-b746-2ffa3fb54799)

* **(1) CPU** lấy lệnh chương trình từ FLASH để xử lý

* **(2) CPU** đọc/ghi những dữ liệu từ/vào các ngoại vi thông qua đường bus ngoại vi

* **(3) Lưu** các dữ liệu vừa đọc được vào RAM 

* **(4) Giao tiếp** với RAM (đọc/ghi các dữ liệu) thông qua đường bus bộ nhớ

**=> CPU bị chiếm dụng quá nhiều, giảm hiệu suất chương trình** 
 


## **9.2.Giới thiệu về DMA**

### **9.2.1.Định nghĩa**

**DMA (Direct Memory Access)** là 1 cơ chế cho phép các thiết bị ngoại vi truyền dữ liệu trực tiếp đến bộ nhớ mà không cần CPU và thực hiện từng bước truyền dữ liệu

![Image](https://github.com/user-attachments/assets/2cf6eb88-05b1-4fe0-b95d-8fb6d5ce901a)

* **CPU** cấu hình và kích hoạt **DMA** hoạt động

* Ngoại vi sẽ sử dụng **DMA Request** để yêu cầu **DMA** gửi/nhận dữ liệu ngoại vi

* **DMA** tiến hành thực hiện yêu cầu từ **DMA Request**

* Lấy dữ liệu từ **SRAM** thông qua bus matrix ,đi qua các đường bus ngoại vi rồi truy cập các thanh ghi của ngoại vi

* Khi **kết thúc**, **DMA** kích hoạt ngắt báo cho **CPU** biết là quá trình hoàn tất

### **9.2.2.DMA trong STM32**

* STM32F1 có 2 bộ DMA, **DMA1** bao gồm **7** kênh, **DMA2** bao gồm **5** kênh


   ◦ Có 2 chế độ hoạt động

   ◦ Mỗi kênh có thể cấu hình riêng

   ◦ Mỗi kênh có thể phục vụ **nhiều ngoại vi** khác nhau nhưng **không đồng thời**

   ◦ Có mức ưu tiên để lập trình cho các kênh

   ◦ Có thể sử dụng DMA với 5 cờ báo ngắt **(DMA Half Transfer, DMA Transfer complete, DMA Transfer Error, DMA FIFO Error, Direct Mode Error)** 

   ![Image](https://github.com/user-attachments/assets/1fd23a47-dda9-4bc1-b4d5-66752f038651)


* **DMA** có 2 chế độ hoạt động alf **normal** và **circular**

  ◦ **Normal Mode:** 

    DMA truyền dữ liệu cho tới khi truyền đủ 1 lượng dữ liệu giới hạn đã khai báo DMA sẽ dừng hoạt động.

    Muốn nó tiếp tục phải khởi động lại

   ◦ **Circular Mode:**

   DMA truyền đủ dữ liệu giới hạn đã khai báo thì nó sẽ truyền tiếp về vị trí ban đầu, không dừng lại


### **9.2.3.Cấu hình DMA trong STM32**

* DMA được cấp xung từ AHB

```
void RCC_Config(void){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
}
```

* Các tham số cần được cấu hình

  ◦ **DMA_PeripheralBaseAddr:** Cấu hình địa chỉ ngoại vi cho DMA.Đây là địa chỉ mà DMA sẽ lấy data hoặc truyền data tới ngoại vi


  ◦ **DMA_MemoryBaseAddr:** Cấu hình địa chỉ vùng nhớ cần ghi/đọc data


  ◦ **DMA_DIR:** Cấu hình hướng truyền DMA, từ ngoại vi tới vùng nhớ hay từ vùng nhớ tới ngoại vi

  ◦ **DMA_BufferSize:** Cấu hình kích cỡ buffer.Số liệu dữ liệu muốn gửi/nhận qua DMA

  ◦ **DMA_PeripheralInc:** Cấu hình địa chỉ ngoại vi có tăng sau tăng hay không

  ◦ **DMA_MemoryInc:** Cấu hình địa chỉ bộ nhớ có tăng hay không

  ◦ **DMA_PeripheralDataSize:** Cấu hình độ lớn data của ngoại vi

  ◦ **DMA_MemoryDataSize:** Cấu hình độ lớn data của bộ nhớ

  ◦ **DMA_Mode:** Cấu hình chế độ hoạt động

  ◦ **DMA_Priority:** Cấu hình độ ưu tiên cho kênh DMA

  ◦ **DMA_M2M:** Cấu hình sử dụng truyền từ bộ nhớ đếm bộ nhớ cho kênh DMA

  ```
  void DMA_Config(void){

	DMA_InitTypeDef DMA_InitStructure;

	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_BufferSize = 35;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)buffer;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI->DR;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium

	DMA_Init(DMA1_Channel2, &DMA_InitStructure);
	DMA_Cmd(DMA1,ENABLE);
	SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Rx, ENABLE);
  }
  ```

### **9.2.4.Các hàm thông dụng**

* Sau khi lưu những cấu hình DMA_Init() và cho phép bộ DMA hoạt động DMA_Cmd(), ta tiến hành cho phép DMA làm việc với ngoại vi bằng hàm <Periph>_DMACmd() 

`void SPI_I2S_DMACmd(SPI_TypeDef* SPIx, uint16_t SPI_I2S_DMAReq, FunctionState NewState)`

`void I2C_DMACmd(I2C_TypeDef* I2Cx, FunctionalState NewState)`

`void USART_DMACmd(USART_TypeDef* USARTx, uint16_t USART_DMAReq, FunctionalState NewState)`

`void ADC_DMACmd(ADC_TypeDef* ADCx, FunctionalState NewState)`


## **9.3.Giới thiệu về PWM**

### **9.3.1.Định nghĩa**

* **PWM (Pulse Width Modulation)** được sử dụng để chỉ định góc mà động cơ Servo sẽ quay đến.Tín hiệu PWM có 2 yếu tố quyết Định

  ◦ Tần số: Số lần tín hiệu lặp lại 1 giây.Với servo,tần số thông thường là 50HZ

  ◦ Độ rộng xung: Là thời gian tín hiệu ở mức cao trong mỗi chu kỳ. Độ rộng xung thường được đo bằng microsecond và quyết định góc mà servo sẽ xoay đến. **Tỉ lệ độ rộng xung với chu kỳ xung được gọi là Duty Cycle**

  
![Image](https://github.com/user-attachments/assets/4e235026-e41a-4169-87bb-c254b332b343)

### **9.3.2.Đặc điểm**

* Độ rộng xung PWM để xoay từ góc 0-180 độ là khoảng 1000µs -> 2000µs


![Image](https://github.com/user-attachments/assets/d05e1663-c5c9-4627-9f8c-53957de8b4f9)

* Công thức tính độ rộng xung

pulseWidth = MIN_PULSE_WIDTH + (MAX_PULSE_WIDTH - MIN_PULSE_WIDTH) * angle / 180;

**MIN_PULSE_WIDTH** là độ rộng xung cho góc 0 độ

**MAX_PULSE_WIDTH** là độ rộng xung cho góc 180 độ

**angle** là góc mà servo quay đến


### **9.3.3.Cấu hình**

* Ngõ ra của xung PWM sẽ được xuất trên các chân GPIO. Ta phải cấu hình chân chế độ AF_PP

```
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //PA0 là TIM2_CH1
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
```

* Xung với chu kỳ là 20ms, độ rộng xung là 1000us đến 2000us

  ◦ Cấu hình timer mỗi 1us đếm lên 1 lần và tràn mỗi 20ms

* Cấu hình timer chế độ PWM trong struct **TIM_OCInitTypeDef**:

  ◦ **TIM_OCMode:** Chế độ hoạt động cho Output Compare.

         + TIM_OCMode_PWM1: Tín hiệu ở mức cao (1) khi giá trị đếm nhỏ hơn TIM_Pulse, mức thấp (0) khi lớn hơn.

         + TIM_OCMode_PWM2: Ngược lại với PWM1 

  ◦ **TIM_OutputState:** 

         + Kích hoạt hoặc vô hiệu hóa đầu ra PWM trên chân GPIO tương ứng.

  ◦ **TIM_Pulse:** 

         + Giá trị so sánh (CCR) , xác định độ rộng xung PWM (duty cycle).

         + Phạm vi: 0 đến TIM_Period

         + Công thức duty cycle: Duty cycle = [TIM_Pulse / (TIM_Period + 1)] x 100%

  ◦ **TIM_OCPolarity:**
  
         + Xác định cực tính tín hiệu đầu ra. 

         + TIM_OCPolarity_High: Tín hiệu ở mức **cao** khi giá trị đếm **nhỏ hơn TIM_Pulse** (trong PWM1).

         + TIM_OCPolarity_Low: Tín hiệu ở mức **thấp** khi giá trị đếm **nhỏ hơn TIM_Pulse**.

  ◦ **TIM_OCIdleState:** (quan trọng cho Timer nâng cao như TIM1)

         + Xác định trạng thái tín hiệu PWM khi Timer ở trạng thái "Idle" (không hoạt động, thường dùng trong điều khiển động cơ).

         + TIM_OCIdleState_Set: Mức cao khi Idle.

         + TIM_OCIdleState_Reset: Mức thấp khi Idle (mặc định).

  ◦ **TIM_OutputNState:** (quan trọng cho Timer nâng cao như TIM1)

         + Kích hoạt hoặc vô hiệu hóa tín hiệu bổ sung (complementary output, ví dụ: TIM1_CH1N)

         + TIM_OutputNState_Enable: Bật tín hiệu bổ sung.

         + TIM_OutputNState_Disable: Tắt tín hiệu bổ sung

  ◦ **TIM_OCNIdleState:** (chỉ áp dụng cho Timer nâng cao)

         + Xác định trạng thái tín hiệu bổ sung khi Timer ở trạng thái Idle.

         + Giá trị: TIM_OCNIdleState_Set hoặc TIM_OCNIdleState_Reset.

* Ngoài các hàm bạn đã đề cập (TIM_OCxInit, TIM_OCxPreloadConfig, TIM_Cmd, TIM_SetComparex), dưới đây là các hàm SPL bổ sung cần chú ý khi làm việc với PWM:

  ◦ **TIM_ARRPreloadConfig(TIMx, FunctionalState):**

         + Bật/tắt tính năng preload cho thanh ghi ARR

         + Khi bật, giá trị TIM_Period chỉ được cập nhật khi xảy ra Update Event, giúp tránh giật tín hiệu PWM khi thay đổi chu kỳ

  
  ◦ **TIM_CtrlPWMOutputs(TIMx, FunctionalState) (chỉ áp dụng cho TIM1):**

         + Kích hoạt đầu ra PWM cho Timer nâng cao (TIM1).

         + Cần gọi TIM_CtrlPWMOutputs(TIM1, ENABLE) để đảm bảo tín hiệu PWM được xuất ra chân GPIO.

         + Nếu không gọi, PWM sẽ không hoạt động trên TIM1.

  
  ◦ **TIM_SetCounter(TIMx, value):**

         + Đặt giá trị hiện tại của bộ đếm Timer. Dùng để đồng bộ hóa hoặc đặt lại Timer trong các ứng dụng PWM đặc biệt.

  ◦ **TIM_GetCapturex(TIMx):**

         + Lấy giá trị hiện tại của thanh ghi so sánh (CCR) cho kênh x. Hữu ích để kiểm tra duty cycle hiện tại.

  ◦ **TIM_BDTRConfig(TIMx, TIM_BDTRInitTypeDef) (chỉ áp dụng cho TIM1):**

         +  Cấu hình các tính năng nâng cao như Break and Dead-time (dùng trong điều khiển động cơ hoặc inverter).

         + Các tham số quan trọng trong TIM_BDTRInitTypeDef:

           TIM_DeadTime: Thời gian chết giữa tín hiệu chính và bổ sung.

           TIM_Break: Bật/tắt chức năng Break (ngắt tín hiệu PWM khi có lỗi)

           IM_AutomaticOutput: Tự động khôi phục đầu ra PWM sau sự kiện Break.
  
  ◦ **TIM_ClearOCxRef(TIMx, TIM_OCx):**

         + Xóa tín hiệu PWM trên kênh x, dùng để tạm dừng PWM mà không tắt Timer.

  </details>

 
<details>
	<summary><strong>BÀI 10: FLASH VÀ BOOTLOADER </strong></summary>

## Bài 10: FLASH VÀ BOOTLOADER

## **10.1.Các loại bộ nhớ cơ bản**


* **RAM (Random Access Memory):**
 
    ◦ Tốc độ đọc/ghi nhanh
    
    ◦ Không cần xóa trước khi ghi, ghi và đọc trực tiếp từng byte

    ◦ Dữ liệu **bị mất** khi ngưng cấp nguồn **(volatile memory)**

    **=> Lưu trữ giá trị tạm thời,các biến, ngăn xếp (stack) và heap trong thời gian chạy chương trình.**

* **FLASH:**

    ◦ Tốc độ đọc nhanh, tốc độ ghi và xóa chậm

    ◦ Phải xóa cả trang trước khi ghi,việc xóa và ghi được thực hiện theo khối (page/sector), không thể ghi đè trực tiếp từng byte
    
    ◦ Dữ liệu **không bị mất** khi ngưng cấp điện **(Nonvolatile memory)**

    ◦ Giới hạn số lần xóa/ghi (thường từ 10k đến 100k chu kỳ)

    **=> Lưu trữ chương trình(firmware, mã máy) và dữ liệu cố định (hằng số, lookup tables)**


* **EEPROM(Electrically Erasable Programmable Read-Only Memory):**

    ◦ Tương tự FLASH, có thể đọc/ghi theo từng byte

    ◦ Phải xóa trước khi ghi (thường là xóa tự động trước mỗi lần ghi)

    ◦ Tốc độ ghi chậm hơn FLASH và số lần ghi/xóa cũng có giới hạn

    **=> Lưu trữ các dữ liệu cần thay đổi không thường xuyên như cấu hình hệ thống, calibration data, log counts**

## **10.2.FLASH**

### **10.2.1.Giới thiệu**

* Flash sẽ được chia thành các **Page (trang)** hoặc **Sector (cấp)** để quản lý

* Kích thước mỗi page/sector phụ thuộc vào dòng vi điều khiển cụ thể (ví dụ: 1KB, 2KB, thậm chí 128KB trên các dòng cao cấp).

* Trước khi ghi phải xóa.Mỗi lần xóa, cả page sẽ bị xóa trắng về giá trị **(0xFF)**

* Khi ghi dữ liệu,**chỉ** có thể ghi từ **0** sang **1** (tức là từ 0xFF ghi thành một giá trị khác)

=> Do đó, chỉ có thể ghi các đơn vị dữ liệu như **half-word (2 byte)** hoặc **word (4 byte)** tại một thời điểm 

* Flash có **giới hạn** về số lần xóa/ghi

* Chương trình (firmware) sẽ được nạp vào vùng nhớ từ**0x08000000**, vùng nhớ phía sau firmware sẽ là trống và người dùng có thể lưu trữ dữ liệu ở vùng này

![Image](https://github.com/user-attachments/assets/229b15b3-74c2-42e9-b0dc-a1df7fb76f18)


### **10.2.2.Xóa FLASH**



![Image](https://github.com/user-attachments/assets/9c5de0e7-da99-4993-aae7-9c9d9e59ed6b)

#### **Flash chỉ có thể được xóa theo từng Page (1Kb mỗi Page) hoặc xóa theo cả Bank (1 Bank)** 


  * **1. Kiểm tra khóa (LOCK):** 
    
    ◦ Nếu FLASH đang bị khóa, cần gửi các khóa (key) cụ thể để mở khóa (Unlock) trước khi có thể thao tác

  * **2. Chờ FLASH sẵn sàng:** 
    
    ◦ Kiểm tra cờ **BSY (Busy)** để đảm bảo không có thao tác nào khác diễn ra

  * **3. Cấu hình chế độ xóa:** 
    
    ◦ Set bit **PER(Page Erase)** trong thanh ghi điều khiển **CR**

  * **4. Chỉ định địa chỉ:** 
    
    ◦ Ghi địa chỉ bắt đầu của Page cần xóa và thanh ghi địa chỉ **AR**

  * **5. Kích hoạt xóa:** 
    
    ◦ Set bit **STRT (Start)** để bắt đầu quá trình xóa phần cứng

  * **6. Chờ hoàn tất:** 
    
    ◦ Tiếp tục kiểm tra cờ **BSY**

  * **7. Kiểm tra kết quả:** 
    
    ◦ Kiểm tra cờ **EOP (End of Operation)** hoặc các cờ lỗi

  * **8. Xóa cờ và khóa lại:** 
    
    ◦ Xóa các cờ và khóa (Lock) FLASH lại để tránh ghi đè ngoài ý muốn.

  
### **10.2.3.Ghi FLASH**

![Image](https://github.com/user-attachments/assets/e6f2c75f-cc9d-4d78-ad8e-d981f000baa6)

   
  * **1. Kiểm tra khóa (LOCK):** 
    
    ◦ Nếu FLASH đang bị khóa, cần gửi các khóa (key) cụ thể để mở khóa (Unlock) trước khi có thể thao tác

  * **2. Chờ FLASH sẵn sàng:** 
    
    ◦ Kiểm tra cờ **BSY (Busy)** để đảm bảo không có thao tác nào khác diễn ra

  * **3. Cấu hình chế độ ghi:** 
    
    ◦ Set bit **PG (Programming)** trong thanh ghi điều khiển **CR**

  * **4. Thực hiện ghi:** 
    
    ◦ Ghi dữ liệu (half-word hoặc word) vào địa chỉ đích. 
     
    => Việc ghi này phải được thực hiện trên một vùng nhớ đã được xóa trước đó (có giá trị 0xFF).

  * **5. Chờ hoàn tất:** 
    
    ◦ Chờ cờ BSY trở về 0 
     
  * **6. Kiểm tra kết quả và kết thúc:** 
    
    ◦ Kiểm tra cờ EOP, xóa cờ và khóa FLASH lại


### **10.2.4.Các hàm thông dụng**

* **Hàm LOCK/UNLOCK FLASH**
```
   ◦ void FLASH_Unlock(void): Hàm này Unlock tất cả vùng nhớ trong Flash

   ◦ void FLASH_UnlockBank1(void): Hàm này chỉ Unlock cho Bank đầu tiên

   ◦ void FLASH_UnlockBank2(void): Hàm này Unlock cho Bank 2

   ◦ void FLASH_Lock(void): Lock bộ điều khiển xóa Flash cho toàn bộ vùng nhờ Flash

   ◦ void FLASH_LockBank1(void) và void FLASH_LockBank2(void) : Lock toàn bộ điều khiển FLASH cho Bank 1 và 2
```

* **Hàm xóa FLASH**

```
   ◦ FLASH_Status FLASH_EraseAllBank1Pages(void): Xóa tất cả các Page trong Bank 1 của FLASH

   ◦ FLASH_Status FLASH_EraseAllBank2Pages(void): Xóa tất cả các Page trong Bank 2 của FLASH

   ◦ FLASH_Status FLASH_EraseAllPages(void): Xóa toàn bộ FLASH

   ◦ FLASH_Status FLASH_ErasePage(uint32_t Page_Address): Xóa 1 Page cụ thể trong Flash, cụ thể là Page bắt đàu bằng địa chỉ Page_Address

```

**Code**

```
void FLASH_Erase(uint32_t addresspage){

  FLASH_Unlock();
  
  // Chờ đến khi bộ nhờ FLASH rảnh
  while(FLASH_GetFlagStatus(FLASH_FLAG_BSY) == 1);

  //Xóa page tương ứng
  FLASH_ErasePage(addresspage);

   // Chờ đến khi bộ nhờ FLASH rảnh
  while(FLASH_GetFlagStatus(FLASH_FLAG_BSY) == 1);

  // Sau khi sử dụng xong thì khóa Flash lại
	FLASH_Lock();
}
```

* **Hàm ghi FLASH**

```
   ◦ FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data) : Ghi dữ liệu vào vùng nhớ Addres với kích thước mỗi 2 byte

   ◦ FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data) : Ghi dữ liệu vào vùng nhớ Address với kích thước mỗi 4 byte
```

* **Hàm kiểm tra trạng thái**

```
   ◦ FLASH_Status FLASH_GetStatus(void)

   ◦ FLASH_Status FLASH_GetFlagStatus(uint32_t FLASH_FLAG): Hàm này trả về trạng thái của FLag

```

**Code**

```
// Hàm xóa 1 Page Flash
void Flash_ErasePage(uint32_t pageAddress) {
    FLASH_Status status = FLASH_COMPLETE;

    // Mở khóa Flash
    FLASH_Unlock();

    // Chờ Flash sẵn sàng
    status = FLASH_WaitForLastOperation(FLASH_ER_PTIMEOUT);

    if (status == FLASH_COMPLETE) {
        // Xóa page
        status = FLASH_ErasePage(pageAddress);
        if (status != FLASH_COMPLETE) {
            // Xử lý lỗi tại đây
        }
    }

    // Khóa Flash lại
    FLASH_Lock();
}

// Hàm ghi một mảng dữ liệu (sử dụng Half-Word)
void Flash_WriteArray(uint32_t startAddress, uint16_t *data, uint16_t length) {
    FLASH_Status status = FLASH_COMPLETE;
    uint32_t address = startAddress;

    // Mở khóa Flash
    FLASH_Unlock();

    for (uint16_t i = 0; i < length; i++) {
        // Chờ Flash sẵn sàng trước mỗi lần ghi
        status = FLASH_WaitForLastOperation(FLASH_ER_PTIMEOUT);
        if (status != FLASH_COMPLETE) break;

        // Ghi dữ liệu
        status = FLASH_ProgramHalfWord(address, data[i]);
        if (status != FLASH_COMPLETE) break;

        address += 2; // Di chuyển đến địa chỉ half-word tiếp theo
    }

    // Khóa Flash lại
    FLASH_Lock();
}
```

## **10.3.BOOTLOADER**

### **10.3.1.Định nghĩa**

* Bootloader là chương trình đặc biệt chạy đầu tiên ngay sau khi vi điều khiển khởi động hoặc reset.Nó có nhiệm vụ quyết định chương trình nào sẽ được thực thi tiếp theo. 

=> Cho phép nâng cấp, sửa đổi firmware (phần mềm) mà không cần sử dụng các công cụ nạp chuyên dụng (programmer) thông qua các giao tiếp như UART, USB, I2C, SPI, Ethernet, v.v.

* **Ứng dụng:** Cập nhật firmware từ xa (FOTA - Firmware Over-The-Air), cập nhật sản phẩm tại hiện trường mà không cần mở hộp.

* Có 2 loại BOOTLOADER:

   ◦ Bootloader do nhà sản xuất cung cấp

   ◦ Bootloader do người dùng từ viết


* Bootloader là để kiểm tra các điều kiện để lựa chọn thực thi 1 trong các chương trình:

   ◦ Firmware Update mới nhất

   ◦ Firmware được nạp vào do hãng cung cấp trước khi xuất ra thị trường

   ◦ Current Firmware



### **10.3.2.Bootloader hoạt động như thế nào**

#### **Quá trình khởi động của ARM Cortex-M**

  **1. Thiết lập chế độ Boot:**

◦  Khi khởi động, vi điều khiển đọc các chân BOOT0 và (thường là) BOOT1 để quyết định vùng nhớ nào sẽ được ánh xạ đến địa chỉ 0x00000000. Thông thường, bộ nhớ Flash gốc (0x08000000) được ánh xạ đến địa chỉ 0.

  **2. Khởi tạo Vector Table:**

◦  CPU lấy giá trị Word đầu tiên (tại địa chỉ 0x00000000) để khởi tạo con trỏ ngăn xếp (MSP - Main Stack Pointer).

◦  CPU lấy Word thứ hai (tại địa chỉ 0x00000004) – đây là địa chỉ của hàm Reset_Handler – và nạp vào thanh ghi PC (Program Counter).
  
  **3. Thực thi Bootloader:**

◦  CPU bắt đầu chạy code trong Reset_Handler, sau đó nhảy đến hàm main() của chương trình được lưu trong vùng nhớ boot (thường là Bootloader).
 
#### **Cơ chế nhảy từ Bootloader sang Application**

  **Giả sử:**

◦  Bootloader nằm ở đầu bộ nhớ Flash (ví dụ: từ 0x08000000 đến 0x0800BFFF).

◦  Bootloader nằm ở đầu bộ nhớ Flash (ví dụ: từ 0x08000000 đến 0x0800BFFF).

  **Bootloader sẽ thực hiện các bước sau để nhảy sang Application:**

◦  1. Kiểm tra Application: Bootloader kiểm tra một điều kiện (ví dụ: phím nhấn, lệnh từ UART, hoặc kiểm tra signature của Application) để quyết định có nhảy sang App hay không. Nếu không, nó có thể ở lại chế độ cập nhật firmware.

◦  2. Tắt mọi ngắt và ngoại lệ: __disable_irq();

◦  3. De-init các ngoại vi: Khởi tạo lại các ngoại vi mà Bootloader đã sử dụng (như UART, SPI) về trạng thái mặc định.
  
◦  4. Cấu hình lại Vector Table: Đây là bước QUAN TRỌNG NHẤT. Thanh ghi SCB->VTOR (Vector Table Offset Register) phải được đặt thành địa chỉ bắt đầu của Application's Vector Table.
   
   ```
   SCB->VTOR = APPLICATION_ADDRESS; // Ví dụ: 0x0800C000
   ```
    
◦  5. Cấu hình lại con trỏ ngăn xếp: Lấy giá trị MSP mới từ word đầu tiên của Application's Vector Table.

   ```
   __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
   ```

◦  6. Nhảy đến Reset Handler của Application: Lấy địa chỉ của Reset Handler (word thứ hai trong Vector Table) và gọi nó như một hàm.

   ```
   void (*app_reset_handler)(void) = (void (*)(void)) *(__IO uint32_t*)(APPLICATION_ADDRESS + 4);

   app_reset_handler(); // Nhảy không trở lại
   ```
◦  Lưu ý:
   
   ```
   Application phải được biên dịch và nạp để chạy tại địa chỉ bộ nhớ đã định (ví dụ: 0x0800C000).

   Trong script linker (.ld) của Application, cần phải thay đổi địa chỉ bắt đầu của bộ nhớ FLASH cho phù hợp.

   Vector Table của Application phải được định nghĩa chính xác tại địa chỉ đó.
   ```

   
</details>


<details>
	<summary><strong>BÀI 11: CAN</strong></summary>

## Bài 11: CAN (Controller Area Network)

## **11.1.Giới thiệu**
 
◦ **CAN:** Giao thức nối tiếp, cho phép vi điều khiển và thiết bị giao tiếp không cần máy tính chủ.
    
◦ **Chức năng:** Giảm dây dẫn, hỗ trợ phát hiện và sửa lỗi tự động.

## **11.2.Kiến trúc**

### **11.2.1.Bus topology**

* **Cấu trúc:** Topo-bus, các node nối song song qua 2 dây CANH (dây tín hiệu cao) và CANL (dây tín hiệu thấp).

* **Tín hiệu:** Vi sai, dựa trên chênh lệch điện áp giữa CANH và CANL.

* **Điện trở kết cuối:** 120 Ohm ở mỗi đầu bus để tránh phản xạ tín hiệu.


### **11.2.2.Các thiết bị trên bus CAN**

![Image](https://github.com/user-attachments/assets/1caaac10-6b1f-43df-ba3a-9e2033cceb5f)

* **Sensors:** Cung cấp dữ liệu

* **Actuator(Thiết bị kích động):** nhận lệnh từ MCU qua bus CAN để thực hiện các hành động vật lý như (mở van, điều khiển động cơ, bật đèn) 

* **CAN Controller:** 

  ◦ Gửi/nhận thông điệp CAN, điều khiển truy cập bus CAN, xử lý lỗi CAN.

  ◦ Cung cấp giao diện giữa MCU và bus CAN

* **CAN Transceiver:** 

  ◦ Chuyển đổi tín hiệu số từ bộ điều khiển CAN thành tín hiệu điện áp dạng vi sai để gửi lên bus CAN và ngược lại

* **MCU:**

  ◦ Xử lý, tạo thông điệp CAN.

  ◦ Quản lý khung dữ liệu, arbitration, lỗi.

### **11.2.3.Đặc điểm giao tiếp**

* **Không cần máy tính chủ**

* **Truyền thông quảng bá**
   
   ◦ Thông điệp phát đến tất cả node, node dùng bộ lọc để chọn thông điệp phù hợp.

* **Arbitration**

   ◦ Mỗi thông điệp CAN có một ID ưu tiên. Node nào có ID ưu tiên thấp hơn (tức độ ưu tiên cao hơn) sẽ chiếm quyền truy cập bus và gửi thông điệp trước

   ◦ Những Node có ID ưu tiên cao hơn thì tự động dừng lại và chờ đợi lượt tiếp theo để gửi thông điệp

   ◦ Quá trình này không gây mất dữ liệu hay làm gián đoạn đến các thiết bị khác => **non-destructive**

* **Giao tiếp song công**

* **Phát hiện và sửa lỗi tự động**

   ◦ Gửi Error Frame khi lỗi, truyền lại thông điệp.

## **11.3.Khung dữ liệu CAN**

### **11.3.1.Các loại khung dữ liệu**

* **Data Frame**

   ◦ Chứa ID và dữ liệu (0-8 byte).

   ◦ ID quyết định ưu tiên (ID thấp = ưu tiên cao).


* **Remote Frame**

   ◦ Sử dụng khi một node trên mạng CAN **yêu cầu dữ liệu** từ node khác

   ◦ **Remote Frame** chứa **ID** của node cần yê cầu và **bit điều khiển RTR**  

* **Error Frame**

   ◦  Báo lỗi 
   
   ◦  **Error Flag** là chuỗi từ 6 đến 12 bit dominant, báo hiệu lỗi

   ◦  **Error Delimiter** là chuỗi 8 bit recessive, kết thúc Error Frame

* **Overload Frame**

   ◦ Báo node bận, tạm dừng truyền thông.

### **11.3.2.Cấu trúc một khung dữ liệu**

#### **11.3.2.1.Start of Frame(SOF)** 

* Là bit bắt đầu của khung dữ liệu, chỉ có giá trị dominant(0)

#### **11.3.2.2.Arbitration Field**

* **ID:** Chứa định danh thông điệp

* **RTR:**  Dominant (Data Frame), recessive (Remote Frame).

#### **11.3.2.3.Control Field**

* Chứa thông tin kích thước của phần dữ liệu

* **DLC:** xác định độ dài của dữ liệu(0 đến 8 byte)

#### **11.3.2.4.Data Field**

* Chứa thông tin mà node muốn truyền tải

#### **11.3.2.5.CRC Field**

* Node nhận sử dụng CRC Field để kiểm tra được truyền chính xác hay chưa.Nếu phát hiện, một Error Frame sẽ được gửi

#### **11.3.2.6.ACK Field**

* Xác nhận thông điệp đã được nhận thành công
 => Gửi bit ACK dominant 

* Nếu không có node nào gửi ACK,báo hiệu rằng có lỗi xảy ra / truyền không đúng cách
 => Truyền lại thông điệp 

#### **11.3.2.7.EOF**

* Chuỗi bit recessive báo hiệu kết thúc.

## **11.4. Arbitration**

### **11.4.1.Cơ chế ưu tiên**

#### **Node có ID thấp hơn tương ứng với mức độ ưu tiên cao hơn sẽ thắng Arbitration**

#### **Mỗi bit ID có thể ở trạng thái dominant (trạng thái ưu tiên - 0) hoặc recessive (trạng thái không ưu tiên - 1). Khi nhiều node cùng gửi dữ liệu CAN sẽ sử dụng quy tắc AND để xem NODE nào được ưu tiên**


![Image](https://github.com/user-attachments/assets/7f8c7fbf-4fb1-4353-98ef-528b624ef2d7)

* **Nguyên lý hoạt động**

  ◦	Khi nhiều node muốn truyền dữ liệu, chúng đều bắt đầu gửi thông điệp của mình lên bus. Tín hiệu được gửi đồng thời và mỗi node sẽ kiểm tra từng bit của dữ liệu trên bus.

  ◦ Mỗi bit truyền từ MSB-LSB.Nếu một node gửi **bit recessive(1)** mà nhận thấy trên bus có **bit dominant(0)** 

  => **Node ngừng truyền và chuyển sang chế độ nghe**

  ◦ Node có **ID thấp hơn(tức có nhiều bit dominant ở đầu)** sẽ tiếp tục truyền cho đến khi toàn bộ ID được gửi, trong khi các node khác ngừng gửi và chuyển sang chế độ chờ

  ◦ Các node **không thắng Arbitration chờ đợi và đợi lượt tiếp theo truyền lại thông điệp của mình**

### **11.4.2.Non-destructive Arbitration**

* Cơ chế có nghĩa là quá trình Arbitrage diễn ra mà **không làm mất dữ liệu** của các node thua nhờ vào **multi-master** và **AND Logic**
  
  ◦ Khi một node thua trong quá trình arbitration, nó sẽ **tạm dừng việc truyền nhưng không xóa dữ liệu** của mình.

  ◦ Node thua sẽ chuyển sang **trạng thái chờ và lắng nghe bus**. Khi bus không còn bận (tức là node thắng đã gửi xong thông điệp), node thua sẽ **thử lại** và tham gia tranh chấp quyền gửi ở lần tiếp theo.

## **11.5.Lỗi trong giao thức CAN**

### **11.5.1.Các loại lỗi trong CAN**

#### **11.5.1.1.Bit Error**

* Xảy ra khi một node **gửi một bit** (dominant hay recessive) lên bus mà **không nhận lại giá trị mong đợi**

* Mỗi node không chỉ gửi dữ liệu mà còn lắng nghe tín hiệu để kiểm tra sự đồng bộ

* Điều này có thể xảy ra khi một node khác có ưu tiên cao hơn đang truyền trên bus dữ liệu hoặc do nhiễu

#### **11.5.1.2.Stuff Error**

* Xảy ra khi **có hơn 5 bit liên tiếp cùng giá trị (0 hoặc 1)**
=> Vi phạm quy tắc **bit stuffing**

* **Bit stuffing:** Trong mạng CAN,sau mỗi chuỗi 5 bit giống nhau liên tiếp,một bit ngược giá trị phải được thêm vào

#### **11.5.1.3.CRC Error**

* Xảy ra khi có **sai lệch trong quá trình kiểm tra CRC**

* Node nhận sẽ tính toán lại giá trị CRC của dữ liệu và so sánh với CRC trong trường CRC Field.Nếu 2 giá trị này này không khớp, lỗi sẽ được phát hiện

#### **11.5.1.4.Form Error**

* Xảy ra khi **cấu trúc khung dữ liệu** không tuân theo quy chuẩn của giao thức CAN

#### **11.5.1.5.ACK Error**

* Xảy ra khi node gửi thông điệp lên bus CAN mà **không nhận được bit ACK** từ bất kỳ node trên mạng

* Khi một node gửi thành công 1 khung dữ liệu, node **nhận** phải gửi 1 bit **ACK dominant** để xác nhận dữ liệu đã được nhận chính xác

* Nếu không có node nào gửi bit ACK => ACK Error => Truyền lại thông điệp

### **11.5.2.Cơ chế phát hiện lỗi**

* **Kiểm tra Bit**

* **Kiểm tra CRC**

* **Kiểm tra Form**

* **Xác nhận**

### **11.5.3.Cơ chế sửa lỗi**

* **CAN có khả năng tự sửa lỗi một cách tự động thông quá quá trình phát Error Frame và truyền lại thông điệp**

  ◦ **Error Frame:** Khi một node phát hiện lỗi (bit error, CRC error, form error, stuff error, hoặc ACK error), nó sẽ gửi một Error Frame để thông báo cho tất cả các node khác trên bus rằng có lỗi đã xảy ra.

  ◦ **Truyền lại thông điệp:** Sau khi Error Frame được phát, các node sẽ dừng giao tiếp và node gửi ban đầu sẽ cố gắng truyền lại thông điệp bị lỗi. Việc này sẽ tiếp tục cho đến khi thông điệp được truyền đi thành công hoặc node gửi bị đưa vào trạng thái bus off nếu lỗi quá nhiều.

### **11.5.4.Các trạng thái lỗi của node**

 **Phát hiện lỗi => tự chuyển đổi giữa 3 trạng thái => đảm bảo ổn định và không gián đoạn**

#### **11.5.4.1. Error Active**

* Node vẫn có khả năng tham gia đầy đủ vào quá trình truyền thông và có thể phát hiện lỗi. Nếu node phát hiện lỗi, nó sẽ gửi một Error Frame để thông báo cho các node khác trên bus rằng đã xảy ra lỗi.

#### **11.5.4.2. Error Passive**

* Trong trạng thái này, node vẫn có thể tham gia truyền thông, nhưng nếu phát hiện lỗi, nó sẽ không gửi Error Frame mạnh mẽ như trong trạng thái Error Active. 

#### **11.5.4.3. Error Passive**

* Khi một node gặp **quá nhiều lỗi** nghiêm trọng, nó sẽ chuyển sang trạng thái Bus Off. Trong trạng thái này, node sẽ hoàn toàn **ngắt kết nối khỏi bus** CAN và không thể tham gia vào quá trình truyền hay nhận dữ liệu. Node chỉ có thể được kết nối lại vào bus sau khi được **khởi động lại (restart) hoặc reset bởi phần mềm.**

## **11.6.Cấu hình và thiết lập CAN**

### **11.6.1.Thiết lập tốc độ Baud**

* **Được tính bằng số bit truyền trên giây(bps)**

![Image](https://github.com/user-attachments/assets/2913e085-b302-4c3b-899f-66400a3a34e4)

* **Sample Point:** 

  ◦ Là thời điểm mà tín hiệu trên bus CAN được đọc để xác định giá trị của 1 bit

  ◦ Mẫu thường được lấy ở vị trí cuối mỗi bit

  ◦ Vị trí của sample point được tính tính toán dựa trên tỷ lệ phần trăm trong 1 khoảng thời gian bit. Lý tương là từ **75% đến 90%**

* **Time Segment:**

  ◦ Là một thành phần quan trọng để đồng bộ hóa và điều chỉnh thời gian truyền thông giữa các node trên bus CAN

  ◦ **Sync Segment:** 
  
  Là đoạn đầu tiên, độ dài cố định là **1 TQ(time quanta)**

  => Đồng bộ node trên bus

  => Node nhận sẽ phát hiện ra cạnh thay đổi (rising edge hoặc falling edge) của tín hiệu CAN tại đoạn này để điều chỉnh thời gian của chính nó

  ◦ **Propagation Segment:** 
  
  **Bù đắp thời gian** cần thiết để tín hiệu lan truyền qua bus CAN từ node gửi đến node nhận.Tín hiệu cần thời gian để di chuyển từ một node đến một node khác, và thời gian này **phụ thuộc vào chiều dài của bus và tốc độ truyền**. Propagation Segment được cấu hình sao cho nó bao gồm **độ trễ lan truyền và thời gian trễ xử lý** của cả phần cứng CAN.

  ◦ **Phase Segment 1 (PS1) và Phase Segment 2 (PS2):**

    ■	PS1 là đoạn thời gian trước điểm lấy mẫu

    ■	PS2 là đoạn thời gian sau điểm lấy mẫu

**Bit Time = Sync Segment + Propagation Segment + PS1 + PS2**


### **11.6.2.Bộ lọc CAN**

* Cho phép các node lựa chọn và chỉ nhận những thông điệp cần thiết, dựa trên ID của thông điệp hoặc các tiêu chí khác

* Hoạt động dựa trên 2 phần chính

  ◦ **Mask:** Quy định những bit nào **trong ID của thông điệp** cần được kiểm tra

  ◦ **Filter:** Được dùng để so sánh các bit của **ID thông điệp** với giá trị quy định trong bộ lọc.Nếu các bit này khớp với **Mask**, thông điệp sẽ được chấp nhận và xử Lý

#### **11.6.2.1.Mask**

* **Mask** là 1 dãy bit mà các bit có giá trị **1** sẽ được **kiểm tra**, **0** sẽ bị bỏ qua

 **=> Xác định phạm vi ID mà node quan tâm**

#### **11.6.2.2.Filter**

* Nếu ID thông điệp **trùng khớp** với giá trị bộ lọc(sau khi đã áp dụng mask),thông điệp sẽ được chấp nhận

* Nếu **không trùng khớp**,thông điệp sẽ bị bỏ qua,node sẽ không xử lý nó

* **Cấu hình bộ lọc**

  ●	**Mask (Mặt nạ):** 0x700-chỉ kiểm tra **3 bit đầu tiên** của ID

  ●	**Filter (Bộ lọc):** 0x100- chỉ nhận thông điệp có ID bắt đầu bằng 0x001

* **Phân tích cấu hình**

  ●	**Mask:** 0x700 (111 0000 0000) có nghĩa là **3 bit đầu tiên** của ID thông điệp sẽ được so sánh với **filter**.Các bit khác(bit từ 8 trở xuống) sẽ không được kiểm tra

  ●	**Filter:** 0x100 (001 0000 0000) có nghĩa là node sẽ chấp nhận những thông điệp có **3 bit đầu tiên là 001**,tức thông điệp có ID nằm trong khoảng từ **0x100** đến **0x1FF**

```
●	ID 0x0F0: Bị bỏ qua vì 3 bit đầu tiên là 000, không trùng khớp với 001 trong filter.
●	ID 0x100: Được chấp nhận vì 3 bit đầu tiên là 001, trùng khớp với filter.
●	ID 0x180: Được chấp nhận vì 3 bit đầu tiên là 001, trùng khớp với filter.
●	ID 0x200: Bị bỏ qua vì 3 bit đầu tiên là 010, không trùng khớp với filter.


```
</details>
  
