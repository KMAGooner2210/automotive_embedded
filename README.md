
	
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
    ```
        **Tham số:**

            RCC_APB2Periph: Chọn peripheral (ví dụ: RCC_APB2Periph_GPIOA cho port A, RCC_APB2Periph_AFIO cho AFIO).

            NewState: ENABLE hoặc DISABLE.
    ```
  ◦ VD:

		  void RCC_Config(){
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
		}
        
* **1.2.2. Cấu hình chân GPIO**

  ◦ Chân GPIO phải ở chế độ Input để nhận tín hiệu ngoài.

  ◦ Sử dụng pull-up/pull-down để tránh trạng thái floating (nhiễu).

  ◦ **Struct:** `GPIO_InitTypeDef`

     ```
        GPIO_Pin: Chọn pin (ví dụ: GPIO_Pin_0 cho pin 0).
     ```
     ```
        GPIO_Mode: Chế độ Input

            GPIO_Mode_IPU: Input với pull-up (phù hợp khi nút bấm nối GND, nhấn → low).
            GPIO_Mode_IPD: Input với pull-down (nút bấm nối VCC, nhấn → high).
            GPIO_Mode_IN_FLOATING: Input không pull (cần đảm bảo tín hiệu ổn định bên ngoài).
    ```
     ```
        GPIO_Speed: Không cần thiết cho Input (có thể bỏ qua hoặc đặt GPIO_Speed_50MHz).
    ```
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

    ```    
    EXTI_Line: Line cụ thể (ví dụ: EXTI_Line0 đến EXTI_Line15).
    ```
    ```
    EXTI_Mode: 

        EXTI_Mode_Interrupt: Gọi ISR khi ngắt xảy ra (thông dụng).
        EXTI_Mode_Event: Không gọi ISR, dùng cho các module khác (như DMA, Wake-up).
    
    ```
    ```
    EXTI_Trigger: 
    
        EXTI_Trigger_Rising: Phát hiện cạnh lên (low → high).
        EXTI_Trigger_Falling: Phát hiện cạnh xuống (high → low).
        EXTI_Trigger_Rising_Falling: Phát hiện cả hai (thay đổi trạng thái).
    ```
    ```    
    EXTI_LineCmd: ENABLE hoặc DISABLE.
    ```
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

* **Timer:** 
        
   ◦ Tạo sự kiện định thời chính xác (ngắt định kỳ, đo thời gian, PWM)

   ◦ Các Timer trên STM32F103: TIM1 (Advanced), TIM2-5 (General Purpose), TIM6-7 (Basic).

* **Ngắt Timer:** 

   ◦ Xảy ra khi counter đạt giá trị Period, tạo Update Event và kích hoạt ISR.

   ◦ Có thể cấu hình thêm ngắt từ các sự kiện khác (Input Capture, Output Compare).

* **Ứng dụng:** 

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

    * **Lưu ý:**

        ◦ Tính toán Prescaler và Period dựa trên thời gian mong muốn. 
        
        ◦ VD: Với Interval = 1 ms, Timer clock = 72 MHz, có thể chọn:

            Prescaler = 7199, Period = 9 → (7200 × 10) / 72 MHz = 1 ms.
            Hoặc Prescaler = 71, Period = 999 → (72 × 1000) / 72 MHz = 1 ms.


Chọn giá trị sao cho dễ tính toán và phù hợp với ứng dụng.
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

#### **2.3. Các chế độ Timer**

* **2.3.1. Input Capture** 

    * **Mục đích:** Đo thời gian của tín hiệu bên ngoài (như độ rộng xung, tần số, chu kỳ). Input Capture ghi lại giá trị counter khi phát hiện cạnh (rising/falling) trên chân Timer.

    * **Ứng dụng:** 

        ◦ Đo thời gian xung (pulse width) từ cảm biến siêu âm.

        ◦ Đo tần số tín hiệu.

        ◦ Phân tích tín hiệu PWM.

    * **Nguyên lý:** 

        ◦ Một chân Timer (ví dụ: PA0 cho TIM2 Channel 1) được cấu hình làm Input Capture.

        ◦ Khi phát hiện cạnh (rising/falling), giá trị counter được lưu vào thanh ghi CCRx (Capture/Compare Register).

        ◦ Ngắt được kích hoạt (nếu bật) để xử lý giá trị capture.
    
    * **Cấu hình:** 

        ◦ **1. Bật clock:** Như phần Timer cơ bản.

        ◦ **2. Cấu hình TimeBase:** Như trên, để xác định tần số đếm (Prescaler, Period).

        ◦ **3. Cấu hình Input Capture**

        ```
        Struct: TIM_ICInitTypeDef
        ```
        ```
        TIM_Channel: Kênh, ví dụ: TIM_Channel_1 (CC1), TIM_Channel_2 (CC2).
        ```
        ```
        TIM_ICPolarity: Cạnh phát hiện:

            TIM_ICPolarity_Rising: Cạnh lên.
            TIM_ICPolarity_Falling: Cạnh xuống.
            TIM_ICPolarity_BothEdge: Cả hai (không khuyến nghị vì phức tạp
        ```        
        ```
        TIM_ICSelection: Chọn nguồn input:

            TIM_ICSelection_DirectTI: Kênh trực tiếp (ví dụ: TI1 cho Channel 1).
            TIM_ICSelection_IndirectTI: Kênh chéo (hiếm dùng).
            TIM_ICSelection_TRC: Từ Trigger Controller (ít dùng).
        ```  
        ```
        TIM_ICPrescaler: Chia tần số tín hiệu vào:

            TIM_ICPSC_DIV1: Không chia.
            TIM_ICPSC_DIV2, TIM_ICPSC_DIV4, TIM_ICPSC_DIV8: Chia 2, 4, 8.
        ```  
        ```
        TIM_ICFilter: Lọc nhiễu (0-15, 0 = không lọc, 15 = lọc mạnh nhất)
        ```
        ```
        Hàm: TIM_ICInit(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct)
        ``` 

        ◦ **4. Bật ngắt Capture:** `TIM_ITConfig(TIMx, TIM_IT_CCx, ENABLE) (x = 1, 2, 3, 4)`.    

        ◦ **5. Cấu hình NVIC:** Như trên, dùng cùng `TIMx_IRQn`. 

        ◦ **6. Bật Timer:** `TIM_Cmd(TIMx, ENABLE)`.`  

    * **VD: Đo độ rộng xung trên TIM2 Channel 1, chân PA0** 

            volatile uint32_t capture_value1 = 0, capture_value2 = 0;
            volatile uint8_t capture_flag = 0;

            void InputCapture_Config() {
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

                // Cấu hình PA0 làm input cho TIM2 Channel 1
                GPIO_InitTypeDef GPIO_InitStruct;
                GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
                GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
                GPIO_Init(GPIOA, &GPIO_InitStruct);

                // Cấu hình TimeBase: Tick = 1 μs
                TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
                TIM_TimeBaseInitStruct.TIM_Prescaler = 71;  // 72 MHz / 72 = 1 MHz
                TIM_TimeBaseInitStruct.TIM_Period = 65535;  // Max để tránh tràn sớm
                TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
                TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
                TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

                // Cấu hình Input Capture
                TIM_ICInitTypeDef TIM_ICInitStruct;
                TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
                TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;  // Bắt đầu với cạnh lên
                TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
                TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
                TIM_ICInitStruct.TIM_ICFilter = 0x0;  // Không lọc
                TIM_ICInit(TIM2, &TIM_ICInitStruct);

                // Bật ngắt Capture Channel 1
                TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

                // Cấu hình NVIC
                NVIC_InitTypeDef NVIC_InitStruct;
                NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
                NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
                NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
                NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
                NVIC_Init(&NVIC_InitStruct);

                // Bật Timer
                TIM_Cmd(TIM2, ENABLE);
            }

            void TIM2_IRQHandler(void) {
                if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) {
                    if (capture_flag == 0) {  // Cạnh lên
                        capture_value1 = TIM_GetCapture1(TIM2);  // Lấy giá trị counter
                        TIM_SetIC1Polarity(TIM2, TIM_ICPolarity_Falling);  // Chuyển sang cạnh xuống
                        capture_flag = 1;
                    } else {  // Cạnh xuống
                        capture_value2 = TIM_GetCapture1(TIM2);
                        TIM_SetIC1Polarity(TIM2, TIM_ICPolarity_Rising);  // Quay lại cạnh lên
                        capture_flag = 0;
                        // Tính độ rộng xung (μs): (capture_value2 - capture_value1) * 1 μs
                    }
                    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
                }
            }      

* **2.3.2. Output Compare** 

    * **Mục đích:** So sánh giá trị counter với một ngưỡng (CCR) để tạo sự kiện, như đặt/xóa/toggle chân GPIO hoặc kích hoạt ngắt.

    * **Ứng dụng:** 

        ◦ Tạo tín hiệu PWM (chế độ PWM là một dạng Output Compare).

        ◦ Tạo xung đơn (One-Pulse Mode).

        ◦ Tạo tín hiệu định kỳ với độ rộng cụ thể.

    * **Nguyên lý:** 

        ◦ Counter được so sánh với giá trị trong thanh ghi CCRx.

        ◦ Khi counter = CCRx, xảy ra sự kiện (ví dụ: đặt chân GPIO lên high/low, toggle, hoặc tạo ngắt).
    
    * **Cấu hình:** 

        ◦ **1. Bật clock:** Như trên

        ◦ **2. Cấu hình TimeBase:** Như trên

        ◦ **3. Cấu hình Output Compare**

        ```
        Struct: TIM_OCInitTypeDef
        ```
        ```
        TIM_OCMode: Chế độ so sánh:

            TIM_OCMode_Timing: Chỉ tạo ngắt, không ảnh hưởng chân GPIO.
            TIM_OCMode_Active: Đặt chân high khi counter = CCR.
            TIM_OCMode_Inactive: Đặt chân low.
            TIM_OCMode_Toggle: Đổi trạng thái chân.
            TIM_OCMode_PWM1: PWM mode 1 (high khi counter < CCR, low khi ≥ CCR).
            TIM_OCMode_PWM2: PWM mode 2 (ngược PWM1).
        ```        
        ```
        TIM_OutputState: TIM_OutputState_Enable (kích hoạt output) hoặc TIM_OutputState_Disable (chỉ ngắt).
        ```  
        ```
        TIM_OCPolarity: Trạng thái output
            
            TIM_OCPolarity_High: High khi active
            TIM_OCPolarity_Low: Low khi active
        ```  
        ```
        Hàm: TIM_OCxInit(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct) (x = 1, 2, 3, 4).
        ``` 

        ◦ **4. Bật ngắt Compare:** `TTIM_ITConfig(TIMx, TIM_IT_CCx, ENABLE)`.    

        ◦ **5. Cấu hình NVIC:** Như trên 

        ◦ **6. Bật Timer:** `TIM_Cmd(TIMx, ENABLE)`.`  

    * **VD: Toggle chân PA0 mỗi 500 μs bằng TIM2 Channel 1** 

            void OutputCompare_Config() {
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

                // Cấu hình PA0 làm output cho TIM2 Channel 1
                GPIO_InitTypeDef GPIO_InitStruct;
                GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
                GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;  // Alternate Function Push-Pull
                GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
                GPIO_Init(GPIOA, &GPIO_InitStruct);

                // Cấu hình TimeBase: Tick = 1 μs
                TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
                TIM_TimeBaseInitStruct.TIM_Prescaler = 71;  // 72 MHz / 72 = 1 MHz
                TIM_TimeBaseInitStruct.TIM_Period = 65535;
                TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
                TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
                TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

                // Cấu hình Output Compare
                TIM_OCInitTypeDef TIM_OCInitStruct;
                TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_Toggle;
                TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
                TIM_OCInitStruct.TIM_Pulse = 500;  // Toggle sau 500 μs
                TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
                TIM_OC1Init(TIM2, &TIM_OCInitStruct);

                // Bật ngắt Compare Channel 1
                TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

                // Cấu hình NVIC
                NVIC_InitTypeDef NVIC_InitStruct;
                NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
                NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
                NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
                NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
                NVIC_Init(&NVIC_InitStruct);

                // Bật Timer
                TIM_Cmd(TIM2, ENABLE);
            }

            void TIM2_IRQHandler(void) {
                if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) {
                    TIM_SetCompare1(TIM2, TIM_GetCapture1(TIM2) + 500);  // Cập nhật CCR1 cho lần toggle tiếp theo
                    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
                }
            }

