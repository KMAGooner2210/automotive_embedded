
	
# AUTOMOTIVE_EMBEDDED

<details>
	<summary><strong>BÀI 1: GPIO</strong></summary>
	
## BÀI 1: GPIO 
![Image](https://github.com/user-attachments/assets/f275f738-034e-41e5-849a-892cb47e31d6)

### **I.Giới thiệu**

#### **1.1. Định nghĩa**

* GPIO (General Purpose Input Output) là các chân đầu vào/đầu ra đa năng trên vi điều khiển STM32F103

* Mỗi port GPIO (A-G) có 16 chân (Pin 0-15), hỗ trợ các chế độ linh hoạt: Input (đọc tín hiệu), Output (ghi tín hiệu), Alternate Function (AF, cho ngoại vi như UART/SPI), và Analog (cho ADC).

#### **1.2. Đặc điểm**

* **Cấu trúc port:** Mỗi port có thanh ghi CRL/CRH (cấu hình chân thấp/cao), IDR (Input Data), ODR (Output Data), BSRR (Bit Set/Reset).

* **Tốc độ:** 2/10/50 MHz (tùy chế độ)

### **II.Quy trình làm việc với GPIO**

#### **2.1. Cấp xung clock (RCC Configuration)**

* **Trên STM32, các ngoại vi đều bị tắt clock mặc định để tiết kiệm năng lượng. Phải kích hoạt clock qua RCC (Reset and Clock Control) trước khi dùng.**

	◦ `void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);`
	
	◦ `void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);` 
	
	◦ `void RCC_AHBPeriphClockCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState);`
 
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

#### **2.2.Cấu hình GPIO**

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

### **III. Các hàm điều khiển GPIO**

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

### **IV.Kiến thức cần chú ý**

#### **4.1.Pull-Up vs Pull-Down**


![Image](https://github.com/user-attachments/assets/2e06645d-579f-4b64-970c-f09a46cf949f)



* **Pull-up:**

    ◦ Được kết nối giữa chân **đầu vào** với **nguồn VCC**

        => Đảm bảo rằng khi **không** có tín hiệu hoặc thiết bị nào **tác động** vào chân nó sẽ **luôn ở mức cao**


    ◦ Mặc định (không tín hiệu):
 
        GPIO-> HIGH (Pull up kéo lên Vcc), (1).

    ◦ Có tín hiệu vào:
 
        GPIO->  HIGH => LOW , (0).

    ◦ Ứng dụng: Nút nhấn (nhấn = LOW, dễ debounce).


* **Pull-down:**

    ◦ Được kết nối giữa chân **đầu vào** với **GND**

        => Kéo chân về mức **thấp** khi **không** có tín hiệu hoặc thiết bị nào **tác động** 


    ◦ Mặc định (không tín hiệu):
 
        GPIO-> LOW (Trở kéo về GND), (0).

    ◦ Có  tín hiệu vào:
 
        GPIO->  LOW => HIGH 

    ◦ Ứng dụng: Công tắc (tắt = LOW).

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
    <summary><strong>BÀI 2: INTERRUPT & NVIC</strong></summary>

## **BÀI 2: INTERRUPT & NVIC**

### **I.  INTERRUPT & POLLING**

#### **1.1. Khái niệm**

##### **1.1.1. Interrupt**

<img width="454" height="341" alt="Image" src="https://github.com/user-attachments/assets/a9881ec0-242f-4334-ba12-8f95e60fb6e9" />

* Ngắt là cơ chế cho phép ngoại vi hoặc sự kiện phần cứng yêu cầu CPU tạm dừng chương trình đang chạy (foreground) để xử lý một công việc khẩn cấp hơn (background).

* Sau khi xử lý xong, CPU sẽ quay trở lại đúng vị trí bị gián đoạn và tiếp tục thực thi.

* Ngắt là một sự kiện bất đồng bộ (asynchronous), nghĩa là nó có thể xảy ra bất cứ lúc nào.  

##### **1.1.2. Polling**

<img width="506" height="424" alt="Image" src="https://github.com/user-attachments/assets/e1f4195d-6130-4799-917b-125ac587d5ec" />

* Polling là cơ chế xử lý vào/ra mang tính **đồng bộ (synchronous)**.

* Trong mô hình này, Bộ xử lý trung tâm (CPU) đóng vai trò chủ động, định kỳ kiểm tra tuần tự bit trạng thái (Status Flag) trên thanh ghi của từng thiết bị ngoại vi để xác định nhu cầu trao đổi dữ liệu

* **Nguyên lý hoạt động:**

	* Chương trình chính (Foreground Program) duy trì quyền kiểm soát luồng thực thi.
	
	* CPU thực hiện một vòng lặp kiểm tra liên tục.
	
	* Nếu cờ trạng thái báo hiệu thiết bị chưa sẵn sàng, CPU sẽ tiếp tục vòng lặp kiểm tra (trạng thái này được gọi là "chờ bận rộn" - Busy Waiting) hoặc chuyển sang kiểm tra thiết bị tiếp theo rồi quay lại sau.  
	
	* Khi phát hiện thiết bị đã sẵn sàng, CPU tiến hành trao đổi dữ liệu và sau đó quay trở lại công việc kiểm tra định kỳ.  


#### **1.2. Phân loại**

* Trên STM32F103, hệ thống ngắt được quản lý bởi Nested Vectored Interrupt Controller (NVIC).

	*  **Ngắt hệ thống:** 
	
		*  **Reset:** 
		
			* Ngắt có mức ưu tiên cao nhất.
			
			* Được kích hoạt khi chân Reset của vi điều khiển được tác động (Power-on Reset, External Reset, Brown-out Reset, Watchdog Reset).
			
			* Khi Reset xảy ra, toàn bộ thanh ghi của CPU (ngoại trừ một số thanh ghi đặc biệt trong một vài chế độ reset) được đưa về giá trị mặc định. 
		
		* **NMI (Non-Maskable Interrupt):**
		
			*  Là ngắt có mức ưu tiên cao thứ hai sau Reset (ví dụ ưu tiên -2) và không thể bị ngăn chặn bởi bit cho phép ngắt toàn cục trong thanh ghi trạng thái.
			
			* Dành riêng cho các sự kiện nguy hiểm đe dọa đến sự toàn vẹn dữ liệu hoặc an toàn hệ thống mà CPU bắt buộc phải xử lý ngay lập tức, bất kể hệ thống đang thực thi tác vụ nào. 

		*  **HardFault:** 
		
			* Là ngoại lệ lỗi chung, mức ưu tiên thường là -1 (có thể cấu hình cao hơn UsageFault/MemManage/BusFault nhưng bắt buộc cao hơn mọi ngắt ngoại vi).
			 
		
		* **MemManage Fault:**
		
			*  Được kích hoạt bởi (MPU – Memory Protection Unit) khi CPU vi phạm quy tắc truy cập bộ nhớ đã được lập trình.

		*  **BusFault:** 
		
			* Được kích hoạt khi giao dịch trên ma trận Bus hệ thống (AHB/APB) gặp lỗi.
		
		*  **UsageFault:** 
		
			*  Xảy ra khi CPU phát hiện lỗi trong quá trình giải mã hoặc thực thi lệnh.

		*  **SVCall:** 

			<img width="732" height="339" alt="Image" src="https://github.com/user-attachments/assets/86906695-b1a9-4de2-ac03-0eec6dd99f8e" />
		
			* Supervisor Call (dùng trong RTOS).
			
			* Cung cấp một cơ chế chuyển đổi an toàn từ chế độ người dùng không có đặc quyền (Unprivileged) sang chế độ hạt nhân có đặc quyền (Privileged) để yêu cầu các dịch vụ của Hệ điều hành (RTOS). 
		
		* **PendSV:**

			<img width="878" height="521" alt="Image" src="https://github.com/user-attachments/assets/871c985f-01e1-4beb-a381-9243df5bd222" />
		
			*  Là một ngắt mềm có thể được lập lịch (pended) bằng cách ghi vào thanh ghi điều khiển ngắt (ICSR).
			
			* Nó hoạt động bất đồng bộ. 
			
			* Đây là cơ chế tiêu chuẩn và hiệu quả nhất để thực hiện chuyển đổi ngữ cảnh (Context Switch) giữa các tiến trình/tác vụ.  

		*  **SysTick:**

			<img width="999" height="420" alt="Image" src="https://github.com/user-attachments/assets/b80b8455-07bb-44b3-a58e-af6a88b8c42a" />
		
			* Là một bộ định thời đếm xuống 24-bit được tích hợp sẵn trong lõi xử lý.
			
			* Tạo nhịp thời gian chuẩn (Tick) cho hệ thống. 
		

	*  **Ngắt ngoại vi:** 
	
		*  **Ngắt ngoài (EXTI – External Interrupt/Event Controller):** 

			<img width="724" height="636" alt="Image" src="https://github.com/user-attachments/assets/1f526b73-17b5-49f1-80a2-ece90e3165b1" />
		
			* Phát hiện các sự kiện thay đổi mức logic trên các chân GPIO (General Purpose Input/Output).
			
			* Bộ điều khiển EXTI cho phép cấu hình kênh ngắt để kích hoạt theo một trong ba sự kiện:
			
				* Sườn lên (Rising Edge)
				
				* Sườn xuống (Falling Edge)
				
				* Cả hai
				
			* **Ứng dụng:** Phát hiện nút nhấn, nhận tín hiệu từ cảm biến tốc độ, ngắt từ module ngoài. 
  		
		* **Ngắt định thời (Timer Interrupts – TIM):**

			<img width="792" height="510" alt="Image" src="https://github.com/user-attachments/assets/8dca72a7-5648-4e30-922f-bf62d45ba08b" />
		
			*  Đây là nhóm ngắt đa năng, phát sinh từ các bộ đếm thời gian phần cứng.
			
			* **Các nguồn ngắt tiêu biểu trong một khối Timer:**


				*  **Update Event (UEV):** 
				
					* Kích hoạt khi bộ đếm bị tràn (overflow) hoặc tràn dưới (underflow).

				*  **Capture/Compare (CCx)**:
				
					* Kích hoạt khi giá trị bộ đếm khớp với thanh ghi so sánh (Compare Match) hoặc khi bắt được một sự kiện cạnh trên chân đầu vào (Input Capture).

				*  **Trigger (TRGI):**
				
					* Kích hoạt bởi tín hiệu kích hoạt từ bên ngoài hoặc từ bộ định thời master khác.

		* **Ngắt truyền thông nối tiếp (Serial Communication Interrupts):**
			
			*  **UART/USART:** 
				
				* RXNE (Receive Not Empty):_ Bộ đệm nhận đã có dữ liệu, yêu cầu CPU đọc.
					
				* TXE (Transmit Empty):_ Bộ đệm truyền trống, sẵn sàng nhận byte dữ liệu tiếp theo để gửi.

				* TC (Transmission Complete):_ Quá trình truyền (bao gồm cả byte cuối cùng trong thanh ghi dịch) đã kết thúc.
					
				* Lỗi (Errors):_ Phát hiện lỗi khung (Frame Error), lỗi nhiễu (Noise Error), hoặc lỗi tràn bộ đệm nhận (Overrun Error).

			*  **SPI:** 
				
				* TXE/RXNE:_ Tương ứng với trạng thái bộ đệm truyền trống và bộ đệm nhận đầy.
					
				* Lỗi: Lỗi chế độ Master (Mode Fault), lỗi tràn.

			*  **I2C:**
				
				* Sự kiện (Events):_ Địa chỉ khớp (Address Matched), byte truyền gửi xong (BTF – Byte Transfer Finished).
					
				* Lỗi: Mất xung nhịp (Bus Error), lỗi bit ACK (Acknowledge Failure).

		* **Ngắt chuyển đổi tương tự - số (ADC Interrupts):**
			
			*  Cảnh báo trạng thái của quá trình biến đổi tín hiệu tương tự sang số.
			
			*  **Các sự kiện kích hoạt:** 
				
				* EOC (End of Conversion):_ Kết thúc chuyển đổi cho một kênh đơn lẻ.
					
				* JEOC (Injected End of Conversion):_ Kết thúc chuyển đổi cho nhóm kênh "tiêm" (Injected Group).

				* Analog Watchdog:_ Giá trị ADC nằm ngoài ngưỡng cài đặt (cao hơn mức cao hoặc thấp hơn mức thấp).

		* **Ngắt truyền thông công nghiệp (CAN, USB, Ethernet):**
	
			*  **CAN (Controller Area Network):**
				
				* Ngắt khi có thông báo mới đang chờ trong FIFO nhận (FIFO0/FIFO1).
					
				* Ngắt khi quá trình truyền trên các hộp thư đã hoàn tất.

				* Ngắt cảnh báo các sự kiện lỗi bus (Bus-off, Error Passive, Arbitration Lost).

			*  **USB:**
				
				* Ngắt khi nhận được gói dữ liệu trên các Endpoint.
					
				* Ngắt sự kiện Reset, Suspend, Resume trên bus.

			*  **Ethernet:**
				
				* Ngắt khi nhận khung dữ liệu từ bộ đệm DMA.
					
				* Ngắt báo hiệu khung truyền đã được đẩy ra đường truyền.

		* **Ngắt điều khiển truy xuất bộ nhớ trực tiếp (DMA Interrupts):**
	
			*  Báo hiệu trạng thái của các kênh DMA mà không cần CPU phải thăm dò.
			
			*  **Các cờ ngắt:** 
				
				* TC (Transfer Complete):_ Truyền xong toàn bộ khối dữ liệu đã cấu hình.

				* HT (Half Transfer):_ Truyền xong một nửa khối dữ liệu (hữu ích cho kỹ thuật bộ đệm kép).

				* TE (Transfer Error):_ Xảy ra lỗi trong quá trình truyền (ví dụ: truy cập vùng nhớ không hợp lệ).

	
#### **1.3. Chu trình hoạt động của CPU khi có ngắt (Stacking → Branching → Restoring)**

* **1. Stacking (Đẩy ngữ cảnh vào Stack):**

	* CPU tự động đẩy các thanh ghi quan trọng vào Stack (Main Stack Pointer - MSP).
		
	* Các thanh ghi được đẩy gồm: `xPSR`, `PC`, `LR`, `R12`, `R3`, `R2`, `R1`, `R0` (8 thanh ghi).
		
	* Quá trình này mất **12 chu kỳ clock** (trên Cortex-M3).

* **2. Branching (Nhảy đến Vector Table):**
		
	* CPU đọc Vector Table (bảng vector ngắt) để lấy địa chỉ của hàm xử lý ngắt tương ứng (Interrupt Service Routine - ISR).

	* Sau đó nhảy đến hàm ISR để thực thi.

	* Vector Table thường nằm ở đầu bộ nhớ Flash (địa chỉ 0x0000 0000).  

* **3. Restoring (Khôi phục ngữ cảnh):**
		
	* Khi hàm ISR thực thi xong (gặp lệnh BX LR hoặc gọi NVIC return), CPU thực hiện Exception Return.

	* Tự động lấy (pop) 8 thanh ghi ra khỏi Stack.

	* Khôi phục lại chương trình chính tại đúng vị trí bị ngắt trước đó.
	
	* Tiếp tục thực thi code foreground. 

* **Sơ đồ chu trình ngắt:**

			Chương trình chính đang chạy
			        ↓ (có ngắt xảy ra)
			   → Stacking (đẩy 8 thanh ghi vào Stack)
			        ↓
			   Đọc Vector Table → Nhảy vào ISR
			        ↓
			   Thực thi hàm xử lý ngắt
			        ↓
			   Exception Return (Restoring)
			        ↓
			Quay lại chương trình chính tiếp tục chạy



### **II. TRÌNH PHỤC VỤ NGẮT (ISR - INTERRUPT SERVICE ROUTINE)**

#### **2.1. Định nghĩa**

<img width="1038" height="547" alt="Image" src="https://github.com/user-attachments/assets/7c620423-bab1-4771-aaae-9d3d346e5700" />

<img width="675" height="561" alt="Image" src="https://github.com/user-attachments/assets/0bac5ef1-6265-47d6-8cc1-5b3146299e07" />

*  ISR (hay còn gọi là Interrupt Handler) là hàm chuyên trách xử lý sự kiện ngắt.

*  Khi ngắt xảy ra, CPU sẽ tạm dừng chương trình chính, lưu ngữ cảnh, và nhảy đến địa chỉ của ISR tương ứng trong Vector Table để thực thi.

* Nguyên tắc quan trọng khi viết ISR:

	* Ngắn gọn nhất có thể
	
		* ISR nên thực thi càng nhanh càng tốt.
		
		* Thời gian nằm trong ISR càng dài → càng làm giảm hiệu suất hệ thống và tăng nguy cơ mất ngắt khác.

	* Không sử dụng hàm Delay
	
		* Tuyệt đối không dùng Delay_ms(), HAL_Delay(), hoặc bất kỳ hàm chờ nào trong ISR.
		
		* Lý do: Làm treo toàn bộ hệ thống, ngăn cản các ngắt có mức ưu tiên cao hơn.

	* Không nên gọi hàm phức tạp
	
		* Tránh gọi hàm có thời gian thực thi dài (printf, floating point calculation, malloc…).
		
		* Nên chỉ đặt cờ (flag) hoặc đưa dữ liệu vào buffer, sau đó xử lý trong vòng lặp chính (main) hoặc task.

	* An toàn với biến chia sẻ
	
		* Biến được truy cập trong ISR và main loop phải được khai báo là `volatile`.
		
		* Khi dùng RTOS, cần sử dụng cơ chế đồng bộ (Semaphore, Queue…).


#### **2.2. Cờ ngắt (Pending Bit)**

* Mỗi ngắt ngoại vi thường có một hoặc nhiều cờ ngắt (Interrupt Flag / Pending Bit)

* Khi sự kiện xảy ra, phần cứng tự động bật cờ này lên. 

*  Nếu không xóa cờ, sau khi thoát ISR, phần cứng vẫn thấy cờ đang bật → sẽ liên tục sinh ngắt (ngắt lặp vô tận).
	
*  Điều này dẫn đến CPU bị kẹt hoàn toàn trong ISR, hệ thống treo hoặc vào HardFault.
	
				void TIM2_IRQHandler(void)
				{
				    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
				    {
				        // Xử lý công việc...
				        timer_tick++;
				        
				        // BẮT BUỘC phải xóa cờ ngắt
				        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
				    }
				}

#### **2.3. Lỗi HardFault do truy cập bộ nhớ sai trong ISR**

* **Nguyên nhân:**

	* Truy cập biến toàn cục chưa được khai báo `volatile`. 
	
	* Truy cập con trỏ NULL hoặc địa chỉ bộ nhớ không hợp lệ.
	
	* Gọi hàm không an toàn trong ISR (ví dụ: hàm có sử dụng floating-point mà FPU chưa được bật).
	
	* Stack overflow do ISR lồng nhau quá sâu.
	
	* Truy cập ngoại vi chưa được bật clock.     


### **III.  VECTOR INTERRUPT TABLE (VIT)**

#### **3.1. Khái niệm**

<img width="880" height="664" alt="Image" src="https://github.com/user-attachments/assets/59c4372f-c1a2-4225-ba2e-b268dcc87936" />

*  Vector Interrupt Table (VIT) hay Vector Table là một bảng chứa địa chỉ của tất cả các hàm xử lý ngắt (ISR) và các exception hệ thống

* Bảng này nằm trong bộ nhớ Flash và được CPU sử dụng để xác định điểm vào (entry point) của từng ngắt.

* Cấu trúc bộ nhớ của Vector Table trên STM32F103:

	*  Vector Table bắt đầu từ địa chỉ 0x0000 0000 (đầu Flash) hoặc có thể được relocate sang SRAM (thường dùng trong Bootloader).
	
	*  Mỗi vector chiếm 4 byte (32-bit), chứa địa chỉ của hàm ISR (Exception Handler).
	
	*  Vector đầu tiên (Vector 0) là Initial Main Stack Pointer (MSP) – giá trị con trỏ ngăn xếp ban đầu.
	


#### **3.2. Vector Number và cách CPU tính toán địa chỉ hàm ISR**

<img width="876" height="555" alt="Image" src="https://github.com/user-attachments/assets/c3f7befb-d3a4-464f-afe9-f3260e5a78cf" />

*  Mỗi ngắt hoặc exception đều có một số thứ tự (Vector Number) cố định do ARM định nghĩa.

* Khi một ngắt xảy ra, CPU thực hiện phép tính sau để tìm địa chỉ của ISR:

			Địa chỉ ISR = Địa chỉ Vector Table + (Vector Number x 4)

* VD:  

	* SysTick Interrupt:
	
		* Vector Number = 15
	
		* Offset = 15 × 4 = 60 (0x3C)
		
		* Địa chỉ Vector = Vector Table Base + 0x3C
		
		* CPU nhảy đến hàm `SysTick_Handler()`   
	

	* USART1 Interrupt:
	
		* Vector Number = 37
	
		* Offset = 37 × 4 = 148 (0x94)
		
		* CPU nhảy đến hàm USART1_IRQHandler()
	
	* CAN1_RX0 Interrupt:
	
		* Vector Number = 50
	
		* Offset = 50 × 4 = 200 (0xC8)
		
		* CPU nhảy đến hàm CAN1_RX0_IRQHandler()
		  
	
* Quy trình khi ngắt xảy ra:

	* **1.** Sự kiện ngắt xảy ra → NVIC nhận và kiểm tra mức ưu tiên.
		
	* **2.** CPU hoàn tất Stacking (đẩy 8 thanh ghi vào Stack).
	
	* **3.** CPU lấy Vector Number của ngắt.
		
	* **4.** Tính toán địa chỉ vector: `VectorTableBase + (VectorNumber × 4)`
	
	* **5.** Đọc địa chỉ ISR từ bảng Vector Table.
	
	* **6.** Nhảy đến hàm ISR và thực thi.
	
	* **7.** Khi ISR xong → thực hiện Exception Return → Restoring ngữ cảnh → quay lại chương trình chính.   

* **Lưu ý:**

	* Vector Table phải được căn chỉnh theo 128 byte (0x80) nếu được relocate.
	
	* Trong file startup assembly, các hàm ISR được khai báo là `DCD` (Define Constant Double word).
	
	* Nếu không định nghĩa ISR, compiler sẽ dùng Default_Handler (thường dẫn đến vòng lặp vô tận hoặc HardFault).
	
	* Có thể di chuyển Vector Table sang SRAM bằng cách viết vào thanh ghi `VTOR` (Vector Table Offset Register) của SCB.

    

### **IV.  NVIC - NESTED VECTORED INTERRUPT CONTROLLER**

#### **4.1. Vai trò**

* NVIC là bộ điều khiển ngắt được tích hợp sẵn trong lõi Cortex-M3

* Nó đóng vai trò trung tâm trong việc xử lý ngắt.

* Chức năng: 

	* Quản lý 68 nguồn ngắt trên STM32F103 (bao gồm System Exceptions và Peripheral Interrupts).
	
	* Hỗ trợ Nested Interrupt (ngắt lồng nhau).
	
	* Quản lý ưu tiên ngắt một cách linh hoạt.
	
	* Tự động xử lý Stacking và Exception Return.
		
	* Hỗ trợ Tail-Chaining (giảm thời gian chuyển đổi giữa các ngắt).
		
	* Che giấu (mask) và bỏ che ngắt.
	
	* Quản lý trạng thái Pending và Active của từng ngắt..      


#### **4.2. Khái niệm**

<img width="919" height="432" alt="Image" src="https://github.com/user-attachments/assets/2a9dd0ca-8aea-4dba-8a73-b91babc849b8" />

*  Nested Interrupt là khả năng một ngắt có mức ưu tiên cao hơn có thể ngắt (chen ngang) giữa chừng một ngắt đang được xử lý (ưu tiên thấp hơn).

*  VD:

	*  Ngắt Timer (ưu tiên thấp) đang chạy.
	
	*  Đột nhiên có ngắt UART nhận dữ liệu khẩn cấp (ưu tiên cao hơn) → NVIC cho phép UART ngắt Timer. 
	
	*  Sau khi xử lý xong UART, CPU quay lại tiếp tục xử lý Timer từ điểm bị ngắt.   

#### **4.3. Mức ưu tiên ngắt (Priority)**

<img width="1063" height="288" alt="Image" src="https://github.com/user-attachments/assets/c1846cd8-5793-45ff-afed-a6ce0ae42acb" />

*  Trên Cortex-M3, mỗi ngắt có một mức ưu tiên được lập trình.

*  STM32F1 hỗ trợ 16 mức ưu tiên (từ 0 đến 15), trong đó 0 là ưu tiên cao nhất.

*  ARM chia mức ưu tiên thành 2 phần: 

	*  **Preemption Priority (Quyền chen ngang / Preemptive Priority):**
	
		*  Quyết định ngắt nào có quyền chen ngang (ngắt) ngắt khác.
		
		*  Giá trị càng nhỏ → càng có quyền chen ngang mạnh.
		
	*  **SubPriority (Sub-priority / Priority within same preemption level):**
	
		*  Khi hai ngắt có cùng Preemption Priority, SubPriority sẽ quyết định ngắt nào được xử lý trước.
		
		*  Dùng khi nhiều ngắt xảy ra gần như cùng lúc (pending).   
		
	* **VD:**
	
		* Ngắt A: Preemption = 2, Sub = 0
		
		* Ngắt B: Preemption = 2, Sub = 1
		
		*  → Ngắt A sẽ được xử lý trước ngắt B nếu cả hai cùng pending.  

#### **4.4. Phân nhóm ưu tiên (NVIC_PriorityGroupConfig)**

*  Vì Cortex-M3 chỉ có 4 bit để lưu mức ưu tiên (16 mức), STM32 cho phép lập trình viên phân bổ số bit giữa Preemption Priority và SubPriority thông qua Priority Group.

#### **4.5. Cấu hình**

*  Cấu trúc NVIC:

				typedef struct
				{
				    uint8_t NVIC_IRQChannel;                    // Số Vector của ngắt
				    uint8_t NVIC_IRQChannelPreemptionPriority;  // Preemption Priority
				    uint8_t NVIC_IRQChannelSubPriority;         // Sub Priority
				    FunctionalState NVIC_IRQChannelCmd;         // ENABLE / DISABLE
				} NVIC_InitTypeDef;	

*  VD:

				void NVIC_Config(void)
				{
				    NVIC_InitTypeDef NVIC_InitStructure;
				    
				    // 1. Cấu hình nhóm ưu tiên (thường cấu hình 1 lần duy nhất)
				    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);   // 4 bit Preemption, 0 bit Sub
				    
				    // 2. Cấu hình ngắt cho USART1
				    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
				    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   // Ưu tiên chen ngang
				    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
				    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
				    NVIC_Init(&NVIC_InitStructure);
				    
				    // 3. Cấu hình ngắt cho Timer 2
				    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
				    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
				    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
				    NVIC_Init(&NVIC_InitStructure);
				    
				    // 4. Cấu hình ngắt cho CAN
				    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
				    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   // Ưu tiên cao
				    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
				    NVIC_Init(&NVIC_InitStructure);
				}

### **V.  THỰC HÀNH EXTERNAL INTERRUPT**

#### **5.1. Sơ đồ phần cứng EXTI và các thanh ghi quan trọng**

* **Cấu trúc:**

	* STM32F103 hỗ trợ 19 đường ngắt ngoài EXTI0 ~ EXTI18.
	
	* EXTI0 ~ EXTI15: Mỗi đường tương ứng với một chân GPIO (ví dụ: EXTI0 có thể là PA0, PB0, PC0…).
	
	* EXTI16: Kết nối với PVD (Programmable Voltage Detector).
	
	* EXTI17: Kết nối với RTC Alarm.
	
	* EXTI18: Kết nối với USB Wakeup.       

* **Các thanh ghi:**

	* **IMR (Interrupt Mask Register):**
	
		* Cho phép ngắt (Interrupt) 

	* **EMR (Event Mask Register):**
	
		* Cho phép sự kiện (Event)
	
	* **RTSR (Rising Trigger Selection Register):**
	
		* Chọn sườn lên (Rising edge)

	* **FTSR (Falling Trigger Selection Register):**
	
		* Chọn sườn xuống (Falling edge)
	
	* **PR (Pending Register):**
		
		* Cờ ngắt (phải xóa sau khi xử lý)
		
#### **5.2. Chức năng AFIO trong việc chọn chân ngắt**

* Vì nhiều chân GPIO có thể ánh xạ vào cùng một đường EXTI (ví dụ: PA0, PB0, PC0 đều có thể là EXTI0), nên cần sử dụng AFIO (Alternate Function Input/Output) để chọn chân GPIO nào sẽ kết nối với đường EXTI.

	* Thanh ghi quan trọng: `AFIO_EXTICR1 ~ AFIO_EXTICR4`
	
	* Mỗi thanh ghi điều khiển 4 đường EXTI.
	
* VD:

	* Muốn dùng PA0 làm EXTI0 → phải cấu hình AFIO_EXTICR1 bits [3:0] = 0000
	
	* Muốn dùng PB0 làm EXTI0 → AFIO_EXTICR1 bits [3:0] = 0001   

#### **5.3. Quy trình cấu hình**

* **Bước 1: Bật Clock**

			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

* **Bước 2: Cấu hình GPIO**

			// PA0 làm Input với Pull-Up (nút nhấn)
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     // Input Pull-Up
			GPIO_Init(GPIOA, &GPIO_InitStructure);

			// PC13 làm Output (LED)
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOC, &GPIO_InitStructure);

