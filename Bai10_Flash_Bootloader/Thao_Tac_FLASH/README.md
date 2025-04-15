
# Thao tác với FLASH

## Tổng quan


* **Xóa một trang FLASH** : Xóa toàn bộ dữ liệu trong 1 trang FLASH (1KB) để ghi dữ liệu mới

* **Ghi nhiều byte** :
Ghi 1 mảng 14 byte vào FLASH

* **Đọc nhiều byte** :
Đọc 12 byte từ FLASH và hiển thị qua UART

* **Ghi 2 byte** :
Ghi 1 giá trị 16 bit (0xAAAA) vào 1 địa chỉ cụ thể FLASH


## Giải thích những đoạn quan trọng

### Biến toàn cục
 
 ```
uint8_t array[14]={5,2,3,1,4,9,6,5,10, 20, 11, 23, 25, 19};
uint8_t buffer[14];

 ```
◦ array[14]: Mảng 14 byte chứa dữ liệu sẽ ghi vào FLASH.

◦ buffer[14]: Bộ đệm để lưu dữ liệu đọc từ FLASH

### Hàm hỗ trợ UART

* void UART_SendChar(char c)

  ◦ Chờ cờ TXE để đảm bảo thanh ghi truyền rỗng

  ◦ Gửi ký tự

  ◦ Chờ cờ TC để đảm bảo đã truyền thành công

* void UART_SendString(const char * str)
 
  ◦ Lặp qua từng ký tự trong chuỗi cho đến khi gặp ký tự kết thúc (\0).

  ◦ Gọi UART_SendChar để gửi từng ký tự.
  
### Hàm thao tác với FLASH

* void Flash_Erase(uint32_t page_address)

```
void Flash_Erase(uint32_t page_address) {
    if (page_address < 0x08000000 || page_address > 0x0801FFFF) {
        UART_SendString("Error: Invalid page address!\r\n");
        return;
    }
    
    FLASH_Unlock();
    FLASH_Status status = FLASH_ErasePage(page_address);
    FLASH_Lock();
    
    if (status == FLASH_COMPLETE) {
        UART_SendString("Page erased successfully\r\n");
    } else {
        UART_SendString("Error: Page erased failed\r\n");
    }
}

```

◦ Kiểm tra địa chỉ :

 Đảm bảo địa chỉ nằm trong khoảng 0x08000000 đến 0x0801FFFF

 Nếu không hợp lệ ,in lỗi qua UART và thoát


◦ Thao tác Xóa :

Mở khóa FLASH bằng `FLASH_Unlock`

Xóa trang bằng `FLASH_ErasePage`

Khóa lại FLASH bằng `FLASH_Lock`


◦  Báo trạng thái :

Nếu thành công ,in "Page erased successfully".

Nếu thất bại, in lỗi.


* void Flash_WriteInt(uint32_t address,uint16_t value)

```
void Flash_WriteInt(uint32_t address, uint16_t value) {
    if (address < 0x08000000 || address > 0x0801FFFF || (address % 2 != 0)) {
        UART_SendString("Error: Invalid addres for half-word\r\n");
        return;
    }
    
    FLASH_Unlock();
    FLASH_Status status = FLASH_ProgramHalfWord(address, value);
    FLASH_Lock();
    
    if (status == FLASH_COMPLETE) {
        UART_SendString("Write half-word successfully\r\n");
    } else {
        UART_SendString("Error: Write half-word failed\r\n");
    }
}
```

◦ Kiểm tra địa chỉ:
  
  Đảm bảo địa chỉ hợp lệ và căn chỉnh(chia hết cho 2,vì ghi half-word 2 byte một)
 
  Sai,in lỗi và thoát

◦ Thao tác ghi:

  Mở khóa FLASH

  Ghi 2 byte bằng `FLASH_ProgramHalfWord`

  Khóa FLASH

◦  Báo trạng thái


* void FLASH_WriteNumByte(uint32_t address, uint8_t * data,uint32_t num)

```
void Flash_WriteNumByte(uint32_t address, uint8_t *data, uint32_t num) {
    if (address < 0x08000000 | address > 0x0801FFFF || (address + num - 1) > 0x0801FFFF) {
        UART_SendString("Error: Invalid addresss range\r\n");
        return;
    }
    
    FLASH_Unlock();
    for (uint32_t i = 0; i < num; i += 2) {
        uint16_t half_word = 0;
        half_word = data[i];
        if (i + 1 < num) {
            half_word |= (uint16_t)data[i + 1] << 8;
        } else {
            half_word |= 0xFF << 8;
        }
        
        FLASH_Status status = FLASH_ProgramHalfWord(address + i, half_word);
        if (status != FLASH_COMPLETE) {
            UART_SendString("Error: Write Byte failed \r\n");
            FLASH_Lock();
            return;
        }
    }
    FLASH_Lock();
    UART_SendString("Write bytes successfully\r\n");
}

```

◦  Kiểm tra địa chỉnh

◦  Thao tác ghi

   SPL chỉ hỗ trợ ghi 2 byte (half_word )nên hàm ghép 2 byte thành một half-word

   Byte đầu làm 8 bit thấp

   Byte thứ hai(nếu có) làm 8 bit cao

   Nếu số byte lẻ, byte cuối được điền 0xFF

◦ Ghi từng half-word bằng `FLASH_ProgramHalfWord.`

◦ Báo trạng thái


* void Flash_ReadNumByte(uint32_t address,uint32_t num)

```
void Flash_ReadNumByte(uint32_t address, uint32_t num) {
    if (address < 0x08000000 || address > 0x0801FFFF || (address + num - 1) > 0x0801FFFF) {
        UART_SendString("Error: Invalid address range\r\n");
        return;
    }
    
    char buffer[50];
    for(uint32_t i=0; i<num;i++){
        buffer[i] = *(uint8_t*)(address+i);
        sprintf(buffer,"0x%02X ",buffer[i]);
        UART_SendString(buffer);
    }
    UART_SendString("\r\n");
}
```
◦ Kiểm tra địa chỉnh

◦ Thao tác đọc
 
  Đọc từng byte bằng con trỏ *(uint8_t*)
  
  Dùng `sprintf` để định dạng mỗi byte thành hex

  Gửi dữ liệu qua UART

◦ Kết quả: In dòng "Data read: " kèm các byte dưới dạng hex.