* **2.3.3. One-Pulse Mode (OPM)** 

    * **Khái niệm:**

        ◦ Timer chỉ thực hiện một chu kỳ đếm (từ 0 đến giá trị TIM_Period hoặc ngược lại, tùy cấu hình) và sau đó tự động dừng, thay vì đếm liên tục như các chế độ Timer thông thường.

        ◦ Chế độ này thường được kết hợp với Output Compare hoặc PWM để tạo ra một xung (pulse) có độ rộng chính xác trên chân GPIO, hoặc chỉ tạo sự kiện ngắt mà không cần output ra chân.        

    * **Mục đích:** 

        ◦ Tạo một xung đơn (single pulse) với độ rộng và thời gian được kiểm soát chính xác.

        ◦ Thực hiện một tác vụ định thời chỉ một lần (ví dụ: kích hoạt thiết bị trong một khoảng thời gian cụ thể).

        ◦ Kết hợp với các tín hiệu Trigger bên ngoài để khởi động Timer, tạo xung theo sự kiện cụ thể.

    * **Ứng dụng:** 

        ◦ Tạo xung đơn với độ rộng chính xác (ví dụ: kích hoạt thiết bị trong thời gian cụ thể).

        ◦ Điều khiển servo hoặc tạo tín hiệu điều chế đặc biệt..

    * **Nguyên lý:** 

        ◦ Timer được cấu hình để chỉ đếm một chu kỳ (từ 0 đến Period) rồi dừng (bit OPM trong thanh ghi CR1 được bật).

        ◦ Khi counter đạt TIM_Period, Timer tạo một Update Event và tự động dừng (bit OPM trong thanh ghi CR1 được đặt thành 1, khiến Timer vô hiệu hóa sau khi hoàn thành chu kỳ).

        ◦ Không cần ngắt Update nếu chỉ dùng để tạo xung, vì Timer tự dừng.

        ◦ Một kênh của Timer (Channel 1-4) được cấu hình ở chế độ Output Compare để điều khiển trạng thái chân GPIO (high, low, hoặc toggle) khi counter đạt giá trị TIM_Pulse (lưu trong thanh ghi CCRx).

        ◦ Timer có thể được cấu hình để khởi động bởi một Trigger (như cạnh lên của GPIO hoặc tín hiệu từ Timer khác), thay vì bật bằng TIM_Cmd.    

    * **Công thức:** 

        ◦ Thời gian của xung (nếu dùng Output Compare):

            Pulse width = TIM_Pulse x Tick time
            
            Với  Tick time = TIM_Prescaler + 1 / Timer Clock

        ◦ Thời gian toàn bộ chu kỳ (từ khi bắt đầu đến khi dừng):

            Total duration = (TIM_Period + 1) x (TIM_Prescaler + 1) / Timer clock

    * **Cấu hình:** 

        ◦ **1. Bật clock:** Như trên

        ◦ **2. Cấu hình TimeBase:** 
        
        ```
        TIM_TimeBaseInitTypeDef, bổ sung trường TIM_OnePulseMode
        ```
        ```
        TIM_OPMode_Single: Bật One-Pulse Mode (Timer dừng sau một chu kỳ).

        TIM_OPMode_Repetitive: Chế độ lặp lại (mặc định, không dùng trong OPM).
        ```  

        ◦ **3. Cấu hình Output Compare:** (nếu cần tạo xung trên chân GPIO): Như phần Output Compare.

        ◦ **4. Bật Timer:** 

        ```
        Dùng TIM_Cmd(TIMx, ENABLE) để khởi động Timer. Timer sẽ chạy một lần và tự dừng.
        ``` 
         
        ```
        Nếu dùng Trigger, cấu hình thêm TIM_SelectInputTrigger và TIM_SelectSlaveMode.
        ``` 
    * **VD: Tạo xung 1 ms trên PA0 với TIM2 Channel 1** 

            void OnePulse_Config() {
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

                // Cấu hình PA0 làm output cho TIM2 Channel 1
                GPIO_InitTypeDef GPIO_InitStruct;
                GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
                GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
                GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
                GPIO_Init(GPIOA, &GPIO_InitStruct);

                // Cấu hình TimeBase: Tick = 1 μs, Period = 1 ms
                TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
                TIM_TimeBaseInitStruct.TIM_Prescaler = 71;  // 72 MHz / 72 = 1 MHz
                TIM_TimeBaseInitStruct.TIM_Period = 999;    // 1000 ticks = 1 ms
                TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
                TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
                TIM_TimeBaseInitStruct.TIM_OnePulseMode = TIM_OPMode_Single;  // One-Pulse
                TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

                // Cấu hình Output Compare: Active khi counter = 500
                TIM_OCInitTypeDef TIM_OCInitStruct;
                TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_Active;
                TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
                TIM_OCInitStruct.TIM_Pulse = 500;  // Xung high tại 500 μs
                TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
                TIM_OC1Init(TIM2, &TIM_OCInitStruct);

                // Bật Timer (chạy 1 lần và dừng)
                TIM_Cmd(TIM2, ENABLE);
            }