* **Bước 3: Liên kết chân GPIO với đường EXTI qua AFIO**

		// Chọn PA0 làm nguồn cho EXTI0
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

* **Bước 4: Cấu hình EXTI**

			EXTI_InitTypeDef EXTI_InitStructure;

			EXTI_InitStructure.EXTI_Line = EXTI_Line0;
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;      // Chế độ ngắt
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  // Sườn xuống (nút nhấn)
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;
			EXTI_Init(&EXTI_InitStructure);

* **Bước 5: Cấu hình NVIC**

			NVIC_InitTypeDef NVIC_InitStructure;

			NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

* **Bước 6: Viết hàm ngắt (ISR)**

			void EXTI0_IRQHandler(void)
			{
			    if (EXTI_GetITStatus(EXTI_Line0) != RESET)
			    {
			        // Xử lý công việc
			        GPIO_WriteBit(GPIOC, GPIO_Pin_13, !GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13));
			        
			        // BẮT BUỘC phải xóa cờ ngắt
			        EXTI_ClearITPendingBit(EXTI_Line0);
			    }
			}

#### **5.4.VD**

* Dùng nút nhấn PA0 đảo trạng thái LED PC13 (không dội phím)


			#include "stm32f10x.h"

			void EXTI0_Config(void)
			{
			    GPIO_InitTypeDef GPIO_InitStructure;
			    EXTI_InitTypeDef EXTI_InitStructure;
			    NVIC_InitTypeDef NVIC_InitStructure;

			    // Bước 1: Bật Clock
			    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

			    // Bước 2: Cấu hình GPIO
			    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
			    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;        // Nút nhấn
			    GPIO_Init(GPIOA, &GPIO_InitStructure);

			    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
			    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     // LED
			    GPIO_Init(GPIOC, &GPIO_InitStructure);

			    // Bước 3: Kết nối AFIO
			    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

			    // Bước 4: Cấu hình EXTI
			    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
			    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
			    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;   // Sườn xuống
			    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
			    EXTI_Init(&EXTI_InitStructure);

			    // Bước 5: Cấu hình NVIC
			    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
			    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
			    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			    NVIC_Init(&NVIC_InitStructure);
			}

			//================== HÀM NGẮT ==================
			void EXTI0_IRQHandler(void)
			{
			    if (EXTI_GetITStatus(EXTI_Line0) != RESET)
			    {
			        // Đảo trạng thái LED PC13
			        GPIOC->ODR ^= GPIO_Pin_13;
			        
			        // Xóa cờ ngắt
			        EXTI_ClearITPendingBit(EXTI_Line0);
			    }
			}

			//================== MAIN =====================
			int main(void)
			{
			    EXTI0_Config();
			    
			    while(1)
			    {
			        // Có thể để trống hoặc làm các công việc khác
			    }
			}	

						
   </details> 


<details>
	<summary><strong>BÀI 3: CÁC CHUẨN GIAO TIẾP CƠ BẢN </strong></summary> 


## **BÀI 3: CÁC CHUẨN GIAO TIẾP CƠ BẢN**

### **I. SPI (Serial Peripheral Interface)**

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

### **II. I2C (Inter-Integrated Circuit)**

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

### **III. UART**

#### **3.1. Đặc điểm**

* **Loại:** Giao tiếp nối tiếp, không đồng bộ, song công (full-duplex).
  
    ◦ **Nối Tiếp (Serial):** Tương tự SPI và I2C, dữ liệu được truyền từng bit một trên một đường dây. 

    ◦ **Không đồng bộ (Asynchronous):** Không có đường tín hiệu xung nhịp (Clock) chung nào để đồng bộ hóa giữa hai thiết bị.
  
    ◦ **Song công (Full-Duplex):** ữ liệu có thể được truyền (TX) và nhận (RX) cùng một lúc vì chúng sử dụng hai đường dây hoàn toàn độc lập

* **Số dây:** 2 dây (Tx, Rx)
  	 
#### **3.2. Sơ đồ chân**

![Image](https://github.com/user-attachments/assets/5d657816-56e4-4871-8c40-7ce476b283c2)

* **Tx (Transmit)**: Chân truyền dữ liệu. Dữ liệu từ thiết bị này phải được nối đến chân RX của thiết bị kia.

* **Rx (Receive)**: Chân nhận dữ liệu. Dữ liệu từ thiết bị kia phải được nối đến chân TX của thiết bị này.

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

### **I.SPI Software (Bit-Banging)**

#### **1.1. Khái niệm**

* Phần này triển khai SPI bằng phần mềm, sử dụng các chân GPIO để tạo xung clock và truyền/nhận dữ liệu. 

* Phương pháp này linh hoạt nhưng chậm hơn phần cứng.

#### **1.2. Xác định các chân GPIO**

* Định nghĩa 4 chân sử dụng SPI

        #define SPI_SCK_Pin    GPIO_Pin_0   // Serial Clock
        #define SPI_MISO_Pin   GPIO_Pin_1   // Master In Slave Out
        #define SPI_MOSI_Pin   GPIO_Pin_2   // Master Out Slave In
        #define SPI_CS_Pin     GPIO_Pin_3   // Chip Select
        #define SPI_GPIO       GPIOA
 

#### **1.3. Cấu hình GPIO**

* **Master Mode:**
  
    ◦ Chân SCK, CS, MOSI: Output Push-Pull (để master kiểm soát clock, select và dữ liệu gửi).

    ◦ Chân MISO: Input Floating (để đọc dữ liệu từ slave).

        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = SPI_SCK_Pin | SPI_CS_Pin | SPI_MOSI_Pin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(SPI_GPIO, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = SPI_MISO_Pin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(SPI_GPIO, &GPIO_InitStructure);

* **Slave Mode:**

    ◦ Chân MISO: Output Push-Pull (slave gửi dữ liệu).

    ◦ Chân MOSI, SCK, CS: Input Floating (slave nhận clock, select và dữ liệu).

        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = SPI_MISO_Pin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(SPI_GPIO, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = SPI_SCK_Pin | SPI_CS_Pin | SPI_MOSI_Pin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(SPI_GPIO, &GPIO_InitStructure);

#### **1.4.Tạo xung clock**

* Hàm tạo một xung clock cơ bản (mức cao rồi thấp) với độ trễ 4ms (có thể điều chỉnh tốc độ).

        void Clock() {
            GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_SET);   // Clock cao
            Delay_ms(4);                                      // Độ trễ
            GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_RESET); // Clock thấp
            Delay_ms(4);                                      // Độ trễ
        }

#### **1.5.Khởi tạo các chân SPI**

* Đặt trạng thái ban đầu: Clock thấp, CS cao (không chọn slave), MISO/MOSI thấp.

        void SPI_Init() {
            GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_RESET);  // Clock thấp (idle)
            GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_SET);     // CS cao (không chọn)
            GPIO_WriteBit(SPI_GPIO, SPI_MISO_Pin, Bit_RESET); // MISO thấp
            GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_RESET); // MOSI thấp
        }

#### **1.5.Khởi tạo các chân SPI**

* Đặt trạng thái ban đầu: Clock thấp, CS cao (không chọn slave), MISO/MOSI thấp.

        void SPI_Init() {
            GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_RESET);  // Clock thấp (idle)
            GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_SET);     // CS cao (không chọn)
            GPIO_WriteBit(SPI_GPIO, SPI_MISO_Pin, Bit_RESET); // MISO thấp
            GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_RESET); // MOSI thấp
        }
#### **1.6.Hàm truyền (Master)**

* Truyền 1 byte dữ liệu từ master sang slave. Sử dụng MSB first, với clock pulse sau mỗi bit.

        void SPI_Master_Transmit(uint8_t u8Data) {
            uint8_t u8Mask = 0x80;
            uint8_t tempData;

            GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_RESET); // Chọn slave (CS thấp)
            Delay_ms(1);

            for (int i = 0; i < 8; i++) {
                tempData = u8Data & u8Mask;
                if (tempData) {
                    GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_SET);   // Gửi bit 1
                } else {
                    GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_RESET); // Gửi bit 0
                }
                Delay_ms(1);
                u8Mask = u8Mask >> 1;  // Dịch mask sang phải (sửa lỗi gốc: dùng >> thay vì << cho mask)
                Clock();               // Tạo xung clock
            }

            GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_SET);    // Bỏ chọn slave (CS cao)
            Delay_ms(1);
        }

#### **1.7.Hàm nhận (Slave)**

* Slave chờ CS thấp, đọc 8 bit từ MOSI theo clock, rồi chờ CS cao.

        uint8_t Slave_Receive(void) {
            uint8_t dataReceive = 0x00;
            uint8_t temp = 0x00;

            while (GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin) == Bit_SET); // Chờ CS thấp (bắt đầu truyền)
            while (GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin) == Bit_SET); // Chờ SCK thấp (idle)

            for (int i = 0; i < 8; i++) {
                while (GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin) == Bit_SET); // Chờ SCK thấp
                while (GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin) == Bit_RESET); // Chờ SCK cao (bắt đầu bit)

                temp = GPIO_ReadInputDataBit(SPI_GPIO, SPI_MOSI_Pin); // Đọc bit từ MOSI
                dataReceive = (dataReceive << 1) | temp;                // Dịch trái và thêm bit

                while (GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin) == Bit_SET); // Chờ SCK thấp (kết thúc bit)
            }

            while (GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin) == Bit_RESET); // Chờ CS cao (kết thúc truyền)
            return dataReceive;
        }

#### **1.8.Hàm main (Ví Dụ Master)**

        uint8_t DataTrans[] = {1, 3, 4, 5, 6, 7, 8, 9}; // 8 phần tử

        int main() {
            RCC_Config();  
            GPIO_Config(); 
            TIM_Config();  
            SPI_Init();   

            while (1) {
                for (int i = 0; i < 8; i++) {  // Duyệt đúng kích thước mảng
                    SPI_Master_Transmit(DataTrans[i]);
                    Delay_ms(10);  // Tăng delay để tránh xung đột
                }
                Delay_ms(1000);  
            }
        }

### **II.SPI Hardware**

#### **2.1.Khái niệm**

* Sử dụng module SPI phần cứng của STM32 , nhanh hơn và ít tốn CPU.

#### **2.2.Xác định chân GPIO**

![Image](https://github.com/user-attachments/assets/6c977197-08fb-4993-9e0b-ce7e83ba6d6d)

        #define SPI1_NSS     GPIO_Pin_4   // Slave Select
        #define SPI1_SCK     GPIO_Pin_5   // Serial Clock
        #define SPI1_MISO    GPIO_Pin_6   // Master In Slave Out
        #define SPI1_MOSI    GPIO_Pin_7   // Master Out Slave In
        #define SPI1_GPIO    GPIOA

#### **2.3.Cấu hình chân GPIO**

* **NSS:** Input/Output hoặc Alternate Function (AF_PP nếu dùng phần cứng).

* **MISO, MOSI, SCK:** Alternate Function Push-Pull (AF_PP).

        void GPIO_Config() {
            GPIO_InitTypeDef GPIO_InitStructure;
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);

            // Cấu hình SCK, MOSI: AF_PP
            GPIO_InitStructure.GPIO_Pin = SPI1_SCK | SPI1_MOSI;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_Init(SPI1_GPIO, &GPIO_InitStructure);

            // Cấu hình MISO: Input Floating
            GPIO_InitStructure.GPIO_Pin = SPI1_MISO;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
            GPIO_Init(SPI1_GPIO, &GPIO_InitStructure);

            // NSS: Output Push-Pull (nếu dùng phần mềm)
            GPIO_InitStructure.GPIO_Pin = SPI1_NSS;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_Init(SPI1_GPIO, &GPIO_InitStructure);
            GPIO_SetBits(SPI1_GPIO, SPI1_NSS); // NSS cao ban đầu
        }

#### **2.4.Cấu hình SPI**

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

        void SPI_Config() {
            SPI_InitTypeDef SPI_InitStructure;

            // Kích hoạt clock SPI1
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

            SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
            SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; // Tốc độ clock/16
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;        // Clock thấp khi idle
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;      // Dữ liệu thay đổi ở cạnh đầu tiên
            SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // 8 bit
            SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB; // LSB first
            SPI_InitStructure.SPI_CRCPolynomial = 7;          // CRC polynomial (tùy chọn)
            SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;         // NSS phần mềm

            SPI_Init(SPI1, &SPI_InitStructure);
            SPI_Cmd(SPI1, ENABLE);
        }

#### **2.5.Các hàm thông dụng**

* `SPI_I2S_SendData(SPI_TypeDef* SPIx,uint16_t Data)` : Gửi dữ liệu.

* `SPI_I2S_ReceiveData(SPI_TypeDef* SPIx)`: Nhận dữ liệu.

* `SPI_I2S_GetFlagStatus(SPI_TypeDef* SPIx,uint16_t SPI_I2S_FLAG)` trả về giá trị 1 cờ trong thanh ghi của SPI

    ◦ **SPI_I2S_FLAG_TXE** : Cờ báo truyền,cờ này sẽ set lên 1 khi truyền xong data trong buffer

    ◦ **SPI_I2S_FLAG_RXNE** : Cờ báo nhận,cờ nãy sẽ set lên 1 khi nhận xong data

    ◦ **SPI_I2S_FLAG_BSY**: Cờ báo bận,cờ này sẽ set lên 1 khi đang bận truyền nhận

#### **2.6.Hàm truyền (Master)**

* Gửi 1 byte, kiểm tra cờ TXE và BSY.

        void SPI_Send1Byte(uint8_t data) {
            GPIO_ResetBits(SPI1_GPIO, SPI1_NSS);  // Chọn slave

            // Chờ buffer truyền rỗng
            while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

            SPI_I2S_SendData(SPI1, data);

            // Chờ kết thúc truyền
            while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);

            GPIO_SetBits(SPI1_GPIO, SPI1_NSS);    // Bỏ chọn slave
        }

#### **2.7.Hàm nhận(Slave)**

* Nhận 1 byte, kiểm tra cờ RXNE và BSY

        uint8_t SPI_Receive1Byte(void) {
            // Chờ kết thúc bận
            while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);

            uint8_t temp = (uint8_t)SPI_I2S_ReceiveData(SPI1);

            // Chờ buffer nhận đầy
            while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

            return temp;
        }

#### **2.8.Hàm main (Ví Dụ Master)**

        uint8_t dataSend[] = {1, 2, 3, 4, 5, 6, 7, 8}; 

        int main() {
            GPIO_Config();  
            TIM_Config();  
            SPI_Config();   

            while (1) {
                for (int i = 0; i < 8; i++) {  
                    SPI_Send1Byte(dataSend[i]);
                    Delay_ms(10);
                }
                Delay_ms(1000);  
            }
        }

        
</details>



<details>
	<summary><strong>BÀI 5: GIAO TIẾP I2C</strong></summary> 

## **Bài 5: GIAO TIẾP I2C**

### **I.I2C Software (Bit-Banging)**

#### **1.1.Xác định các chân GPIO**

* Định nghĩa 2 chân sử dụng I2C: **SCL,SDA**

        #define I2C_SCL_Pin    GPIO_Pin_6  // Serial Clock
        #define I2C_SDA_Pin    GPIO_Pin_7  // Serial Data
        #define I2C_GPIO       GPIOB       // Cổng GPIO
 

#### **1.2.Cấu hình GPIO**

* Cấu hình cả SCL và SDA ở chế độ Open-Drain (OD) với tốc độ 50MHz, để hỗ trợ pull-up.

        void GPIO_Config() {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  // Kích hoạt clock GPIOB

            GPIO_InitTypeDef GPIO_InitStructure;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;       // Open-Drain
            GPIO_InitStructure.GPIO_Pin = I2C_SCL_Pin | I2C_SDA_Pin;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
        }


#### **1.3.Cấu hình I2C**

* **Macro điều khiển chân:**

        #define WRITE_SDA_0    GPIO_ResetBits(I2C_GPIO, I2C_SDA_Pin)  // Kéo SDA xuống 0
        #define WRITE_SDA_1    GPIO_SetBits(I2C_GPIO, I2C_SDA_Pin)    // Kéo SDA lên 1 (qua pull-up)
        #define WRITE_SCL_0    GPIO_ResetBits(I2C_GPIO, I2C_SCL_Pin)  // Kéo SCL xuống 0
        #define WRITE_SCL_1    GPIO_SetBits(I2C_GPIO, I2C_SCL_Pin)    // Kéo SCL lên 1 (qua pull-up)
        #define READ_SDA_VAL   GPIO_ReadInputDataBit(I2C_GPIO, I2C_SDA_Pin)  // Đọc SDA

* **Khởi tạo I2C (đặt bus idle: SCL và SDA cao)**

        void I2C_Config() {
            WRITE_SDA_1;
            delay_us(1);
            WRITE_SCL_1;
            delay_us(1);
        }

* **Hàm Start (SDA xuống thấp trước SCL)**

        void I2C_Start() {
            WRITE_SCL_1;
            delay_us(3);
            WRITE_SDA_1;
            delay_us(3);
            WRITE_SDA_0;  // Start condition
            delay_us(3);
            WRITE_SCL_0;
            delay_us(3);
        }

* **Hàm Stop (SCL lên cao trước SDA)**

        void I2C_Stop() {
            WRITE_SDA_0;
            delay_us(3);
            WRITE_SCL_1;
            delay_us(3);
            WRITE_SDA_1;  // Stop condition
            delay_us(3);
        }

#### **1.4.Hàm truyền (Master)**

