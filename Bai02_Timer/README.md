
# **Bài tập**

## **Bài 1: Sử dụng Timer 2 để tạo thời gian trễ và điều khiển LED**
```
Mục tiêu: Cấu hình Timer 2 tạo ngắt mỗi 1 giây, điều khiển LED tại PA1 nhấp nháy với chu kỳ 2 giây (1 giây bật, 1 giây tắt).
```
## **Bài 2: Tạo xung PWM**
```
Mục tiêu: Cấu hình Timer 3 để tạo PWM tại PA6 (TIM3 kênh 1) với tần số 1 kHz, duty cycle 50%.
```
## **Bài 3: Thay đổi duty cycle PWM**
```
Mục tiêu: Sử dụng hai nút nhấn (PB0 tăng, PB1 giảm) để thay đổi duty cycle PWM tại PA6 từ 0% đến 100% (bước 10%), điều khiển độ sáng LED.
```
## **Bài 4: Đếm thời gian qua UART**
```
Sử dụng Timer 2 để đếm thời gian, hiển thị số giây trôi qua qua UART1.
```
## **Bài 5: PWM điều khiển bằng biến trở**
```
Tạo PWM tại PA6 (Timer 3), dùng biến trở (PA0-ADC) để thay đổi tần số (500 Hz đến 2 kHz) và duty cycle (0-100%).
```
## **Bài 6: Tạo âm thanh bằng Timer**
```
Mục tiêu: Sử dụng Timer 3 tạo xung vuông tại PA6 với tần số 500 Hz, 1 kHz, 2 kHz để tạo âm thanh qua còi.
```
## **Bài 7: Đo tín hiệu PWM đầu vào**
```
Mục tiêu: Sử dụng Timer 2 ở chế độ PWM Input tại PA0 để đo tần số và duty cycle của tín hiệu PWM ngoài, hiển thị qua UART1.
```
## **Bài 8: Đếm xung từ encoder**
```
Mục tiêu: Sử dụng Timer 4 ở chế độ External Clock tại PA0 (tín hiệu từ encoder), hiển thị giá trị đếm qua UART1.
```
## **Bài 9: Đo độ rộng xung**
```
Mục tiêu: Sử dụng Timer 2 ở chế độ Input Capture tại PA0 để đo độ rộng xung của tín hiệu đầu vào, hiển thị qua UART1
```
## **Bài 10: Điều khiển servo**
```
Mục tiêu: Sử dụng Timer 3 tạo PWM tại PA6 (chu kỳ 20 ms, độ rộng xung 1-2 ms) để điều khiển servo SG90 từ 0 đến 180 độ.
```
## **Bài 11: Bộ đếm thời gian thực (RTC) đơn giản**
```
Mục tiêu: Sử dụng Timer 2 tạo RTC, hiển thị giờ/phút/giây qua UART1.
```
## **Bài 12: Điều khiển tốc độ động cơ DC**
```
Mục tiêu: Sử dụng Timer 3 (PWM tại PA6) để điều khiển tốc độ động cơ DC, Timer 4 (PA0) đếm xung encoder để đo tốc độ thực tế, điều chỉnh PWM để đạt tốc độ mong muốn
```
## **Bài 13: Hiển thị thời gian và tốc độ trên OLED**
```
Mục tiêu: Sử dụng Timer 2 (RTC) và Timer 4 (encoder) để hiển thị thời gian và tốc độ động cơ trên OLED SSD1306, trạng thái qua UART1.
```
## **Bài 14: Gửi dữ liệu PWM qua Wi-Fi**
```
Mục tiêu: Sử dụng Timer 3 (PWM tại PA6) điều khiển servo, gửi góc quay qua ESP8266 (UART2), trạng thái qua UART1.
```
## **Bài 15: Lưu dữ liệu tốc độ vào thẻ SD**
```
Mục tiêu: Sử dụng Timer 4 (encoder) và Timer 2 (RTC) để lưu tốc độ động cơ và thời gian vào thẻ SD qua SPI1, trạng thái qua UART1
```