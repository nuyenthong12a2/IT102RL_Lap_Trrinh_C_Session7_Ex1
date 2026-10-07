# Bài tập 01: Dò vết lỗi truy xuất thuộc tính struct thiết bị thông minh

## 1. Phân tích lỗi

### Dòng lệnh sai

```c
heater.power_watt = heater.voltage * heater.current;
```

Dòng này nằm **trước** khối kiểm tra quá tải, và sau khi ngắt điện không có dòng nào cập nhật lại `power_watt`.

### Truy vết luồng dữ liệu (trace) của mã gốc

| Bước | Lệnh | voltage | current | is_active | is_overloaded | power_watt |
|------|------|---------|---------|-----------|---------------|------------|
| 1 | Gán giá trị ban đầu | 220.0 | 35.0 | 1 | 0 | (chưa gán) |
| 2 | `power_watt = voltage * current` | 220.0 | 35.0 | 1 | 0 | **7700.0** |
| 3 | `if (current > 30.0f)` đúng, gán `is_overloaded = 1`, `is_active = 0` | 220.0 | 35.0 | **0** | 1 | **7700.0** |
| 4 | In kết quả | | | TAT | CO | **7700.00 W** |

### Nguyên nhân kỹ thuật

- Ở bước 2, thiết bị còn đang BẬT nên công suất được tính ra 7700 W là hợp lệ tại thời điểm đó.
- Ở bước 3, thiết bị bị ngắt (`is_active = 0`) nhưng thuộc tính `power_watt` vẫn giữ nguyên giá trị cũ vì không có lệnh nào ghi lại nó. Các member của struct độc lập với nhau, đổi `is_active` không tự làm đổi `power_watt`.
- Kết quả là trạng thái (TAT) và công suất (7700 W) mâu thuẫn, vi phạm quy tắc: thiết bị TẮT thì `power_watt` phải bằng 0.0 W.
- Mã gốc còn một lỗi tiềm ẩn: nó tính `P = V * I` mà không xét `is_active`, nên thiết bị đang TẮT nhưng có dòng rò vẫn bị tính ra công suất khác 0 (xem Test Case 2).

### Cách sửa

Đổi thứ tự xử lý: kiểm tra quá tải và cập nhật trạng thái trước, sau đó mới tính công suất dựa trên trạng thái cuối cùng:

1. Nếu thiết bị đang BẬT và `current > 30.0` thì đặt `is_overloaded = 1` và `is_active = 0`.
2. Nếu `is_active == 1` thì `power_watt = voltage * current`, ngược lại `power_watt = 0.0`.

## 2. Bảng Test Cases

| Trường hợp kiểm thử | Dữ liệu đầu vào | Kết quả sai thực tế (mã gốc) | Kết quả đúng mong đợi (mã đã sửa) |
|---|---|---|---|
| TC1: Quá tải, tự ngắt điện | voltage = 220.0 V, current = 35.0 A, is_active = 1 | Trạng thái: TAT; Công suất: **7700.00 W**; Cảnh báo: CO (DA NGAT DIEN) | Trạng thái: TAT; Công suất: **0.00 W**; Cảnh báo: CO (DA NGAT DIEN) |
| TC2: Thiết bị tắt có dòng rò | voltage = 220.0 V, current = 2.0 A (dòng rò), is_active = 0 | Trạng thái: TAT; Công suất: **440.00 W**; Cảnh báo: KHONG | Trạng thái: TAT; Công suất: **0.00 W**; Cảnh báo: KHONG |

Kiểm tra thêm (mã đã sửa vẫn đúng với trường hợp bình thường): voltage = 220.0 V, current = 20.0 A, is_active = 1 cho kết quả BAT, 4400.00 W, KHONG.

## 3. Biên dịch và chạy

```
gcc -Wall -Wextra -std=c11 main.c -o main
./main
```

Kết quả với dữ liệu của đề (35.0 A):

```
=== HETHONG GIAM SAT THIET BI SMART HOME ===
Ma thiet bi: 101
Trang thai hoat dong: TAT
Cong suat tieu thu: 0.00 W
Canh bao qua tai (>30A): CO (DA NGAT DIEN)
```