* Hàm truyền sẽ truyền lần lượt 8 bit trong byte dữ liệu
  
  ◦ Truyền 1 bit

  ◦ Tạo clock

  ◦ Dịch 1 bit

        typedef enum { 
            OK = 0, 
            NOT_OK = 1 
            } status_t;  

        status_t I2C_Write(uint8_t u8Data) {
            uint8_t i;
            status_t stRet;

            for (i = 0; i < 8; i++) {
                // Ghi bit MSB first
                if (u8Data & 0x80) {
                    WRITE_SDA_1;
                } else {
                    WRITE_SDA_0;
                }
                delay_us(3);

                // Tạo xung clock
                WRITE_SCL_1;
                delay_us(5);
                WRITE_SCL_0;
                delay_us(2);

                u8Data <<= 1;  // Dịch trái cho bit tiếp theo
            }

            // Bit ACK (thứ 9): Release SDA, đọc ACK từ slave 
            WRITE_SDA_1;
            delay_us(3);
            WRITE_SCL_1;
            delay_us(3);

            if (READ_SDA_VAL) {
                stRet = NOT_OK;  // NACK
            } else {
                stRet = OK;      // ACK
            }

            delay_us(2);
            WRITE_SCL_0;
            delay_us(5);

            return stRet;
        }

#### **1.5.Hàm nhận (Master)**

* Kéo SDA lên 1

   ◦ Đọc data trên SDA, ghi vào biến.

   ◦ Dịch 1 bit

* Gửi lại 1 tín hiệu ACK ở xung thứ 9.


        typedef enum { 
            ACK = 1, 
            NACK = 0 
            } ACK_Bit_t; 

        uint8_t I2C_Read(ACK_Bit_t _ACK) {
            uint8_t i;
            uint8_t u8Ret = 0x00;

            WRITE_SDA_1;  // Release SDA để đọc
            delay_us(3);

            for (i = 0; i < 8; i++) {
                u8Ret <<= 1;
                WRITE_SCL_1;
                delay_us(3);

                // Đọc bit từ SDA
                if (READ_SDA_VAL) {
                    u8Ret |= 0x01;
                }
                delay_us(2);
                WRITE_SCL_0;
                delay_us(5);
            }

            // Bit ACK/NACK (thứ 9)
            if (_ACK == ACK) {
                WRITE_SDA_0;  // Gửi ACK
            } else {
                WRITE_SDA_1;  // Gửi NACK
            }
            delay_us(3);

            WRITE_SCL_1;
            delay_us(5);
            WRITE_SCL_0;
            delay_us(5);

            return u8Ret;
        }

#### **1.6. Hàm Main (Ví Dụ Software)**

        int main() {
            RCC_Config();  
            GPIO_Config();
            TIM_Config();  
            I2C_Config();

            uint8_t slaveAddr = 0x50;  // Địa chỉ slave
            uint8_t txData = 0xAA;
            uint8_t rxData;

            while (1) {
                // Truyền 1 byte
                I2C_Master_WriteByte(slaveAddr, txData);

                // Nhận 1 byte (giả định)
                I2C_Start();
                I2C_Write((slaveAddr << 1) | 0x01);  // Address + Read
                rxData = I2C_Read(NACK);  // NACK cho byte cuối
                I2C_Stop();

                Delay_ms(1000);  // Delay
            }
        }

### **II.I2C Hardware**

#### **2.1.Xác định chân GPIO**

![Image](https://github.com/user-attachments/assets/b80e6194-eab7-4f07-bb89-a4e85871ee0f)

        #define I2C1_SCL_Pin   GPIO_Pin_6  // PB6
        #define I2C1_SDA_Pin   GPIO_Pin_7  // PB7
        #define I2C1_GPIO      GPIOB

#### **2.2.Cấu hình chân GPIO**

* Alternate Function Open-Drain (AF_OD) cho SCL/SDA.

        void GPIO_Config() {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

            GPIO_InitTypeDef GPIO_InitStructure;
            GPIO_InitStructure.GPIO_Pin = I2C1_SCL_Pin | I2C1_SDA_Pin;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;  // AF Open-Drain
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_Init(I2C1_GPIO, &GPIO_InitStructure);
        }

#### **2.3.Cấu hình I2C**

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


        void I2C_Config() {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);  // Kích hoạt clock I2C1 (APB1)

            I2C_InitTypeDef I2C_InitStructure;
            I2C_InitStructure.I2C_ClockSpeed = 400000;             // 400 kHz (Fast Mode)
            I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
            I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;    // Duty cycle 2:1
            I2C_InitStructure.I2C_OwnAddress1 = 0x33;              // Địa chỉ own (nếu slave)
            I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
            I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

            I2C_Init(I2C1, &I2C_InitStructure);
            I2C_Cmd(I2C1, ENABLE);
        }

#### **2.4.Các hàm thông dụng**

* `I2C_GenerateSTART(I2C_TypeDef* I2Cx, FunctionalState NewState)`: Tạo START.

* `I2C_Send7bitAddress(I2C_TypeDef* I2Cx,uint8_t Address,uint8_t Direction)` : Gửi địa chỉ (<<1 cho RW bit: 0=Write, 1=Read).

* `I2C_SendData(I2C_TypeDef* I2Cx,uint8_t Data)` : Gửi 1 byte.

* `I2C_ReceiveData(I2C_TypeDef* I2Cx)` : Nhận 1 byte.

* `I2C_AcknowledgeConfig(I2C_TypeDef* I2Cx, FunctionalState NewState)`: Bật/tắt ACK.

* `I2C_CheckEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)` trả về kết quả kiểm tra I2C_EVENT tương ứng:

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

### **2.5.Hàm truyền (Master)**

* Bắt đầu truyền nhận ,bộ I2C sẽ tạo 1 tín hiệu START,đợi tín hiệu báo bus sẵn sàng

* Gửi 7 bit địa chỉ để xác định slave,đợi slave xác nhận

* Gửi/đọc các byte data,đợi truyền xong

* Sau đó kết thúc bằng tín hiệu STOP


        void I2C_Write(uint8_t address, uint8_t *data, uint8_t length) {
            // Tạo START
            I2C_GenerateSTART(I2C1, ENABLE);
            while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

            // Gửi địa chỉ + Write
            I2C_Send7bitAddress(I2C1, address << 1, I2C_Direction_Transmitter);
            while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

            // Gửi dữ liệu
            for (uint8_t i = 0; i < length; i++) {
                I2C_SendData(I2C1, data[i]);
                while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
            }

            // Tạo STOP
            I2C_GenerateSTOP(I2C1, ENABLE);
        }



#### **2.6.Hàm nhận (Master)**

* Tắt ACK cho byte cuối để báo NACK.

        void I2C_Read(uint8_t address, uint8_t *data, uint8_t length) {
            // Tạo START
            I2C_GenerateSTART(I2C1, ENABLE);
            while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

            // Gửi địa chỉ + Read
            I2C_Send7bitAddress(I2C1, address << 1, I2C_Direction_Receiver);
            while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

            for (uint8_t i = 0; i < length; i++) {
                if (i == length - 1) {
                    I2C_AcknowledgeConfig(I2C1, DISABLE);  // NACK cho byte cuối
                }
                while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
                data[i] = I2C_ReceiveData(I2C1);
            }

            I2C_GenerateSTOP(I2C1, ENABLE);
            I2C_AcknowledgeConfig(I2C1, ENABLE);  // Bật lại ACK
        }

#### **2.7.Hàm main**

        int main() {
            RCC_Config();  
            GPIO_Config();
            TIM_Config(); 
            I2C_Config();

            uint8_t writeData[] = {0x01, 0x02, 0x03};
            uint8_t readData[3];
            uint8_t slaveAddr = 0x50;

            while (1) {
                // Ghi 3 byte
                I2C_Write(slaveAddr, writeData, 3);

                Delay_ms(10);  

                // Đọc 3 byte
                I2C_Read(slaveAddr, readData, 3);

                Delay_ms(1000);  // Delay chu kỳ
            }
        }
</details>


<details>
	<summary><strong>BÀI 6: GIAO TIẾP UART</strong></summary> 


## Bài 6: GIAO TIẾP UART 

### **I.UART Software (Bit-Banging)**

#### **1.1.Xác định các chân GPIO**

* Định nghĩa 2 chân sử dụng UART: **TX,RX**

* **Baudrate:**

    ◦ Baudrate = Số bit truyền được / 1s

    ◦ Tốc độ baudrate thường dùng là 9600, ứng với mỗi bit là 105us.

        #define TX_Pin        GPIO_Pin_0   // Transmit
        #define RX_Pin        GPIO_Pin_1   // Receive
        #define UART_GPIO     GPIOA

        #define BaudrateTime  104  // us (1/9600 * 1e6)

#### **1.2.Cấu hình GPIO**

* **TX:** Cấu hình Output Push-Pull, tốc độ cao (50 MHz).

* **RX:** Cấu hình Input Floating để đọc tín hiệu từ thiết bị gửi.

        void GPIO_Config() {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // Kích hoạt clock (bổ sung)

            GPIO_InitTypeDef GPIO_InitStructure;

            // TX: Output Push-Pull
            GPIO_InitStructure.GPIO_Pin = TX_Pin;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_Init(UART_GPIO, &GPIO_InitStructure);

            // RX: Input Floating
            GPIO_InitStructure.GPIO_Pin = RX_Pin;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
            GPIO_Init(UART_GPIO, &GPIO_InitStructure);
        }


#### **1.3.Cấu hình UART**

* **Khởi tạo UART**

    //Truyền dữ liệu nên kéo chân TX lên 1,vì mục đích giao tiếp 1 chiều nên không cần chân RX

        void UART_Config()
        {
            GPIO_SetBits(UART_GPIO,TX_Pin);
            Delay_us(100);
        }



### **1.4.Hàm truyền (TX)**

* Hàm truyền sẽ truyền lần lượt 8 bit trong byte dữ liệu,sau khi tín hiệu start được gửi đi

* Tạo start,delay 1 period time

  ◦ Truyền 1 bit dữ liệu, mỗi bit truyền trong 1 period time

  ◦ Dịch 1 bit

* Tạo stop,delay tương ứng với số bit stop

![Image](https://github.com/user-attachments/assets/b96946d0-8eba-4382-b469-97e1eac2c14a)

        void UART_Transmit(unsigned char c) {  
            // Start Bit (thấp)
            GPIO_ResetBits(UART_GPIO, TX_Pin);
            Delay_us(BaudrateTime);

            // 8 Data Bits (LSB first)
            for (int i = 0; i < 8; i++) {
                if (c & (1 << i)) {
                    GPIO_SetBits(UART_GPIO, TX_Pin);     // Bit 1
                } else {
                    GPIO_ResetBits(UART_GPIO, TX_Pin);   // Bit 0
                }
                Delay_us(BaudrateTime);
            }

            // Stop Bit (cao)
            GPIO_SetBits(UART_GPIO, TX_Pin);
            Delay_us(BaudrateTime);  // Ít nhất 1 period cho stop
        }

#### **1.5.Hàm nhận (RX)**


* Chờ tín hiệu START từ thiết bị gửi

* Delay 1,5 period time

    ◦ Đọc data trên RX,ghi vào biến

    ◦ Dịch 1 bit

    ◦ Delay 1 period time

* Delay 1 period time và đợi stop bit

![Image](https://github.com/user-attachments/assets/cce64408-ea97-4726-936f-0a3be009b5e8)


        unsigned char stop_bit_error = 0;  // Biến global theo dõi lỗi

        unsigned char UART_Receive() {
            unsigned char c = 0;

            // Chờ Start Bit (thấp)
            while (GPIO_ReadInputDataBit(UART_GPIO, RX_Pin) == 1);

            // Lấy mẫu giữa bit đầu (1.5 period)
            Delay_us(BaudrateTime + BaudrateTime / 2);

            // 8 Data Bits (LSB first)
            for (int i = 0; i < 8; i++) {
                if (GPIO_ReadInputDataBit(UART_GPIO, RX_Pin)) {
                    c |= (1 << i);
                }
                Delay_us(BaudrateTime);  // Delay đầy period cho bit tiếp theo
            }

            // Kiểm tra Stop Bit (cao, lấy mẫu giữa)
            Delay_us(BaudrateTime / 2);
            if (GPIO_ReadInputDataBit(UART_GPIO, RX_Pin) != 1) {
                stop_bit_error = 1;  // Lỗi framing
            } else {
                stop_bit_error = 0;
            }

            return c;
        }

#### **1.6.Parity**

* Bit chẵn/lẻ được thêm vào cuối Data

* Tùy vào cấu hình parity là chẵn hay lẻ mà thiết bị truyền có thể thêm bit parity là 0 hoặc 1.

* Phía nhận cấu hình parity giống như phía truyền, sau khi nhận đủ các bit sẽ kiểm tra parity có đúng hay không.

        typedef enum {
            Parity_Mode_NONE,
            Parity_Mode_ODD,   // Số 1 lẻ (parity=1 nếu tổng data lẻ)
            Parity_Mode_EVEN   // Số 1 chẵn (parity=0 nếu tổng data lẻ)
        } Parity_Mode_t;

        // Ví dụ tính Even Parity
        uint8_t CalculateEvenParity(uint8_t data) {
            uint8_t parity = 0;
            for (int i = 0; i < 8; i++) {
                parity ^= (data & (1 << i)) ? 1 : 0;
            }
            return parity ? 0 : 1;  // Even: parity bit làm tổng chẵn
        }

        // Sử dụng: Trong Transmit, thêm sau data bits; Trong Receive, kiểm tra sau 8 bits.

#### **1.7.Hàm Main (Ví Dụ Software)**

        int main() {
            RCC_Config();  
            GPIO_Config();
            TIM_Config(); 
            UART_Config();

            while (1) {
                UART_Transmit('A');  // Gửi ký tự
                Delay_ms(1000);

                unsigned char rx = UART_Receive();  // Nhận và kiểm tra lỗi
                if (!stop_bit_error) {
                    UART_Transmit(rx);  // Echo nếu OK
                }
            }
        }

### **II.UART Hardware**

#### **2.1.Xác định chân GPIO**

![Image](https://github.com/user-attachments/assets/399d3f04-2761-45cc-ba06-308a1a452d8d)

        #define TX_Pin        GPIO_Pin_9
        #define RX_Pin        GPIO_Pin_10
        #define USART1_GPIO   GPIOA

#### **2.2.Cấu hình chân GPIO**

* **TX:** Alternate Function Push-Pull (AF_PP) để kết nối với UART phần cứng

* **RX:** Input Floating để nhận tín hiệu

        void GPIO_Config() {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

            GPIO_InitTypeDef GPIO_InitStructure;

            // TX: AF Push-Pull
            GPIO_InitStructure.GPIO_Pin = TX_Pin;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_Init(USART1_GPIO, &GPIO_InitStructure);

            // RX: Input Floating
            GPIO_InitStructure.GPIO_Pin = RX_Pin;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
            GPIO_Init(USART1_GPIO, &GPIO_InitStructure);
        }

#### **2.3.Cấu hình UART**

* **USART_Mode**: Quy định chế độ hoạt động UART
   
    ◦ **USART_Mode_Tx**: Cấu hình truyền

    ◦ **USART_Mode_Rx**: Cấu hình nhận

    ◦ Cấu hình cả 2 cùng lúc (song công)

* **USART_BaudRate**: Cấu hình tốc độ baudrate cho UART
    
* **USART_HardwareFlowControl**: Cấu hình chế độ bắt tay cho UART

* **USART_WordLength**: Cấu hình số bit mỗi lần truyền

* **USART_StopBits**: Cấu hình số lượng stopbits

* **USART_Parity**: Cấu hình bit kiểm tra chẵn ,lẻ


        void USART_Config() {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  // Kích hoạt clock USART1 (bổ sung)

            USART_InitTypeDef USART_InitStructure;
            USART_InitStructure.USART_BaudRate = 9600;
            USART_InitStructure.USART_WordLength = USART_WordLength_8b;
            USART_InitStructure.USART_StopBits = USART_StopBits_1;
            USART_InitStructure.USART_Parity = USART_Parity_No;  // Hoặc USART_Parity_Even/Odd
            USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
            USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

            USART_Init(USART1, &USART_InitStructure);
            USART_Cmd(USART1, ENABLE);
        }



#### **2.4.Các hàm thông dụng**

*  `USART_SendData(USART_TypeDef * USARTx,uint16_t Data)` : Gửi data (tự thêm parity nếu cấu hình).

* `USART_ReceiveData(USART_TypeDef* USARTx)` : Nhận data.

* Hàm `USART_GetFlagStatus(USART_TypeDef* USARTx,uint16_t USART_FLAG)` trả về trạng thái cờ USART_FLAG tương ứng

    ◦ **USART_FLAG_TXE** : Cờ báo thanh ghi chứa dữ liệu truyền đi (DR) **đang trống**
 
    ◦ **USART_FLAG_RXNE** : Cờ báo thanh ghi chứa dữ liệu nhận (DR) **đã có** dữ liệu 

    ◦ **USART_FLAG_IDLE** : Cờ báo đường truyền đang ở chế độ rảnh

    ◦ **USART_FLAG_PE**   : Cờ báo lỗi Parity

    ◦ **USART_FLAG_TC**   : Cờ báo đã hoàn thành quá trình truyền dữ liệu
    
#### **2.5.Hàm truyền**

*  **Truyền byte**
  
    ◦ 1. Chờ cờ TXE

    ◦ 2. Gửi byte qua USART_SendData.

    ◦ 3. Chờ cờ TC (truyền hoàn tất).

        void USART_TransmitByte(uint8_t byte) {
            // Chờ TXE
            while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

            // Gửi
            USART_SendData(USART1, byte);

            // Chờ TC
            while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        }

*  **Truyền chuỗi**
  
    ◦ Lặp qua từng ký tự, gọi hàm truyền byte.

        void USART_TransmitString(const char *str) {
            while (*str != '\0') {
                USART_TransmitByte(*str); 
                str++;
            }
        }

#### **2.6.Hàm nhận**

*   **Nhận byte**
 
    ◦ 1. Chờ cờ RXNE

    ◦ 2. Đọc dữ liệu từ USART_ReceiveData.

        uint8_t USART_ReceiveByte(void) {
            uint8_t temp = 0x00;

            // Chờ RXNE
            while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

            // Đọc
            temp = USART_ReceiveData(USART1);
            return temp;
        }

#### **2.7.Hàm main**

* Echo server: Nhận và gửi lại.

        int main() {
            RCC_Config();  
            GPIO_Config();
            USART_Config();

            const char *msg = "KMA!\r\n";
            USART_TransmitString(msg);

            while (1) {
                uint8_t received_char = USART_ReceiveByte();
                USART_TransmitByte(received_char);
                USART_TransmitByte('\r');
                USART_TransmitByte('\n');
            }
        }



#### **2.8. Các lỗi UART**

* **Parity Error (PE):** Parity nhận không khớp với cấu hình.
  
* **Framing Error (FE):** Stop bit không đúng (mức 0 thay vì 1).
  
* **Overrun Error (ORE):** Dữ liệu mới đến trước khi đọc dữ liệu cũ.
  
* **Noise Error (NE):** Nhiễu trên đường truyền.
  
* **Xử lý:** Kiểm tra cờ lỗi (PE, FE, ORE, NE) trong trình xử lý ngắt, gửi thông báo hoặc bỏ dữ liệu lỗi.

        uint8_t USART_ReceiveByteWithError(void) {
            while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
            if (USART_GetFlagStatus(USART1, USART_FLAG_PE | USART_FLAG_FE | USART_FLAG_ORE) != RESET) {
                USART_ClearFlag(USART1, USART_FLAG_PE | USART_FLAG_FE | USART_FLAG_ORE);  // Clear lỗi
                USART_TransmitByte('E');  // Báo lỗi
                return 0xFF;  // Giá trị lỗi
            }
            return USART_ReceiveData(USART1);
        }

#### **2.9.Hardware Flow Control (RTS/CTS)**

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

### **I. Ngắt ngoài (EXTI)**

* **Sơ đồ**

![Image](https://github.com/user-attachments/assets/b8531dc8-d8a1-4fea-b10b-90365810da53)


#### **1.1. Tổng quan**

* **Số lượng line ngắt:**

    ◦ STM32F103 hỗ trợ 16 EXTI line (Line 0 đến Line 15), tương ứng với các chân GPIO cùng số thứ tự (ví dụ: PA0, PB0, PC0 cho Line 0).

* **Kết nối với GPIO:** 

    ◦ Mỗi EXTI line có thể được liên kết với các chân GPIO có cùng số thứ tự

    ◦ Tuy nhiên, chỉ một chân duy nhất trong số các chân cùng thứ tự được chọn làm nguồn ngắt cho một EXTI line.
    
    ◦ Việc chọn được thực hiện qua thanh ghi AFIO_EXTICRx.

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

##### **1.2.1. Bật clock**

    ◦ **Bật clock cho GPIO port chứa chân ngắt và AFIO (để cấu hình liên kết EXTI).**

        void RCC_Config() {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
        }
                
##### **1.2.2. Cấu hình chân GPIO**

    ◦ Chân GPIO phải ở chế độ Input để nhận tín hiệu ngoài.

    ◦ Sử dụng pull-up/pull-down để tránh trạng thái floating (nhiễu).

    ◦ **Struct:** `GPIO_InitTypeDef`

        void GPIO_Config() {
            GPIO_InitTypeDef GPIO_InitStructure;
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  // PB0 ví dụ
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // Pull-up (nút nối GND)
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_Init(GPIOB, &GPIO_InitStructure);
        }

##### **1.2.3. Liên kết chân GPIO với EXTI Line**

    ◦ **Hàm:** `GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)`
  
    ◦ **Tham số:*

        GPIO_PortSource: Port nguồn (ví dụ: GPIO_PortSourceGPIOB cho port B).

        GPIO_PinSource: Số thứ tự pin (ví dụ: GPIO_PinSource0 cho pin 0).
    
    ◦ VD:

        GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0); // Liên kết PB0 với EXTI Line 0.

##### **1.2.4. Cấu hình EXTI**

    ◦ **Struct:** `EXTI_InitTypeDef`

        EXTI_Line: Line cụ thể (ví dụ: EXTI_Line0 đến EXTI_Line15).

        EXTI_Mode: 

            EXTI_Mode_Interrupt: Gọi ISR khi ngắt xảy ra (thông dụng).
            EXTI_Mode_Event: Không gọi ISR, dùng cho các module khác (như DMA, Wake-up).
    

        EXTI_Trigger: 
        
            EXTI_Trigger_Rising: Phát hiện cạnh lên (low → high).
            EXTI_Trigger_Falling: Phát hiện cạnh xuống (high → low).
            EXTI_Trigger_Rising_Falling: Phát hiện cả hai (thay đổi trạng thái).
  
        EXTI_LineCmd: ENABLE hoặc DISABLE.

        ◦ **VD:**

        void EXTI_Config() {
            EXTI_InitTypeDef EXTI_InitStructure;
            EXTI_InitStructure.EXTI_Line = EXTI_Line0;
            EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
            EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  // Cạnh xuống (nhấn nút)
            EXTI_InitStructure.EXTI_LineCmd = ENABLE;
            EXTI_Init(&EXTI_InitStructure);
        }


##### **1.2.5. Cấu hình NVIC**

        void NVIC_Config() {
            NVIC_InitTypeDef NVIC_InitStructure;
            NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;  // Line 0
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);
        }

##### **1.2.6. Hàm xử lý ngắt (ISR)**

        volatile uint8_t button_pressed = 0;

        void EXTI0_IRQHandler(void) {
            if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
                button_pressed = 1;  // Xử lý sự kiện (giữ ngắn gọn)
                EXTI_ClearITPendingBit(EXTI_Line0);  // Xóa pending bit
            }
        }

##### **1.2.7. Ví Dụ Main (EXTI Với Nút Bấm)**

        int main() {
            RCC_Config();
            GPIO_Config();
            EXTI_Link_Config();
            EXTI_Config();
            NVIC_Config();

            while (1) {
                if (button_pressed) {
                    GPIO_ToggleBits(GPIOC, GPIO_Pin_13);  // Toggle LED PC13
                    button_pressed = 0;
                }
            }
        }

### **II. Ngắt Timer**

#### **2.1. Tổng quan**

* **Sơ đồ**

![Image](https://github.com/user-attachments/assets/b0736d5c-3a49-41bf-95db-063762fdb254)

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

##### **2.2.1. Bật clock** 

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); (TIM2-5 trên APB1, TIM1 trên APB2).

##### **2.2.2. Cấu hình TimeBase**

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

            void TIM_TimeBase_Config() {
                TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
                TIM_TimeBaseStructure.TIM_Prescaler = 7199;  // 72MHz / 7200 = 10kHz (0.1ms/tick)
                TIM_TimeBaseStructure.TIM_Period = 9;        // 10 ticks = 1ms
                TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
                TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
                TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
            }

* **Lưu ý:**

    ◦ Tính toán Prescaler và Period dựa trên thời gian mong muốn. 
        
    ◦ VD: Với Interval = 1 ms, Timer clock = 72 MHz, có thể chọn:

            Prescaler = 7199, Period = 9 → (7200 × 10) / 72 MHz = 1 ms.
            Hoặc Prescaler = 71, Period = 999 → (72 × 1000) / 72 MHz = 1 ms.

##### **2.2.3. Bật ngắt Và Timer**

* **Kích hoạt ngắt:** `TIM_ITConfig(TIM_TypeDef* TIMx, uint16_t TIM_IT, FunctionalState NewState)`

    ◦ **Tham số:** 

            TIM_IT_Update cho Update Event, NewState = ENABLE.

    ◦ **VD:** 

            TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

 * **Bật Timer:** `TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState)`

    ◦ **VD:** 

            TIM_Cmd(TIM2, ENABLE);

##### **2.2.4. Cấu hình NVIC** 

* **Struct:** `NVIC_InitTypeDef`

    ◦ NVIC_IRQChannel: Kênh IRQ (ví dụ: TIM2_IRQn cho TIM2).

    ◦ NVIC_IRQChannelPreemptionPriority: Độ ưu tiên preempt (0-15, thấp hơn = ưu tiên cao hơn).

    ◦ NVIC_IRQChannelSubPriority: Độ ưu tiên sub (0-15).

    ◦ NVIC_IRQChannelCmd: ENABLE.

* **Hàm:** `NVIC_Init(NVIC_InitTypeDef* NVIC_InitStruct)`

            NVIC_InitTypeDef NVIC_InitStruct;
            NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
            NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;  // Ưu tiên cao
            NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStruct);

