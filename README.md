
# AUTOMOTIVE_EMBEDDED
## BÀI 1: GPIO (General Purpose Input Output)
![Image](https://github.com/user-attachments/assets/f275f738-034e-41e5-849a-892cb47e31d6)

## **Cấp Clock cho ngoại vi**

* **Module RCC** cung cấp các hàm để cấu hình xung clock

`RCC_APB1PeriphClockCmd`

`RCC_APB2PeriphClockCmd` 

`RCC_AHBPeriphClockCmd`
 
* **Nhận 2 tham số**:

 ◦ Ngoại vi Clock

 ◦ ENABLE/DISABLE

## **Cấu hình ngoại vi**


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

## **Sử dụng ngoại vi**


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









## **Pull-Up vs Pull-Down ???**


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




## **Các chế độ input khác**

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
    

## **Các chế độ output**

* **Output Push-Pull:**

    ◦ **Mục đích:**  Xuất tín hiệu số kéo VDD (HIGH) và kéo xuống GND (LOW) 1 cách chủ động


    ◦ **Sử dụng:** Điều khiển relay,led,...



* **Output Open-Drain (Đầu ra hở mạch):**

    ◦ **Mục đích:**  Xuất tín hiệu số kéo chỉ có khả năng kéo GND (LOW) 1 cách chủ động.Kéo lên HIGH cần có điện trở kéo bên ngoài

    ◦ **Cấu hình điện:** Khi xuất mức HIGH ,transistor tắt,GPIO thả nổi và cần điện trở kéo lên bên ngoài

    ◦ **Sử dụng:** I2C,...   



## **Các chế độ Alternate Function**


* **AF-Push Pull:**

    ◦ **Mục đích:** Sử dụng chân GPIO để xuất tín hiệu từ các peripheral

    ◦ **Sử dụng:** Xuất tín hiệu PWM từ Timer, clock từ SPI,...   


* **AF-Open Drain:**

    ◦ **Mục đích:** Sử dụng cho các giao thức yêu cầu đường truyền 2 chiều

    ◦ **Sử dụng:** GPIO cho giao tiếp I2C (SCL,SDA) ,UART (truyền nhận)


  

## BÀI 2: INTERRUPT (Phần 1)


## **Khái niệm**

![Image](https://github.com/user-attachments/assets/eb1762a6-057e-4212-91cd-7d216830df0d)


* **Ngắt** là 1 sự kiện khẩn cấp xảy ra trong hay ngoài vi điều khiển.Nó yêu cầu MCU phải **dừng chương trình chính** và **thực thi chương trình ngắt**



## **Interrupt Service Routine (Trình phục vụ ngắt)**

![Image](https://github.com/user-attachments/assets/90c8c28e-edea-4755-b2b3-8c84be788a61)


 * Mỗi ngắt có 1 trình phục vụ riêng
 * Trình phục vụ ngắt là 1 đoạn chương trình được thực hiện khi ngắt xảy ra
 * Địa chỉ trong bộ nhớ của ISR được gọi là **vector ngắt**



## **Vector Interrupt Table (Bảng vector ngắt)**

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

  
```c
   Quy trình bắt đầu khi một sự kiện ngắt xảy ra.

   Sự kiện này có thể đến từ nhiều nguồn khác nhau, ví dụ:

Ngắt ngoại vi: Một module ngoại vi như UART, Timer, ADC, GPIO tạo ra ngắt khi hoàn thành một tác vụ hoặc có một sự kiện cụ thể (ví dụ: UART nhận dữ liệu, Timer tràn, ADC chuyển đổi xong, GPIO pin thay đổi trạng thái).

Ngắt nội bộ: Các ngắt do bộ vi điều khiển tạo ra, ví dụ: SysTick timer ngắt định kỳ, Software Interrupt (SWI), PendSV (Pendable Service Call).

Ngắt lỗi: Các lỗi phần cứng hoặc phần mềm như HardFault, Memory Management Fault.
```
  

   
    ◦NVIC xác định vector number

   
    ◦Tra cứu VIT bằng cách sử dụng vector number làm offset để tra cứu bảng 
    
   ```c
Base_Address of VIT + (n * 4).

      Base_Address of VIT: Đây là địa chỉ bộ nhớ bắt đầu của Vector Interrupt Table. Như đã thảo luận trước đó, địa chỉ này thường cố định và nằm ở đầu bộ nhớ Flash (ví dụ: 0x00000000).

      n (vector number): Là số vector mà NVIC đã xác định cho ngắt đang xảy ra.

      4: Là kích thước của mỗi mục (vector) trong VIT, tính bằng byte. Trong kiến trúc 32-bit, mỗi địa chỉ bộ nhớ thường là 32-bit (4 byte).

      n * 4: Tính toán offset (độ lệch) từ địa chỉ bắt đầu của VIT. Vector number n được nhân với 4 để tính ra độ lệch byte tương ứng trong bảng VIT.

Base_Address of VIT + (n * 4): Kết quả là địa chỉ bộ nhớ trong VIT, nơi chứa địa chỉ của ISR tương ứng với vector number n.

  ```


   ◦Lấy địa chỉ ISR
   
     Giá trị tra cứu được trong VIT chính là địa chỉ của bộ nhớ hàm ISR tương ứng


   ◦Nhảy đến ISR
   
   ◦Kết thúc ISR





## **NVIC (Nested Vectored Interrupt Controller)**

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
 
  ◦ **Priority Group:**  xác định cách phân chia bit giữa Preemption Priority và Subpriority. Sử dụng hàm **NVIC_PriorityGroupConfig(uint32_t PriorityGroup)** để chọn priority group cho NVIC

  ◦ **NVIC_IRQChannel:** Xác định mã của kênh ngắt cần được cấu hình

  ◦ **NVIC_IRQChannelPreemptionPriority:** Xác định mức độ ưu tiên Preemption Priority cho kênh ngắt.

  ◦ **NVIC_IRQChannelSubPriority:** Xác định mức độ ưu tiên phụ Subpriority cho kênh ngắt.

  ◦ **NVIC_IRQChannelSubPriority:** Cho phép ngắt


```c
NVIC_InitTypeDef NVICInitStruct;
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
NVICInitStruct.NVIC_IRQChannel = EXTI0_IRQn;
NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
NVICInitStruct.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVICInitStruct);
```



## BÀI 2: INTERRUPT (PHẦN 2) 

## **Ngắt ngoài**


* **Sơ đồ**

![Image](https://github.com/user-attachments/assets/b8531dc8-d8a1-4fea-b10b-90365810da53)


  ◦ Để sử dụng được ngắt ngoài, ngoài bật clock cho GPIO tương ứng cần bật thêm clock cho **AFIO**.

```c
  void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}
```
 ◦ Ngắt ngoài của chip STM32F103 bao gồm có 16 line riêng biệt
 ```c
 	Line0 sẽ chung cho tất cả chân Px0 ở tất cả các Port, với x là tên của Port A, B…
     Line0 nếu chúng ta đã chọn chân PA0 (chân 0 ở port A) làm chân ngắt thì tất cả các chân 0 ở các Port khác không được khai báo làm chân ngắt ngoài nữa
```
 

 * **Cấu hình ngắt ngoài**

    ◦ Cấu hình chân ngắt ngoài là Input. 

    ◦  Có thể cấu hình thêm trở kéo lên/xuống tùy theo cạnh ngắt được sử dụng.

   ```c
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


   ```c
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

    ```c
    NVIC_InitTypeDef NVICInitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVICInitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVICInitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVICInitStruct);
    ```      



◦ Các hàm quan trọng trong EXTI:

   Ngắt trên từng line có hàm phục riêng của từng line. Có tên cố định: **EXTIx_IRQHandler()** (x là line ngắt tương ứng).
     
   Hàm **EXTI_GetITStatus(EXTI_Linex)**, Kiểm tra cờ ngắt của line x tương ứng. 

   Hàm **EXTI_ClearITPendingBit(EXTI_Linex)**: Xóa cờ ngắt ở line x.


◦ Ngắt ngoài sẽ được thực hiện theo:

   Kiểm tra ngắt đến từ line nào, có đúng là line cần thực thi hay không?

   Thực hiện các lệnh, các hàm.

   Xóa cờ ngắt ở line.

```c
void EXTI0_IRQHandler()
{	
if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{

	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}
```


## **Ngắt Timer**

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

     
     ```c
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

Hàm xóa cờ ngắt của line x:
  **TIM_ClearITPendingBit(TIMx,TIM_IT_Update)**
 
     

   
     uint16_t count;
     void delay(int time){

	  count = 0; 
	  while(count < time)
     
      {

      }

    }

     void TIM2_IRQHandler(){

      if(TIM_GetITStatus(TIM2, TIM_IT_Update))
        {
		  count++;
		  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        }

    }

  


## **Ngắt truyền thông**

### Ngắt UART

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

   ```c
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



## BÀI 3: CÁC CHUẨN GIAO TIẾP CƠ BẢN 

## **SPI**

### **Đặc điểm**
* Chuẩn giao tiếp nối tiếp,đồng bộ
* Hoạt động ở chế độ song công
* Sử dụng 4 dây giao tiếp

### **Sơ đồ chân**
![Image](https://github.com/user-attachments/assets/9e55733b-bf9f-4d37-912e-84e5faae3086)


* **SCK (Serial Clock)**: Thiết bị Master tạo xung tín hiệu SCK và cung cấp cho Slave

* **MISO (Master Input Slave Output)**: Tín hiệu tạo bởi thiết bị Slave và nhận bởi thiết bị Master.

* **MOSI (Master Output Slave Input)**: Tín hiệu tạo bởi thiết bị Master và nhận bởi thiết bị Slave.

* **SS (Slave Select) / CS (Chip Select)**: Chọn thiết bị Slave cụ thể để giao tiếp. Để chọn Slave giao tiếp thiết bị Master chủ động kéo đường SS tương ứng xuống mức 0 (Low).


### **Quá trình truyền nhận**  

* Master kéo chân SS của chân Slave muốn giao tiếp xuống mức 0 để báo hiệu muốn truyền nhận

* Clock sẽ được cấp bởi master, tùy vào chế độ được cài, với mỗi xung clock,  1 bit sẽ được truyền từ master đến slave và slave cũng truyền 1 bit cho master.

* Các thanh ghi cập nhật giá trị và dịch 1 bit.

* Lặp lại quá trình trên đến khi truyền xong 8 bit trong thanh ghi.

### **Các chế độ hoạt động**

* Có 4 chế độ hoạt động phụ thuộc **Clock Polarity (CPOL)** và **Clock Phase (CPHA)**.

* CPOL:

  CPOL = 0: Xung clock ban đầu ở mức 0.

  CPOL = 1: Xung clock ban đầu ở mức 1.

* CPHA:

  CPHA = 0: Truyền bit trước rồi mới cấp xung.
  
  CPHA = 1: Cấp xung trước rồi mới truyền bit.

  ![Image](https://github.com/user-attachments/assets/7035fcb0-fd0e-4d24-976b-0a56d80a1207)



## **I2C**

### **Đặc điểm**

* Chuẩn giao tiếp nối tiếp,đồng bộ
* Hoạt động ở chế độ bán song công
* Sử dụng 2 dây giao tiếp

### **Sơ đồ chân**

![Image](https://github.com/user-attachments/assets/34dd609d-2f2e-421d-b416-0cabcbf9670a)


* **SCL (Serial Clock)**: Tạo xung tín hiệu để đồng bộ việc truyền/nhận dữ liệu với các Slave.

* **SDA (Serial Data)**: Chân chứa dữ liệu được truyền đi


### **Quá trình truyền nhận**

* Start: Điều kiện: Chân SDA xuống mức 0 trước chân SCL.
* Truyền các bit địa chỉ để tìm Slave muốn giao tiếp.

* Bit R/W: Master gửi dữ liệu đi ứng với bit '0', nhận dữ liệu ứng với bit '1'.

* ACK: Chờ phản hồi, '0' là nhận và '1' là không nhận. Nếu không có Slave nào phản hồi, dùng Timer để thoát ra.

* Sau khi chọn được Slave để giao tiếp, bắt đầu truyền các bit dữ liệu đến Slave.
* Tương tự cũng có ACK để chờ phản hồi.
* Stop: Điều kiện: Chân SDA lên mức 1 sau chân SCL.



## **UART**

### **Đặc điểm**

* Chuẩn giao tiếp nối tiếp
* Không đồng bộ
* Hoạt động ở chế độ song công
* Sử dụng 2 dây giao tiếp
* Chỉ 2 thiết bị giao tiếp

### **Sơ đồ chân**

![Image](https://github.com/user-attachments/assets/5d657816-56e4-4871-8c40-7ce476b283c2)

**Tx (Transmit)**: Chân truyền dữ liệu 

**Rx (Receive)**: Chân nhận dữ liệu

### **Quá trình truyền nhận**

![Image](https://github.com/user-attachments/assets/e345c226-4ada-4991-963b-1dcb7e0ae24c)

* Start: 1 bit.

* Bit dữ liệu: 5 đến 9 bit.

* Bit chẵn lẻ:

    Quy luật chẵn: Thêm một bit '0' hoặc '1' để số bit '1' là số chẵn.
    Quy luật lẻ: Thêm một bit '0' hoặc '1' để số bit '1' là số lẻ.
* Stop: 1 đến 2 bit.