* **2.3.4. Encoder Interface Mode** 

    * **Khái niệm:** 

        ◦ Encoder Interface Mode là một chế độ chuyên biệt của Timer trên STM32F103 (chủ yếu hỗ trợ trên TIM1-5, vì TIM6-7 là Basic Timer không có kênh Input)

        ◦ Cho phép Timer giao tiếp trực tiếp với các loại encoder quay (quadrature encoder hoặc incremental encoder)

        ◦ Chế độ này biến Timer thành một bộ đếm encoder, tự động xử lý tín hiệu từ encoder để đo lường vị trí, hướng quay, và tốc độ mà không cần CPU can thiệp liên tục. 

    * **Mục đích:** 

        ◦ Đo vị trí góc quay (position) hoặc số vòng quay (revolutions).

        ◦ Phát hiện hướng quay (thuận chiều hay ngược chiều).

        ◦ Tính toán tốc độ quay (speed) dựa trên sự thay đổi counter.

        ◦ Giảm tải cho CPU bằng cách để phần cứng Timer xử lý tín hiệu encoder.

    * **Ứng dụng:** 

        ◦ Điều khiển robot: Đo vị trí bánh xe để tính toán quãng đường di chuyển, định vị robot (odometry), hoặc điều khiển PID cho động cơ.

        ◦ Điều khiển động cơ: Theo dõi góc quay của động cơ DC, stepper, hoặc servo với encoder tích hợp.
    
        ◦ Hệ thống đo lường: Đếm số xung từ encoder để đo tốc độ

        ◦ Giao tiếp thiết bị: Kết nối với encoder quang học, từ tính, hoặc Hall-effect để đo chuyển động tuyến tính hoặc quay.

    * **Nguyên lý:** 

        ◦ Encoder quadrature (còn gọi là encoder hai pha) tạo ra hai tín hiệu vuông A và B (thường là tín hiệu TTL 5V hoặc 3.3V), lệch pha 90°.

        ◦ Hướng quay: Được xác định bằng thứ tự pha

               Nếu A dẫn trước B: Quay thuận (counter tăng)
               Nếu B dẫn trước A: Quay ngược (counter giảm).

        ◦ Mỗi encoder có độ phân giải (pulses per revolution - PPR), ví dụ: 1000 PPR nghĩa là 1000 xung mỗi vòng quay.

    * **Vai trò của Timer:** 

        ◦ Timer sử dụng hai kênh Input Capture (Channel 1 và Channel 2) để đọc tín hiệu A (kết nối với TI1) và B (kết nối với TI2).

        ◦ Counter của Timer (CNT) tự động tăng/giảm dựa trên cạnh (rising/falling) của tín hiệu A và B, mà không cần phần mềm xử lý từng xung.

        ◦ Hỗ trợ các chế độ:

               Encoder Mode 1 (TIM_EncoderMode_TI1): Đếm chỉ dựa trên cạnh của Channel 1 (A), Channel 2 (B) chỉ dùng để xác định hướng. Độ phân giải thấp hơn (1x PPR).


               Encoder Mode 2 (TIM_EncoderMode_TI2): Đếm chỉ dựa trên cạnh của Channel 2 (B), Channel 1 (A) xác định hướng. Tương tự Mode 1.


               Encoder Mode 3 (TIM_EncoderMode_TI12): Đếm dựa trên cạnh của cả hai kênh (A và B), tăng độ phân giải lên gấp 4 lần (4x PPR), vì đếm cả rising và falling edge.

        ◦ Polarity: Xác định cạnh phát hiện (rising hoặc falling) để phù hợp với tín hiệu encoder (thường là rising cho cả hai).

    * **Hoạt động counter:** 

        ◦ Counter bắt đầu từ 0 (hoặc giá trị tùy chỉnh)

        ◦ Mỗi xung encoder làm counter tăng/giảm 1 (hoặc nhiều hơn tùy mode)

        ◦ Khi counter đạt TIM_Period (thường 65535 cho Timer 16-bit), tạo Update Event và có thể ngắt nếu bật.

    * **Công thức:** 

        ◦ Vị trí góc (degree)


        <img width="569" height="116" alt="Image" src="https://github.com/user-attachments/assets/1e764654-720c-4d94-8de2-5c35f207cb6c" />



        ◦ Tốc độ quay (RPM - revolutions per minute)


        <img width="688" height="143" alt="Image" src="https://github.com/user-attachments/assets/5768429c-c068-47df-8f34-08feea848958" />



        ◦ Số vòng quay


        <img width="387" height="89" alt="Image" src="https://github.com/user-attachments/assets/82265445-e70d-4c8b-bf36-cf7f757a9efe" />


    * **Cấu hình:** 

        ◦ **1. Bật clock:** Như trên

        ◦ **2. Cấu hình TimeBase:** Chỉ cần Prescaler và Period (Period thường đặt tối đa để tránh tràn).
   
        ◦ **3. Cấu hình Encoder**

        ```
        Hàm: TIM_EncoderInterfaceConfig(TIM_TypeDef* TIMx, uint16_t TIM_EncoderMode, uint16_t TIM_IC1Polarity, uint16_t TIM_IC2Polarity)
        ```
        ```
        TIM_EncoderMode:

            TIM_EncoderMode_TI1 : Đếm trên TI1 (Channel 1).
            TIM_EncoderMode_TI2 : Đếm trên TI2 (Channel 2).
            IM_EncoderMode_TI12 : Đếm trên cả TI1 và TI2 (khuyến nghị cho độ chính xác cao).
        ```        
        ```
        TIM_IC1Polarity, TIM_IC2Polarity:

            TIM_ICPolarity_Rising : Phát hiện cạnh lên.
            TIM_ICPolarity_Falling : Phát hiện cạnh xuống (ít dùng, tùy encoder)
        ```  
        

        ◦ **4. Bật Timer:** Counter sẽ tự động đếm dựa trên tín hiệu encoder.     

    * **VD: Đọc encoder trên TIM2, Channel 1: PA0, Channel 2: PA1** 

            void Encoder_Config() {
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

                // Cấu hình PA0, PA1 làm input cho encoder
                GPIO_InitTypeDef GPIO_InitStruct;
                GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
                GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
                GPIO_Init(GPIOA, &GPIO_InitStruct);

                // Cấu hình TimeBase
                TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
                TIM_TimeBaseInitStruct.TIM_Prescaler = 0;  // Không chia để đếm chính xác
                TIM_TimeBaseInitStruct.TIM_Period = 65535;  // Max để tránh tràn
                TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
                TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
                TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

                // Cấu hình Encoder Mode
                TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

                // Bật Timer
                TIM_Cmd(TIM2, ENABLE);
            }

            // Đọc giá trị encoder
            uint16_t Read_Encoder() {
                return TIM_GetCounter(TIM2);  // Lấy giá trị counter
            }