##### **2.2.5. Hàm xử lý ngắt Timer** 

* **Tên cố định:** `TIMx_IRQHandler(void)` `(ví dụ: TIM2_IRQHandler).`

* **Quy trình:** 

    ◦ Kiểm tra cờ: TIM_GetITStatus(TIM_TypeDef* TIMx, uint16_t TIM_IT) → Trả về SET nếu ngắt từ Update.

    ◦ Thực thi tác vụ (ví dụ: tăng biến đếm).

    ◦ Xóa cờ: TIM_ClearITPendingBit(TIM_TypeDef* TIMx, uint16_t TIM_IT)

    ◦ **VD:** 

        volatile uint16_t count = 0;

        void TIM2_IRQHandler(void) {
            if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
                count++;  // Tăng mỗi 1ms
                TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
            }
        }

        void delay_ms(uint16_t time) {
            count = 0;
            while (count < time);
        }

    ◦ **Lưu ý:**  Không xóa cờ → Ngắt lặp vô tận. Giữ ISR ngắn để tránh stack overflow.


#### **2.3. Các chế độ Timer**

##### **2.3.1. Input Capture** 

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

##### **2.3.2. Output Compare** 

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

##### **2.3.3. One-Pulse Mode (OPM)** 

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

##### **2.3.4. Encoder Interface Mode** 

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


##### **2.3.5. Hall Sensor Interface** 

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

#### **2.3.6. Trigger Mode và Slave Mode** 

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

#### **2.3.7. Break and Dead-Time (TIM1)** 

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

### **III. Ngắt truyền thông**

#### **3.1. Tổng quan**

* Ngắt cho giao tiếp (UART, SPI, I2C) để xử lý dữ liệu RX/TX mà không block CPU.

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
            if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
                uint8_t data = USART_ReceiveData(USART1);  // Nhận
                if (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == SET) {
                    USART_SendData(USART1, data);  // Echo
                }
                USART_ClearITPendingBit(USART1, USART_IT_RXNE);  // Xóa
            }
        }            

</details>




<details>
	<summary><strong>BÀI 8: ADC </strong></summary> 


## BÀI 8: ADC

### **I. ADC**

#### **1.1. Định nghĩa**

* **ADC (Analog-to-Digital Converter)** là một mạch điện tử lấy điện áp tương tự (analog) làm đầu vào và chuyển đổi nó thành dữ liệu số (digital) – một giá trị đại diện cho mức điện áp dưới dạng mã nhị phân.

* ADC có chức năng chuyển đổi tín hiệu analog (liên tục) thành tín hiệu số (rời rạc) để vi điều khiển có thể xử lý.

![Image](https://github.com/user-attachments/assets/bb2f4a55-28b8-4af8-bd70-7f111aead2ec)
 


#### **1.2. Độ phân giải**

* **Định nghĩa:**
    
    ◦ Độ phân giải xác định số mức (levels) mà tín hiệu analog được chia trong quá trình chuyển đổi.

    ◦ Với **n bit**, ADC chia phạm vi điện áp thành **2^n mức**.

        STM32F103C8 có ADC 12-bit, tức 2^12 = 4096 mức.

* **Ý nghĩa:**
    
    ◦ Độ phân giải cao hơn → biểu diễn tín hiệu chính xác hơn, nhưng tốn nhiều tài nguyên (thời gian chuyển đổi và bộ nhớ).

* **Độ chính xác của mỗi mức được tính bằng:**

        Độ chính xác = Điện áp tham chiếu / Số mức = Vref / 2^n

    ◦ VD:

        Với điện áp tham chiếu 3.3V, độ phân giải 12bit cho độ chính xác mỗi mức là 3.3/4096 ≈ 0.805 mV/mức.

![Image](https://github.com/user-attachments/assets/893e3052-892a-42b7-bcc0-6fafbabe90ea)

* **Công thức chuyển đổi giá trị ADC sang điện áp thực tế:**

        V_in = (ADC_value × Vref) / 2^n

    ◦ VD:

        ADC_value = 2048 , Vref = 3.3V → V_in = (2048 × 3.3) / 4096 ≈ 1.65V.

#### **1.3. Tần số lấy mẫu**

* **Định nghĩa:** Số lần ADC lấy mẫu tín hiệu analog trong một giây (đơn vị: Hz).

* **Nguyên lý Nyquist-Shannon:** Để tái tạo chính xác, tần số lấy mẫu `f_s ≥ 2 × f_max`

    ◦  f_max: tần số cao nhất của tín hiệu

* **Trong STM32:** 

    ◦  Tần số lấy mẫu phụ thuộc vào **thời gian lấy mẫu (ADC_SampleTime)** và **tần số clock của ADC.**

    ◦  Tần số lấy mẫu càng cao, càng thu thập được nhiều mẫu, dẫn đến kết quả chính xác hơn, nhưng tiêu tốn nhiều tài nguyên CPU hơn.   

#### **1.4. Các thông số khác**

* **Điện áp tham chiếu (Vref):** 

    ◦  Là phạm vi điện áp analog mà ADC có thể đo, thường là 0 đến 3.3V hoặc 0 đến 5V trên STM32.

    ◦  Giá trị ADC nằm từ 0 đến 2^n - 1.

* **Thời gian chuyển đổi:** 

    ◦  Là thời gian cần để ADC hoàn thành một lần chuyển đổi từ tín hiệu analog sang số.

    ◦  Công thức: `T_conv = (T_sample / f_ADC) + (12.5 / f_ADC)  [giây]`

        T_sample: Thời gian lấy mẫu (có thể cấu hình, ví dụ: 1.5, 7.5, 55.5 chu kỳ).
        T_conversion: Thời gian chuyển đổi cố định (thường 12.5 chu kỳ cho STM32).

### **II. Sử dụng ADC trong STM32**

#### **2.1. Đặc điểm**

* STM32F103C8 được trang bị 2 bộ ADC (`ADC1` và `ADC2`) với các đặc điểm chính:

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

    ◦ Thích hợp cho đo cảm biến đơn lẻ.

* **Single Continuous Conversion**: 

    ◦ Đọc liên tục 1 kênh, tự lặp sau EOC.

    ◦ Giám sát liên tục (kết hợp DMA)

* **Scan Mode (Multi-Channel)** 

    ◦ ADC quét qua nhiều kênh, đọc lần lượt từng kênh khi được yêu cầu.

    ◦ Phù hợp khi cần đo nhiều tín hiệu analog từ các cảm biến khác nhau.

* **Scan Continuous Mode:** 
  
    ◦ Tương tự Scan Mode, nhưng chuyển đổi liên tục trên nhiều kênh, lặp lại sau khi hoàn thành một chu kỳ quét.

    ◦ Thu thập dữ liệu đa kênh thời gian thực.

##### **2.2.2. Injected Conversion(Chuyển đổi ưu tiên)**
   
* Dùng để xử lý các kênh ưu tiên cao, có thể ngắt các chuyển đổi tuần tự (regular) đang thực hiện.

* Khi gặp **Injected Trigger** thì ngay lập tức kênh đang hoạt động bị ngưng lại để kênh được ưu tiên kia có thể hoạt động.

* Số kênh: Tối đa 4 kênh trong chế độ injected.

        // Cấu hình Injected Channel (ví dụ: kênh 1, rank 1)

        ADC_InjectedChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);
        ADC_InjectedSequencerLengthConfig(ADC1, 1);  // Số kênh injected
        ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjConv_T2_TRGO);  // Trigger từ TIM2


### **III. Cấu hình ADC**

#### **3.1. Cấu hình Clock cho ADC**

* **Các bộ ADC được cấp xung từ RCC APB2**

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 | RCC_APB2Periph_GPIOA, ENABLE);

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

* **VD2:Multi-channel Scan Mode (2 kênh: PA0, PA1).** 

        ADC_InitStruct.ADC_NbrOfChannel = 2;
        ADC_InitStruct.ADC_ScanConvMode = ENABLE;
        ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);  // Rank 1: PA0
        ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);  // Rank 2: PA1

#### **3.4. Kích hoạt và đọc ADC**

* **Kích hoạt chuyển đổi:**

        ADC_SoftwareStartConvCmd(ADC1, ENABLE); //(cho single/scan).

* **Đọc giá trị:**

        while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
        uint16_t adc_value = ADC_GetConversionValue(ADC1);
        float voltage = (adc_value * 3.3f) / 4096.0f;  // Chuyển sang V

* **Ngắt ADC:**

        ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);  // Bật ngắt EOC
        // Trong ISR: void ADC1_Seq0_IRQHandler(void) { ... ADC_ClearITPendingBit(ADC1, ADC_IT_EOC); }


#### **3.5. Thời gian lấy mẫu**

* Thời gian lấy mẫu **(ADC_SampleTime)** quyết định thời gian ADC thu thập tín hiệu analog trước khi chuyển đổi:

    ◦ Các giá trị: 1.5, 7.5, 13.5, 28.5, 41.5, 55.5, 71.5, 239.5 chu kỳ.

    ◦ Ví dụ: `ADC_SampleTime_55Cycles5` cho thời gian lấy mẫu dài, tăng độ chính xác cho tín hiệu thay đổi chậm (như biến trở).

* Công thức thời gian chuyển đổi:

    ◦ `Tconv = Tsample + 12.5 chu kỳ`

### **IV. Các hàm  thông dụng**

#### **4.1. Hàm cấu hình kênh**


        ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel,uint8_t Rank, uint8_t ADC_SampleTime)


*  **ADCx:** Con trỏ đến khối ADC (ADC1 hoặc ADC2). Xác định ADC nào được cấu hình.

*  **ADC_Channel:** Số kênh (0-17, ví dụ: ADC_Channel_0 cho PA0).

*  **Rank:** Thứ tự kênh trong nhóm regular (1-16). Trong scan mode, ADC đọc theo thứ tự rank tăng dần.

*  **ADC_SampleTime:** Thời gian lấy mẫu (ví dụ: ADC_SampleTime_55Cycles5).

#### **4.2. Hàm bắt đầu chuyển đổi**


        ADC_SoftwareStartConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);


*  **ADCx:** Con trỏ đến khối ADC (ADC1 hoặc ADC2). Xác định ADC nào được cấu hình.

*  **NewState:** ENABLE để bắt đầu chuyển đổi bằng phần mềm; DISABLE để dừng.


#### **4.3. Hàm đọc giá trị ADC**


        uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx);


*  **ADCx:** Con trỏ đến khối ADC (ADC1 hoặc ADC2). Xác định ADC nào được cấu hình.

*  Trả về giá trị 16-bit từ thanh ghi DR (dữ liệu thực tế chỉ dùng 12-bit thấp nhất nếu căn phải).


#### **4.4. Hàm đọc giá trị ở chế độ kép**


        uint32_t ADC_GetDualModeConversionValue(void);


*  Không có tham số. Trả về giá trị 32-bit kết hợp từ ADC1 (16-bit thấp) và ADC2 (16-bit cao) trong chế độ dual.

#### **4.5. Hàm kiểm tra trạng thái**


        FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, uint8_t ADC_FLAG);

*  **ADCx:** Con trỏ đến khối ADC (ADC1 hoặc ADC2). Xác định ADC nào được cấu hình.

*  **ADC_FLAG:** Cờ trạng thái (ví dụ: ADC_FLAG_EOC cho end of conversion, ADC_FLAG_AWD cho analog watchdog).

#### **4.6. Các hàm hiệu chỉnh:**

        void ADC_ResetCalibration(ADC_TypeDef* ADCx);

*  **Reset thanh ghi hiệu chỉnh để chuẩn bị.**

        FlagStatus ADC_GetResetCalibrationStatus(ADC_TypeDef* ADCx);

*  **Trả về SET nếu đang reset, RESET nếu hoàn tất.**

        void ADC_StartCalibration(ADC_TypeDef* ADCx);

*  **Bắt đầu hiệu chỉnh (đo offset và gain nội bộ).**

        FlagStatus ADC_GetCalibrationStatus(ADC_TypeDef* ADCx);

*  **Tương tự, kiểm tra tiến trình hiệu chỉnh.**


### **V. Bộ lọc Kalman**

#### **5.1.Định nghĩa**

* Bộ lọc Kalman là thuật toán toán học ước lượng trạng thái tối ưu của hệ thống tuyến tính từ các quan sát bị nhiễu (noise).

* Ứng dụng: Điều khiển (robot), định vị (GPS), xử lý tín hiệu (lọc nhiễu ADC). Nó kết hợp mô hình dự đoán (dựa trên vật lý) và dữ liệu đo để giảm sai số.

#### **5.2.Đặc điểm**

##### **5.2.1. Giai đoạn dự đoán(Prediction):** 

* Dự đoán trạng thái tiếp theo dựa trên mô hình hệ thống và trạng thái trước đó.

* Tính toán độ không chắc chắn (covariance) của dự đoán.

