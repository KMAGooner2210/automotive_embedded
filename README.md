
	
# AUTOMOTIVE_EMBEDDED

<details>
	<summary><strong>BÀI 1: GPIO</strong></summary>
	
## BÀI 1: GPIO (General Purpose Input Output)
![Image](https://github.com/user-attachments/assets/f275f738-034e-41e5-849a-892cb47e31d6)

### **1.1.Cấp Clock cho ngoại vi**

* **Module RCC** cung cấp các hàm để cấu hình xung clock

`RCC_APB1PeriphClockCmd`

`RCC_APB2PeriphClockCmd` 

`RCC_AHBPeriphClockCmd`
 
* **Nhận 2 tham số**:

 ◦ Ngoại vi Clock

 ◦ ENABLE/DISABLE

### **1.2.Cấu hình ngoại vi**


* **GPIO_Pin:** Chân cần được cấu hình 

`GPIO_Pin_<chân cần được cấu hình>`


 
* **GPIO_Mode:** Chế độ muốn cấu hình
    ```
    typedef enum {

        GPIO_Mode_AIN = 0x00,            //Analog Input
        GPIO_Mode_IN_FLOATING = 0x04,    //Input bình thường
        GPIO_Mode_IPD = 0x28,            //Input có điện trở kéo xuống    
        GPIO_Mode_IPU = 0x48,            //Input có điện trở kéo lên
        GPIO_Mode_Out_OD = 0x14,         //Output dạng open-drain
        GPIO_Mode_Out_PP = 0x10,         //Output dạng push-pull
        GPIO_Mode_AF_OD = 0x1C,          //Chế độ ngoại vi khác dạng open-drain
        GPIO_Mode_AF_PP = 0x18           //Chế độ ngoại vi khác dạng push-pull
    } GPIOMode_TypeDef;
    ```
* **GPIO_Speed:** Tốc độ đáp ứng của chân

`GPIO_Speed_<tốc độ muốn cấu hình>`    

### **1.3.Sử dụng ngoại vi**


* **Để gắn các giá trị muốn cấu hình vào các thanh ghi** thì ta sử dụng hàm "GPIO_Init" có 2 tham số cung cấp các hàm để cấu hình xung clock


  ◦Tham số đầu là tên ngoại vi muốn cấu hình

  ◦Tham số thứ hai là con trỏ đến struct **"GPIO_InitTypeDef"**

```
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
```

* **Các hàm thông dụng để điều khiển GPIO** 

```
    uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);            \\Đọc giá trị 1 bit trong cổng GPIO được cấu hình là INPUT (IDR), có thể đọc nhiều pin nhờ toán tử OR
    uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);                                 \\Đọc giá trị nguyên cổng GPIO được cấu hình là INPUT (IDR)
    uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);           \\Đọc giá trị 1 bit trong cổng GPIO được cấu hình là OUTPUT (ODR), có thể đọc nhiều pin nhờ toán tử OR
    uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);                                \\Đọc giá trị nguyên cổng GPIO được cấu hình là OUTPUT (ODR)
    void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);                        \\Cho giá trị 1 bit trong cổng GPIO = 1, có thể ghi nhiều pin nhờ toán tử OR
    void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);                      \\Cho giá trị 1 bit trong cổng GPIO = 0, có thể ghi nhiều pin nhờ toán tử OR
    void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);     \\Ghi giá trị "BitVal" vào 1 bit trong cổng GPIO, có thể ghi nhiều pin nhờ toán tử OR
    void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);                           \\Ghi giá trị "PortVal" vào nguyên cổng GPIO
```

### **1.4.Kiến thức cần chú ý**

#### **1.4.1.Pull-Up vs Pull-Down ???**


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




#### **1.4.2.Các chế độ input khác**

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
    

#### **1.4.3.Các chế độ output**

* **Output Push-Pull:**

    ◦ **Mục đích:**  Xuất tín hiệu số kéo VDD (HIGH) và kéo xuống GND (LOW) 1 cách chủ động


    ◦ **Sử dụng:** Điều khiển relay,led,...



* **Output Open-Drain (Đầu ra hở mạch):**

    ◦ **Mục đích:**  Xuất tín hiệu số kéo chỉ có khả năng kéo GND (LOW) 1 cách chủ động.Kéo lên HIGH cần có điện trở kéo bên ngoài

    ◦ **Cấu hình điện:** Khi xuất mức HIGH ,transistor tắt,GPIO thả nổi và cần điện trở kéo lên bên ngoài

    ◦ **Sử dụng:** I2C,...   



#### **1.4.4.Các chế độ Alternate Function**


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


### **2.1.Khái niệm**

![Image](https://github.com/user-attachments/assets/eb1762a6-057e-4212-91cd-7d216830df0d)


* **Ngắt** là 1 sự kiện khẩn cấp xảy ra trong hay ngoài vi điều khiển.Nó yêu cầu MCU phải **dừng chương trình chính** và **thực thi chương trình ngắt**



### **2.2.Interrupt Service Routine (Trình phục vụ ngắt)**

![Image](https://github.com/user-attachments/assets/90c8c28e-edea-4755-b2b3-8c84be788a61)


 * Mỗi ngắt có 1 trình phục vụ riêng
 * Trình phục vụ ngắt là 1 đoạn chương trình được thực hiện khi ngắt xảy ra
 * Địa chỉ trong bộ nhớ của ISR được gọi là **vector ngắt**



### **2.3.Vector Interrupt Table (Bảng vector ngắt)**

![Image](https://github.com/user-attachments/assets/038312e2-516d-4eb2-8125-438cf5885fc6)


 * Chứa danh sách các địa chỉ bộ nhớ,mỗi địa chỉ trỏ đến hàm xử lý ngắt tương đương với 1 nguồn ngắt cụ thể


 * Vai trò trung tâm trong xử lý ngắt

    ◦ Khi 1 ngắt xảy ra, phần cứng CPU (cụ thể **NVIC**) sử dụng bảng **VIT** để xác định ISR nào cần được thực thi
 

 * Địa chỉ cố định trong bộ nhớ Flash
 
    ◦ VIT thường được đặt ở địa chỉ bắt đầu bộ nhớ Flash.Điều này do khi vi điều khiển khơi động / reset, nó sẽ bắt đầu thực thi từ địa chỉ này

 * Cấu trúc VIT

    ◦ **Bảng các địa chỉ:** VIT về cơ bản là một mảng (table) các địa chỉ bộ nhớ. Mỗi mục trong bảng này chứa một địa chỉ.

    ◦ Mỗi mục tương ứng với 1 nguồn ngắt

    ◦ **Kích thước:** Kích thước của mỗi mục (mỗi địa chỉ vector) thường là 4 byte (32-bit address) trên các kiến trúc 32-bit như ARM Cortex-M, hoặc 2 byte (16-bit address) trên các kiến trúc 16-bit
 

* **Cách VIT hoạt động:**


    ◦Ngắt xảy ra

     ```
   Quy trình bắt đầu khi một sự kiện ngắt xảy ra.

   Sự kiện này có thể đến từ nhiều nguồn khác nhau, ví dụ:

   Ngắt ngoại vi: Một module ngoại vi như UART, Timer, ADC, GPIO tạo ra ngắt khi hoàn thành một tác vụ hoặc có một sự kiện cụ thể (ví dụ: UART nhận dữ liệu, Timer tràn, ADC chuyển đổi xong, GPIO pin thay đổi trạng thái).

   Ngắt nội bộ: Các ngắt do bộ vi điều khiển tạo ra, ví dụ: SysTick timer ngắt định kỳ, Software Interrupt (SWI), PendSV (Pendable Service Call).

   Ngắt lỗi: Các lỗi phần cứng hoặc phần mềm như HardFault, Memory Management Fault.
     ```
  
    ◦NVIC xác định vector number

    ◦Tra cứu VIT bằng cách sử dụng vector number làm offset để tra cứu bảng 
    
      
      Base_Address of VIT + (n * 4).
      Base_Address of VIT: Đây là địa chỉ bộ nhớ bắt đầu của Vector Interrupt Table. Như đã thảo luận trước đó, địa chỉ này thường cố định và nằm ở đầu bộ nhớ Flash (ví dụ: 0x00000000).
      n (vector number): Là số vector mà NVIC đã xác định cho ngắt đang xảy ra.
      4: Là kích thước của mỗi mục (vector) trong VIT, tính bằng byte. Trong kiến trúc 32-bit, mỗi địa chỉ bộ nhớ thường là 32-bit (4 byte).
      n * 4: Tính toán offset (độ lệch) từ địa chỉ bắt đầu của VIT. Vector number n được nhân với 4 để tính ra độ lệch byte tương ứng trong bảng VIT.
      Base_Address of VIT + (n * 4): Kết quả là địa chỉ bộ nhớ trong VIT, nơi chứa địa chỉ của ISR tương ứng với vector number n.
      

   ◦Lấy địa chỉ ISR
   
     Giá trị tra cứu được trong VIT chính là địa chỉ của bộ nhớ hàm ISR tương ứng

   ◦Nhảy đến ISR
   
   ◦Kết thúc ISR





### **2.4.NVIC (Nested Vectored Interrupt Controller)**

* **Khái niệm**

   ◦Quản lý tất cả các ngắt
   
   ◦Quyết định ngắt nào được xử lý dựa trên mức độ ưu tiên

* **Chức năng NVIC**

   ◦ Bật/Tắt từng nguồn ngắt cụ thể

   ◦ Bật/Tắt ngắt toàn cục
  

* **Quản lý ưu tiên ngắt**

 
   ◦ Cho phép 16 mức ưu tiên (0-15)
   
   
   ◦ Mức ưu tiên thấp hơn (số nhỏ hơn) có độ ưu tiên cao hơn
    
![Image](https://github.com/user-attachments/assets/9ca9134e-b2e2-4de9-b352-74ad5a47331e)
 
     +  Preemption Priority: 
        Ngắt có Preemption Priority cao hơn (số nhỏ hơn) sẽ chiếm quuyền ngắt có Priority thấp hơn (số to hơn): 
        Ngắt có Preemption Priority cao hơn (số nhỏ hơn) sẽ chiếm quuyền ngắt có Priority thấp hơn (số to hơn)

     +  SubPriority: 
        Khi nhiều ngắt có cùng Preemption Priority xảy ra đồng thời
        => SubPriority quyết định thứ tự xử lý
        => Ngắt có SubPriority cao hơn (số nhỏ hơn) sẽ được xử lý trước


* **Cấu hình NVIC**
 
  ◦ **Priority Group:**  xác định cách phân chia bit giữa Preemption Priority và Subpriority.
  
   Sử dụng hàm **NVIC_PriorityGroupConfig(uint32_t PriorityGroup)** để chọn priority group cho NVIC

  ◦ **NVIC_IRQChannel:** Xác định mã của kênh ngắt cần được cấu hình

  ◦ **NVIC_IRQChannelPreemptionPriority:** Xác định mức độ ưu tiên Preemption Priority cho kênh ngắt.

  ◦ **NVIC_IRQChannelSubPriority:** Xác định mức độ ưu tiên phụ Subpriority cho kênh ngắt.

  ◦ **NVIC_IRQChannelSubPriority:** Cho phép ngắt


```
NVIC_InitTypeDef NVICInitStruct;
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
NVICInitStruct.NVIC_IRQChannel = EXTI0_IRQn;
NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
NVICInitStruct.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVICInitStruct);
```
</details>


<details>
	<summary><strong>BÀI 3: CÁC CHUẨN GIAO TIẾP CƠ BẢN </strong></summary> 


## **BÀI 3: CÁC CHUẨN GIAO TIẾP CƠ BẢN**

## **3.1.SPI**


### **3.1.1.Đặc điểm**
* Chuẩn giao tiếp nối tiếp,đồng bộ
* Hoạt động ở chế độ song công
* Sử dụng 4 dây giao tiếp

### **3.1.2.Sơ đồ chân**
![Image](https://github.com/user-attachments/assets/9e55733b-bf9f-4d37-912e-84e5faae3086)


* **SCK (Serial Clock)**: Thiết bị Master tạo xung tín hiệu SCK và cung cấp cho Slave

* **MISO (Master Input Slave Output)**: Tín hiệu tạo bởi thiết bị Slave và nhận bởi thiết bị Master.

* **MOSI (Master Output Slave Input)**: Tín hiệu tạo bởi thiết bị Master và nhận bởi thiết bị Slave.

* **SS (Slave Select) / CS (Chip Select)**: Chọn thiết bị Slave cụ thể để giao tiếp. Để chọn Slave giao tiếp thiết bị Master chủ động kéo đường SS tương ứng xuống mức 0 (Low).


### **3.1.3.Quá trình truyền nhận**  

* Master kéo chân SS của chân Slave muốn giao tiếp xuống mức 0 để báo hiệu muốn truyền nhận

* Clock sẽ được cấp bởi master, tùy vào chế độ được cài, với mỗi xung clock,  1 bit sẽ được truyền từ master đến slave và slave cũng truyền 1 bit cho master.

* Các thanh ghi cập nhật giá trị và dịch 1 bit.

* Lặp lại quá trình trên đến khi truyền xong 8 bit trong thanh ghi.

### **3.1.4.Các chế độ hoạt động**

* Có 4 chế độ hoạt động phụ thuộc **Clock Polarity (CPOL)** và **Clock Phase (CPHA)**.

* **CPOL:**

  **CPOL = 0:** Xung clock ban đầu ở mức 0.

  **CPOL = 1:** Xung clock ban đầu ở mức 1.

* **CPHA:**

  CPHA = 0 (1Edge):
  
         Dữ liệu được **lấy mẫu (sampled) trên cạnh đầu tiên (leading edge)** của chu kỳ đồng hồ và được **thay đổi (shifted) trên cạnh thứ hai (trailing edge)**.
         Dữ liệu phải sẵn sàng trên đường truyền (MOSI/MISO) trước khi chu kỳ đồng hồ bắt đầu.**
  
  CPHA = 1 (2Edge):
  
         Dữ liệu được **thay đổi (shifted) trên cạnh đầu tiên (leading edge)** và được **lấy mẫu (sampled) trên cạnh thứ hai (trailing edge)**.
         Dữ liệu chỉ xuất hiện trên đường truyền sau khi chu kỳ đồng hồ bắt đầu.
  
  ![Image](https://github.com/user-attachments/assets/7035fcb0-fd0e-4d24-976b-0a56d80a1207)
  
```
CPOL = 0 (đồng hồ nghỉ ở mức thấp):
      1Edge (CPHA = 0): Lấy mẫu trên rising edge (cạnh tăng), thay đổi trên falling edge (cạnh giảm).
      2Edge (CPHA = 1): Thay đổi trên rising edge, lấy mẫu trên falling edge.

CPOL = 1 (đồng hồ nghỉ ở mức cao):
      1Edge (CPHA = 0): Lấy mẫu trên falling edge (cạnh giảm), thay đổi trên rising edge (cạnh tăng).
      2Edge (CPHA = 1): Thay đổi trên falling edge, lấy mẫu trên rising edge.
```
## **3.2.I2C**

### **3.2.1Đặc điểm**

* Chuẩn giao tiếp nối tiếp,đồng bộ
* Hoạt động ở chế độ bán song công
* Sử dụng 2 dây giao tiếp

### **3.2.2.Sơ đồ chân**

![Image](https://github.com/user-attachments/assets/34dd609d-2f2e-421d-b416-0cabcbf9670a)


* **SCL (Serial Clock)**: Tạo xung tín hiệu để đồng bộ việc truyền/nhận dữ liệu với các Slave.

* **SDA (Serial Data)**: Chân chứa dữ liệu được truyền đi


### **3.2.3.Quá trình truyền nhận**

* Start: Điều kiện: Chân SDA xuống mức 0 trước chân SCL.
* Truyền các bit địa chỉ để tìm Slave muốn giao tiếp.

* Bit R/W: Master gửi dữ liệu đi ứng với bit '0', nhận dữ liệu ứng với bit '1'.

* ACK: Chờ phản hồi, '0' là nhận và '1' là không nhận. Nếu không có Slave nào phản hồi, dùng Timer để thoát ra.

* Sau khi chọn được Slave để giao tiếp, bắt đầu truyền các bit dữ liệu đến Slave.
* Tương tự cũng có ACK để chờ phản hồi.
* Stop: Điều kiện: Chân SDA lên mức 1 sau chân SCL.



## **3.3.UART**

### **3.3.1Đặc điểm**

* Chuẩn giao tiếp nối tiếp
* Không đồng bộ
* Hoạt động ở chế độ song công
* Sử dụng 2 dây giao tiếp
* Chỉ 2 thiết bị giao tiếp

### **3.3.2.Sơ đồ chân**

![Image](https://github.com/user-attachments/assets/5d657816-56e4-4871-8c40-7ce476b283c2)

**Tx (Transmit)**: Chân truyền dữ liệu 

**Rx (Receive)**: Chân nhận dữ liệu

### **3.3.3.Quá trình truyền nhận**

![Image](https://github.com/user-attachments/assets/e345c226-4ada-4991-963b-1dcb7e0ae24c)

* Start: 1 bit.

* Bit dữ liệu: 5 đến 9 bit.

* Bit chẵn lẻ:

    Quy luật chẵn: Thêm một bit '0' hoặc '1' để số bit '1' là số chẵn.
    Quy luật lẻ: Thêm một bit '0' hoặc '1' để số bit '1' là số lẻ.
* Stop: 1 đến 2 bit.
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
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}

```

### **6.2.4.Các hàm  thông dụng**

* `USART_SendData(USART_TypeDef * USARTx,uint16_t Data)` truyền data từ UARTx.Data này đã được thêm bit chẵn/lẻ tùy cấu hình.

* `USART_ReceiveData(USART_TypeDef* USARTx)` nhận data từ UARTx

* Hàm `USART_GetFlagStatus(USART_TypeDef* USARTx,uint16_t USART_FLAG)` trả về trạng thái cờ USART_FLAG tương ứng

  ◦ **USART_FLAG_TXE** : Cờ báo thanh ghi chứa dữ liệu truyền đi (DR) **đang trống**
 
  ◦ **USART_FLAG_RXNE** : Cờ báo thanh ghi chứa dữ liệu nhận (DR) **đã có** dữ liệu 

  ◦ **USART_FLAG_IDLE** : Cờ báo đường truyền đang ở chế độ rảnh

  ◦ **USART_FLAG_PE**   : Cờ báo lỗi Parity

  ◦ **USART_FLAG_TC**   : Cờ báo đã hoàn thành quá trình truyền dữ liệu  
### **6.2.5.Hàm truyền**

* Hàm truyền 1 ký tự

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

* Hàm truyền 1 chuỗi ký tự

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
</details>

<details>
	<summary><strong>BÀI 7: CÁC LOẠI NGẮT </strong></summary> 

## BÀI 7: CÁC LOẠI NGẮT

### **7.1.Ngắt ngoài**


* **Sơ đồ**

![Image](https://github.com/user-attachments/assets/b8531dc8-d8a1-4fea-b10b-90365810da53)


  ◦ Để sử dụng được ngắt ngoài, ngoài bật clock cho GPIO tương ứng cần bật thêm clock cho **AFIO**.

```
  void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}
```
 ◦ Ngắt ngoài của chip STM32F103 bao gồm có 16 line riêng biệt
```
Line0 sẽ chung cho tất cả chân Px0 ở tất cả các Port, với x là tên của Port A, B…
Line0 nếu chúng ta đã chọn chân PA0 (chân 0 ở port A) làm chân ngắt thì tất cả các chân 0 ở các Port khác không được khai báo làm chân ngắt ngoài nữa
```
 

 * **Cấu hình ngắt ngoài**

    ◦ Cấu hình chân ngắt ngoài là Input. 

    ◦  Có thể cấu hình thêm trở kéo lên/xuống tùy theo cạnh ngắt được sử dụng.

   ```
    void GPIO_Config(){
        GPIO_InitTypeDef GPIOInitStruct;

	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIOInitStruct);
    }
    ```



      ◦ Hàm **GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)** liên kết 1 chân với một EXTI line để cấu hình chân ở chế độ sử dụng ngắt ngoài:

GPIO_PortSource: Chọn Port để sử dụng làm nguồn cho ngắt ngoài.
GPIO_PinSource: Chọn Pin để cấu hình.

       ◦ Các tham số ngắt ngoài được cấu hình trong struct EXTI_InitTypeDef, gồm:

**EXTI_Line:** Xác định EXTI line cụ thể sẽ được cấu hình.
**EXTI_Mode:** Xác định chế độ hoạt động của EXTI, có hai chế độ là Interrupt hoặc Event.
**EXTI_Trigger:** Xác định loại cạnh xung sẽ kích hoạt ngắt.
**EXTI_LineCmd:** Kích hoạt (ENABLE) hoặc vô hiệu hóa (DISABLE) EXTI line.


    
    void EXTI_Config(){
	EXTI_InitTypeDef EXTIInitStruct;

        EXTIInitStruct.EXTI_Line = EXTI_Line0;
	EXTIInitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTIInitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTIInitStruct.EXTI_LineCmd = ENABLE;
	
	EXTI_Init(&EXTIInitStruct);
}
      ```


◦ Tiếp đến cấu hình NVIC:

    
        NVIC_InitTypeDef NVICInitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVICInitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
        NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVICInitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVICInitStruct);

◦ Các hàm quan trọng trong EXTI:

   Ngắt trên từng line có hàm phục riêng của từng line. Có tên cố định: **EXTIx_IRQHandler()** (x là line ngắt tương ứng).
   
   Hàm **EXTI_GetITStatus(EXTI_Linex)**, Kiểm tra cờ ngắt của line x tương ứng. 
   
   Hàm **EXTI_ClearITPendingBit(EXTI_Linex)**: Xóa cờ ngắt ở line x.


◦ Ngắt ngoài sẽ được thực hiện theo:

   Kiểm tra ngắt đến từ line nào, có đúng là line cần thực thi hay không?

   Thực hiện các lệnh, các hàm.

   Xóa cờ ngắt ở line.

```
void EXTI0_IRQHandler(){	
        if(EXTI_GetITStatus(EXTI_Line0) != RESET)
        {}
	EXTI_ClearITPendingBit(EXTI_Line0);
}
```


### **7.2.Ngắt Timer**

* **Sơ đồ**


![Image](https://github.com/user-attachments/assets/b0736d5c-3a49-41bf-95db-063762fdb254)


  ◦ Sử dụng ngắt Timer,ta vẫn cấu hình các tham số trong **TIM_TimeBaseInitTypeDef** bình thường

  ◦ Riêng **TIM_Period**,đây là số lần đếm mà sau đó timer sẽ ngắt


* **Cấu hình ngắt Timer**

  ◦ Cấu hình **Timer**

     Hàm **TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE)** kích hoạt ngắt cho TIMERx tương ứng

      
      Yêu cầu: cài đặt Period = 10-1 ứng với ngắt mỗi 1ms
      void TIM_Config()
      {
      TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

      TIM_TimeBaseInitStruct.TIM_Prescaler = 7200-1;
      //Ngắt mỗi 1ms => 1 ms = ?/72MHz => ? = 7200

      TIM_TimeBaseInitStruct.TIM_Period = 10-1;
      TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
      TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
      TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

      TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
      TIM_Cmd(TIM2, ENABLE);
      }
      

  ◦ Cấu hình **NVIC**

     
     ```
          NVIC_InitTypeDef NVIC_InitStruct;

          NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

	  NVIC_Init(&NVIC_InitStruct);
     ```



  ◦ Cấu hình **hàm phục vụ ngắt Timer**


    
 Hàm phục vụ ngắt Timer được đặt tên: **TIMx_IRQHandler** với x là timer tương ứng
     
 Hàm kiểm tra cờ ngắt của line x tương ứng: **TIM_GetITStatus(TIMx,TIM_IT_Update)**

 Hàm xóa cờ ngắt của line x: **TIM_ClearITPendingBit(TIMx,TIM_IT_Update)**
   
     uint16_t count;
     void delay(int time){
	  count = 0; 
	  while(count < time)
      {}
    }

     void TIM2_IRQHandler(){
      if(TIM_GetITStatus(TIM2, TIM_IT_Update))
        {
		  count++;
		  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        }
    }

  


### **7.3.Ngắt truyền thông**

#### Ngắt UART

![Image](https://github.com/user-attachments/assets/9affbbd5-a18d-40d2-9c22-588a47d195df)

* Trước khi cho phép UART hoạt động, cần kích hoạt ngắt UART bằng cách gọi hàm **USART_ITConfig()**;

* Ở NVIC, ta cấu hình tương tự như ngắt ngoài EXTI, tuy nhiên NVIC_IRQChannel được đổi thành **USART_IRQn** để khớp với line ngắt timer.

* Hàm phục vụ ngắt UART được đặt tên : **USARTx_IRQHandler()**

   ◦ Kiểm tra ngắt 

   ◦ Nhận và lưu data từ USART1

   ◦ Kiểm tra cờ ngắt truyền,đảm bảo UART đang rỗi

   ◦ Truyền lại data vừa nhận được sang máy tính

   ◦ Xóa cờ ngắt,thoát khỏi hàm

* Hàm kiểm tra cờ ngắt : **USART_GetITStatus**

* Hàm kiểm tra trạng thái của quá trình truyền dữ liệu : **USART_GetFlagStatus(USART_InitTypeDef * USARTx,uint32_t USART_FLAG)**

   ```
   void UART1_IRQHandler(){
      uint8_t data = 0x00;
      if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET){
         while(!USART_GetFlagStatus(USART1,USART_FLAG_RXNE));
         data = USART_ReceiveData(USART1);
           if(USART_GetITStatus(USART1,USART_IT_TXE) != RESET){
              USART_SendData(USART1,data);
              while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET) ;
          }
      }
      USART_ClearITPendingBit(USART1,USART_IT_RXNE);  
   }
  ```
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

* Độ phân giải càng cao => càng nhiều mức sẽ cho được kết quả càng chính xác

![Image](https://github.com/user-attachments/assets/893e3052-892a-42b7-bcc0-6fafbabe90ea)


### **8.1.3.Tần số lấy mẫu**


* Quy định tần suất mà tín hiệu tương tự được lấy mẫu

* Tần số lấy mẫu càng cao => lấy được nhiều mẫu => kết quả càng chính xác



## **8.2.Sử dụng ADC trong STM32**

### **8.2.1.Đặc điểm**

* STM32F103C8 có 2 bộ ADC.Kết quả chuyển đổi được lưu trữ trong thanh ghi 16 bit

   ◦ Độ phân giải 12 bit

   ◦ Có các ngắt hỗ trợ

   ◦ Có thể điều khiển hoạt động ADC bằng xung Trigger

   ◦ Thời gian chuyển đổi nhanh

   ◦ Có bộ DMA hỗ trợ giúp tăng tốc độ xử lý

### **8.2.2.Các chế độ của ADC**

  #### **Regular Conversion**

  * **Single**: ADC chỉ đọc 1 kênh duy nhất và chỉ đọc khi nào được yêu cầu

  * **Single Continuous**: ADC chỉ đọc 1 kênh duy nhất,nhưng đọc dữ liệu nhiều lần liên tiếp (Có thể được biết đến như sử dụng DMA để đọc dữ liệu và ghi vào bộ nhớ)

  * **Scan:Multi-Channels:** Quét qua và đọc dữ liệu nhiều kênh,nhưng chỉ đọc khi nào được yêu cầu

  * **Scan: Continuous Multi-Channel Repeat:** Quét qua và đọc dữ liệu nhiều kênh,nhưng đọc liên tiếp nhiều lần giống như Single Continous. 


  ####   **Injected Conversion**
   
   * Trong trường hợp nhiều kênh hoạt động khi kênh có  mức độ ưu tiên cao hơn có thể tạo ra một **Injected Trigger**. 
   * Khi gặp **Injected Trigger** thì ngay lập tức kênh đang hoạt động bị ngưng lại để kênh được ưu tiên kia có thể hoạt động.


  
## **8.3.Cấu hình ADC**


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
 #### **Tham số cấu hình cho ADC**

* **ADC_Mode:** Cấu hình chế độ hoạt động cho ADC là đơn(Independent) hay đa,ngoài ra còn có các chế độ ADC chuyển đổi tuần tự các kênh (regularly) hay chuyển đổi khi có kích hoạt (Injected)

* **ADC_NbrOfChannel:** Số kênh ADC để cấu hình

* **ADC_ContinuousConvMode:** Cấu hình bộ ADC có chuyển đổi liên tục hay không , **ENABLE** để cấu hình ADC chuyển đổi liên tục, **DISABLE** thì ta phải gọi lại lệnh đọc ADC để bắt đầu quá trình chuyển đổi

* **ADC_ExternalTrigConv:** Enable để sử dụng tín hiệu Trigger

* **ADC_ScanConvMode:** Cấu hình chế độ quét ADC lần lượt từng kênh, ENABLE nếu sử dụng chế độ quét này

* **ADC_DataAlign:** Cấu hình căn lề cho data.Vì bộ ADC xuất ra giá trị 12bit, được lưu vào biến 16 hoặc 32 bit nên phải căn lề các bit về trái hoặc phải.

```
void ADC_Config(){
	ADC_InitTypeDef ADC_InitStruct;
	
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStruct.ADC_NbrOfChannel = 1;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	
	ADC_Init(ADC1, &ADC_InitStruct);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


```
## **8.4.Các hàm  thông dụng**

* Cần cấu hình thêm thời gian lấy mẫu ,thứ tự kênh ADC khi quét 
 `ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel,uint8_t Rank, uint8_t ADC_SampleTime)`:

   **Rank:** Ưu tiên của kênh ADC

   **SampleTime:** Thời gian lấy mẫu tín hiệu

* `ADC_SoftwareStartConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState):`

    Bắt đầu quá trình chuyển đổi

* `ADC_GetConversionValue(ADC_TypeDef* ADCx):`

    Đọc giá trị chuyển đổi được ở các kênh ADC tuần tự

* `ADC_GetDualModeConversionValue(void):`

   Trả về giá trị chuyển đổi cuối cùng của ADC1,ADC2 ở chế độ kép


## **8.5.Bộ lọc Kalman**

### **8.5.1.Định nghĩa**

* Bộ lọc Kalman được sử dụng để ước lượng trạng thái của hệ thống từ các dữ liệu quan sát bị nhiễu

### **8.5.2.Đặc điểm**

* **Giai đoạn dự đoán(Prediction):** 

Trong giai đoạn này,bộ lọc dự đoán trạng thái tiếp theo của hệ thống dựa trên mô hình toán học của hệ thống và dữ liệu từ bước trước

![Image](https://github.com/user-attachments/assets/f638e348-1525-4589-8793-7e51285f67b4)

* **Giai đoạn cập nhật(Correction):**

Sau khi nhận được dữ liệu quan sát mới từ cảm biến (có thể chứa nhiễu),bộ lọc sẽ cập nhật ước lượng trạng thái của hệ thống bằng cách kết hợp thông tin từ dự đoán trước đó với dữ liệu quan sát, đồng thời giảm thiểu ảnh hưởng của nhiễu.

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