* **2.3.5. Hall Sensor Interface** 

    * **Khái niệm:** 

        ◦ Hall Sensor Interface Mode là một chế độ đặc biệt của Timer trên STM32F103 (chủ yếu hỗ trợ trên TIM1 và TIM2-4, vì TIM6-7 là Basic Timer không có kênh Input/Output)

        ◦ Được thiết kế để giao tiếp với các cảm biến Hall trong động cơ không chổi (Brushless DC - BLDC)     

        ◦ Chế độ này cho phép Timer tự động phát hiện và xử lý tín hiệu từ ba cảm biến Hall (Hall A, B, C) để xác định vị trí rotor, từ đó hỗ trợ điều khiển chính xác quá trình chuyển pha (commutation) của động cơ BLDC.

    * **Mục đích:** 
    
        ◦ Xác định vị trí rotor của động cơ BLDC để điều khiển thời điểm chuyển pha.

        ◦ Tự động capture giá trị counter khi có sự thay đổi tín hiệu Hall, từ đó đo thời gian giữa các trạng thái pha.

        ◦ Giảm tải cho CPU bằng cách để phần cứng Timer xử lý tín hiệu Hall và tạo ngắt hoặc Trigger cho các tác vụ điều khiển.

    * **Ứng dụng:** 

        ◦ Điều khiển động cơ BLDC: Sử dụng trong robot, quạt, xe điện, hoặc máy bơm, nơi động cơ BLDC yêu cầu điều khiển pha chính xác dựa trên vị trí rotor.

    * **Nguyên lý:** 

        ◦ Động cơ BLDC thường có 3 cảm biến Hall (Hall A, Hall B, Hall C) được đặt cách nhau 120° điện, tạo ra tín hiệu số (0 hoặc 1) dựa trên vị trí từ trường của rotor.

        ◦ Mỗi cảm biến Hall xuất ra tín hiệu vuông (TTL 5V/3.3V), và tổ hợp trạng thái của ba tín hiệu (6 trạng thái chính) cho biết vị trí rotor tương ứng với các pha (U, V, W).

            Hall A | Hall B | Hall C | Rotor Position | Commutation Step
            -------|-------|-------|----------------|-----------------
            1    |  0    |  1    |     0°-60°     | Step 1
            1    |  0    |  0    |    60°-120°    | Step 2
            1    |  1    |  0    |   120°-180°    | Step 3
            0    |  1    |  0    |   180°-240°    | Step 4
            0    |  1    |  1    |   240°-300°    | Step 5
            0    |  0    |  1    |   300°-360°    | Step 6


    * **Vai trò của Timer:** 

        ◦ Timer sử dụng 3 kênh Input Capture (Channel 1, 2, 3) để đọc tín hiệu từ ba cảm biến Hall (thường kết nối với chân TI1, TI2, TI3).

        ◦ Các tín hiệu Hall được kết hợp thành một tín hiệu XOR nội bộ (trong Timer), tạo ra tín hiệu Trigger mỗi khi có sự thay đổi trạng thái (commutation event).

        ◦ Khi Trigger xảy ra, giá trị counter của Timer được capture vào thanh ghi CCR1 (Channel 1), và một ngắt có thể được tạo để CPU xử lý (ví dụ: cập nhật PWM cho pha tiếp theo).

        ◦ Timer cũng có thể xuất Trigger để kích hoạt Timer khác (ví dụ: Timer PWM) hoặc DMA.        

    * **Chế độ hoạt động:** 

        ◦ **Hall Sensor Mode:** Bật bằng `TIM_SelectHallSensor(TIMx, ENABLE)`

        ◦ Tín hiệu XOR được lọc (digital filter) để giảm nhiễu trước khi tạo Trigger.

        ◦ Capture giá trị counter giúp đo thời gian giữa các lần chuyển pha, từ đó tính tốc độ quay:


        <img width="926" height="100" alt="Image" src="https://github.com/user-attachments/assets/e4e1dcfa-9ceb-41d0-aff6-3b4fe70bffec" />


    * **Cấu hình:** 

        ◦ **1. Bật clock:** 

             Dùng RCC_APB1PeriphClockCmd (cho TIM2-4) hoặc RCC_APB2PeriphClockCmd (cho TIM1).
             Bật clock cho GPIO chứa chân Hall (thường là Channel 1, 2, 3) và AFIO.

        ◦ **2. Cấu hình TimeBase:** Chỉ cần Prescaler và Period (Period thường đặt tối đa để tránh tràn).
   
        ◦ **3. Cấu hình Hall Sensor**

        ```
        Hàm: TIM_SelectHallSensor(TIM_TypeDef* TIMx, FunctionalState NewState)
        NewState: ENABLE để bật chế độ Hall.
        ```
        ```
        Cấu hình Input Capture trên Channel 1 để capture tín hiệu XOR:
        
        ```
        ```
        Struct: TIM_ICInitTypeDef

        TIM_Channel: TIM_Channel_1 (capture trên TI1FP1, tín hiệu XOR của Hall).
        TIM_ICPolarity: TIM_ICPolarity_Rising hoặc TIM_ICPolarity_Falling (tùy cảm biến Hall).
        TIM_ICSelection: TIM_ICSelection_TRC (chọn tín hiệu XOR nội bộ).
        TIM_ICPrescaler: TIM_ICPSC_DIV1 (không chia).
        TIM_ICFilter: Lọc nhiễu (0-15, ví dụ: 0x4 để giảm nhiễu nhẹ).
        ```        
        ```
        Hàm: TIM_ICInit(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct)
        ```  
        

        ◦ **4. Bật ngắt Capture (nếu cần):** `TIM_ITConfig(TIMx, TIM_IT_CC1, ENABLE)` để tạo ngắt khi capture giá trị counter.

        ◦ **5. Cấu hình NVIC**

        ◦ **6. Bật Timer** `TIM_Cmd(TIMx, ENABLE)` để bắt đầu đếm.
        
    * **VD: Đọc tín hiệu Hall trên TIM2 (PA0, PA1, PA2)** 

        ◦ Giả sử động cơ BLDC có 3 cảm biến Hall kết nối với PA0 (TIM2_CH1), PA1 (TIM2_CH2), PA2 (TIM2_CH3).

            volatile uint32_t hall_time1 = 0, hall_time2 = 0;
            volatile float speed_rpm = 0;

            void HallSensor_Config() {
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

                // Cấu hình PA0, PA1, PA2 làm input cho cảm biến Hall
                GPIO_InitTypeDef GPIO_InitStruct;
                GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
                GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;  // Input không pull
                GPIO_Init(GPIOA, &GPIO_InitStruct);

                // Cấu hình TimeBase
                TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
                TIM_TimeBaseInitStruct.TIM_Prescaler = 71;  // 72 MHz / 72 = 1 MHz (1 μs/tick)
                TIM_TimeBaseInitStruct.TIM_Period = 65535;  // Max để tránh tràn
                TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
                TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
                TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

                // Bật chế độ Hall Sensor
                TIM_SelectHallSensor(TIM2, ENABLE);

                // Cấu hình Input Capture trên Channel 1 (tín hiệu XOR)
                TIM_ICInitTypeDef TIM_ICInitStruct;
                TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
                TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;  // Phát hiện cạnh lên của XOR
                TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_TRC;   // Tín hiệu XOR nội bộ
                TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
                TIM_ICInitStruct.TIM_ICFilter = 0x4;  // Lọc nhiễu nhẹ
                TIM_ICInit(TIM2, &TIM_ICInitStruct);

                // Bật ngắt Capture Channel 1
                TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

                // Cấu hình NVIC
                NVIC_InitTypeDef NVIC_InitStruct;
                NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
                NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
                NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
                NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
                NVIC_Init(&NVIC_InitStruct);

                // Bật Timer
                TIM_Cmd(TIM2, ENABLE);
            }

            // Hàm xử lý ngắt
            void TIM2_IRQHandler(void) {
                if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) {
                    hall_time2 = TIM_GetCapture1(TIM2);  // Lấy giá trị counter
                    uint32_t delta_time = hall_time2 - hall_time1;  // Thời gian giữa 2 lần capture
                    // Tính tốc độ (RPM) với động cơ có 2 cặp cực (pole pairs = 2)
                    speed_rpm = (60.0 * 1000000) / (delta_time * 2 * 6);  // 6 trạng thái mỗi vòng
                    hall_time1 = hall_time2;  // Cập nhật giá trị trước
                    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
                }
            }

* **2.3.6. Trigger Mode và Slave Mode** 

    * **Khái niệm:** 

        ◦ Trigger Mode và Slave Mode là các chế độ của Timer trên STM32F103 (hỗ trợ trên TIM1-5, vì TIM6-7 là Basic Timer chỉ có chức năng đếm cơ bản) nhằm đồng bộ hóa hoạt động của Timer với các sự kiện bên ngoài hoặc nội bộ

        ◦ Trong chế độ này, Timer hoạt động như một slave, được điều khiển bởi một nguồn Trigger (như tín hiệu từ GPIO, Timer khác, hoặc sự kiện nội bộ). 

        ◦ Cho phép Timer khởi động, dừng, reset, hoặc đếm theo các sự kiện cụ thể, thay vì chạy tự do.

    * **Mục đích:** 
    
        ◦ Đồng bộ nhiều Timer để tạo các tín hiệu PWM lệch pha hoặc các sự kiện định thời liên quan.

        ◦ Kích hoạt Timer bằng tín hiệu ngoài (như nhấn nút, cảm biến, hoặc tín hiệu từ thiết bị ngoại vi).

        ◦ Điều khiển chính xác thời gian hoạt động của Timer dựa trên sự kiện Trigger.

        ◦ Kết hợp với các chế độ khác (như PWM, Input Capture) để tạo hệ thống điều khiển phức tạp.       

    * **Ứng dụng:** 

        ◦ Đồng bộ PWM lệch pha: Nhiều Timer được đồng bộ để tạo tín hiệu PWM cho động cơ BLDC, inverter, hoặc hệ thống năng lượng.

        ◦ Đo thời gian sự kiện: Dùng Trigger để reset hoặc khởi động Timer, đo khoảng thời gian giữa các sự kiện.   

        ◦ Điều khiển động cơ: Đồng bộ Timer PWM với Timer đo tín hiệu Hall hoặc encoder

        ◦ Hệ thống thời gian thực: Tạo các chuỗi sự kiện định thời chính xác, ví dụ: gửi dữ liệu UART sau khi nhận Trigger từ Timer khác.

    * **Nguyên lý:** 

        ◦ **Slave Mode:** Timer hoạt động như một slave, nghĩa là nó không tự chạy mà được điều khiển bởi một nguồn Trigger thông qua thanh ghi SMCR (Slave Mode Control Register).

            Các chế độ Slave Mode:

            TIM_SlaveMode_Reset: Reset counter (CNT) về 0 khi nhận Trigger. Dùng để đồng bộ counter với sự kiện.

            TIM_SlaveMode_Gated: Timer chỉ đếm khi tín hiệu Trigger ở mức cao (hoặc thấp, tùy cấu hình). Dùng để bật/tắt Timer theo tín hiệu ngoài.

            TIM_SlaveMode_Trigger: Timer chỉ khởi động khi nhận Trigger (cạnh lên hoặc xuống). Dùng để bắt đầu đếm theo sự kiện.
            
            TIM_SlaveMode_External1: Dùng tín hiệu ngoài làm clock cho Timer (ít phổ biến).            

        ◦ **Trigger Source:** 

            Nguồn Trigger được chọn qua thanh ghi SMCR, bao gồm:

                GPIO: Tín hiệu từ chân GPIO (TI1FP1, TI2FP2, hoặc filtered input).
                Timer khác: Trigger Output (TRGO) từ Timer khác (TIMx_TRGO).
                Sự kiện nội bộ: Như tín hiệu từ ADC, RTC, hoặc PVD.                

            Các nguồn Trigger phổ biến (TIM_InputTriggerSource):

                TIM_TS_ITR0, TIM_TS_ITR1, ...: Trigger từ Timer khác (ITR = Internal Trigger).
                TIM_TS_TI1FP1, TIM_TS_TI2FP2: Tín hiệu từ GPIO qua bộ lọc (filtered input).
                TIM_TS_TI1F_ED: Tín hiệu từ Channel 1 với cạnh bất kỳ (rising/falling).
                TIM_TS_ETRF: External Trigger (thường từ chân ETR). 

    * **Hoạt động:** 

        ◦ Timer chờ Trigger để thực hiện hành động (reset, start, gate)

        ◦ Khi Trigger xảy ra, Timer thực hiện hành động theo chế độ Slave Mode

            Reset: CNT = 0, Timer tiếp tục đếm nếu đã bật.
            Gated: Timer đếm khi Trigger = 1, dừng khi Trigger = 0.
            Trigger: Timer bắt đầu đếm từ 0 đến TIM_Period, tạo Update Event.             

    * **Công thức:** 

        ◦ Thời gian đếm (nếu Trigger khởi động Timer):

        <img width="592" height="92" alt="Image" src="https://github.com/user-attachments/assets/94073b06-8ecc-4941-b2f4-ca20da4c3137" />

        ◦ Tần số Trigger (nếu dùng để đo)

        <img width="426" height="84" alt="Image" src="https://github.com/user-attachments/assets/602a3214-c7c3-4fa8-83e3-ab3170aac137" />

    * **Cấu hình:** 

        ◦ **1. Bật clock:** 

             Dùng RCC_APB1PeriphClockCmd (cho TIM2-5) hoặc RCC_APB2PeriphClockCmd (cho TIM1).
             Bật clock cho GPIO nếu dùng Trigger từ chân GPIO.

        ◦ **2. Cấu hình TimeBase:** Chỉ cần Prescaler và Period (Period thường đặt tối đa để tránh tràn).
   
        ◦ **3. Cấu hình Slave Mode và Trigger**

        ```
        Hàm: TIM_SelectSlaveMode(TIM_TypeDef* TIMx, uint16_t TIM_SlaveMode)
        ```
        ```
        TIM_SlaveMode:

        TIM_SlaveMode_Reset: Reset counter khi Trigger.
        TIM_SlaveMode_Gated: Đếm khi Trigger active.
        TIM_SlaveMode_Trigger: Khởi động Timer khi Trigger.
        TIM_SlaveMode_External1: Dùng Trigger làm clock (hiếm dùng).
        
        ```
        ```
        Hàm: TIM_SelectInputTrigger(TIM_TypeDef* TIMx, uint16_t TIM_InputTriggerSource)
        ```
        ```
        TIM_InputTriggerSource:

        TIM_TS_ITR0, TIM_TS_ITR1, ...: Trigger từ Timer khác.
        TIM_TS_TI1FP1, TIM_TS_TI2FP2: Trigger từ GPIO (Channel 1, 2).
        TIM_TS_ETRF: Trigger từ chân ETR (External Trigger).
        
        ```
        ```
        Nếu dùng GPIO, cấu hình chân làm input (thường GPIO_Mode_IN_FLOATING).
        ```  
        

        ◦ **4. Bật ngắt (tùy chọn):** `TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE)` để tạo ngắt khi Timer đạt TIM_Period.

        ◦ **5. Cấu hình NVIC**

        ◦ **6. Bật Timer:** `TIM_Cmd(TIMx, ENABLE)` Timer chờ Trigger (nếu ở Trigger Mode) hoặc đếm ngay (nếu ở Reset/Gated Mode).

    * **VD: TIM2 khởi động bởi cạnh lên của PA0** 

            volatile uint32_t counter_value = 0;

            void Trigger_Config() {
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

                // Cấu hình PA0 làm Trigger input (TIM2_CH1)
                GPIO_InitTypeDef GPIO_InitStruct;
                GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
                GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
                GPIO_Init(GPIOA, &GPIO_InitStruct);

                // Cấu hình TimeBase
                TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
                TIM_TimeBaseInitStruct.TIM_Prescaler = 71;  // 72 MHz / 72 = 1 MHz (1 μs/tick)
                TIM_TimeBaseInitStruct.TIM_Period = 999;    // 1 ms
                TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
                TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
                TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

                // Cấu hình Trigger và Slave Mode
                TIM_SelectInputTrigger(TIM2, TIM_TS_TI1FP1);  // Trigger từ PA0 (Channel 1)
                TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Trigger);  // Khởi động Timer khi Trigger

                // Bật ngắt Update (tùy chọn)
                TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

                // Cấu hình NVIC
                NVIC_InitTypeDef NVIC_InitStruct;
                NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
                NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
                NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
                NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
                NVIC_Init(&NVIC_InitStruct);

                // Bật Timer (chờ Trigger)
                TIM_Cmd(TIM2, ENABLE);
            }

            // Hàm xử lý ngắt
            void TIM2_IRQHandler(void) {
                if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
                    counter_value = TIM_GetCounter(TIM2);  // Lấy giá trị counter
                    GPIO_ToggleBits(GPIOC, GPIO_Pin_13);  // Bật/tắt LED (giả sử PC13)
                    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
                }
            }