![Image](https://github.com/user-attachments/assets/f638e348-1525-4589-8793-7e51285f67b4)

##### **5.2.2. Giai đoạn cập nhật(Correction):**

* Kết hợp dữ liệu đo mới (có thể chứa nhiễu) với dự đoán để cập nhật trạng thái.

* Tính toán Kalman Gain để cân bằng giữa dự đoán và đo lường thực tế.

![Image](https://github.com/user-attachments/assets/22643b2a-9310-4376-a93e-0ae6e8b2b3ee)

##### **5.2.3. Các tham số cần chú ý**

* `mea_e (R):` Sai số nhiễu đo lường, đại diện cho độ tin cậy của cảm biến.

* `est_e (P):` Sai số ước lượng ban đầu, phản ánh độ không chắc chắn của dự đoán.

* `q (Q:` Nhiễu quá trình, biểu thị sự không chắc chắn trong mô hình hệ thống.

##### **5.2.4. Hàm khởi tạo bộ lọc Kalman**

        void SimpleKalmanFilter(float mea_e, float est_e, float q) {
            _err_measure = mea_e;
            _err_estimate = est_e;
            _q = q;
            _current_estimate = 0;
            _last_estimate = 0;
        }


##### **5.2.5. Hàm cập nhật giá trị ước lượng qua bộ lọc Kalman**

        #include <math.h>

        float updateEstimate(float mea) {
            _kalman_gain = _err_estimate / (_err_estimate + _err_measure);
            _current_estimate = _last_estimate + _kalman_gain * (mea - _last_estimate);
            _err_estimate = (1.0 - _kalman_gain) * _err_estimate + fabs(_last_estimate - _current_estimate) * _q;
            _last_estimate = _current_estimate;
            return _current_estimate;
        }


##### **5.2.6. VD**

        #include "stm32f10x.h"  
        #include <math.h>

        // Biến toàn cục Kalman
        float _err_measure, _err_estimate, _q;
        float _current_estimate, _last_estimate, _kalman_gain;

        void SimpleKalmanFilter(float mea_e, float est_e, float q) {
            _err_measure = mea_e;
            _err_estimate = est_e;
            _q = q;
            _current_estimate = 0;
            _last_estimate = 0;
        }

        float updateEstimate(float mea) {
            _kalman_gain = _err_estimate / (_err_estimate + _err_measure);
            _current_estimate = _last_estimate + _kalman_gain * (mea - _last_estimate);
            _err_estimate = (1.0f - _kalman_gain) * _err_estimate + fabs(_last_estimate - _current_estimate) * _q;
            _last_estimate = _current_estimate;
            return _current_estimate;
        }

        int main(void) {

            ADC_Config();
            GPIO_Config();
            RCC_Config();  // Clock

            float val, val_update;
            SimpleKalmanFilter(1.0f, 2.0f, 0.001f);  // R=1, P=2, Q=0.001

            while (1) {
                // Đọc ADC (giả sử đã kích hoạt continuous)
                while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
                val = (float)ADC_GetConversionValue(ADC1);

                // Lọc Kalman (normalize nếu cần: val /= 4096.0f * 3.3f cho voltage)
                val_update = updateEstimate(val);

                Delay_ms(100); 
            }
        }

</details>



<details>
	<summary><strong>BÀI 9: DMA và PWM</strong></summary> 

## Bài 9: DMA và PWM

### **I.CPU**

#### **1.1. Đặc điểm**

* Trong hệ thống nhúng như STM32 (dựa trên lõi ARM Cortex-M3), CPU (Central Processing Unit)  chịu trách nhiệm thực thi các lệnh chương trình, xử lý dữ liệu và điều khiển ngoại vi.

* CPU hoạt động theo **kiến trúc Von Neumann** (bộ nhớ thống nhất cho lệnh và dữ liệu), với các thành phần chính:

    ◦ **ALU (Arithmetic Logic Unit):** Thực hiện các phép toán số học và logic.

    ◦ **Control Unit:** Giải mã lệnh và điều khiển luồng thực thi.

    ◦ **Registers:** Các thanh ghi nội bộ lưu trữ dữ liệu tạm thời (ví dụ: R0-R15 trong ARM)

    ◦ **Bus Interface:** Kết nối với bộ nhớ (AHB/APB bus) và ngoại vi.

#### **1.2. Chu kỳ hoạt động của CPU**


* **1. Fetch:** 

    ◦ Lấy lệnh từ bộ nhớ (FLASH) qua bus địa chỉ.

* **2. Decode:** 

    ◦ Giải mã lệnh để hiểu hành động cần thực hiện.

* **3. Execute::** 

    ◦ Thực thi lệnh, có thể bao gồm đọc/ghi dữ liệu từ RAM hoặc ngoại vi.

* **4. Store/Write-back:** 

    ◦ Lưu kết quả nếu cần.

#### **1.3. Chu kỳ hoạt động của CPU trong STM32**

<img width="499" height="330" alt="Image" src="https://github.com/user-attachments/assets/d110d322-e545-4a6e-866b-b7f33546fd28" />

* **(1)** CPU lấy lệnh chương trình từ FLASH để xử lý

* **(2)**  CPUđọc/ghi những dữ liệu từ/vào các ngoại vi thông qua đường bus ngoại vi

* **(3)** Lưu các dữ liệu vừa đọc được vào RAM 

* **(4)** Giao tiếp với RAM (đọc/ghi các dữ liệu) thông qua đường bus bộ nhớ

**Hậu quả: CPU bị chiếm dụng quá nhiều, giảm hiệu suất chương trình** 
 
**=> Để khắc phục, sử dụng DMA (Direct Memory Access)** 

### **II.Giới thiệu về DMA**

#### **2.1.Định nghĩa**

* **DMA (Direct Memory Access)** là 1 cơ chế cho phép các thiết bị ngoại vi truyền dữ liệu trực tiếp đến bộ nhớ mà không cần CPU can thiệp và thực hiện từng bước truyền dữ liệu

![Image](https://github.com/user-attachments/assets/2cf6eb88-05b1-4fe0-b95d-8fb6d5ce901a)

* **Quy trình hoạt động:** 

    ◦ **CPU** cấu hình và kích hoạt **DMA** hoạt động

    ◦ Ngoại vi sẽ sử dụng **DMA Request** để yêu cầu **DMA** gửi/nhận dữ liệu ngoại vi

    ◦ **DMA** tiến hành thực hiện yêu cầu từ **DMA Request**

    ◦ Lấy dữ liệu từ **SRAM** thông qua bus matrix ,đi qua các đường bus ngoại vi rồi truy cập các thanh ghi của ngoại vi

    ◦ Khi **kết thúc**, **DMA** kích hoạt ngắt báo cho **CPU** biết là quá trình hoàn tất

#### **2.2.DMA trong STM32**

* **STM32F1 có 2 bộ DMA:**

    ◦  **DMA1:** 7 kênh
    
    ◦  **DMA2:** 5 kênh

* **Đặc điểm chính:**

    ◦ Có 2 chế độ hoạt động (**Normal và Circular**).

    ◦ Mỗi kênh có thể cấu hình riêng

    ◦ Mỗi kênh có thể phục vụ **nhiều ngoại vi** khác nhau nhưng **không đồng thời**

    ◦ Có mức ưu tiên (Priority) để lập trình cho các kênh (Low, Medium, High, Very High).

    ◦ Có thể sử dụng DMA với 5 cờ báo ngắt: **DMA Half Transfer**, **DMA Transfer complete**, **DMA Transfer Error**, **DMA FIFO Error**, **Direct Mode Error** 

   ![Image](https://github.com/user-attachments/assets/1fd23a47-dda9-4bc1-b4d5-66752f038651)


* **Chế độ hoạt động:**

  ◦ **Normal Mode:** 

        DMA truyền dữ liệu cho tới khi truyền đủ 1 lượng dữ liệu giới hạn đã khai báo DMA sẽ dừng hoạt động.

        Muốn nó tiếp tục phải khởi động lại

   ◦ **Circular Mode:**

        DMA truyền đủ dữ liệu giới hạn đã khai báo thì nó sẽ truyền tiếp về vị trí ban đầu, không dừng lại


#### **2.3.Cấu hình DMA trong STM32**

* **Bước 1:** Cấp xung clock từ AHB.

        void RCC_Config(void){
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
        }


* **Bước 2:** Cấu hình các tham số chính (sử dụng struct `DMA_InitTypeDef`):

    ◦ **DMA_PeripheralBaseAddr:** Cấu hình địa chỉ ngoại vi cho DMA.

        Đây là địa chỉ mà DMA sẽ lấy data hoặc truyền data tới ngoại vi

    ◦ **DMA_MemoryBaseAddr:** Cấu hình địa chỉ vùng nhớ cần ghi/đọc data

    ◦ **DMA_DIR:** Cấu hình hướng truyền DMA, từ ngoại vi tới vùng nhớ hay từ vùng nhớ tới ngoại vi

        DMA_DIR_PeripheralSRC (từ ngoại vi → nhớ)

        DMA_DIR_PeripheralDST (từ nhớ → ngoại vi)

        DMA_DIR_MemoryToMemory

    ◦ **DMA_BufferSize:** Kích thước buffer (số lượng data cần truyền).

    ◦ **DMA_PeripheralInc:** Cấu hình địa chỉ ngoại vi có tăng sau tăng hay không

        DMA_PeripheralInc_Enable/Disable

    ◦ **DMA_MemoryInc:** Cấu hình địa chỉ bộ nhớ có tăng hay không

        DMA_MemoryInc_Enable/Disable

    ◦ **DMA_PeripheralDataSize:** Kích thước data ngoại vi (Byte, HalfWord, Word).

    ◦ **DMA_MemoryDataSize:** Kích thước data bộ nhớ (Byte, HalfWord, Word).

    ◦ **DMA_Mode:** Chế độ: DMA_Mode_Normal hoặc DMA_Mode_Circular.

    ◦ **DMA_Priority:** Cấu hình độ ưu tiên cho kênh DMA

        DMA_Priority_Low/Medium/High/Very_High

    ◦ **DMA_M2M:** Cấu hình sử dụng truyền từ bộ nhớ đếm bộ nhớ cho kênh DMA

        Truyền giữa bộ nhớ: DMA_M2M_Enable/Disable

* **VD:cấu hình (DMA1_Channel2 cho SPI Rx)**


        void DMA_Config(void) {
            DMA_InitTypeDef DMA_InitStructure;
            
            DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
            DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  // Từ ngoại vi → nhớ
            DMA_InitStructure.DMA_BufferSize = 35;
            DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)buffer;  // Địa chỉ buffer SRAM
            DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
            DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
            DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;  // Thanh ghi SPI Data
            DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
            DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
            DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
            DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
            
            DMA_Init(DMA1_Channel2, &DMA_InitStructure);
            DMA_Cmd(DMA1_Channel2, ENABLE);  // Kích hoạt kênh DMA
            
            // Kích hoạt DMA cho ngoại vi (ví dụ: SPI)
            SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);
        }


#### **2.4.Các hàm thông dụng**

* Sau khi lưu những cấu hình `DMA_Init()` và cho phép bộ DMA hoạt động `DMA_Cmd()`, ta tiến hành cho phép DMA làm việc với ngoại vi bằng hàm `<Periph>_DMACmd()` 

    ◦   **`void SPI_I2S_DMACmd(SPI_TypeDef* SPIx, uint16_t SPI_I2S_DMAReq, FunctionState NewState)`** : Cho SPI/I2S.

    ◦   **`void I2C_DMACmd(I2C_TypeDef* I2Cx, FunctionalState NewState)`**  : Cho I2C.

    ◦   **`void USART_DMACmd(USART_TypeDef* USARTx, uint16_t USART_DMAReq, FunctionalState NewState)`** : Cho USART.

    ◦   **`void ADC_DMACmd(ADC_TypeDef* ADCx, FunctionalState NewState)`** : Cho ADC.

* Để xử lý ngắt DMA, sử dụng `DMA_ITConfig()` để kích hoạt ngắt và hàm callback trong NVIC để xử lý sự kiện hoàn tất.

        DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);

### **III.PWM**

#### **3.1.Định nghĩa**

* **PWM (Pulse Width Modulation)** là kỹ thuật tạo tín hiệu xung vuông với độ rộng xung thay đổi để điều khiển công suất hoặc vị trí (ví dụ: góc quay của động cơ Servo).

* Tín hiệu PWM có **2 yếu tố quyết định:**

    ◦ **Tần số (Frequency):** Số lần tín hiệu lặp lại 1 giây. Với servo,tần số thông thường là 50HZ (chu kỳ 20 ms).

    ◦ **Độ rộng xung (Pulse Width):** Thời gian tín hiệu ở mức cao trong mỗi chu kỳ (đo bằng µs). Quyết định góc quay Servo (Duty Cycle = Pulse Width / Chu kỳ).

  
![Image](https://github.com/user-attachments/assets/4e235026-e41a-4169-87bb-c254b332b343)

#### **3.2.Đặc điểm**

* Phạm vi độ rộng xung cho Servo (`0° - 180°`): `1000 µs (0°)` → `2000 µs (180°)`.


![Image](https://github.com/user-attachments/assets/d05e1663-c5c9-4627-9f8c-53957de8b4f9)

* Công thức tính độ rộng xung

        pulseWidth = MIN_PULSE_WIDTH + (MAX_PULSE_WIDTH - MIN_PULSE_WIDTH) * (angle / 180);

    ◦ **MIN_PULSE_WIDTH:** 1000 µs (góc 0°).

    ◦ **MAX_PULSE_WIDTH:** 2000 µs (góc 180°).

    ◦ **angle:** Góc mong muốn (0-180°).

* Công thức tính Duty Cycle

        Duty Cycle (%) = (Pulse Width / Chu kỳ) × 100.

    ◦ Với chu kỳ 20 ms, Duty Cycle từ 5% (1000 µs) đến 10% (2000 µs).

#### **3.3.Cấu hình**

* PWM được tạo bởi **Timer (TIM)** và xuất ra chân GPIO ở chế độ **AF_PP (Alternate Function Push-Pull)**.

* **Bước 1:** Cấu hình GPIO.

        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  // Ví dụ: PA0 cho TIM2_CH1
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

* **Bước 2:** Cấu hình Timer Time Base (để đạt tần số 50 Hz, tick 1 µs).

    ◦ Sử dụng Prescaler và Period (ARR) để Timer đếm mỗi 1 µs và tràn mỗi 20 ms.

    ◦ Giả sử hệ clock 72 MHz (STM32F1):

        Prescaler = 71 → Tick = 72 MHz / (71+1) = 1 MHz (1 µs/tick).

        Period (ARR) = 19999 → Chu kỳ = 20 ms (20000 ticks).   


            TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

            TIM_TimeBaseStructure.TIM_Period = 19999;  // ARR = 20 ms - 1
            TIM_TimeBaseStructure.TIM_Prescaler = 71;  // 1 µs/tick
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
            TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
            TIM_ARRPreloadConfig(TIM2, ENABLE);  // Bật preload ARR để tránh giật

* **Bước 3:** Cấu hình Timer Output Compare (PWM Mode) (struct `TIM_OCInitTypeDef`):

    ◦ TIM_OCMode

        Chế độ: TIM_OCMode_PWM1 (cao khi đếm < CCR),

        TIM_OCMode_PWM2 (ngược lại).


    ◦ TIM_OutputState

        Kích hoạt đầu ra: TIM_OutputState_Enable/Disable

    ◦ TIM_OCMode

        Chế độ: TIM_OCMode_PWM1 (cao khi đếm < CCR),

        TIM_OCMode_PWM2 (ngược lại).


    ◦ TIM_Pulse

        Giá trị CCR (độ rộng xung): 1000-2000 cho Servo. Duty = [CCR / (ARR+1)] × 100%.

    ◦ TIM_OCPolarity

        Cực tính: TIM_OCPolarity_High (cao khi < CCR)

        TIM_OCPolarity_Low (thấp khi < CCR)

    ◦ TIM_OCIdleState

        Xác định trạng thái tín hiệu PWM khi Timer ở trạng thái "Idle" (không hoạt động, thường dùng trong điều khiển động cơ).

        TIM_OCIdleState_Set: Mức cao khi Idle.

        TIM_OCIdleState_Reset: Mức thấp khi Idle (mặc định).

    ◦ TIM_OutputNState

        Kích hoạt hoặc vô hiệu hóa tín hiệu bổ sung (complementary output, ví dụ: TIM1_CH1N)

        TIM_OutputNState_Enable: Bật tín hiệu bổ sung.

        TIM_OutputNState_Disable: Tắt tín hiệu bổ sung

    ◦ TIM_OCNIdleState

        Xác định trạng thái tín hiệu bổ sung khi Timer ở trạng thái Idle.

        Giá trị: TIM_OCNIdleState_Set hoặc TIM_OCNIdleState_Reset.

* Ngoài các hàm bạn đã đề cập (TIM_OCxInit, TIM_OCxPreloadConfig, TIM_Cmd, TIM_SetComparex), dưới đây là các hàm SPL bổ sung cần chú ý khi làm việc với PWM:

    ◦ **TIM_ARRPreloadConfig(TIMx, FunctionalState):**

        Bật/tắt tính năng preload cho thanh ghi ARR

        Khi bật, giá trị TIM_Period chỉ được cập nhật khi xảy ra Update Event, giúp tránh giật tín hiệu PWM khi thay đổi chu kỳ

  
    ◦ **TIM_CtrlPWMOutputs(TIMx, FunctionalState) (chỉ áp dụng cho TIM1):**

        Kích hoạt đầu ra PWM cho Timer nâng cao (TIM1).

        Cần gọi TIM_CtrlPWMOutputs(TIM1, ENABLE) để đảm bảo tín hiệu PWM được xuất ra chân GPIO.

        Nếu không gọi, PWM sẽ không hoạt động trên TIM1.

  
    ◦ **TIM_SetCounter(TIMx, value):**

        Đặt giá trị hiện tại của bộ đếm Timer. Dùng để đồng bộ hóa hoặc đặt lại Timer trong các ứng dụng PWM đặc biệt.

    ◦ **TIM_GetCapturex(TIMx):**

        Lấy giá trị hiện tại của thanh ghi so sánh (CCR) cho kênh x. Hữu ích để kiểm tra duty cycle hiện tại.

    ◦ **TIM_BDTRConfig(TIMx, TIM_BDTRInitTypeDef) (chỉ áp dụng cho TIM1):**

        Cấu hình các tính năng nâng cao như Break and Dead-time (dùng trong điều khiển động cơ hoặc inverter).

        Các tham số quan trọng trong TIM_BDTRInitTypeDef:

           TIM_DeadTime: Thời gian chết giữa tín hiệu chính và bổ sung.

           TIM_Break: Bật/tắt chức năng Break (ngắt tín hiệu PWM khi có lỗi)

           IM_AutomaticOutput: Tự động khôi phục đầu ra PWM sau sự kiện Break.
  
    ◦ **TIM_ClearOCxRef(TIMx, TIM_OCx):**

        Xóa tín hiệu PWM trên kênh x, dùng để tạm dừng PWM mà không tắt Timer.

* **Xử lý ngắt:** Sử dụng `TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE)` cho sự kiện tràn (update event), kết nối với NVIC để cập nhật duty cycle động.

  </details>


 
<details>
	<summary><strong>BÀI 10: FLASH VÀ BOOTLOADER </strong></summary>

## Bài 10: FLASH VÀ BOOTLOADER

### **I.Các loại bộ nhớ cơ bản**

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

### **II.FLASH**

#### **2.1.Giới thiệu**

##### **2.1.1.Quản lý**

* Flash sẽ được chia thành các **Page (trang)** hoặc **Sector (cấp)** để quản lý

* Kích thước mỗi page/sector phụ thuộc vào dòng vi điều khiển cụ thể (ví dụ: 1KB, 2KB, thậm chí 128KB trên các dòng cao cấp).

* Trước khi ghi phải xóa.Mỗi lần xóa, cả page sẽ bị xóa trắng về giá trị **(0xFF)**

##### **2.1.2.Ghi dữ liệu**

* Khi ghi dữ liệu,**chỉ** có thể ghi từ **0** sang **1** (tức là từ 0xFF ghi thành một giá trị khác)

    => Do đó, chỉ có thể ghi các đơn vị dữ liệu như **half-word (2 byte)** hoặc **word (4 byte)** tại một thời điểm 

##### **2.1.3.Giới hạn**

* Flash có **giới hạn** về số lần xóa/ghi

* Chương trình (firmware) sẽ được nạp vào vùng nhớ từ**0x08000000**, vùng nhớ phía sau firmware sẽ là trống và người dùng có thể lưu trữ dữ liệu ở vùng này

![Image](https://github.com/user-attachments/assets/229b15b3-74c2-42e9-b0dc-a1df7fb76f18)


#### **2.2.Xóa FLASH**

##### **2.2.1.Các bước**

![Image](https://github.com/user-attachments/assets/9c5de0e7-da99-4993-aae7-9c9d9e59ed6b)

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

##### **2.2.2.VD**

*  Xóa một page

        #include "stm32f10x_flash.h"  // Thư viện FLASH

        #define FLASH_ER_PTIMEOUT  (uint32_t)0x00B4FFFFU  // Timeout xóa

        FLASH_Status Flash_ErasePage(uint32_t pageAddress) {
            FLASH_Status status = FLASH_COMPLETE;

            // 1. Unlock
            FLASH_Unlock();

            // 2. Chờ sẵn sàng
            status = FLASH_WaitForLastOperation(FLASH_ER_PTIMEOUT);
            if (status == FLASH_COMPLETE) {
                // 3-5. Xóa page
                status = FLASH_ErasePage(pageAddress);  // pageAddress phải align 2KB
            }

            // 6-7. Chờ và kiểm tra lỗi
            status = FLASH_WaitForLastOperation(FLASH_ER_PTIMEOUT);
            if (status != FLASH_COMPLETE) {
                // Xử lý lỗi: Xóa cờ lỗi
                FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
            }

            // 8. Lock
            FLASH_Lock();

            return status;
        }

  
#### **2.3.Ghi FLASH**

##### **2.3.1. Các bước**

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

##### **2.3.2. VD**

* Ghi mảng half-word

        void Flash_WriteArray(uint32_t startAddress, uint16_t *data, uint16_t length) {
            FLASH_Status status = FLASH_COMPLETE;
            uint32_t address = startAddress;  // Phải align 2-byte

            FLASH_Unlock();

            for (uint16_t i = 0; i < length; i++) {
                status = FLASH_WaitForLastOperation(FLASH_PG_PTIMEOUT);  // Timeout ghi
                if (status != FLASH_COMPLETE) break;

                status = FLASH_ProgramHalfWord(address, data[i]);  // Ghi 2 byte
                if (status != FLASH_COMPLETE) break;

                address += 2;
            }

            FLASH_WaitForLastOperation(FLASH_PG_PTIMEOUT);  // Chờ cuối
            if (status != FLASH_COMPLETE) {
                FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
            }

            FLASH_Lock();
        }

#### **2.4.Các hàm thông dụng**

* **Hàm LOCK/UNLOCK FLASH**

    ◦ void FLASH_Unlock(void): Hàm này Unlock tất cả vùng nhớ trong Flash

    ◦ void FLASH_UnlockBank1(void): Hàm này chỉ Unlock cho Bank đầu tiên

    ◦ void FLASH_UnlockBank2(void): Hàm này Unlock cho Bank 2

    ◦ void FLASH_Lock(void): Lock bộ điều khiển xóa Flash cho toàn bộ vùng nhờ Flash

    ◦ void FLASH_LockBank1(void) và void FLASH_LockBank2(void) : Lock toàn bộ điều khiển FLASH cho Bank 1 và 2


* **Hàm xóa FLASH**


    ◦ FLASH_Status FLASH_EraseAllBank1Pages(void): Xóa tất cả các Page trong Bank 1 của FLASH

    ◦ FLASH_Status FLASH_EraseAllBank2Pages(void): Xóa tất cả các Page trong Bank 2 của FLASH

    ◦ FLASH_Status FLASH_EraseAllPages(void): Xóa toàn bộ FLASH

    ◦ FLASH_Status FLASH_ErasePage(uint32_t Page_Address): Xóa 1 Page cụ thể trong Flash, cụ thể là Page bắt đàu bằng địa chỉ Page_Address


* **Hàm ghi FLASH**


    ◦ FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data) : Ghi dữ liệu vào vùng nhớ Addres với kích thước mỗi 2 byte

    ◦ FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data) : Ghi dữ liệu vào vùng nhớ Address với kích thước mỗi 4 byte


* **Hàm kiểm tra trạng thái**


    ◦ FLASH_Status FLASH_GetStatus(void)

    ◦ FLASH_Status FLASH_GetFlagStatus(uint32_t FLASH_FLAG): Hàm này trả về trạng thái của FLag



* **VD:**


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


### **III.BOOTLOADER**

#### **3.1.Định nghĩa**

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



#### **3.2.Bootloader hoạt động như thế nào**

##### **3.2.1.Quá trình khởi động của ARM Cortex-M**

*  **1. Thiết lập chế độ Boot**

    ◦  Khi khởi động, vi điều khiển đọc các chân BOOT0 và (thường là) BOOT1 để quyết định vùng nhớ nào sẽ được ánh xạ đến địa chỉ 0x00000000. 
    
    ◦  Thông thường, bộ nhớ Flash gốc (0x08000000) được ánh xạ đến địa chỉ 0.

        Đọc BOOT0/BOOT1 (BOOT0=0: Flash; =1: System Memory).

*  **2. Khởi tạo Vector Table:**

    ◦  CPU lấy giá trị Word đầu tiên (tại địa chỉ 0x00000000) để khởi tạo con trỏ ngăn xếp (MSP - Main Stack Pointer).

    ◦  CPU lấy Word thứ hai (tại địa chỉ 0x00000004) – đây là địa chỉ của hàm Reset_Handler – và nạp vào thanh ghi PC (Program Counter).

        MSP từ 0x00000000; PC từ 0x00000004 (Reset_Handler).

*  **3. Thực thi Bootloader:**

    ◦  CPU bắt đầu chạy code trong Reset_Handler, sau đó nhảy đến hàm main() của chương trình được lưu trong vùng nhớ boot (thường là Bootloader).

        Từ Reset_Handler → main() của Bootloader.

##### **3.2.2. Cơ chế nhảy từ Bootloader sang Application**

* **Giả sử:**

    ◦  Bootloader nằm ở đầu bộ nhớ Flash (ví dụ: từ 0x08000000 đến 0x0800BFFF).

    ◦  **Bootloader sẽ thực hiện các bước sau để nhảy sang Application:**

        1. Kiểm tra Application: Bootloader kiểm tra một điều kiện (ví dụ: phím nhấn, lệnh từ UART, hoặc kiểm tra signature của Application) để quyết định có nhảy sang App hay không. Nếu không, nó có thể ở lại chế độ cập nhật firmware.

        2. Tắt mọi ngắt và ngoại lệ: __disable_irq();

        3. De-init các ngoại vi: Khởi tạo lại các ngoại vi mà Bootloader đã sử dụng (như UART, SPI) về trạng thái mặc định.
  
        4. Cấu hình lại Vector Table: Đây là bước QUAN TRỌNG NHẤT. Thanh ghi SCB->VTOR (Vector Table Offset Register) phải được đặt thành địa chỉ bắt đầu của Application's Vector Table.
   

            SCB->VTOR = APPLICATION_ADDRESS; // Ví dụ: 0x0800C000

    
        5. Cấu hình lại con trỏ ngăn xếp: Lấy giá trị MSP mới từ word đầu tiên của Application's Vector Table.


            __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);


        6. Nhảy đến Reset Handler của Application: Lấy địa chỉ của Reset Handler (word thứ hai trong Vector Table) và gọi nó như một hàm.

            void (*app_reset_handler)(void) = (void (*)(void)) *(__IO uint32_t*)(APPLICATION_ADDRESS + 4);

            app_reset_handler(); // Nhảy không trở lại

    ◦  Lưu ý:
   

        Application phải được biên dịch và nạp để chạy tại địa chỉ bộ nhớ đã định (ví dụ: 0x0800C000).

        Trong script linker (.ld) của Application, cần phải thay đổi địa chỉ bắt đầu của bộ nhớ FLASH cho phù hợp.

        Vector Table của Application phải được định nghĩa chính xác tại địa chỉ đó.

    ◦  vd:

        #include "stm32f10x.h"  // Core peripherals
        #include "core_cm3.h"   // SCB cho VTOR

        #define APP_ADDRESS 0x0800C000  // Địa chỉ App

        // Kiểm tra App tồn tại (ví dụ: kiểm tra stack pointer != 0)
        uint8_t CheckAppValid(void) {
            if ((*(uint32_t*)APP_ADDRESS & 0x2FFE0000) == 0x20000000) {  // MSP hợp lệ
                return 1;  // OK
            }
            return 0;  // Không có App
        }

        void JumpToApp(void) {
            if (!CheckAppValid()) return;  // Ở lại Bootloader

            // 1. Tắt ngắt
            __disable_irq();

            // 2. De-init ngoại vi (ví dụ: UART, Timer)
            RCC_DeInit();
            // ... (de-init cụ thể)

            // 3. Set Vector Table Offset
            SCB->VTOR = APP_ADDRESS;

            // 4. Set MSP từ App Vector Table
            __set_MSP(*(__IO uint32_t*)APP_ADDRESS);

            // 5. Nhảy đến Reset_Handler của App
            void (*appResetHandler)(void) = (void*) *((uint32_t*)(APP_ADDRESS + 4));
            appResetHandler();  // Không return
        }

        int main() {
            // Init UART cho lệnh cập nhật...
            // ...

            // Kiểm tra nút bấm hoặc lệnh UART
            if (/* Điều kiện cập nhật */) {
                // Nhận firmware mới, xóa/ghi App (dùng Flash_WriteArray)
                // ...
            } else {
                JumpToApp();  // Nhảy sang App
            }

            while (1) {  // Chế độ cập nhật
                // Xử lý lệnh UART...
            }
        }

</details>



<details>
    <summary><strong>BÀI 11: CAN (CONTROLLER AREA NETWORK)</strong></summary>

## **BÀI 11: CAN (CONTROLLER AREA NETWORK)**

### **PHẦN 1: LÝ THUYẾT**

### **I.  TỔNG QUAN VỀ CAN**

#### **1.1. Khái niệm**

<img width="451" height="348" alt="Image" src="https://github.com/user-attachments/assets/ca97b73f-5a72-4d40-b0f7-e6ecce4e11e9" />

* CAN là một giao thức truyền thông nối tiếp được thiết kế để cho phép nhiều bộ điều khiển điện tử (ECU) hoặc các nút (nodes) giao tiếp với nhau một cách hiệu quả, đáng tin cậy trên cùng một đường truyền bus chung

<img width="733" height="317" alt="Image" src="https://github.com/user-attachments/assets/f6feac89-3dbe-4b0d-87f7-a6683834ea48" />


* CAN hoạt động theo nguyên tắc multi-master (đa chủ), không cần máy chủ trung tâm, và sử dụng cơ chế arbitration bitwise (trọng tài theo bit) để giải quyết xung đột khi nhiều nút truyền dữ liệu đồng thời.

<img width="438" height="222" alt="Image" src="https://github.com/user-attachments/assets/9f6ed6d9-3cfd-4b6c-8e94-db08bb279445" />

* CAN sử dụng hai dây dẫn (CAN High và CAN Low) với kỹ thuật truyền differential signaling (tín hiệu vi sai), giúp truyền dữ liệu dưới dạng sự chênh lệch điện áp giữa hai dây.  

#### **1.2. Giới thiệu**

* CAN là giao thức lớp vật lý và lớp liên liên kết dữ liệu (theo mô hình OSI)

*  Nó cho phép các thiết bị trao đổi thông điệp ngắn (thường 0 - 8 byte ở Classical CAN, hoặc cao hơn ở CAN FD) theo kiểu broadcast

	* Mọi nút trên bus đều nhận được thông điệp, nhưng chỉ các nút quan tâm mới xử lý dựa trên ID  

* Không có địa chỉ đích cụ thể, thông điệp được xác định bởi ID 
	
	* 11 bit ở CAN 2.0A hoặc 29 bit ở CAN 2.0B
	
	* Đồng thời ID cũng quyết định mức ưu tiên
	
* Sử dụng các cơ chế phát hiện lỗi:

	* CRC (Cyclic Redundancy Check) 15-bit
	
	* Acknowledgment
	
	* Error Counters
	
	* Các trạng thái lỗi  (Error Active, Error Passive, Bus Off).      

#### **1.3. Ứng dụng**

<img width="678" height="480" alt="Image" src="https://github.com/user-attachments/assets/9d072cc7-529b-41fb-a4ea-8eaf6d269fe0" />


* Trong ô tô:

	* CAN là bộ não của xe hơi hiện đại
	
	* Một chiếc xe ngày nay có thể có 50-70 ECU hoặc hơn
		
		* ECU động cơ (Engine Control Unit)
		
		* Hộp số
		
		* ABS (phanh chống bó cứng)
		
		* Túi khí (Airbag)
		
		* ADAS       

	* CAN giúp trao đổi dữ liệu thời gian thực: Tốc độ xe, vị trí ga, góc vô lăng, tình trạng phanh... giữa các ECU.
	
	* Chẩn đoán lỗi (Diagnostics) qua OBD-II (On-Board Diagnostics).
	
	* Được sử dụng trong hệ thống  thân xe (Body Control Module), hệ thống tiện nghi

<img width="1002" height="436" alt="Image" src="https://github.com/user-attachments/assets/170b6cae-f6d7-4d3a-94d3-dcdc28bbe173" />

* Trong công nghiệp:

	* CAN được sử dụng rộng rãi qua các giao thức lớp ứng dụng như CANopen (phổ biến ở châu Âu), DeviceNet (phổ biến ở Bắc Mỹ), và các biến thể khác.
	
		* Dây chuyền sản xuất: Kết nối PLC, cảm biến, actuator, robot, máy CNC, băng chuyền.
		
		*  Điều khiển quá trình (Process Control): Nhà máy hóa chất, thực phẩm, giám sát nhiệt độ, áp suất, lưu lượng.
	
	*  Trong tự động hóa, CAN cho phép phân tán điều khiển (decentralized control), giảm nhu cầu dây dẫn và tăng tính linh hoạt khi mở rộng hệ thống.   

#### **1.3. Đặc điểm**

<img width="697" height="304" alt="Image" src="https://github.com/user-attachments/assets/4508e67c-1e88-439f-8cab-f544a6ac1587" />

* Giảm đáng kể số lượng dây dẫn:

	* Trước CAN: Mỗi cặp ECU/cảm biến cần dây dẫn riêng (point-to-point), dẫn đến hệ thống dây harness cồng kềnh, nặng nề, chi phí cao và khó bảo trì.
	
	* Với CAN: Tất cả các nút chia sẻ chỉ 2 dây dẫn (twisted pair) theo cấu trúc bus. 
	
		* Một ECU chỉ cần một giao tiếp CAN duy nhất thay vì hàng chục đường tín hiệu analog/digital riêng lẻ.

<img width="712" height="232" alt="Image" src="https://github.com/user-attachments/assets/3c950042-4ea5-4ea7-b2e0-689343173531" />

* Chống nhiễu tốt:

	* Sử dụng differential signaling trên cặp dây xoắn (twisted pair): 
	
		* Dữ liệu được truyền dưới dạng chênh lệch điện áp giữa CAN-H và CAN-L.
	
	* Nhiễu điện từ (EMI) thường ảnh hưởng giống nhau lên cả hai dây → nhiễu bị triệt tiêu khi lấy hiệu
	
	* CAN vẫn hoạt động ổn định ngay cả khi một dây bị hỏng (có thể chuyển sang chế độ single-wire tạm thời).

* Độ tin cậy cao:

	* Cơ chế phát hiện và xử lý lỗi tốt:
	
		* Mỗi frame có CRC 15-bit, bit stuffing, acknowledgment từ receiver, và error confinement (các bộ đếm lỗi dẫn đến các trạng thái Error Active → Passive → Bus Off).
	
	* Không có máy chủ trung tâm → hệ thống vẫn hoạt động nếu một nút hỏng (fault-tolerant).
	
	* Ưu tiên thông điệp dựa trên ID: Thông điệp quan trọng (ví dụ: phanh, túi khí) luôn được truyền trước.
	
	* Tốc độ và độ trễ xác định → phù hợp cho hệ thống thời gian thực và an toàn chức năng 



### **II.  PHYSICAL LAYER**

#### **2.1. Topology-Bus và Termination Resistor**

<img width="919" height="436" alt="Image" src="https://github.com/user-attachments/assets/93e56512-a77e-45d9-a538-64295e172ca7" />

*  **Bus Topology:**

	*  CAN sử dụng kiến trúc Bus tuyến tính (Linear Bus Topology) hoặc còn gọi là multi-drop bus.
	
	*  Tất cả các nút (nodes/ECU) được kết nối song song vào cùng một đường truyền chung gồm 2 dây:
	
		* CAN_H (CAN High) và CAN_L (CAN Low)
		
		* Đây là cặp dây xoắn đôi (twisted pair) để giảm nhiễu.
		
	* Không sử dụng topology dạng sao (star) hoặc vòng (ring) vì dễ gây phản xạ sóng và làm giảm chất lượng tín hiệu.
	
	* Các nút có thể được nối vào bus qua các nhánh ngắn (stub), nhưng độ dài stub nên càng ngắn càng tốt (thường < 0.3m ở tốc độ cao) để tránh phản xạ.     

<img width="402" height="335" alt="Image" src="https://github.com/user-attachments/assets/1cf927ba-abf1-4704-a67a-5f9d2417196b" />

* **Termination Resistor:**

	* Để ngăn chặn hiện tượng phản xạ sóng (signal reflection) ở hai đầu bus, cần đặt hai điện trở 120Ω:
	
		* Một điện trở 120Ω ở đầu đầu tiên của bus.
		
		* Một điện trở 120Ω ở đầu cuối cùng của bus.  

#### **2.2. Các thành phần trên một node CAN**

<img width="696" height="339" alt="Image" src="https://github.com/user-attachments/assets/885d84d7-e368-45cf-8d0a-c41a54c964db" />


* Một Node CAN (thường gọi là ECU trong ô tô) bao gồm các thành phần phần cứng chính sau:


	*  **1. Sensor (Cảm biến) và Actuator (Bộ chấp hành)**:
	
		*  Cảm biến: Thu thập thông tin từ môi trường (nhiệt độ, áp suất, tốc độ, góc quay, gia tốc…).
		
		*  Actuator: Thực hiện lệnh (động cơ bước, van điện từ, relay, motor…).
		
		*  Chúng giao tiếp với MCU qua các giao tiếp analog, digital (ADC, PWM, GPIO…).  

	*  **2. MCU**:
	
		*  Bộ não của node, chạy phần mềm ứng dụng.
		
		*  Xử lý dữ liệu từ cảm biến, đưa ra quyết định điều khiển actuator.
		
		*  Giao tiếp với CAN Controller qua các chân TXD (Transmit Data) và RXD (Receive Data) ở mức logic (thường 3.3V hoặc 5V).
	
	*  **3. CAN Controller**:
	
		*  Thực hiện các chức năng của Data Link Layer (tầng liên kết dữ liệu).
		
		*  Đóng gói dữ liệu thành frame CAN (thêm SOF, Arbitration ID, Control field, Data, CRC, ACK…).
		
		*  Thực hiện cơ chế Arbitration (trọng tài bitwise).
		
		*   Phát hiện lỗi, quản lý Error Counters, gửi Error Frame nếu cần.
		
		*   Quản lý bộ đệm truyền/nhận (Transmit/Receive buffers).

	*  **4. CAN Transceiver (Bộ thu phát CAN)**:
	
		*  Là cầu nối giữa CAN Controller (mức logic) và bus vật lý (mức điện áp vi sai).
		
		*  Nhận tín hiệu TXD từ Controller → chuyển thành mức điện áp vi sai trên CAN_H / CAN_L.
		
		*  Nhận tín hiệu vi sai từ bus → chuyển về mức logic RXD gửi cho Controller.
		
		*  Các chip transceiver phổ biến: TJA1040, TJA1050, MCP2551, SN65HVD23x, TCAN1042… 


#### **2.3. Đặc điểm tín hiệu điện áp vi sai**

<img width="901" height="508" alt="Image" src="https://github.com/user-attachments/assets/b0d3eb8b-9434-4117-9b89-5cda8f360f15" />

* CAN sử dụng tín hiệu vi sai (Differential Signaling) giữa CAN_H và CAN_L.

* Logic được xác định bởi hiệu điện áp (`Vdiff = CAN_H − CAN_L`), không phải mức tuyệt đối.

	<img width="852" height="599" alt="Image" src="https://github.com/user-attachments/assets/a251ad42-e5bd-4840-9a7c-acec23e70b2d" />

	*  **Recessive State (Trạng thái Recessive - Logic 1)**:
	
		*  CAN_H ≈ 2.5V, CAN_L ≈ 2.5V.
		
		*  Hiệu điện áp Vdiff ≈ 0V (thực tế < 0.5V).
		
		*  Trạng thái này do hai điện trở  120Ω kéo về mức chung.   
		
		*  Tất cả các node đều ở chế độ high-impedance (không chủ động lái bus).  

	*  **Dominant State (Trạng thái Dominant - Logic 0)**:
	
		*  CAN_H ≈ 3.5V (thường từ 3.0V đến 4.0V).
		
		*  CAN_L ≈ 1.5V (thường từ 1.0V đến 2.0V).
		
		*  Hiệu điện áp Vdiff ≈ 2V (tối thiểu 0.9V để nhận diện là Dominant).
		
		*  Một hoặc nhiều node chủ động lái: CAN_H kéo lên cao hơn, CAN_L kéo xuống thấp hơn.
		
* **Lưu ý:**

	* Dominant luôn thắng Recessive (Wired-AND logic).
	
		*  Đây là nền tảng cho cơ chế arbitration bitwise — bit 0 (Dominant) có quyền ưu tiên cao hơn bit 1 (Recessive).
		
	* Receiver coi Vdiff < 0.5V là Recessive, Vdiff > 0.9V là Dominant.
	
	* Common-mode voltage (điện áp chung) thường nằm trong khoảng 1.5V – 3.5V.        


#### **2.4. Đặc tính giao tiếp**

* **Không có máy chủ trung tâm (Multi-Master / No Central Master):**

	* Mọi node đều có quyền ngang nhau để khởi tạo truyền dữ liệu bất kỳ lúc nào (multi-master).

	*  Không phụ thuộc vào một node chủ (master), tăng tính chịu lỗi (fault-tolerant). 
		
		* Nếu một node hỏng, các node khác vẫn hoạt động.
	
* **Truyền quảng bá (Broadcast Communication):**

	* Khi một node truyền frame, tất cả các node khác trên bus đều nhận được frame đó.

	*  Mỗi node tự quyết định có xử lý frame hay không dựa vào Identifier (ID) của frame (filtering bằng hardware hoặc software).
		
	* Không có địa chỉ đích cụ thể.

* **Bán song công (Half-Duplex):**

	* Bus chỉ cho phép truyền hoặc nhận tại một thời điểm trên cùng một cặp dây.

	*  Không thể truyền và nhận đồng thời trên cùng một bus (khác với Full-Duplex như một số giao thức khác).
		
	* Tuy nhiên, nhờ cơ chế arbitration, nhiều node có thể cùng cố gắng truyền nhưng chỉ một node thắng và tiếp tục truyền mà không bị phá hủy frame.


### **III.  KHUNG DỮ LIỆU CAN (CAN FRAMES)**

#### **3.1. Giới thiệu**

*  CAN định nghĩa 4 loại khung (frames) khác nhau để thực hiện truyền thông, báo lỗi và yêu cầu dữ liệu.

* Tất cả các frame đều được truyền theo kiểu NRZ (Non-Return to Zero) với cơ chế Bit Stuffing (chèn bit ngược sau 5 bit liên tiếp giống nhau) để đảm bảo đồng bộ clock. 

#### **3.2. CAN 2.0A (Standard 11-bit ID) và CAN 2.0B (Extended 29-bit ID)**

*  **CAN 2.0A (Standard Format):**

	* Độ dài Identifier (ID): 11 bit
	
	* Số lượng ID tối đa : 2^11 = 2048
	

*  **CAN 2.0B (Extended Format):**

	* Độ dài Identifier (ID): 29 bit
	
	* Số lượng ID tối đa : 2^29 

	* Hỗ trợ cả 11-bit và 29-bit
	
	* Identifier 29-bit gồm 11-bit Base ID + 18-bit Extended ID.
	
* **IDE (Identifier Extension) bit:**

	* IDE = 0 (Dominant) → Standard Frame (CAN 2.0A)
	
	* IDE = 1 (Recessive) → Extended Frame (CAN 2.0B)
		
	* Hai định dạng có thể tồn tại cùng trên một bus, nhưng cần thiết kế cẩn thận để tránh xung đột arbitration.

#### **3.3. Các loại khung dữ liệu**

* CAN có đúng 4 loại khung:

	*  **1. Data Frame (Khung dữ liệu)**:
	
		*  Loại khung phổ biến nhất.
		
		*  Dùng để truyền dữ liệu thực tế (0–8 byte ở Classical CAN).
		
		*  Mang thông tin từ nút gửi đến các nút khác.

	*  **2. Remote Frame (Khung yêu cầu từ xa)**:
	
		*  Dùng để yêu cầu một Data Frame có cùng Identifier.
		
		*  Không chứa trường Data.
		
		*  RTR bit = 1 (Recessive) để phân biệt với Data Frame (RTR = 0).
		
		*  DLC phải bằng với DLC của Data Frame được yêu cầu.  
		
		*  Mục đích: Một nút muốn lấy dữ liệu mới nhất từ nút khác mà không cần nút kia tự động gửi định kỳ. 

	*  **3. Error Frame (Khung báo lỗi)**:
	
		*  Được bất kỳ nút nào phát hiện lỗi đều có quyền truyền ngay lập tức.
		
		*  Cấu trúc:
		
			*  Error Flag: 6 bit đồng pha (tất cả Dominant hoặc tất cả Recessive).
			
			*  Error Delimiter: 8 bit Recessive.
			
		*   Vi phạm quy tắc Bit Stuffing nên tất cả các nút khác đều phát hiện và đồng bộ lỗi.
		
		*  Sau Error Frame thường có Intermission (3 bit Recessive).

 
	*  **4. Overload Frame (Khung quá tải)**:
	
		*  Dùng để yêu cầu trì hoãn thêm thời gian giữa các frame (khi nút chưa sẵn sàng xử lý frame tiếp theo).
		
		* Cấu trúc tương tự Error Frame: Overload Flag (6 bit Dominant) + Overload Delimiter (8 bit Recessive).
		
		* Không được dùng để báo lỗi.   

#### **3.4. Cấu trúc chi tiết của Data Frame**

<img width="1570" height="605" alt="Image" src="https://github.com/user-attachments/assets/dc6b0f72-bc8a-4174-ad8b-dfc384f5edec" />


* **1. SOF – Start of Frame (1 bit)**

	*  Luôn là Dominant (0).
	
	*  Đánh dấu bắt đầu của một frame mới, dùng để đồng bộ tất cả các nút trên bus.

* **2. Arbitration Field (Trường trọng tài) – Quyết định quyền ưu tiên truyền**

	*  Standard Frame (11-bit ID):
	
		*  11-bit Identifier
		
		*  RTR (Remote Transmission Request) – 1 bit 

	*  Extended Frame (29-bit ID):
	
		*  11-bit Base Identifier
		
		*  SRR (Substitute Remote Request) – 1 bit (luôn Recessive)
		
		*  IDE (Identifier Extension) – 1 bit (= 1)
		
		* 18-bit Extended Identifier
		
		*  RTR – 1 bit
		
	* **Lưu ý:** Identifier càng nhỏ (càng nhiều bit 0) → ưu tiên càng cao vì Dominant (0) thắng Recessive (1).     

* **3. Control Field (Trường điều khiển) – 6 bit**

	*  **Standard Frame**
	
		*  IDE = 0 (1 bit)
		
		*  r0 (Reserved bit) = 0 (1 bit)
		
		*  DLC (Data Length Code) - 4 bit
		
	* **Extended Frame:**  

		*  IDE = 1
		
		*  r1 (Reserved bit) = 0 (1 bit)
		
		*  r0 = 0
		
		*  DLC - 4 bit
		
	* DLC (Data Length Code): Chỉ độ dài trường Data (0 đến 8 byte). Giá trị DLC từ 0000 (0 byte) đến 1000 (8 byte).  

* **4. Data Field (Trường dữ liệu)**

	*  Độ dài: 0 đến 8 byte (64 bit).
	
	*  Nội dung dữ liệu thực tế của ứng dụng (ví dụ: tốc độ xe, nhiệt độ động cơ…).
	
	*  Truyền từ byte cao nhất (MSB) trước.  

* **5. CRC Field (Cyclic Redundancy Check)**

	*  15 bit CRC + 1 bit CRC Delimiter (luôn Recessive).
	
	*  Dùng để kiểm tra lỗi truyền. CRC được tính trên toàn bộ frame từ SOF đến cuối Data Field.



* **6. ACK Field (Acknowledgement) – 2 bit**

	*  **ACK Slot (1 bit):**
	
		*  Receiver gửi Dominant (0) nếu nhận frame đúng (CRC đúng, định dạng đúng).
		
	*  **ACK Delimiter (1 bit):**
	
		*  Luôn Recessive.
		
	* Transmitter kiểm tra ACK Slot. Nếu vẫn là Recessive → ACK Error.

* **7. EOF – End of Frame (7 bit)**

	*  7 bit Recessive.
	
	*  Đánh dấu kết thúc frame.

* **8. Intermission (3 bit Recessive)**

	*  Không thuộc frame, dùng để phân cách giữa các frame.
	
* Tổng số bit tối thiểu của một Data Frame (không có Data):

	* Standard: 44 bit + 8×DLC
	
	* Extended: 64 bit + 8×DLC
	
* **Bit Stuffing**: Áp dụng từ SOF đến cuối CRC (không áp dụng cho CRC Delimiter, ACK và EOF). 

	* Sau 5 bit giống nhau phải chèn 1 bit ngược lại.      


### **IV.  CƠ CHẾ PHÂN XỬ QUYỀN TRUY CẬP BUS (BUS ARBITRATION)**

#### **4.1. Giới thiệu**

*  Trong mạng CAN, nhiều nút (node) có thể cố gắng truyền dữ liệu đồng thời vì đây là kiến trúc multi-master

* Để giải quyết xung đột mà không gây mất dữ liệu hoặc làm hỏng bus, CAN sử dụng cơ chế Arbitration , được thực hiện từng bit một trong trường Arbitration Field của Frame 

#### **4.2. Cơ chế ưu tiên dựa trên ID**

<img width="708" height="398" alt="Image" src="https://github.com/user-attachments/assets/e4d6b91e-dbee-4cbf-8de3-68928ee5e9e8" />

<img width="748" height="326" alt="Image" src="https://github.com/user-attachments/assets/f8d4f86a-d0c5-4d94-ac4f-2836a1e7139f" />


*  **ID càng nhỏ (càng nhiều bit 0) thì ưu tiên càng cao**

* Trong trường Arbitration Field, bit MSB được truyền trước 

* Identifier được thiết kế sao cho bit 0 (Dominant) có quyền ưu tiên cao hơn bit 1 (Recessive)

* **VD:**  
	
	* Giả sử có 3 nút muốn truyền cùng lúc:
	
		* Node A: ID = 0x100 (binary: 001 0000 0000)
		
		* Node B: ID = 0x080 (binary: 000 1000 0000)
		
		* Node C: ID = 0x200 (binary: 010 0000 0000)
		
	* Khi truyền từ MSB:
	
		* Bit đầu tiên (bit 10): Cả 3 đều truyền 0 -> bus = Dominant (0)
		
		* Tiếp tục đến bit vị trí khác biệt đầu tiên:
		
			* Node B có nhiều bit 0 hơn ở vị trí quan trọng -> Node B thắng arbitration
			
			* Node A và Node C phát hiện bus khác với bit chúng truyền -> Chúng ngừng truyền ngay lập tức và chuyển sang chế độ nhận
			
	* Thông điệp có ID thấp nhất (ưu tiên cao nhất) sẽ chiếm bus và truyền tiếp mà không bị gián đoạn.          
	
#### **4.3. Nguyên lý Wire-And Logic**

<img width="653" height="422" alt="Image" src="https://github.com/user-attachments/assets/65f24ca3-24df-4ee6-acfd-1f987fb421bc" />


*  Cơ chế arbitration của CAN dựa trên Wired-AND Logic nhờ đặc tính điện của bus:

	*  **Dominant (0)** = mức điện áp vi sai mạnh (CAN_H cao, CAN_L thấp) → một node chủ động “kéo” bus về mức Dominant.
	
	* **Recessive (1)** = mức điện áp gần như không chủ động (khoảng 2.5V trên cả hai dây). 

* **Đặc tính:**

	* Nếu bất kỳ node nào truyền bit Dominant (0), toàn bộ bus sẽ ở mức Dominant
	
	* Chỉ khi tất cả các node đều truyền bit Recessive (1) thì bus mới ở mức Recessive
	
	* Giống với phép AND logic: **0 AND 1 = 0 (Dominant luôn thắng)**    

* Mỗi node vừa truyền bit của mình, vừa giám sát (monitor) liên tục mức điện áp thực tế trên bus (qua transceiver).

	* Nếu node truyền Recessive (1) nhưng đọc lại bus là Dominant (0) → node biết có node khác có ưu tiên cao hơn → node thua arbitration và ngừng truyền ngay lập tức.
	
	* Node thắng arbitration (ID thấp nhất) sẽ tiếp tục truyền toàn bộ frame mà không bị gián đoạn.


#### **4.4. Cơ chế Non-destructive Arbitration**

<img width="711" height="398" alt="Image" src="https://github.com/user-attachments/assets/69856be9-0581-4549-ba6a-84aec4a5bbc6" />

<img width="1032" height="234" alt="Image" src="https://github.com/user-attachments/assets/599d5276-6854-466a-813c-8418a991341b" />

*  **Non-destructive** nghĩa là khi xảy ra xung đột, frame của nút thắng arbitration không bị hỏng hoặc mất dữ liệu

* Nút thua chỉ đơn giản ngừng truyền và chờ bus rảnh để thử lại sau

* **Quá trình:**

	* 1. Nhiều node bắt đầu truyền cùng lúc (sau khi bus Idle >= 3 bit Recessive)
	
	* 2. Chúng truyền Arbitration Field bit by bit
	
	* 3. Nút có ID thấp nhất thắng -> tiếp tục truyền Control Field , Data, CRC
	
	* 4. Các nút thua arbitration tự động chuyển sang chế độ nghe (receive) và sẽ thử truyền lại khi bus trả về trạng thái Idle
	
* **Lưu ý:**

	* Không được phép có hai node cùng truyền một ID (trừ trường hợp Remote Frame)
	
	* Arbitration chỉ diễn ra trong Arbitration Field (bao gồm ID + RTR/SRR/IDE)        

### **V.  CƠ CHẾ XỬ LÝ LỖI TRONG GIAO THỨC CAN**

#### **5.1. Các loại lỗi**

<img width="436" height="198" alt="Image" src="https://github.com/user-attachments/assets/cf79838b-dabd-4e4f-a3a6-c96dfe29ca2b" />

*  **Bit Error:**

	* Node truyền bit Dominant nhưng đọc lại trên bus là Recessive hoặc ngược lại (trừ trường hợp Arbitration Field và ACK Slot) 

	* Điều kiện phát hiện:
	
		* Node phát hiện sự không khớp giữa bit truyền và bit đọc về

*  **Stuff Error:**

	* Vi phạm quy tắc Bit Stuffing (xuất hiện 6 bit liên tiếp giống nhau từ SOF đến CRC)

	* Điều kiện phát hiện:
	
		* Phát hiện 6 bit Dominant hoặc 6 bit Recessive liên tiếp 

*  **CRC Error:**

	* Giá trị CRC nhận được không khớp với CRC mà node tính toán
	
	* Điều kiện phát hiện: 

		* Receiver tính CRC và so sánh với CRC trong Frame

*  **Form Error:**

	* Cấu trúc frame không đúng quy định

	* Điều kiện phát hiện:
	
		* Phát hiện 6 bit Dominant hoặc 6 bit Recessive liên tiếp 

*  **ACK Error:**

	* Transmitter không nhận được ACK (ACK Slot vẫn ở mức Recessive)

	* Điều kiện phát hiện:
	
		* Không có node nào xác nhận đã nhận đúng frame
		
* **Lưu ý:**

	* Bit Error và Stuff Error chỉ được kiểm tra bởi transmitter (node đang truyền).
	
	* CRC Error, Form Error và ACK Error chủ yếu được phát hiện bởi receiver.
	
	* Khi bất kỳ lỗi nào được phát hiện, node sẽ ngay lập tức ngừng hoạt động hiện tại và truyền Error Frame.    


#### **5.2. Cơ chế phát hiện và sửa lỗi tự động (Truyền lại thông điệp)**

*  **Quy trình xử lý lỗi:**

	* **1. Phát hiện lỗi:**

		* Bất kỳ node nào trên bus phát hiện lỗi đều có quyền truyền Error Flag ngay lập tức (ngay cả trong lúc đang nhận frame).

	* **2. Truyền Error Frame:**

		* Gồm Error Flag (6 bit Dominant nếu Error Active) + Error Delimiter (8 bit Recessive).
		
		* Error Flag vi phạm quy tắc Bit Stuffing → tất cả các node khác trên bus đều nhanh chóng phát hiện lỗi và đồng bộ.
	
	* **3. Hủy frame hiện tại:**

		* Toàn bộ frame đang truyền bị coi là không hợp lệ và bị hủy bỏ.
		
	* **4. Tự động truyền lại:**

		* Node truyền (transmitter) sẽ tự động xếp lại thông điệp vào hàng đợi truyền và thử gửi lại khi bus trở về trạng thái Idle (sau 3 bit Recessive của Intermission).
		
		* Quá trình này diễn ra hoàn toàn bởi phần cứng, rất nhanh và trong suốt với lớp ứng dụng.  
	
#### **5.3. Bộ đếm lỗi (Transmit Error Counter - TEC & Receive Error Counter - REC)**

* Mỗi node CAN duy trì hai bộ đếm lỗi riêng biệt:

	<img width="743" height="472" alt="Image" src="https://github.com/user-attachments/assets/197a44d1-c321-4732-a1bb-af00d328e13d" />
	
	*  **TEC (Transmit Error Counter):** Đếm lỗi liên quan đến việc truyền.
	
	* **REC (Receive Error Counter):** Đếm lỗi liên quan đến việc nhận.

#### **5.4. Các trạng thái hoạt động của Node (Error Active, Error Passive, Bus Off)**

<img width="674" height="519" alt="Image" src="https://github.com/user-attachments/assets/a408e11b-2f5a-4994-8047-1015129c884f" />

* Dựa trên giá trị của TEC và REC, mỗi node CAN có 3 trạng thái hoạt động:

	*  **Error Active:** 
	
		* **Điều kiện:**
  
  				`TEC <= 127` và `REC <= 127`
		
			* Thực tế kỹ thuật: Bộ đếm thường ở mức 0, hoặc dao động nhẹ quanh 0 nếu thỉnh thoảng có lỗi CRC do nhiễu. 

		* **Hành vi khi phát hiện lỗi:**

  			* Node sẽ gửi Active Error Flag, bao gồm 6 bit Dominant liên tiếp. 
  
		* **Tác động lên bus:**

  			* Tín hiệu Dominant sẽ ghi đè lên tín hiệu Recessive trên bus
     
	 		* Việc xuất hiện 6 bit Dominant liên tiếp vi phạm quy tắc nhồi bit (Bit Stuffing), do đó toàn bộ các node khác trên mạng đều phát hiện được lỗi và đồng loạt gửi Active Error Flag
    
			* Hệ quả là frame hiện tại bị hủy bỏ hoàn toàn và node gửi phải thực hiện truyền lại.    
 
  
	*  **Error Passive:** 

		* **Điều kiện:**
  
  				`TEC ≥ 128 hoặc REC ≥ 128.`

		* **Hành vi khi phát hiện lỗi:**

  			* Node gửi Passive Error Flag, bao gồm 6 bit Recessive liên tiếp.
  
		* **Đặc điểm khác biệt:**

  			* Do các bit Recessive không có khả năng ghi đè bit Dominant, node trong trạng thái Error Passive không thể tự mình hủy bỏ frame của node khác.
     
	 		* Chỉ khi các node Error Active khác cùng phát hiện lỗi và gửi Active Error Flag, frame mới bị hủy.
    
			* Sau khi kết thúc quá trình truyền một frame, node Error Passive phải tuân thủ thời gian Suspend Transmission (tạm dừng truyền) kéo dài 8 bit Recessive trước khi được phép khởi tạo frame tiếp theo.
   
   			* Quy định này nhằm ưu tiên quyền truy nhập bus cho các node đang ở trạng thái Error Active. 
 
   
		* **Khả năng phục hồi:**

			* Nếu nguyên nhân gây lỗi chỉ là nhiễu tạm thời, sau một số lần truyền/nhận thành công, giá trị TEC và REC sẽ giảm dần và node sẽ tự động quay trở lại trạng thái Error Active.

	*  **Bus Off:** 

		* **Điều kiện:**
  
  				`TEC ≥ 256`. (Lưu ý: chỉ có TEC gây ra trạng thái Bus Off, REC không có khả năng này).
 
    		* Khi một node liên tục gửi lỗi, nó phá hủy frame của các node khác, gây ảnh hưởng nghiêm trọng đến toàn bộ mạng.
      
	  		* Vì vậy, mức phạt cho lỗi truyền (TEC tăng 8 đơn vị) cao hơn lỗi nhận (REC tăng 1 đơn vị) và TEC là chỉ số để quyết định trục xuất node.  

		* **Hành vi khi phát hiện lỗi:**

  			* Node bị ngắt kết nối logic hoàn toàn khỏi bus, không được phép gửi bất kỳ bit nào (kể cả Error Flag hay Acknowledge) và thường chuyển sang chế độ chỉ giám sát bus.

    
		* **Quy trình phục hồi:**

			* **1.** Phần mềm ứng dụng ghi nhận sự kiện Bus Off (thường thông qua ngắt) và lưu mã lỗi chẩn đoán (DTC).
 
			* **2.** Chờ một khoảng thời gian trễ do người lập trình quy định (ví dụ 100ms đến 200ms).
     
			* **3.** Xóa cờ Bus Off trong thanh ghi điều khiển.
 
			* **4.** Giám sát bus cho đến khi phát hiện 128 lần liên tiếp chuỗi 11 bit Recessive.
		
			* **5.** Sau khi điều kiện trên thỏa mãn, phần cứng tự động đặt lại TEC và REC về 0 và node chuyển sang trạng thái Error Active.
 
			* **6.** Nếu node liên tục rơi vào Bus Off sau các lần phục hồi, phần mềm có thể quyết định dừng nỗ lực phục hồi và xác nhận đây là lỗi vĩnh viễn.
		
* **Quá trình chuyển trạng thái:**

	* Khi TEC hoặc REC vượt quá 127 → node lập tức chuyển từ trạng thái Error Active sang Error Passive
	
	* Khi TEC đạt 256 → node vào Bus Off.
	
	* Để thoát khỏi Bus Off:
	
		* Node phải theo dõi bus Idle (11 bit Recessive liên tiếp) trong khoảng thời gian quy định (thường 128 × 11 bit)
		
		* Sau đó reset TEC và REC về 0 và quay lại trạng thái Error Active.     

* **Mục đích:**

	* Phát hiện node bị hỏng và cách ly nó ra khỏi bus (Bus Off) để tránh làm ảnh hưởng đến toàn bộ mạng.
	
	* Cho phép node bị lỗi tạm thời (do nhiễu) có cơ hội tự phục hồi.
	
	* Đảm bảo tính fault-tolerant (chịu lỗi) của hệ thống. 

### **VI.  NGUYÊN LÝ VỀ BAUDRATE VÀ BỘ LỌC CAN**

#### **6.1. Giới thiệu**

*  Baudrate (tốc độ truyền) trong CAN là số lượng bit được truyền mỗi giây (thường tính bằng kbps hoặc Mbps)

* Baudrate quyết định thời gian của một bit (Nominal Bit Time) và ảnh hưởng trực tiếp đến tốc độ giao tiếp cũng như khoảng cách truyền tối đa.

* VD:
	
	* High-Speed CAN: 125 kbps, 250 kbps, 500 kbps, 1 Mbps.
	
	* CAN FD: Có thể lên đến 8 Mbps ở phần Data.
	
* Để đạt được baudrate chính xác và đồng bộ giữa các node, CAN chia mỗi bit thành nhiều đơn vị thời gian nhỏ gọi là Time Quantum (TQ).   




#### **6.2. Nguyên lý phân chia Bit Time và Điểm lấy mẫu**

<img width="1044" height="271" alt="Image" src="https://github.com/user-attachments/assets/75d869f7-4071-44d1-9150-a29b29424e25" />

*  Mỗi Nominal Bit Time (thời gian của 1 bit) được chia thành 4 phân đoạn (segments) không chồng lấn, mỗi phân đoạn gồm nhiều Time Quantum (TQ)

* Time Quantum là đơn vị thời gian nhỏ nhất, được tạo ra bằng cách chia tần số clock của CAN Controller.

* Cấu trúc phân chia Bit Time theo tiêu chuẩn:

	* **1. Sync_Seg (Synchronization Segment):**

		* Độ dài cố định: 1 TQ.
		
		* Nhiệm vụ: Đồng bộ cạnh chuyển tiếp (edge) của tín hiệu trên bus với clock nội bộ của các node.
		
		* Cạnh chuyển tiếp (từ Recessive sang Dominant hoặc ngược lại) được mong đợi xảy ra trong khoảng này.  

	* **2. Prop_Seg (Propagation Segment):**

		* Độ dài: Có thể lập trình từ 1 đến 8 TQ.
		
		* Nhiệm vụ: Bù đắp cho độ trễ truyền lan (propagation delay) trên đường bus và transceiver (bao gồm thời gian lan truyền hai chiều).
		
		* Độ dài Prop_Seg càng lớn thì khoảng cách bus có thể càng dài.  

	* **3. Phase_Seg1 (Phase Segment 1):**

		* Độ dài: Có thể lập trình từ 1 đến 8 TQ (một số controller lên đến 16 TQ khi kết hợp với Prop_Seg).
		
		* Nhiệm vụ: Bù đắp cho sai số pha (phase error) dương. Có thể được kéo dài trong quá trình resynchronization.

	* **4. Phase_Seg2 (Phase Segment 2):**

		* Độ dài: Có thể lập trình từ 1 đến 8 TQ.
		
		* Nhiệm vụ: Bù đắp cho sai số pha âm. Có thể được rút ngắn trong quá trình resynchronization.

* **Điểm lấy mẫu (Sample Point):**

	* Là vị trí mà node đọc và lấy giá trị logic của bit trên bus.
	
	* Vị trí: Cuối cùng của Phase_Seg1 (tức là sau Sync_Seg + Prop_Seg + Phase_Seg1).
	
	* Giá trị mẫu điển hình: 75% ~ 87.5% của Bit Time (thường khuyến nghị khoảng 80%). 
	
		* Đặt Sample Point ở phía sau giúp có đủ thời gian để tín hiệu ổn định sau các nhiễu và độ trễ, nhưng vẫn để lại đủ Phase_Seg2 để resynchronize. 

* **Công thức tính Sample Point:**

	<img width="748" height="105" alt="Image" src="https://github.com/user-attachments/assets/1764b479-2d00-4ad5-b46d-b0a25e0c2517" />

* **Synchronization Jump Width (SJW):**

	* Giá trị từ 1 đến 4 TQ (không được lớn hơn Phase_Seg1 hoặc Phase_Seg2).
	
	* Quy định mức độ điều chỉnh tối đa cho Phase_Seg1 và Phase_Seg2 trong quá trình đồng bộ lại (resynchronization) để bù đắp sai lệch clock giữa các node.


#### **6.3. Nguyên lý hoạt động của Mask (Mặt nạ) và Filter (Bộ lọc)**

*  Vì CAN là giao thức broadcast, mọi node trên bus đều nhận được tất cả các frame.

* Để giảm tải cho MCU (không phải xử lý tất cả thông điệp), CAN Controller tích hợp Acceptance Filter (bộ lọc chấp nhận) hoạt động hoàn toàn bằng phần cứng.

* **Nguyên lý:**

	* Mỗi node chỉ quan tâm đến một số Identifier (ID) nhất định.
	
	* Bộ lọc sẽ so sánh ID của frame nhận được với các giá trị đã cấu hình
	
	* Chỉ frame đúng mới được đưa vào FIFO/Buffer để MCU xử lý; các frame khác bị loại bỏ ngay tại phần cứng.  

* **Hai thành phần chính:**

	<img width="1356" height="617" alt="Image" src="https://github.com/user-attachments/assets/eb02d267-7bd9-42c8-9b09-8834db6c80ee" />

	* **Filter (Bộ lọc / Acceptance Code):**
	
		* Chứa giá trị ID mong muốn (hoặc một phần của ID).
		
		* Xác định các bit phải khớp chính xác. 

	* **Mask (Mặt nạ / Acceptance Mask):**

		* Là “mặt nạ bit” quyết định bit nào cần so sánh và bit nào được bỏ qua
		
		* Bit 1 trong Mask: Bit tương ứng trong ID phải khớp chính xác với bit trong Filter.
		
		* Bit 0 trong Mask: Bit tương ứng được bỏ qua (có thể là 0 hoặc 1 đều chấp nhận). 

* **Cách hoạt động (Mask Mode):**

	* **Giả sử dùng Standard 11-bit ID:**
	
		* Filter = `0000 0100 0000` (ID mong muốn = 0x040)
		
		* Mask = `1111 1111 0000`
		
	* Chỉ các ID có 8 bit cao nhất giống hệt Filter mới được chấp nhận (4 bit thấp không quan tâm).
	
	* Chấp nhận tất cả ID từ `0x040` đến `0x04F`.   

* **Các chế độ lọc phổ biến:**

	* **Mask Filter Mode: Lọc theo vùng ID (range)**
	
		* Một bộ lọc gồm:

  			* **Filter ID (giá trị mẫu)**
     
	 		* **Mask (bit nào cần so sánh)**
    
		* Mask bit = 1 -> bắt buộc phải khớp Filter ID tại bit đó

  		* Mask bit = 0 -> bỏ qua (không quan tâm bit đó)
    
		* VD:

  			* **VD1: Nhận tất cả ID từ 0x100 đến 0x1FF**
 
     			* Filter ID: 0x100 = 001 0000 0000
        
				* Mask: 0x700 = 111 0000 0000
     
	 			* Cách tính vùng ID:   	
 
							Filter ID: 001 0000 0000
							Mask:      111 0000 0000
							          ↓↓↓ ↓↓↓↓ ↓↓↓↓
							Bắt buộc: 001 xxxx xxxx

       			* Các ID từ `001 0000 0000` (0x100) đến `001 1111 1111` (0x1FF) đều khớp

    			* Một ECU nhận tất cả thông báo chẩn đoán từ gateway, gateway dùng ID vùng 0x100-0x1FF cho dịch vụ chẩn đoán. 
           
  			* **VD2: Chỉ nhận đúng một ID duy nhất (0x123)**
 
     			* Filter ID: 0x123 = 001 0010 0011
        
				* Mask: 0x7FF = 111 1111 1111
     
	 			* Cách tính vùng ID:   	
 
							Filter ID: 001 0010 0011
							Mask:      111 1111 1111
							          ↓↓↓ ↓↓↓↓ ↓↓↓↓
							Bắt buộc: 001 0010 0011  → Chỉ đúng 0x123

       			* Mask toàn bit 1 nghĩa là không bỏ qua bit nào, nên chỉ khớp chính xác 0x123.

  			* **VD3: Nhận tất cả ID có bit 7:6 = 01**
 
     			* Filter ID: 0x040 = 000 0100 0000
        
				* Mask: 0x060 = 000 0110 0000
     
	 			* Cách tính vùng ID:   	
 
							Filter ID: 000 01 00 0000
							Mask:      000 11 00 0000
							            ↑   ↑
							         bit [7:6]

       			* Tất cả ID có dạng `xxx 01xx xxxx` đều khớp.

    			* Nhận tất cả frame từ 2 module có cùng chức năng (mỗi module dùng 1 bit còn lại để phân biệt). 
   
  			* **VD4: Extended ID 29-bit – Nhận vùng ECU thân xe (0x18DA00F0 - 0x18DA00FF)**
 
     			* Filter ID (29-bit): 0x18DA00F0
        
				* Mask (29-bit): 0x1FFFFF00
     
	 			* Cách tính vùng ID:   	
 
							Filter ID:   1 1000 1101 1010 0000 0000 1111 0000
							Mask:        1 1111 1111 1111 1111 1111 0000 0000
							                                          ↓↓↓↓ ↓↓↓↓
							                                      bỏ qua byte cuối
  
     
     
	* **List Filter Mode (Identifier Mode):**
	
		* So sánh trực tiếp với nhiều ID cụ thể (không dùng mask).
		
		* Hỗ trợ cả Standard (11-bit) và Extended (29-bit) ID.
		
		* Nhiều bộ lọc (Filter Banks): Các controller hiện đại có từ 4 đến 32 bộ lọc (ví dụ: STM32 có 28 filter banks).

  		* VD: 


  			* **VD1: STM32 chế độ 32-bit Standard ID (2 ID trong 1 bank)**
 
     			* Một filter bank 32-bit chứa được 2 ID chuẩn 11-bit.
        
					* ID 1: 0x123 → gán vào STDID[10:0] của thanh ghi filter thứ nhất
	     
		 			* ID 2: 0x456 → gán vào STDID[10:0] của thanh ghi filter thứ hai
 
							[31:16] STDID2[10:0] + RTR + IDE + EXID[17:15]
							[15:0]  STDID1[10:0] + RTR + IDE + EXID[17:15]

       			* Ứng dụng: ECU chỉ nhận đúng 2 ID điều khiển từ BCM: 0x123 (lệnh bật đèn) và 0x456 (lệnh tắt đèn).

  			* **VD2: STM32 chế độ 32-bit Extended ID (1 ID extended trong 1 bank)**
 
     			* Cần nhận chính xác ID extended 0x18FFA123.
        
				* Chỉ một ID được lưu, toàn bộ 29-bit được so sánh.
     
	 			* Filter bank chỉ khớp đúng 0x18FFA123.
     
	 			* Ứng dụng: Nhận frame cấu hình riêng từ nhà sản xuất, không muốn lẫn với bất kỳ node nào khác.    	
 
  			* **VD3: STM32 chế độ 16-bit (4 ID Standard trong 1 bank)**
 
     			* Mỗi filter bank chia thành 4 phần 16-bit, mỗi phần chứa 1 ID 11-bit + bit điều khiển.
        
					* ID 1: 0x300
	     
		 			* ID 2: 0x301
	     
		 			* ID 3: 0x302
        
					* ID 4: 0x303
        
				* Ứng dụng: Nhận 4 ID phản hồi từ 4 cảm biến nhiệt độ giống hệt nhau, đánh địa chỉ tuần tự từ 0x300 - 0x303. 
  

* **Lưu ý:**

	* Phải cấu hình Filter/Mask đúng trước khi vào chế độ Normal.
	
	* Có thể kết hợp nhiều filter để tạo bộ lọc phức tạp (acceptance + rejection).
	
	* Sai cấu hình filter là nguyên nhân phổ biến gây ra tình trạng node “không nhận được dữ liệu”.      

### **PHẦN 2: TRIỂN KHAI CAN VỚI STM32F103C8**

### **VII.  TỔNG QUAN VỀ BXCAN**

#### **7.1. Khái niệm**

* STM32F103 sử dụng bộ điều khiển CAN được gọi là bxCAN (basic extended CAN).

* Hỗ trợ đầy đủ CAN 2.0A (Standard 11-bit) và CAN 2.0B (Extended 29-bit), tốc độ lên đến 1 Mbps.

* bxCAN được thiết kế với kiến trúc Mailbox + FIFO giúp giảm tải cho CPU, hỗ trợ thời gian thực tốt và linh hoạt trong việc quản lý truyền/nhận thông điệp.

* Các tính năng chính:

	* 3 Transmit Mailboxes (hộp thư truyền).
	
	* 2 Receive FIFOs (hộp thư nhận kiểu FIFO).   
	
	* Bộ lọc chấp nhận (Acceptance Filter) với tối đa 28 Filter Banks
	
	* Các chế độ hoạt động: Initialization, Normal, Sleep, Loopback, Silent, Loopback + Silent.
	
	* Hỗ trợ ngắt (Interrupt) cho truyền, nhận, lỗi.   

#### **7.2. Kiến trúc bxCAN (Hộp thư truyền - Tx Mailboxes, Hộp thư nhận - Rx FIFOs)**

##### **7.2.1. Transmit Mailboxes (Hộp thư truyền)**

* **bxCAN có 3 Transmit Mailboxes độc lập:**

	* Mỗi mailbox có thể chứa 1 frame CAN hoàn chỉnh (Identifier, DLC, Data, RTR…).
	
	* CPU chỉ cần nạp dữ liệu vào mailbox → bxCAN tự động xử lý việc truyền.
	
	* Hỗ trợ priority-driven transmission: Mailbox có thể được cấu hình mức ưu tiên
	
	* Nếu nhiều mailbox đều có dữ liệu sẵn sàng, bxCAN sẽ tự động chọn mailbox có ID ưu tiên cao nhất để truyền trước (theo cơ chế arbitration của CAN).

	* Khi một mailbox truyền xong (nhận được ACK), nó sẽ tự động báo ngắt Transmit Interrupt (nếu được bật) và chuyển sang trạng thái Empty.

* **Cấu trúc một Transmit Mailbox bao gồm các thanh ghi chính:**
	
	* TIxR (Identifier Register)
	
	* TDTxR (Data Length and Time Stamp)
	
	* TDLxR / TDHxR (Data Low/High)
	
##### **7.2.2. Receive FIFOs (Hộp thư nhận)**

* **bxCAN có 2 Receive FIFOs (FIFO0 và FIFO1):**

	* Mỗi FIFO là một hàng đợi kiểu FIFO (First In First Out) với độ sâu 3 messages.
	
	* Toàn bộ thông điệp nhận được từ bus sẽ được đưa vào một trong hai FIFO tùy thuộc vào Acceptance Filter (bộ lọc).
	
	* Giảm nguy cơ overrun (mất dữ liệu) khi có nhiều thông điệp đến nhanh.
	
	* Mỗi FIFO có ngắt riêng: FMPx (FIFO Message Pending) – báo khi có ít nhất 1 message trong FIFO. 

* **Quy trình nhận:**

	* 1. Frame đến bus → Acceptance Filter kiểm tra ID.
	
	* 2. Nếu khớp → frame được lưu vào FIFO0 hoặc FIFO1.
	
	* 3. Khi FIFO có dữ liệu → sinh ngắt (nếu bật).
	
	* 4. CPU đọc message từ FIFO → bxCAN tự động giải phóng slot.  


#### **7.3. Các chân GPIO hỗ trợ CAN**

* Trên STM32F103, khối CAN1 (bxCAN) có hai bộ chân GPIO có thể sử dụng:

	* Cấu hình mặc định (No Remap):
	
		* CAN_RX: PA11
		
		* CAN_TX: PA12
		
	* Cấu hình Remap (Remapped):
		
		* CAN_RX: PB8
		
		* CAN_TX: PB9
		
	* Để chuyển sang bộ chân PB8/PB9, cần bật remapping thông qua thanh ghi AFIO_MAPR.

* Lưu ý:
	
	* Cả hai chân đều phải được cấu hình ở chế độ Alternate Function.
	
	* CAN_TX (PA12 hoặc PB9): Alternate function Push-Pull, tốc độ cao (High Speed - 50MHz khuyến nghị).
	
	* CAN_RX (PA11 hoặc PB8): Floating Input hoặc Input Pull-Up (thường để Floating Input).

	* Khi sử dụng PA11/PA12, cần chú ý xung đột với USB (vì USB_DM = PA11, USB_DP = PA12)
	
### **VIII.  KHỞI TẠO VÀ CẤU HÌNH CAN**

#### **8.1. Cấu hình Clock cho CAN và GPIO**

		// Bật Clock cho CAN1 và AFIO 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

#### **8.2. Cấu hình GPIO cho CAN**


			GPIO_InitTypeDef GPIO_InitStructure;

			// ----------CAN_TX----------
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz'
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOA, &GPPIO_InitStructure);

			// ----------CAN_RX---------- 
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
			GPIO_Init(GPIOA, &GPIO_InitStructure);


* Nếu bật remap CAN sang PB8/PB9

		//Bật Remap
		GPIO_PinRemapConfig(GPIO_Remap_CAN1, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);



#### **8.3. Khởi tạo các tham số CAN (CAN_InitTypeDef)**

		CAN_InitTypeDef CAN_InitStructure;
		CAN_DeInit(CAN1);			//Reset bxCAN trước khi khởi tạo

		CAN_InitStructure.CAN_TTCM = DISABLE;
		CAN_InitStructure.CAN_ABOM = ENABLE;			// Auto Bus-Off Management
		CAN_InitStructure.CAN_AWUM = DISABLE;
		CAN_InitStructure.CAN_NART = DISABLE;			// Tự động retransmission
		CAN_InitStructure.CAN_RFLM = DISABLE;
		CAN_InitStructure.CAN_TXFP = DISABLE; 			// Ưu tiên theo ID 
			
		// Cấu hình Baudrate 
		CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
		CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
		CAN_InitStructure.CAN_BS1 = CAN_BS1_8tq;			// TimeSeg1
		CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;			// TimeSeg2
		CAN_InitStructure.CAN_Prescaler = 6; 				// Prescaler = 6


		// Tính baudrate:
		// TQ = PCLK1 / PRESCALER = 36MHz / 6 = 6MHz
		// Tổng TQ = 1 + 8 + 3 = 12 TQ
		// Baudrate = 6MHz / 12 = 500 kbps
		
		if (CAN_Init(CAN1, &CAN_InitStructure) != CAN_InitStatus_Success) { 
		// Xử lý lỗi 
		}


#### **8.4. Các chế độ hoạt động (CAN_Mode)**

* SPL hỗ trợ 4 chế độ chính:

		CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;           // Chế độ thực tế
		// CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;      // Test nội bộ
		// CAN_InitStructure.CAN_Mode = CAN_Mode_Silent;        // Chỉ nghe
		// CAN_InitStructure.CAN_Mode = CAN_Mode_Silent_LoopBack;

#### **8.5. Các thông số quản lý nâng cao**

* **CAN_ABOM (Automatic Bus-Off Management):**

	*  Bit này điều khiển cơ chế tự động phục hồi khi node rơi vào trạng thái Bus-Off.
	
	*  Khi được ENABLE:
 
 		* Phần cứng sẽ tự động giám sát bus, phát hiện điều kiện 128 lần liên tiếp chuỗi 11 bit Recessive và tự động đặt lại TEC, REC về 0 để đưa node quay lại trạng thái Error Active mà không cần sự can thiệp của phần mềm  

	*  Khi được DISABLE:
 
 		* Việc phục hồi từ Bus-Off phải do phần mềm chủ động thực hiện bằng cách ghi vào thanh ghi điều khiển.
 

* **CAN_NART (No Automatic Re-Transmission):**

	*  Bit này điều khiển hành vi tự động truyền lại frame khi quá trình truyền bị thất bại.
	
	*  Khi ở mặc định (DISABLE):
 
 		* Phần cứng sẽ tự động xếp lại frame thất bại vào hàng đợi và cố gắng truyền lại ngay khi bus rảnh.

	*  Khi được ENABLE:
 
 		*  Frame thất bại sẽ bị loại bỏ khỏi bộ đệm truyền
 
   		*  Phần mềm có trách nhiệm quyết định có truyền lại frame đó hay không.  

* **CAN_TXFP (Transmit FIFO Priority):**

	*  Bit này quyết định phương thức phân xử quyền ưu tiên truyền giữa các mailbox đang chờ trong cùng một hộp thư truyền (Transmit Mailbox).
	
	*  Khi được ENABLE:
 
 		* Ưu tiên truyền được xác định theo cơ chế FIFO (First-In, First-Out).
   
   		* Mailbox nào được nạp frame vào trước sẽ được truyền trước, bất kể giá trị ID của frame đó.

	*  Khi được DISABLE:
 
 		*  Ưu tiên truyền được xác định theo giá trị CAN ID.
   
   		*  Mailbox nào chứa frame có ID nhỏ hơn (mức ưu tiên cao hơn trong phân xử bus CAN) sẽ được truyền trước, bất kể thứ tự nạp vào hộp thư.   

* **CAN_RFLM (Receive FIFO Locked Mode):**

	*  Bit này điều khiển hành vi của bộ đệm nhận FIFO khi bộ đệm đã đầy (tràn FIFO).
	
	*  Khi được ENABLE:
 
 		* Chế độ Lock
   
   		* Khi FIFO đầy, frame mới đến sẽ bị từ chối (lock) cho đến khi phần mềm đọc và giải phóng ít nhất một ô nhớ trong FIFO.

	*  Khi được DISABLE:
 
 		*  Chế độ Overwrite
   
   		*  Khi FIFO đầy, frame mới nhất đến sẽ ghi đè lên frame cũ nhất đang được lưu trong FIFO.
     
	 	*  Frame cũ nhất sẽ bị mất vĩnh viễn.

* **CAN_AWUM (Automatic Wake-Up Mode):**

	*  Bit này điều khiển khả năng tự động đánh thức vi điều khiển (MCU) từ chế độ ngủ (Sleep/Standby) khi có hoạt động trên bus CAN.
	
	*  Khi được ENABLE:
 
 		* Bộ thu phát CAN sẽ giám sát bus ngay cả khi MCU đang ở chế độ tiết kiệm năng lượng.
   
   		* Khi phát hiện có tín hiệu Dominant trên bus (báo hiệu có frame đang được truyền), nó sẽ tạo ra tín hiệu đánh thức MCU.

	*  Khi được DISABLE:
 
 		*  Tính năng tự động đánh thức bị vô hiệu hóa.

* **CAN_TTCM (Time Triggered Communication Mode):**

	*  Bit này kích hoạt chế độ giao tiếp CAN theo cơ chế Time-Triggered, được định nghĩa trong chuẩn ISO 11898-4 (TTCAN).
	
	*  Khi được ENABLE:
 
 		* Bộ điều khiển CAN hoạt động trong chế độ TTCAN, sử dụng Time Master để đồng bộ thời gian toàn cục và tạo lịch trình truyền thông theo khe thời gian (Time Slot)
   
   		* Chế độ này đòi hỏi cấu hình phức tạp và chỉ hoạt động khi tất cả các node trên mạng cùng hỗ trợ TTCAN.

	*  Khi được DISABLE:
 
 		*  Bộ điều khiển CAN hoạt động trong chế độ Event-Triggered truyền thống theo chuẩn CAN 2.0B.


#### **8.6. Trình tự khởi tạo CAN bằng SPL**

* 1. Cấu hình System Clock → PCLK1 = 36 MHz

* 2. Bật Clock CAN1 + AFIO
	
* 3. Cấu hình GPIO (TX: AF_PP, RX: IPU/Floating)

* 4. Remap CAN nếu cần

* 5. Khởi tạo CAN_InitTypeDef

* 6. Gọi CAN_Init(CAN1, &CAN_InitStructure)

* 7. Cấu hình Acceptance Filter

* 8. Bật CAN: CAN_ITConfig() và CAN_Cmd(CAN1, ENABLE)

### **IX.  CẤU HÌNH BỘ LỌC CAN**

#### **9.1. Các chế độ lọc**

* bxCAN hỗ trợ hai chế độ lọc chính:

	*  **Mask Mode (CAN_FilterMode_IdMask)**
	
		* Chế độ Mask Mode hoạt động dựa trên phép toán so sánh có che bit.

  		* Mỗi bộ lọc được cấu hình với hai giá trị:
    
			* Filter_ID (Giá trị mẫu): Chứa giá trị ID mong muốn tại các vị trí bit quan trọng.
    
			* Mask (Mặt nạ che): Xác định bit nào cần so sánh và bit nào được bỏ qua.  
		
		* Nguyên lý: `(Received_ID & Mask) == (Filter_ID & Mask)`
		
		* Chế độ này đặc biệt phù hợp khi một node cần nhận toàn bộ frame thuộc về một dải ID liên tục (ID range).
		
		* Ví dụ: Giả sử node cần nhận tất cả các frame có ID từ 0x200 đến 0x2FF (tương đương dải 256 ID liên tục).

  			* Phân tích dải ID: 

	  			* 0x200 = 010 0000 0000 (nhị phân 11 bit)
	
	  			* 0x2FF = 010 1111 1111 (nhị phân 11 bit)
	
				* Nhận xét: 3 bit cao (bit [10:8]) cố định là 010, toàn bộ 8 bit thấp (bit [7:0]) thay đổi từ 00000000 đến 11111111. 

  			* Cấu hình bộ lọc:

	  			* Filter_ID = 0x200 (010 0000 0000)
	
	  			* Mask = 0x700 (111 0000 0000)

  			* Kiểm tra nguyên lý với ID 0x2A5 (010 1010 0101):

					(010 1010 0101 & 111 0000 0000) == (010 0000 0000 & 111 0000 0000)
					→ 010 0000 0000 == 010 0000 0000   → Kết quả: Đúng → Frame được nhận.
        
	*  **Identifier List Mode (CAN_FilterMode_IdList)**
	
		* Chế độ Identifier List Mode thực hiện so sánh chính xác tuyệt đối giữa ID của frame nhận được với từng ID trong một danh sách đã được lập trình sẵn.
		
		* Không sử dụng mặt nạ che (Mask), mỗi vị trí trong bộ lọc chứa một giá trị ID cụ thể.
		
		* Frame được chấp nhận nếu ID của nó khớp hoàn toàn với bất kỳ ID nào trong danh sách.

  		* Chế độ này phù hợp khi node chỉ cần nhận một số lượng nhỏ các ID rời rạc, không liên tục (discrete IDs).
    
		* VD: Một ECU điều khiển đèn trên ô tô chỉ cần nhận đúng ba lệnh từ BCM (Body Control Module)

  			* 0x310: Lệnh bật đèn pha.
     
	 		* 0x315: Lệnh bật đèn sương mù.
    
			* 0x3A2: Lệnh bật đèn xi-nhan.     

#### **9.2. Kích thước bộ lọc**

* Mỗi Filter Bank có thể được cấu hình với hai mức độ rộng:

	* **32-bit Scale (CAN_FilterScale_32bit):**
	
		*  Nguyên lý hoạt động:
 
  			* Trong chế độ này, toàn bộ không gian lưu trữ của một Filter Bank (32 bit cho phần ID và các bit điều khiển) được sử dụng như một bộ lọc duy nhất.
     
	 		* Bộ lọc này có khả năng chứa và so sánh toàn bộ 29 bit của Extended ID hoặc 11 bit của Standard ID (được đặt trong trường tương ứng của thanh ghi).   
		
		*  Đặc điểm:

  			* Mỗi Filter Bank chỉ chứa 1 filter duy nhất.
     
	 		* Hỗ trợ đầy đủ cả Standard ID (11-bit) và Extended ID (29-bit).
    
			* Khi hoạt động ở chế độ Identifier List Mode, một filter bank 32-bit có thể lưu trữ 2 ID Standard (mỗi ID chiếm 16 bit trong trường dữ liệu) hoặc 1 ID Extended (chiếm toàn bộ 32 bit).      
		
		*  Chế độ 32-bit được khuyến nghị sử dụng trong các trường hợp sau:

  			* Mạng CAN sử dụng Extended ID (29-bit), ví dụ trong giao thức J1939 (xe tải, máy nông nghiệp) hoặc CANopen.
     
	 		* Yêu cầu lọc có độ chính xác rất cao, cần kiểm tra toàn bộ các bit của ID mà không bỏ qua bất kỳ bit nào.
    
			* Ứng dụng cần lọc Extended ID theo cơ chế Mask Mode với phạm vi che linh hoạt trên toàn bộ 29 bit.     
		
	* **16-bit Scale (CAN_FilterScale_16bit):**
	
		*  Nguyên lý hoạt động:
 
  			* Trong chế độ này, không gian lưu trữ 32 bit của một Filter Bank được chia thành hai nửa độc lập, mỗi nửa 16 bit hoạt động như một bộ lọc riêng biệt.
     
	 		* Như vậy, mỗi Filter Bank cung cấp 2 filter có thể cấu hình với các tham số lọc khác nhau.  
		
		*  Đặc điểm:

  			* Mỗi Filter Bank chứa 2 filter độc lập.
     
	 		* Mỗi filter 16 bit chỉ chứa trường dữ liệu đủ cho Standard ID (11-bit) cùng các bit điều khiển liên quan.
    
			* Khi tất cả các Filter Bank trong bxCAN đều được cấu hình ở chế độ 16-bit, tổng số filter tối đa có thể đạt được là 56 filter (28 Filter Bank × 2 filter mỗi bank).
		
		*  Chế độ 16-bit được khuyến nghị sử dụng trong các trường hợp sau:

  			* Mạng CAN chỉ sử dụng Standard ID (11-bit).
     
	 		* Ứng dụng cần lọc nhiều ID rời rạc và muốn tối đa hóa số lượng filter khả dụng để không bị giới hạn bởi số lượng Filter Bank vật lý.
    
			* Tiết kiệm tài nguyên Filter Bank, cho phép một Filter Bank phục vụ đồng thời hai nhu cầu lọc độc lập.        

#### **9.3. Phân bổ dữ liệu vào FIFO (FIFO0 hoặc FIFO1)**

* Mỗi Filter Bank có thể được gán vào một trong hai FIFO:

	* **CAN_Filter_FIFO0:**
	
		*  Thông điệp khớp filter sẽ được đưa vào Receive FIFO0.

	* **CAN_Filter_FIFO1:**
	
		*  Thông điệp khớp filter sẽ được đưa vào Receive FIFO1.
		
* Mỗi FIFO có ngắt riêng (`CAN_IT_FMP0, CAN_IT_FMP1`).

#### **9.4. Triển khai Code**

* Cấu trúc:

			CAN_FilterInitTypeDef CAN_FilterInitStructure;

* **Ví dụ 1: Mask Mode 32-bit – Nhận tất cả Standard ID (thường dùng để test):**
	
		void CAN_Filter_Config(void)
		{
		    CAN_FilterInitStructure.CAN_FilterNumber = 0;                    // Filter Bank 0
		    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;  // Mask Mode
		    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
		    
		    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;               // ID muốn nhận
		    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
		    
		    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;           // Mask = 0 → nhận tất cả
		    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
		    
		    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
		    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
		    
		    CAN_FilterInit(&CAN_FilterInitStructure);
		}

* **Ví dụ 2: Mask Mode 32-bit – Nhận ID từ 0x200 đến 0x2FF (Standard ID)**
	
	    CAN_FilterInitStructure.CAN_FilterNumber = 0;
	    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	    
	    CAN_FilterInitStructure.CAN_FilterIdHigh = (0x200 << 5);     // ID cơ sở
	    CAN_FilterInitStructure.CAN_FilterIdLow  = 0x0000;
	    
	    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x700 << 5); // Mask che 9 bit thấp → nhận 0x200~0x2FF
	    CAN_FilterInitStructure.CAN_FilterMaskIdLow  = 0x0000;
	    
	    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	    
	    CAN_FilterInit(&CAN_FilterInitStructure);

