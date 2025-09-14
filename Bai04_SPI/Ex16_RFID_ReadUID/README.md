
# THANH GHI RC522 VÀ CHỨC NĂNG CÁC LỆNH RC522,PCD VÀ MIFARE

## **THANH GHI RC522**

### **RC522_CommandReg (0x01)**

<img width="721" height="434" alt="Image" src="https://github.com/user-attachments/assets/a179a280-bb48-47e0-b341-fb8f534b873a" />

* **Chức năng:** Điều khiển lệnh thực thi của module RC522

* **Nội dung:** Thanh ghi chứa mã lệnh để module thực hiện các tác vụ như Reset, Transceive, Authenticate


### **RC522_ComIEnReg (0x02)**

<img width="645" height="732" alt="Image" src="https://github.com/user-attachments/assets/25de94f2-8216-4fc4-a4f3-e05d646543f8" />

* **Chức năng:** Điều khiển lệnh thực thi của module RC522

* **Nội dung:** Thanh ghi chứa mã lệnh để module thực hiện các tác vụ như Reset, Transceive, Authenticate

### **RC522_DivIEnReg (0x03)**

<img width="576" height="340" alt="Image" src="https://github.com/user-attachments/assets/16ab8153-b9fe-40e6-93e3-37bbe9d87525" />

* **Chức năng:** Cấu hình ngắt cho các sự kiện liên quan đến bộ chia tần (Divider Interrupt Enable).

* **Nội dung:** Kích hoạt các ngắt liên quan đến bộ chia tần số hoặc các sự kiện khác trong quá trình xử lý tín hiệu.

### **RC522_ComIrqReg (0x04)**

<img width="648" height="735" alt="Image" src="https://github.com/user-attachments/assets/dcd03aa9-96ef-4036-88bf-b0e4c361a5eb" />

* **Chức năng:** Thanh ghi trạng thái ngắt của bộ chia tần (Divider Interrupt Request).

* **Nội dung:** Lưu trạng thái các ngắt liên quan đến bộ chia tần, chẳng hạn như ngắt khi bộ đếm đạt giá trị nhất định.

### **RC522_ErrorReg (0x06)**

<img width="603" height="599" alt="Image" src="https://github.com/user-attachments/assets/3616c52c-dbf7-455a-8c85-a64fb9c7c5f0" />

* **Chức năng:** Lưu trạng thái lỗi của module.

* **Nội dung:** Báo cáo các lỗi như lỗi giao thức, lỗi va chạm bit, hoặc lỗi kiểm tra CRC.

### **RC522_Status1Reg (0x07)**

<img width="568" height="609" alt="Image" src="https://github.com/user-attachments/assets/4445a8b2-ac5a-4ad4-8494-b05eca9da4e7" />

* **Chức năng:** Lưu trạng thái hiện tại của module (phần 1)

* **Nội dung:** Cung cấp thông tin về trạng thái hoạt động, ví dụ như trạng thái FIFO hoặc trạng thái giao tiếp RF.

### **RC522_Status2Reg (0x08)**

<img width="568" height="604" alt="Image" src="https://github.com/user-attachments/assets/66d6bda4-2a48-44dc-965e-13420e914930" />

* **Chức năng:** Lưu trạng thái hiện tại của module (phần 2).

* **Nội dung:** Bao gồm các thông tin như trạng thái modem hoặc trạng thái mã hóa.

### **RC522_FIFODataReg (0x09)**

<img width="568" height="609" alt="Image" src="https://github.com/user-attachments/assets/4445a8b2-ac5a-4ad4-8494-b05eca9da4e7" />

* **Chức năng:** Thanh ghi dữ liệu FIFO.

* **Nội dung:** Dùng để ghi hoặc đọc dữ liệu vào/ra bộ đệm FIFO, nơi lưu trữ dữ liệu gửi/nhận trong quá trình giao tiếp với thẻ RFID.

### **RC522_FIFOLevelReg (0x0A)**

<img width="562" height="257" alt="Image" src="https://github.com/user-attachments/assets/f2f99b36-fae7-4c01-92d1-2d772d8d1262" />

* **Chức năng:** Hiển thị số byte hiện có trong FIFO.

* **Nội dung:** Cho biết mức độ đầy của bộ đệm FIFO (số byte đang lưu trữ).

### **RC522_ControlReg (0x0C)**

<img width="542" height="291" alt="Image" src="https://github.com/user-attachments/assets/8f5802c5-2915-4169-8ffb-9fa97a795cd5" />

* **Chức năng:** Điều khiển các chức năng bổ sung của module.

* **Nội dung:** Lưu trữ các bit điều khiển, ví dụ như số bit cuối cùng của byte cuối cùng trong FIFO.

### **RC522_BitFramingReg (0x0D)**

<img width="516" height="406" alt="Image" src="https://github.com/user-attachments/assets/f7d0955c-0a2a-4fd8-822e-7bbcc9042e22" />

* **Chức năng:** Cấu hình định dạng khung bit (bit framing).

* **Nội dung:** Điều chỉnh số bit cần gửi/nhận trong giao tiếp, thường dùng trong lệnh Transceive để bắt đầu truyền dữ liệu.

### **RC522_ModeReg (0x11)**

<img width="521" height="472" alt="Image" src="https://github.com/user-attachments/assets/360620ce-8a6f-4c10-9ca4-99abec5343a2" />

* **Chức năng:** Cấu hình chế độ hoạt động của module.

* **Nội dung:** Xác định các thông số như chế độ CRC, polar của tín hiệu RF, hoặc các chế độ giao tiếp khác

### **RC522_TxModeReg (0x12)**

<img width="538" height="645" alt="Image" src="https://github.com/user-attachments/assets/422a36ff-044f-47d6-b44f-fa443a2f27d6" />

* **Chức năng:** Cấu hình chế độ truyền (transmit).

* **Nội dung:** Thiết lập tốc độ truyền, kiểu điều chế, hoặc các thông số liên quan đến truyền dữ liệu qua RF.

### **RC522_RxModeReg (0x13)**

<img width="547" height="740" alt="Image" src="https://github.com/user-attachments/assets/454bf67f-17ae-43be-88d5-701c021ba0fd" />

* **Chức năng:** Cấu hình chế độ nhận (receive).

* **Nội dung:** Thiết lập tốc độ nhận, kiểu giải điều chế, hoặc các thông số liên quan đến nhận dữ liệu qua RF.

### **RC522_TxControlReg (0x14)**

<img width="504" height="318" alt="Image" src="https://github.com/user-attachments/assets/9b3d242b-7147-4c14-84f9-a3d1000c5d7a" />

* **Chức năng:** Điều khiển mạch truyền RF.

* **Nội dung:** Bật/tắt antenna hoặc điều chỉnh các thông số của mạch truyền, ví dụ như bật antenna bằng cách đặt bit 0x03.