* **2.3.7. Break and Dead-Time (TIM1)** 

    * **Khái niệm:** 

        ◦ Break and Dead-Time là các tính năng nâng cao của TIM1 (Advanced Timer) trên STM32F103, được thiết kế để bảo vệ và tối ưu hóa điều khiển PWM trong các ứng dụng liên quan đến động cơ, đặc biệt là động cơ không chổi (BLDC) hoặc các mạch H-bridge

        ◦ Break: Ngắt khẩn cấp tín hiệu PWM khi phát hiện lỗi (như quá dòng, quá áp, hoặc tín hiệu lỗi từ ngoài), ngăn chặn hư hỏng phần cứng.

        ◦ Dead-Time: Tạo khoảng thời gian trễ (dead-time) giữa các tín hiệu PWM bổ sung (complementary PWM) để tránh hiện tượng ngắn mạch (shoot-through) trong H-bridge.

    * **Mục đích:** 
    
        ◦ Bảo vệ hệ thống: Dừng PWM ngay lập tức khi có lỗi để bảo vệ động cơ hoặc mạch công suất.

        ◦ Tối ưu PWM: Đảm bảo các tín hiệu PWM bổ sung (high-side và low-side) không bật cùng lúc, tránh ngắn mạch.
      

    * **Ứng dụng:** 

        ◦ Điều khiển động cơ BLDC: Sử dụng trong robot, quạt, xe điện, hoặc máy bơm, nơi cần PWM bổ sung và bảo vệ quá dòng.

        ◦ Mạch H-bridge hoặc inverter: Ngăn ngắn mạch trong các mạch công suất sử dụng transistor (MOSFET/IGBT). 

        ◦ Hệ thống năng lượng: Điều khiển PWM trong bộ biến tần (inverter) hoặc bộ nguồn chuyển mạch (SMPS).


    * **Nguyên lý:** 

     ◦ **Break:**

    ```
    Chức năng:

    Khi phát hiện lỗi (qua chân BKIN hoặc nguồn nội bộ), Timer ngay lập tức vô hiệu hóa tất cả tín hiệu PWM (đặt các chân PWM về trạng thái an toàn, thường là low hoặc high-impedance).
    ```
    
    ```
    Nguồn Break:

            Chân BKIN (Break Input): Một chân GPIO (thường là PA6 trên TIM1) nhận tín hiệu lỗi từ mạch bảo vệ ngoài (như cảm biến quá dòng).
            
            Nguồn nội bộ: Lỗi từ comparator, PVD (Power Voltage Detector), hoặc phần mềm (bit BKE trong thanh ghi BDTR).      
    ```
    ```
    Hành vi:

            Khi Break xảy ra, tất cả kênh PWM của TIM1 (bao gồm cả kênh bổ sung) bị tắt.
            
            Có thể cấu hình trạng thái an toàn (OSS = Off-State Selection) khi Break:

                TIM_BreakPolarity_Low: Tín hiệu BKIN active-low (mức thấp = lỗi).
                TIM_BreakPolarity_High: Tín hiệu BKIN active-high (mức cao = lỗi).

            Ngắt Break có thể được bật để CPU xử lý
    ```

    ◦ **Dead-Time:** 

    ```
    Chức năng:

            Tạo khoảng thời gian trễ (dead-time) giữa hai tín hiệu PWM bổ sung (complementary PWM) trên cùng một kênh để đảm bảo transistor high-side và low-side không bật đồng thời.         
    ```
    ```
    Cơ chế:

            Trong H-bridge, mỗi pha cần hai tín hiệu PWM (chính và bổ sung) để điều khiển transistor high-side và low-side.

            Nếu cả hai transistor bật cùng lúc, xảy ra ngắn mạch (shoot-through), gây hư hỏng.

            Dead-time chèn một khoảng trễ (thường vài trăm ns đến vài μs) giữa thời điểm tắt tín hiệu này và bật tín hiệu kia. 
    ```

    * **Công thức:** 

        ◦ Thời gian Dead-Time:

             Dead-Time = DTG x t_DT

            ◦ DTG (Dead-Time Generator): Giá trị 0-255 trong thanh ghi BDTR.

            ◦ t_DT (đơn vị thời gian): Phụ thuộc cấu hình clock và DTG


    * **Cấu hình:** 

        ◦ **1. Bật clock:** 

             Dùng RCC_APB2PeriphClockCmd cho TIM1 (Advanced Timer).

             Bật clock cho GPIO (cho PWM và BKIN) và AFIO.

        ◦ **2. Cấu hình TimeBase** 
   
        ◦ **3. Cấu hình PWM**

            Sử dụng struct TIM_OCInitTypeDef để cấu hình các kênh PWM (chính và bổ sung).

            Cần bật complementary output (TIM_OutputNState_Enable).

        ◦ **4. Cấu hình Break và Dead-Time**

        ```
        Struct: TIM_BDTRInitTypeDef
        ```
        ```
        TIM_OSSRState: Off-State Selection for Run mode (trạng thái PWM khi tắt).

        TIM_OSSIState: Off-State Selection for Idle mode (trạng thái PWM khi Break).

        TIM_LOCKLevel: Lock cấu hình BDTR (0-3, 0 = không lock).

        TIM_DeadTime: Giá trị DTG (0-255) xác định dead-time.

        TIM_Break: TIM_Break_Enable hoặc TIM_Break_Disable.

        TIM_BreakPolarity: TIM_BreakPolarity_Low hoặc TIM_BreakPolarity_High.

        TIM_AutomaticOutput: TIM_AutomaticOutput_Enable (tự động khôi phục PWM sau Break).
        
        ```
        ```
        Hàm: TIM_BDTRConfig(TIM_TypeDef* TIMx, TIM_BDTRInitTypeDef* TIM_BDTRInitStruct)
        ```

        ◦ **5. Bật ngắt Break (tùy chọn):** `TIM_ITConfig(TIM1, TIM_IT_Break, ENABLE)` để xử lý sự kiện Break.

        ◦ **6. Cấu hình NVIC:** Dùng `TIM1_BRK_IRQn hoặc TIM1_UP_IRQn`.

        ◦ **7. Bật Timer và PWM :** `TIM_Cmd(TIM1, ENABLE)` để chạy Timer.