* **Ví dụ 3: List Mode 16-bit – Nhận 4 ID cụ thể (0x100, 0x200, 0x300, 0x400)**
	
	        CAN_FilterInitStructure.CAN_FilterNumber = 1;
		    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;   // List Mode
		    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;  // 2 ID mỗi bank
		    
		    // Filter 1A: ID 0x100
		    CAN_FilterInitStructure.CAN_FilterIdHigh = (0x100 << 5);
		    CAN_FilterInitStructure.CAN_FilterIdLow  = (0x200 << 5);   // Filter 1B: ID 0x200
		    
		    // Filter 2A: ID 0x300
		    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x300 << 5);
		    CAN_FilterInitStructure.CAN_FilterMaskIdLow  = (0x400 << 5); // Filter 2B: ID 0x400
		    
		    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
		    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
		    
		    CAN_FilterInit(&CAN_FilterInitStructure);


* **Lưu ý:**

	* Phải gọi `CAN_FilterInit()` sau `CAN_Init()`. 
	
	* Filter Activation = `ENABLE`. 
	
	* Sau khi cấu hình xong tất cả filter → bật CAN: `CAN_Cmd(CAN1, ENABLE)`; 
	
	* Nếu muốn nhận tất cả thông điệp → dùng Mask = 0x0000. 


