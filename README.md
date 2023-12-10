# Water-Quality-Monitoring-System

# Introduction to the problem
Air bersih memainkan peran sentral dalam berbagai sisi kehidupan manusia. Terkadang, penjagaan kualitas air sulit untuk dilakukan karena adanya karakteristik yang tidak dapat ditentukan melalui observasi biasa. Water Quality Monitoring System (WQMS) memberikan solusi untuk masalah ini dengan mengaplikasikan konsep Internet of Things.

### Solution
WQMS adalah sebuah proyek IoT yang bertujuan untuk mengukur dan memantau kualitas air minum, yaitu suhu dan kekeruhan air. Suhu dan kekeruhan air adalah dua parameter penting yang dapat menunjukkan tingkat kemurnian dan kesegaran air. Sistem ini menggunakan modul WiFi ESP32, sensor TDS, sensor turbidity, sensor suhu DS18B20, dan PCB.

# Hardware design and implementation details
Pada ESP32 untuk pembacaan sensor, sensor DS18B20 Temperature Sensor bertugas mengukur suhu pada air. Selain itu, sebuah senor SEN0244 TDS Sensor bertugas mengukur total dissolved Solid (TDS) dalam air, dan sebuah SEN0189 Turbidity Sensor bertugas mengukur tingkat kekeruhan dalam air. Data yang dibaca oleh ESP32 ini akan diteruskan dengan menggunakan protokol MQTT ke ESP32 server yang berfungsi untuk menerima dan mengirimkan data ke Thingsboard. Kedua ESP32 yang digunakan akan terkoneksi dengan internet melalui jaringan Wi-Fi. 

### Components
* Modul WiFi ESP321
* Sensor turbidity2
* Sensor TDS
* Sensor suhu DS18B203
* Resistor 4.7K
* Kabel jumper
* Breadboard

### How it works
Sistem ini bekerja dengan cara berikut:
* Sensor turbidity mengukur jumlah partikel terlarut dalam air dengan cara mengirimkan sinyal AC ke probe elektroda dan menerima sinyal analog yang berbanding lurus dengan kekeruhan air.
* Sensor suhu DS18B20 mengukur suhu air dengan cara mengirimkan sinyal digital ke modul ESP32 melalui pin data yang dihubungkan dengan resistor pull-up 4.7K.
* Modul ESP32 membaca nilai analog dari sensor turbidity dan nilai digital dari sensor suhu, lalu mengubahnya menjadi satuan ppm (part per million) untuk kekeruhan dan derajat Celcius untuk suhu.
* Modul ESP32 juga mengirimkan data kekeruhan dan suhu ke server IoT Thingspeak melalui koneksi WiFi, sehingga data dapat dipantau secara online dari mana saja.
* Data kekeruhan dan suhu juga ditampilkan pada layar OLED 0.96" yang terhubung dengan modul ESP32 melalui protokol I2C.

### Schematic and real-world implementation
![image](https://github.com/frrp16/Water-Quality-Monitoring-System/assets/91055987/573d1974-b6d6-48b8-b6cb-59784f468900)


# Network infrastructure
* ESP32 yang berperan sebagai **master** membuat dua buah client koneksi Wi-Fi, yaitu untuk mengendalikan koneksi dari MQTT dan untuk mengendalikan koneksi ke Thingsboard. Perangkat akan melakukan koneksi ke Wi-Fi menggunakan WiFiSecure yang dilanjutkan dengan menjalankan task untuk koneksi ke MQTT setelah koneksi WiFi berhasil. Setelah koneksi MQTT berhasil dan telah berlangganan dengan topik yang dispesifikasikan, perangkat akan mengirimkan pesan pembuka dan siap untuk membaca data dari ESP32 sensor. Koneksi ke Thingsboard akan dijalankan pada fungsi loop dan akan melakukan reconnection bila koneksi terputus.
* ESP32 yang berperan sebagai **slave** (sensor) membuat satu buah client koneksi Wi-Fi, yaitu untuk mengirimkan data ke MQTT. Protokol koneksi Wi-Fi dan MQTT untuk sensor sama dengan master.

# Software implementation details
* ESP32 yang berperan sebagai **master** menerima data dari MQTT menggunakan fungsi callback dalam format JSON. Data yang dikirimkan ke Thingsboard mencakup tiga nilai, yaitu TDS, suhu, dan kekeruhan.
* ESP32 yang berperan sebagai **slave** (sensor) mengambil data dari sensor, di mana tiap sensor memiliki interval pembacaan yang bervariasi. Pengiriman data ke ESP32 master melalui MQTT dilakukan setiap lima detik untuk sinkronisasi dan mencegah overload. Data yang dikirimkan memiliki format JSON.
* Thingsboard menampilkan dan memonitor hasil pembacaan sensor yang disebut Telemetry. Sebuah line chart yang terdapat dalam dashboard menunjukkan alur pembacaan sensor dan notifikasi alarm jika data dari Telemetry melebihi ambang batas. Untuk melakukan fungsi logic dari rule chain, hardware yang terhubung memiliki sebuah attribute khusus.

### Flowcharts
Perangkat Master dan Slave

![image](https://github.com/frrp16/Water-Quality-Monitoring-System/assets/91055987/08c2e9d3-602c-4fee-a747-6eff645cce32)

Thingsboard

![image](https://github.com/frrp16/Water-Quality-Monitoring-System/assets/91055987/8e577322-81a4-41f2-adf7-05ce3d337029)

Alur Rule Chain Alarm

![image](https://github.com/frrp16/Water-Quality-Monitoring-System/assets/91055987/5433e66f-4497-495d-b5bf-9e00ffbfb325)

### Thingsboard
Dashboard

![image](https://github.com/frrp16/Water-Quality-Monitoring-System/assets/91055987/bcdd67d4-7c30-4a8f-85ec-cbec72133cc5)

Data

![image](https://github.com/frrp16/Water-Quality-Monitoring-System/assets/91055987/a0e25583-9e09-493d-a0be-8e52d91560f6)
![image](https://github.com/frrp16/Water-Quality-Monitoring-System/assets/91055987/7b908544-10cd-4eb0-917b-90f5884d8719)

# Test results and performance evaluation

# Conclusion and future work