`TIM_CtrlPWMOutputs(TIM1, ENABLE)` để bật PWM output.


   * **VD: Cấu hình Break và Dead-Time trên TIM1 (PWM trên PA8, BKIN trên PA6)** 

        ◦ Giả sử cần tạo PWM 10 kHz trên Channel 1 (PA8) và complementary PWM trên Channel 1N, với dead-time 1 μs và Break khi PA6 (BKIN) active-low.

            void Break_DeadTime_Config() {
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

                // Cấu hình PA8 (TIM1_CH1) và PA7 (TIM1_CH1N) cho PWM
                GPIO_InitTypeDef GPIO_InitStruct;
                GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_7;
                GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
                GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
                GPIO_Init(GPIOA, &GPIO_InitStruct);

                // Cấu hình PA6 (BKIN) làm input
                GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
                GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
                GPIO_Init(GPIOA, &GPIO_InitStruct);

                // Cấu hình TimeBase: PWM 10 kHz
                TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
                TIM_TimeBaseInitStruct.TIM_Prescaler = 0;  // 72 MHz (no prescaler)
                TIM_TimeBaseInitStruct.TIM_Period = 7199;  // 72 MHz / 7200 = 10 kHz
                TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
                TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
                TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);

                // Cấu hình PWM Channel 1
                TIM_OCInitTypeDef TIM_OCInitStruct;
                TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
                TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
                TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;  // Complementary PWM
                TIM_OCInitStruct.TIM_Pulse = 3600;  // Duty cycle 50% (3600/7200)
                TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
                TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
                TIM_OC1Init(TIM1, &TIM_OCInitStruct);

                // Cấu hình Break và Dead-Time
                TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
                TIM_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Enable;
                TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Enable;
                TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
                TIM_BDTRInitStruct.TIM_DeadTime = 72;  // ~1 μs (72 * 13.89 ns)
                TIM_BDTRInitStruct.TIM_Break = TIM_Break_Enable;
                TIM_BDTRInitStruct.TIM_BreakPolarity = TIM_BreakPolarity_Low;  // Active-low
                TIM_BDTRInitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
                TIM_BDTRConfig(TIM1, &TIM_BDTRInitStruct);

                // Bật ngắt Break
                TIM_ITConfig(TIM1, TIM_IT_Break, ENABLE);

                // Cấu hình NVIC
                NVIC_InitTypeDef NVIC_InitStruct;
                NVIC_InitStruct.NVIC_IRQChannel = TIM1_BRK_IRQn;
                NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
                NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
                NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
                NVIC_Init(&NVIC_InitStruct);

                // Bật Timer và PWM
                TIM_Cmd(TIM1, ENABLE);
                TIM_CtrlPWMOutputs(TIM1, ENABLE);
            }

            // Hàm xử lý ngắt Break
            void TIM1_BRK_IRQHandler(void) {
                if (TIM_GetITStatus(TIM1, TIM_IT_Break) != RESET) {
                    GPIO_SetBits(GPIOC, GPIO_Pin_13);  // Bật LED báo lỗi (giả sử PC13)
                    TIM_ClearITPendingBit(TIM1, TIM_IT_Break);
                }
            }


* **2.3.8. DMA Burst Mode** 

    * **Khái niệm:** 

        ◦ DMA Burst Mode là một chế độ nâng cao của Timer trên STM32F103 (hỗ trợ trên TIM1-5, vì TIM6-7 là Basic Timer chỉ hỗ trợ DMA cho Update Event)

        ◦ Cho phép kết hợp Timer với DMA (Direct Memory Access) để tự động truyền dữ liệu từ bộ nhớ đến các thanh ghi của Timer (như CCR1, ARR, hoặc PSC) mà không cần CPU can thiệp.

        ◦ Trong chế độ này, Timer kích hoạt yêu cầu DMA tại các sự kiện cụ thể (như Update Event hoặc Capture/Compare), và DMA truyền một khối dữ liệu (burst) vào các thanh ghi Timer để cập nhật liên tục.

    * **Mục đích:** 
    
        ◦ Tự động cập nhật các tham số Timer (như duty cycle PWM, period, hoặc prescaler) mà không cần CPU xử lý từng lần.

        ◦ Tạo các mẫu PWM phức tạp hoặc thay đổi động mà không làm gián đoạn hoạt động của Timer.

        ◦ Cho phép truyền dữ liệu định kỳ (như bảng giá trị PWM) để điều khiển thiết bị ngoại vi.       
      

    * **Ứng dụng:** 

        ◦ **Tạo mẫu PWM phức tạp:** Thay đổi duty cycle hoặc tần số PWM theo một chuỗi giá trị được định sẵn, ví dụ:

                Điều khiển động cơ BLDC với tốc độ thay đổi mượt mà.
                Tạo sóng sin PWM cho inverter hoặc bộ nguồn.            

        ◦ **Truyền dữ liệu định kỳ:** Cập nhật giá trị CCR (Capture/Compare Register) để điều chỉnh PWM hoặc đo thời gian trong Input Capture.

        ◦ **Hệ thống âm thanh hoặc tín hiệu:** Dùng DMA để truyền dữ liệu vào CCR để tạo sóng PWM mô phỏng tín hiệu analog.


    * **Nguyên lý:** 

        ◦ **DMA Burst Mode:**

            Trong chế độ này, Timer gửi yêu cầu DMA (DMA Request) khi xảy ra một sự kiện cụ thể (như Update Event, Capture/Compare Event).

            DMA truyền một khối dữ liệu (burst) từ bộ nhớ (RAM hoặc Flash) đến một hoặc nhiều thanh ghi của Timer (như CCR1, CCR2, ARR, PSC) trong một lần truyền.
            
            Burst có nghĩa là truyền nhiều byte/word liên tiếp vào các thanh ghi liên tục (ví dụ: CCR1, CCR2, CCR3, CCR4).

        ◦ **Sự kiện kích hoạt DMA:** 

            TIM_DMASource:

                TIM_DMA_Update: Kích hoạt DMA khi Timer tạo Update Event (counter đạt TIM_Period hoặc reset về 0).

                TIM_DMA_CC1, TIM_DMA_CC2, ...: Kích hoạt DMA khi xảy ra Capture/Compare trên kênh 1, 2, ...

                TIM_DMA_Trigger: Kích hoạt DMA khi Timer nhận Trigger (nếu dùng Trigger Mode).

            Thanh ghi đích:

                DMA thường truyền dữ liệu vào thanh ghi TIMx_DMAR (DMA Address Register), từ đó dữ liệu được phân phối đến các thanh ghi như CCR1, ARR, ...

                Số lượng thanh ghi được truyền được xác định bởi Burst Length (trong cấu hình DMA).
    

    * **Công thức:** 

        ◦ Thời gian giữa các lần truyền DMA (nếu dùng Update Event):



        <img width="623" height="82" alt="Image" src="https://github.com/user-attachments/assets/af856670-96c9-4d1d-818d-07211c587159" />



        ◦ Số lần truyền DMA:

            Number of transfers = DMA Burst Length

        ◦ Tần số PWM (nếu cập nhật ARR)



        <img width="527" height="85" alt="Image" src="https://github.com/user-attachments/assets/ecea0961-d9e2-408f-a289-c7f5101f00e6" />




    * **Cấu hình:** 

        ◦ **1. Bật clock:** 

             Dùng RCC_APB2PeriphClockCmd (cho TIM1) hoặc RCC_APB1PeriphClockCmd (cho TIM2-5).

             Bật clock cho DMA (thường RCC_AHBPeriph_DMA1).

             Bật clock cho GPIO nếu dùng PWM output.

        ◦ **2. Cấu hình TimeBase** 
   
        ◦ **3. Cấu hình PWM (nếu dùng)**

            Sử dụng struct TIM_OCInitTypeDef để cấu hình các kênh PWM 

        ◦ **4. Cấu hình DMA**

        ```
        Struct: DMA_InitTypeDef
        ```
        ```
        DMA_PeripheralBaseAddr: Địa chỉ thanh ghi Timer (TIMx_DMAR).

        DMA_MemoryBaseAddr: Địa chỉ mảng dữ liệu trong RAM.

        DMA_DIR: DMA_DIR_PeripheralDST (truyền từ RAM → Timer).

        DMA_BufferSize: Số lượng dữ liệu truyền (Burst Length).

        DMA_PeripheralInc: DMA_PeripheralInc_Disable (giữ địa chỉ TIMx_DMAR).

        DMA_MemoryInc: DMA_MemoryInc_Enable (tăng địa chỉ RAM).

        DMA_PeripheralDataSize, DMA_MemoryDataSize: Thường DMA_DataSize_Word (32-bit).

        DMA_Mode: DMA_Mode_Circular (lặp lại) hoặc DMA_Mode_Normal (một lần).
        
        ```
        ```
        Hàm: DMA_Init(DMA_Channel_TypeDef* DMAy_Channelx, DMA_InitTypeDef* DMA_InitStruct)
        ```

        ◦ **5. Bật DMA cho Timer:** `TIM_DMACmd(TIM_TypeDef* TIMx, uint16_t TIM_DMASource, FunctionalState NewState)`

        ◦ **6. Cấu hình Burst Mode:**

        ```
        Hàm: TIM_DMAConfig(TIM_TypeDef* TIMx, uint16_t TIM_DMABase, uint16_t TIM_DMABurstLength)
        ```
        ```
        TIM_DMABase: Thanh ghi bắt đầu truyền (thường TIM_DMABase_CCR1).

        TIM_DMABurstLength: Số thanh ghi truyền (1-18, ví dụ: TIM_DMABurstLength_1Transfer).
        ```

        ◦ **7. Bật ngắt DMA (tùy chọn):** `DMA_ITConfig(DMAy_Channelx, DMA_IT_TC, ENABLE)` để xử lý khi truyền hoàn tất.

        ◦ **8. Cấu hình NVIC (nếu dùng ngắt):** Dùng kênh IRQ của DMA (ví dụ: `DMA1_Channel5_IRQn`)

        ◦ **9. Bật Timer và DMA:** `TIM_Cmd(TIMx, ENABLE) và DMA_Cmd(DMAy_Channelx, ENABLE)`

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

            ENABLE hoặc DISABLE
            
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


