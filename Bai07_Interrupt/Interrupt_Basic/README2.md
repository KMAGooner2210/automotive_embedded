
# Automative_Embedded
## Bài 2: Interrupt 


#### **Khái niệm**

![Image](https://github.com/user-attachments/assets/eb1762a6-057e-4212-91cd-7d216830df0d)


* **Ngắt** là 1 sự kiện khẩn cấp xảy ra trong hay ngoài vi điều khiển.Nó yêu cầu MCU phải **dừng chương trình chính** và **thực thi chương trình ngắt**



#### **Interrupt Service Routine (Trình phục vụ ngắt)**

![Image](https://github.com/user-attachments/assets/90c8c28e-edea-4755-b2b3-8c84be788a61)


 * Mỗi ngắt có 1 trình phục vụ riêng
 * Trình phục vụ ngắt là 1 đoạn chương trình được thực hiện khi ngắt xảy ra
 * Địa chỉ trong bộ nhớ của ISR được gọi là **vector ngắt**



#### **Vector Interrupt Table (Bảng vector ngắt)**

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





#### **NVIC (Nested Vectored Interrupt Controller)**

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





#### **Các loại ngắt**


* **Ngắt ngoài**

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


## Acknowledgements

 - [Awesome Readme Templates](https://awesomeopensource.com/project/elangosundar/awesome-README-templates)
 - [Awesome README](https://github.com/matiassingers/awesome-readme)
 - [How to write a Good readme](https://bulldogjob.com/news/449-how-to-write-a-good-readme-for-your-github-project)

## Color Reference

| Color             | Hex                                                                |
| ----------------- | ------------------------------------------------------------------ |
| Example Color | ![#0a192f](https://via.placeholder.com/10/0a192f?text=+) #0a192f |
| Example Color | ![#f8f8f8](https://via.placeholder.com/10/f8f8f8?text=+) #f8f8f8 |
| Example Color | ![#00b48a](https://via.placeholder.com/10/00b48a?text=+) #00b48a |
| Example Color | ![#00d1a0](https://via.placeholder.com/10/00b48a?text=+) #00d1a0 |