### **X.  KỸ THUẬT GỬI VÀ NHẬN DỮ LIỆU CAN**

#### **10.1. Gửi dữ liệu (Hàm CAN_Transmit và quản lý Tx Mailbox)**

* Để gửi một frame CAN, ta sử dụng hàm `CAN_Transmit()`

	*  **Cấu trúc dữ liệu:**

			CanTxMsg TxMessage;

			TxMessage.StdId = 0x123;					// Standard ID (11 bit)
			TxMessage.ExtId = 0x00;					// Không dùng nếu là Standard
			TxMessage.IDE  = CAN_Id_Standard;			// Hoặc CAN_Id_Extended
			TxMessage.RTR = CAN_RTR_Data;				// CAN_RTR_Remote nếu là Remote Frame
			TxMessage.DLC = 8;							// Độ dài dữ liệu: 0 - 8

			// Dữ liệu
			TxMessage.Data[0] = 0x01; 
			TxMessage.Data[1] = 0x02; 
			TxMessage.Data[2] = 0x03; 
			TxMessage.Data[3] = 0x04; 
			TxMessage.Data[4] = 0x05; 
			TxMessage.Data[5] = 0x06; 
			TxMessage.Data[6] = 0x07; 
			TxMessage.Data[7] = 0x08;			
	  
	*  **Hàm gửi dữ liệu:**
	
				uint8_t CAN_Send_Message(uint32_t id, uint8_t *data, uint8_t len){
				CanTxMsg TxMessage;
				if (len > 8) len = 8;

				TxMessage.StdId = id;
				TxMessage.ExtId = 0;
				TxMessage.IDE = CAN_Id_Standard;
				TxMessage.RTR = CAN_RTR_Data;
				TxMessage.DLC = len;

				for(uint8_t i = 0; i < len; i++){
					TxMessage.Data[i] = data[i];
				}

				// Gửi và trả về Mailbox được sử dụng (0,1,2) hoặc CAN_TxStatus_NoMailBox nếu đầy
				uint8_t mailbox = CAN_Transmit(CAN1, &TxMessage);
				return mailbox;
				}


	*  **Kiểm tra trạng thái Mailbox:**
	
			// Giả sử đã gọi: uint8_t mailbox = CAN_Transmit(CAN1, &TxMessage);
			
			// 1. Kiểm tra xem thông điệp có được đưa vào Mailbox thành công không
			if(mailbox == CAN_TxStatus_NoMailBox){
				// Cả 3 Transmit Mailbox đều đang đầy, thông điệp chưa được gửi.
				
				// Xử lý: Đưa vào buffer mềm hoặc thử lại sau
			}else{
			
				// 2. Chờ cho đến khi Mailbox truyền xong dữ liệu ra Bus
				// Trong hệ thống lớn, nên dùng ngắt truyền (Transmit Interrupt)
					uint8_t status = CAN_TxStatus_Pending;
					while(status == CAN_TxStatus_Pending){
						status = CAN_TransmitStatus(CAN1, mailbox);	 // Kiểm tra Mailbox đang dùng
					}

				// 3. Đánh giá kết quả truyền
				if (status == CAN_TxStatus_Ok){
					// Truyền thành công (Đã nhận được cờ ACK từ các Node khác)
				}
				else if( status == CAN_TxStatus_Failed){
					// Truyền thất bại
				}
			}