## BÀI 8: ADC

## **I. ADC**

### **1. Giới thiệu**

#### **1.1. Định nghĩa**

* ADC (Analog-to-Digital Converter) là 1 mạch điện tử lấy điện áp tương tự làm đầu vào và chuyển đổi nó thành dữ liệu số (1 giá trị đại diện cho mức điện áp trong mã nhị phân).

* ADC có chức năng chuyển đổi tín hiệu analog (liên tục) thành tín hiệu số (rời rạc) để vi điều khiển có thể xử lý.

![Image](https://github.com/user-attachments/assets/bb2f4a55-28b8-4af8-bd70-7f111aead2ec)
 


#### **1.2. Độ phân giải**

* Độ phân giải của ADC xác định số mức (levels) mà tín hiệu analog được chia ra trong quá trình chuyển đổi.

* **Định nghĩa:**
    
   ◦ Độ phân giải là số bit mà ADC sử dụng để biểu diễn giá trị số. Với  n bit, ADC chia phạm vi điện áp thành 2^n mức

* **VD:** 

   ◦ Với STM32F103C8, ADC có độ phân giải 12 bit, tức là tín hiệu analog được chia thành 2^12 = 4096 mức

* **Ý nghĩa:**
    
   ◦ Độ phân giải càng cao, tín hiệu analog được biểu diễn càng chính xác.

   ◦ Độ chính xác của mỗi mức được tính bằng:

        Độ chính xác = Điện áp tham chiếu / Số mức = Vref / 2^n

   ◦ VD:

        Với điện áp tham chiếu 3.3V, độ phân giải 12bit cho độ chính xác mỗi mức là 33/4096 = 0.805 mV


![Image](https://github.com/user-attachments/assets/893e3052-892a-42b7-bcc0-6fafbabe90ea)


#### **1.3. Tần số lấy mẫu**


* Tần số lấy mẫu xác định tần suất mà ADC thực hiện việc lấy mẫu tín hiệu analog.

* **Định nghĩa:** Là số lần ADC lấy mẫu tín hiệu trong một giây, đo bằng đơn vị Hertz (Hz).

* **Nguyên lý Nyquist-Shannon:** Tần số lấy mẫu phải ít nhất gấp đôi tần số cao nhất của tín hiệu analog để tái tạo chính xác tín hiệu:

   ◦  f_s >= 2f_max   

* **Trong STM32:** 

   ◦  Tần số lấy mẫu phụ thuộc vào **thời gian lấy mẫu (ADC_SampleTime)** và **tần số clock của ADC.**

   ◦  Tần số lấy mẫu càng cao, càng thu thập được nhiều mẫu, dẫn đến kết quả chính xác hơn, nhưng tiêu tốn nhiều tài nguyên CPU hơn.   

#### **1.4. Các thông số khác**

* **Điện áp tham chiếu:** 

   ◦  Là phạm vi điện áp analog mà ADC có thể đo, thường là 0 đến 3.3V hoặc 0 đến 5V trên STM32.

   ◦  Giá trị đo được sẽ nằm trong khoảng từ 0 đến 2^n - 1

* **Thời gian chuyển đổi:** 

   ◦  Là thời gian cần để ADC hoàn thành một lần chuyển đổi từ tín hiệu analog sang số.

   ◦  Công thức:

        T_conv = T_sample + T_conversion

        Trong đó:

        T_sample: Thời gian lấy mẫu (có thể cấu hình, ví dụ: 1.5, 7.5, 55.5 chu kỳ).
        T_conversion: Thời gian chuyển đổi cố định (thường 12.5 chu kỳ cho STM32).

### **2. Sử dụng ADC trong STM32**

#### **2.1. Đặc điểm**

* STM32F103C8 được trang bị 2 bộ ADC (ADC1 và ADC2) với các đặc điểm chính:

   ◦ **Độ phân giải:** 12 bit(4096 mức)

   ◦ **Thanh ghi dữ liệu:** Giá trị chuyển đổi được lưu trong thanh ghi 16 bit (căn lề trái hoặc phải).

   ◦ **Ngắt (Interrupt):** Hỗ trợ ngắt khi hoàn thành chuyển đổi (EOC), lỗi, hoặc các sự kiện khác.

   ◦ **Trigger ngoài:** Có thể kích hoạt chuyển đổi bằng tín hiệu từ timer, GPIO, hoặc nguồn khác.

   ◦ **DMA (Direct Memory Access):** Hỗ trợ truyền dữ liệu trực tiếp từ ADC đến bộ nhớ, giảm tải cho CPU.

   ◦ **Thời gian chuyển đổi nhanh:** Tùy thuộc vào cấu hình, thường từ vài chu kỳ clock.

#### **2.2. Các chế độ của ADC**

##### **2.2.1. Regular Conversion(Chuyển đổi tuần tự)**

  * **Single Conversion**: 

   ◦ ADC chỉ đọc 1 kênh duy nhất và chỉ đọc khi nào được yêu cầu

   ◦ Thích hợp cho các ứng dụng chỉ cần đo một tín hiệu cụ thể.

  * **Single Continuous Conversion**: 

   ◦ ADC liên tục đọc một kênh duy nhất, tự động lặp lại sau mỗi lần chuyển đổi.

   ◦ Thường kết hợp với DMA để lưu trữ dữ liệu liên tục vào bộ nhớ.

  * **Scan Mode (Multi-Channel)** 

   ◦ ADC quét qua nhiều kênh, đọc lần lượt từng kênh khi được yêu cầu.

   ◦ Phù hợp khi cần đo nhiều tín hiệu analog từ các cảm biến khác nhau.

  * **Scan Continuous Mode:** 
  
   ◦ Tương tự Scan Mode, nhưng chuyển đổi liên tục trên nhiều kênh, lặp lại sau khi hoàn thành một chu kỳ quét.


##### **2.2.2. Injected Conversion(Chuyển đổi ưu tiên)**
   
  * Dùng để xử lý các kênh ưu tiên cao, có thể ngắt các chuyển đổi tuần tự (regular) đang thực hiện.

  * Khi gặp **Injected Trigger** thì ngay lập tức kênh đang hoạt động bị ngưng lại để kênh được ưu tiên kia có thể hoạt động.

  * Số kênh: Tối đa 4 kênh trong chế độ injected.
  
### **3. Cấu hình ADC**

#### **3.1. Cấu hình Clock cho ADC**

* **Các bộ ADC được cấp xung từ RCC APB2**

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);




![Image](https://github.com/user-attachments/assets/f59f57ea-5a7a-4fb7-811b-3ea2222a156e)

#### **3.2. Cấu hình GPIO cho ADC**

* Các chân GPIO dùng cho ADC cần được cấu hình ở chế độ **Analog Input (AIN)** để đảm bảo tín hiệu không bị ảnh hưởng bởi các chế độ số.

* **VD:** Cấu hình GPIO cho chân PA0:

        void GPIO_Config(){
            GPIO_InitTypeDef GPIO_InitStructure;  
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_Init(GPIOA, &GPIO_InitStructure);
        }


#### **3.3. Cấu hình ADC**

* **VD:** Cấu hình ADC1 cho một kênh (PA0) ở chế độ chuyển đổi liên tục

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


* **Các thông số:**

* **ADC_Mode:** 
    
    ◦ **Independent:** ADC hoạt động độc lập, không đồng bộ với ADC khác.


    ◦ **DualMode:** ADC1 và ADC2 hoạt động đồng thời, dùng cho chế độ kép (dual mode) để đo hai kênh cùng lúc.


* **ADC_NbrOfChannel:** 

    ◦  Số kênh cần đọc (1 cho PA0).

* **ADC_ScanConvMode:** 

    ◦  ENABLE: Kích hoạt chế độ quét nhiều kênh.

    ◦  DISABLE: Chỉ đọc một kênh duy nhất.

* **ADC_ContinuousConvMode:** 

    ◦ ENABLE: ADC tự động chuyển đổi liên tục sau khi hoàn thành một lần chuyển đổi.

    ◦ DISABLE: Chỉ chuyển đổi một lần khi được kích hoạt.

* **ADC_ExternalTrigConv:** 

    ◦ ADC_ExternalTrigConv_None: Không sử dụng trigger ngoài (chuyển đổi được kích hoạt bằng phần mềm).

* **ADC_DataAlign:** 

    ◦ ADC_DataAlign_Right: Dữ liệu được căn lề phải trong thanh ghi 16 bit.

    ◦ ADC_DataAlign_Left: Dữ liệu được căn lề trái.

* **ADC_RegularChannelConfig:** 

    ◦ Cấu hình kênh cụ thể (ADC_Channel_?), thứ tự (Rank = ?), và thời gian lấy mẫu (? chu kỳ để đảm bảo độ chính xác).

#### **3.4. Kích hoạt và đọc ADC**

* **Kích hoạt chuyển đổi:**

        ADC_SoftwareStartConvCmd(ADC1, ENABLE);

* **Đọc giá trị:**

        while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)); // Chờ chuyển đổi hoàn tất
        uint16_t adc_value = ADC_GetConversionValue(ADC1); // Đọc giá trị


