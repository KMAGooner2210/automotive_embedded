
# **Bài tập**

## **Bài 1: Điều khiển nhiều LED theo thứ tự**
```
Mục tiêu: Sử dụng 4 LED tại PA1-PA4, điều khiển sáng tuần tự (LED1 → LED2 → LED3 → LED4 → tất cả tắt) với chu kỳ 0.5 giây mỗi bước
```
## **Bài 2: Điều khiển độ sáng LED bằng nút nhấn**
```
Mục tiêu: Sử dụng 2 nút nhấn tại PB1 (tăng) và PB2 (giảm) để thay đổi độ sáng LED tại PA1 bằng cách điều chỉnh thời gian sáng/tắt (mô phỏng PWM).
```
## **Bài 3: Chuyển đổi chế độ nhấp nháy LED bằng nút nhấn**
```
Mục tiêu: Sử dụng nút nhấn tại PB1 để thay đổi chế độ nhấp nháy LED tại PA1: lần 1 (1 giây), lần 2 (0.5 giây), lần 3 (0.25 giây), lần 4 (tắt).
```
## **Bài 4: Hiệu ứng LED chạy**
```
Quét 8 LED tại PA0-PA7, tạo hiệu ứng sáng lần lượt từ trái sang phải, mỗi LED sáng 0.2 giây.
```
## **Bài 5: Quét nhiều nút nhấn**
```
Sử dụng 4 nút nhấn tại PB0-PB3 để điều khiển 4 LED tương ứng tại PA0-PA3 (nhấn nút nào, bật LED tương ứng).
```
## **Bài 6: Hiển thị số trên LED 7 thanh**
```
Mục tiêu: Sử dụng LED 7 thanh (PA0-PA6) để hiển thị số 0-9, nút nhấn PB0 (tăng), PB1 (giảm) để thay đổi số.
```
## **Bài 7: Hiệu ứng fading LED**
```
Mục tiêu: Điều khiển LED tại PA1 sáng dần và tắt dần (fading) bằng cách thay đổi thời gian sáng/tắt.
```
## **Bài 8: Điều khiển relay bằng nút nhấn**
```
Mục tiêu: Sử dụng nút nhấn tại PB0 để điều khiển relay (PA0, qua transistor BC547). Nhấn nút bật relay, thả thì tắt.
```
## **Bài 9: Ma trận LED 8x8**
```
Mục tiêu: Sử dụng ma trận LED 8x8 (hàng: PA0-PA7, cột: PB0-PB7) để hiển thị ký tự "A".
```
## **Bài 10: Hiển thị trạng thái LED trên OLED**
```
Mục tiêu: Điều khiển 4 LED (PA1-PA4) như Bài 1, hiển thị trạng thái LED và số lần nhấn nút (PB0) trên OLED SSD1306, gửi trạng thái qua UART1
```
## **Bài 11: Gửi trạng thái relay qua Wi-Fi**
```
Mục tiêu: Điều khiển relay (PA0) bằng nút nhấn (PB0) như Bài 8, gửi trạng thái relay qua ESP8266 (UART2), hiển thị trạng thái qua UART1.
```