*  **Lưu ý:**

	* Nếu cả 3 Mailbox dều đầy -> `CAN_Transmit()` trả về `CAN_TxStatus_NoMailBox`
	
	* Nên kiểm tra trước khi gửi hoặc dùng ngắt Transmit Complete  

#### **10.2. Nhận dữ liệu bằng phương pháp Polling**

			void CAN_Receive_Polling(void){
				if(CAN_MessagePending(CAN1, CAN_FIFO0) > 0){   // Kiểm tra FIFO có dữ liệu không
					CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);  // Đọc dữ liệu từ FIFO0

					// Xử lý dữ liệu nhận được
					printf("ID: 0x%X, DLC: %d, Data: ", RxMessage.StdId, RxMessage.DLC);
					for(uint8_t i = 0; i < RxMessage.DLC; i++){
						printf("%02X ", RxMessage.Data[i]);
					}
					printf("\r\n");
				}
				// Có thể kiểm tra thêm FIFO1 nếu cần
				// if (CAN_MessagePending(CAN1, CAN_FIFO1) > 0) ...
			}
					
#### **10.3. Nhận dữ liệu bằng phương pháp Interrupt**

* **Bước 1: Cấu hình ngắt trong hàm khởi tạo**

			void CAN_Interrupt_Config(void){
				// Bật ngắt cho FIFO0 (có message pending)
				CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);		// FIFO0 Message Pending
				// CAN_ITConfig(CAN1, CAN_IT_FMP1, ENABLE);		// Nếu dùng FIFO1

				// Tùy chọn: Bật ngắt lỗi và Bus-Off
				CAN_ITConfig(CAN1, CAN_IT_ERR | CAN_IT_BOF, ENABLE);
			}

* **Bước 2: Viết hàm xử lý ngắt (IRQ Handler)**

			void USB_LP_CAN1_RX0_IRQHandler(void){			// Ngắt FIFO0
				CanRxMsg  RxMessage;

				if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET){
					CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);			// Đọc dữ liệu
					// Xử lý dữ liệu ở đây
					CAN_Process_RxMessage(&RxMessage);
					CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);				// Xóa cờ ngắt
				}
			}				

	* **Hàm xử lý dữ liệu nhận được**

			void CAN_Process_RxMessage(CanRxMsg *RxMessage){
				switch(RxMessage->StdId){
					case 0x100:
						// Xử lý thông điệp ID 0x100
						break;
					case 0x200:
						// Xử lý thông điệp ID 0x200
						break;
					default:
						break;
					}
			}

* **Bước 3: Bật ngắt toàn cục trong NVIC**

		NVIC_InitTypeDef NVIC_InitStructure;

		NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;   // Ngắt CAN RX0
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

							
   </details> 