#### **3.5. Thời gian lấy mẫu**

* Thời gian lấy mẫu **(ADC_SampleTime)** quyết định thời gian ADC thu thập tín hiệu analog trước khi chuyển đổi:

    ◦ Các giá trị: 1.5, 7.5, 13.5, 28.5, 41.5, 55.5, 71.5, 239.5 chu kỳ.

    ◦ Ví dụ: ADC_SampleTime_55Cycles5 cho thời gian lấy mẫu dài, tăng độ chính xác cho tín hiệu thay đổi chậm (như biến trở).

* Công thức thời gian chuyển đổi:

    ◦ Tconv = Tsample + 12.5 chu kỳ

### **4. Các hàm  thông dụng**

#### **4.1. Hàm cấu hình kênh**

```
 ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel,uint8_t Rank, uint8_t ADC_SampleTime)
```

*  **ADCx:** Con trỏ đến khối ADC (ADC1 hoặc ADC2). Xác định ADC nào được cấu hình.

*  **ADC_Channel:** Số kênh (0-17, ví dụ: ADC_Channel_0 cho PA0).

*  **Rank:** Thứ tự kênh trong nhóm regular (1-16). Trong scan mode, ADC đọc theo thứ tự rank tăng dần.

*  **ADC_SampleTime:** Thời gian lấy mẫu (ví dụ: ADC_SampleTime_55Cycles5).

#### **4.2. Hàm bắt đầu chuyển đổi**

```
ADC_SoftwareStartConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
```

*  **ADCx:** Con trỏ đến khối ADC (ADC1 hoặc ADC2). Xác định ADC nào được cấu hình.

*  **NewState:** ENABLE để bắt đầu chuyển đổi bằng phần mềm; DISABLE để dừng.


#### **4.3. Hàm đọc giá trị ADC**

```
uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx);
```

*  **ADCx:** Con trỏ đến khối ADC (ADC1 hoặc ADC2). Xác định ADC nào được cấu hình.

*  Trả về giá trị 16-bit từ thanh ghi DR (dữ liệu thực tế chỉ dùng 12-bit thấp nhất nếu căn phải).


#### **4.4. Hàm đọc giá trị ở chế độ kép**

```
uint32_t ADC_GetDualModeConversionValue(void);
```

*  Không có tham số. Trả về giá trị 32-bit kết hợp từ ADC1 (16-bit thấp) và ADC2 (16-bit cao) trong chế độ dual.

#### **4.5. Hàm kiểm tra trạng thái**

```
FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, uint8_t ADC_FLAG);
```
*  **ADCx:** Con trỏ đến khối ADC (ADC1 hoặc ADC2). Xác định ADC nào được cấu hình.

*  **ADC_FLAG:** Cờ trạng thái (ví dụ: ADC_FLAG_EOC cho end of conversion, ADC_FLAG_AWD cho analog watchdog).

#### **4.6. Các hàm hiệu chỉnh:**

```
ADC_ResetCalibration(ADC_TypeDef ADCx)
```

*  **Reset thanh ghi hiệu chỉnh để chuẩn bị.**

```
ADC_GetResetCalibrationStatus(ADC_TypeDef ADCx)
```

*  **Trả về SET nếu đang reset, RESET nếu hoàn tất.**

```
ADC_StartCalibration(ADC_TypeDef ADCx)
```
*  **Bắt đầu hiệu chỉnh (đo offset và gain nội bộ).**

```
ADC_GetCalibrationStatus(ADC_TypeDef ADCx)
```
*  **Tương tự, kiểm tra tiến trình hiệu chỉnh.**


## **II. Bộ lọc Kalman**

### **2.1.Định nghĩa**

* Bộ lọc Kalman là một thuật toán toán học dùng để ước lượng trạng thái tối ưu của một hệ thống tuyến tính từ các quan sát bị nhiễu.

* Nó được sử dụng rộng rãi trong các ứng dụng như điều khiển, định vị, và xử lý tín hiệu, nhờ khả năng kết hợp thông tin từ mô hình dự đoán và dữ liệu đo lường để giảm thiểu sai số.

### **2.2.Đặc điểm**

* **Bộ lọc Kalman hoạt động dựa trên hai giai đoạn chính**

#### **2.2.1. Giai đoạn dự đoán(Prediction):** 

    ◦ Dự đoán trạng thái tiếp theo dựa trên mô hình hệ thống và trạng thái trước đó.

    ◦ Tính toán độ không chắc chắn (covariance) của dự đoán.

![Image](https://github.com/user-attachments/assets/f638e348-1525-4589-8793-7e51285f67b4)

#### **2.2.2. Giai đoạn cập nhật(Correction):**

    ◦ Kết hợp dữ liệu đo mới (có thể chứa nhiễu) với dự đoán để cập nhật trạng thái.

    ◦  Tính toán Kalman Gain để cân bằng giữa dự đoán và đo lường thực tế.

![Image](https://github.com/user-attachments/assets/22643b2a-9310-4376-a93e-0ae6e8b2b3ee)

#### **2.2.3. Các tham số cần chú ý**

* `mea_e (R):` Sai số nhiễu đo lường, đại diện cho độ tin cậy của cảm biến.

* `est_e (P):` Sai số ước lượng ban đầu, phản ánh độ không chắc chắn của dự đoán.

* `q (Q:` Nhiễu quá trình, biểu thị sự không chắc chắn trong mô hình hệ thống.

#### **2.2.4. Hàm khởi tạo bộ lọc Kalman**

```
void SimpleKalmanFilter(float mea_e, float est_e, float q) {
    _err_measure = mea_e;
    _err_estimate = est_e;
    _q = q;
    _current_estimate = 0;
    _last_estimate = 0;
}
```

#### **2.2.5. Hàm cập nhật giá trị ước lượng qua bộ lọc Kalman**

```
float updateEstimate(float mea) {
    _kalman_gain = _err_estimate / (_err_estimate + _err_measure);
    _current_estimate = _last_estimate + _kalman_gain * (mea - _last_estimate);
    _err_estimate = (1.0 - _kalman_gain) * _err_estimate + fabs(_last_estimate - _current_estimate) * _q;
    _last_estimate = _current_estimate;
    return _current_estimate;
}
```

#### **2.2.6. VD**

```
#include <math.h>

// Biến toàn cục cho bộ lọc Kalman
float _err_measure;     // Sai số nhiễu đo lường (R)
float _err_estimate;    // Sai số ước lượng (P)
float _q;               // Nhiễu quá trình (Q)
float _current_estimate; // Giá trị ước lượng hiện tại
float _last_estimate;   // Giá trị ước lượng trước đó
float _kalman_gain;     // Hệ số Kalman Gain

// Hàm khởi tạo bộ lọc Kalman
void SimpleKalmanFilter(float mea_e, float est_e, float q) {
    _err_measure = mea_e;
    _err_estimate = est_e;
    _q = q;
    _current_estimate = 0;
    _last_estimate = 0;
}

// Hàm cập nhật giá trị ước lượng qua bộ lọc Kalman
float updateEstimate(float mea) {
    _kalman_gain = _err_estimate / (_err_estimate + _err_measure);
    _current_estimate = _last_estimate + _kalman_gain * (mea - _last_estimate);
    _err_estimate = (1.0 - _kalman_gain) * _err_estimate + fabs(_last_estimate - _current_estimate) * _q;
    _last_estimate = _current_estimate;
    return _current_estimate;
}

// Hàm chính xử lý dữ liệu từ ADC
void main(void) {
    float val;          // Giá trị ADC thô
    float val_Update;   // Giá trị sau khi lọc Kalman

    // Khởi tạo bộ lọc Kalman với R=1, P=2, Q=0.001
    SimpleKalmanFilter(1, 2, 0.001);

    while (1) {
        // Lấy giá trị từ ADC
        val = ADC_GetConversionValue(ADC1);
        // Lọc giá trị qua bộ lọc Kalman
        val_Update = updateEstimate((float)val);
        // Đợi 100ms trước khi lấy mẫu tiếp theo
        Delay_ms(100);
    }
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
  
