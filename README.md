# Automative_Embedded
## Bài 1:GPIO
### Thư viện SPL(Standard Peripherals Firmware Library)
Thư viện SPL là một tập hợp các hàm API  . Thay vì phải thao tác trực tiếp với các thanh ghi (registers) phức tạp, SPL cung cấp một lớp trừu tượng, cho phép cấu hình và điều khiển ngoại vi thông qua các cấu trúc dữ liệu (struct) và kiểu liệt kê (enum). 

### Cấu hình và sử dụng ngoại với thư viện SPL
* **CẤP CLOCK CHO NGOẠI VI**
  
Hàm `RCC_<bus>ClockCmd` giúp cung cấp clock cho ngoại vi nằm trên đường bus tương ứng trong tên hàm

  **Các đường bus là:  AHB,APB1,APB2**

  **Hàm này chứa 2 tham số:**  

 **Tham số đầu tiên:** Xác định ngoại vi muốn cấp/ngắt clock

`RCC_<bus>Periph_<tên ngoại vi>`

**Tham số thứ hai:** Xác định trạng thái cấp clock

Cấp Clock: Enable

Ngắt Clock: Disable

`RCC_APB2ClockCmd(RCC_APB2Periph_GPIOC, ENABLE);`

`RCC_APB1ClockCmd(RCC_APB1Periph_TIM2, DISABLE);`

`RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, DISABLE);`

* **CẤU HÌNH NGOẠI VI**
  
Khai báo một biến có tên là GPIO_InitStruct, và biến này có kiểu dữ liệu là GPIO_InitTypeDef. GPIO_InitStruct là một biến cấu trúc, có thể chứa các thông tin cấu hình cho GPIO.

`GPIO_InitTypeDef GPIO_InitStruct;`

Các tham số cấu hình cho GPIO được tổ chức trong 1 struct "GPIO_InitTypeDef" với các tham số sau:

**GPIO_Pin: Chân cần được cấu hình**

GPIO_Pin_<chân cần cấu hình>

**GPIO_Mode: Chế độ chân muốn cấu hình**

GPIO_Mode_<chế độ cần cấu hình>

`GPIO_Mode_AIN //Analog Input`

*Dùng để đo điện áp analog (tín hiệu tương tự). Không thể đọc giá trị logic (0/1). Vi điều khiển sẽ nhận giá trị điện áp.*

`GPIO_Mode_IN_FLOATING //Input bình thường`

*Cần có điện trở kéo lên/xuống bên ngoài để xác định mức logic khi không có tín hiệu*

`GPIO_Mode_IPD //Input có điện trở kéo xuống  `

*Mức logic mặc định là 0 (LOW) khi không có tín hiệu ngoài,khi có tín hiệu, mức logic chuyển sang 1 (HIGH).*

`GPIO_Mode_IPU  //Input có điện trở kéo lên`

*Mức logic mặc định là 1 (HIGH) khi không có tín hiệu ngoài,khi có tín hiệu, mức logic chuyển sang 0 (LOW)*

`GPIO_Mode_Out_OD //Output dạng open-drain`

*Chỉ có thể kéo chân xuống mức thấp (0), để lên mức cao (1) cần điện trở kéo lên bên ngoài, nhiều thiết bị cùng điều khiển 1 đường*

`GPIO_Mode_Out_PP //Output dạng push-pull`

*Có thể  kéo chân lên mức cao (1) hoặc xuống mức thấp (0).*

`GPIO_Mode_AF_OD //Chế độ ngoại vi khác dạng open-drain`

*Chân GPIO được điều khiển bởi một ngoại vi khác, ở chế độ open-drain*

`GPIO_Mode_AF_PP //Chế độ ngoại vi khác dạng push-pull`

*Chân GPIO được điều khiển bởi một ngoại vi khác, ở chế độ push-pull*

**GPIO_Speed: Tốc độ chân muốn cấu hình**

GPIO_Speed_<tốc độ>

**Gán các giá trị muốn cấu hình vào thanh ghi**

 GPIO_Init(Tham số thứ nhất,tham số thứ hai)

 **Tham số thứ nhất:**   Tên ngoại vi muốn cấu hình                                                         
 **Tham số thứ hai:**  Tham số thứ hai là con trỏ trỏ đến địa chỉ của struct"GPIO_InitTypeDef"                                          
`GPIO_Init(GPIOC, &GPIO_InitStruct);`

* **CÁC HÀM ĐIỀU KHIỂN NGOẠI VI**

    `uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin); ` 

  \\\Đọc giá trị 1 bit trong cổng GPIO được cấu hình là INPUT (IDR), có thể đọc nhiều pin nhờ toán tử OR

    `uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);`                                
     \\\Đọc giá trị nguyên cổng GPIO được cấu hình là INPUT (IDR)

    `uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);`
  
     \\\Đọc giá trị 1 bit trong cổng GPIO được cấu hình là OUTPUT (ODR), có thể đọc nhiều pin nhờ toán tử OR

    `uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx); `                               
     \\\Đọc giá trị nguyên cổng GPIO được cấu hình là OUTPUT (ODR)

    `void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin); `                       
     \\\Cho giá trị 1 bit trong cổng GPIO = 1, có thể ghi nhiều pin nhờ toán tử OR

    `void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);  `                    
    
     \\\Cho giá trị 1 bit trong cổng GPIO = 0, có thể ghi nhiều pin nhờ toán tử OR

    `void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);  `  
     \\\Ghi giá trị "BitVal" vào 1 bit trong cổng GPIO, có thể ghi nhiều pin nhờ toán tử OR

    `void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);  `                        
     \\\Ghi giá trị "PortVal" vào nguyên cổng GPIO
