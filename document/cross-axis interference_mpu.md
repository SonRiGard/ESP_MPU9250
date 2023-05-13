**Lỗi khi tính góc bằng bộ lọc Madgwick với mpu6050 và gy271**

1. Vấn đề

![](Aspose.Words.7c512fcb-68aa-4263-a80e-7884298dbaf1.001.png)

Biểu đồ biểu diễn sai số roll và pitch khi quay góc yall

Chú thích biểu đồ:	

-Xanh lá góc yaw sau khi qua madgwick filter 

-Đỏ góc roll

-Vàng góc pitch 


Nguyên nhân

Lỗi góc pitch và roll thay đổi khi quay mpu6050 theo trục z (góc yaw) có thể được giải thích bằng hiện tượng gọi là "crosstalk" hoặc "cross-axis interference". Đây là một vấn đề phổ biến xảy ra trong các cảm biến IMU (Inertial Measurement Unit) như MPU6050. 

Khi bạn quay mpu6050 theo trục z (góc yaw), các trục khác (roll và pitch) cũng bị ảnh hưởng do độ nhạy và sai số của cảm biến. Điều này dẫn đến sự tương tác giữa các trục và làm thay đổi giá trị của góc roll và pitch.

Theo biểu đồ trên và thí nghiệm cho thấy:

Khi quay góc yall thì góc roll và pitch bị lệch khi xoay nhưng có xu hướng trở về 0 khi góc yall không thay đổi => sai số xảy ra ở gyroscope, dữ liệu đọc từ Accelerometer là đúng vì theo giời gian thì góc sẽ được đưa về giá trị góc được tính bởi Accelerometer.

1. Biểu đồ về giá trị các góc đo được khi quay góc yall

![](Aspose.Words.7c512fcb-68aa-4263-a80e-7884298dbaf1.002.png)

Tăng yaw từ 0-> 80

Roll : 0,1-> -3 

Pitch : 0,6 -> -1,2 

![](Aspose.Words.7c512fcb-68aa-4263-a80e-7884298dbaf1.003.png)

Yaw 0 -> -60

Roll 0 -> 2,25

Pitch 0,75 -> -0,5




1. Biểu đồ về giá trị vận tốc góc (Gyro) khi quay góc yall
- Chú thích:
  - Đỏ(gd – gyro down): gz trục hướng xuống
  - Xanh (gn – gyro North): gx hướng bắc
  - Tím (ge- gyro East): hướng đông

![](Aspose.Words.7c512fcb-68aa-4263-a80e-7884298dbaf1.004.png)

Gd(gz): 0->-80 

Gn(gx): 0->3,4

Ge(gy): 0->0,5

![](Aspose.Words.7c512fcb-68aa-4263-a80e-7884298dbaf1.005.png)

|<p>Gd(gz): 0->148 </p><p>Gn(gx): 0->-6,1</p><p>Ge(gy): 0->-1</p><p></p>|<p>Gd(gz): 0->-120 </p><p>Gn(gx): 0->5</p><p>Ge(gy): 0->0,4</p><p></p>|
| :- | :- |

\*Kết quả: khi trục góc yall(Gz): tăng thì các trục còn lại roll, pitch có xu hướng giảm và ngược lại.

Ta hiệu chỉnh giá trị gyro đọc được từ IMU bằng cách trừ đi phần trăm theo gd(gz) đọc được:

`	`Vd: gn = gn+%gn.gd)

%gn(gx) = (3,4/80+6,1/148+5/120)/3 = 0,042

%ge(gy) = (0,5/80+1/148+0,4/120)/3 = 0,016




\*So sánh kết quả cũ và mới:

Lúc ban đầu:![](Aspose.Words.7c512fcb-68aa-4263-a80e-7884298dbaf1.006.png)

Kết quả sau

![](Aspose.Words.7c512fcb-68aa-4263-a80e-7884298dbaf1.007.png)

Góc roll và pitch đã được cải thiện về độ lệch khi góc yaw quay từ 5 độ -> +- 1độ

\*Kết luận: Có thể dùng phương pháp này để hiểu chỉnh sai số khi góc yall quay cho roll và pitch

`	`Vì mỗi mpu có hệ số khác nhau nên cần thêm chương trình tự động tính hệ số %gn và %ge trong chương trình calibrate MPU.


