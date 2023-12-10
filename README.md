# Water-Quality-Monitoring-System

## Introduction to the problem
Air bersih memainkan peran sentral dalam berbagai sisi kehidupan manusia. Terkadang, penjagaan kualitas air sulit untuk dilakukan karena adanya karakteristik yang tidak dapat ditentukan melalui observasi biasa. Water Quality Monitoring System (WQMS) memberikan solusi untuk masalah ini dengan mengaplikasikan konsep Internet of Things (IoT).

### Solution
WQMS adalah sebuah proyek IoT yang bertujuan untuk mengukur dan memantau kualitas air minum, yaitu suhu dan kekeruhan air. Suhu dan kekeruhan air adalah dua parameter penting yang dapat menunjukkan tingkat kemurnian dan kesegaran air. Sistem ini menggunakan modul WiFi ESP32, sensor TDS, sensor turbidity, sensor suhu DS18B20, dan PCB.

## Hardware design and implementation details
Pada ESP32 untuk pembacaan sensor, sensor DS18B20 Temperature Sensor bertugas mengukur suhu pada air. Selain itu, sebuah senor SEN0244 TDS Sensor bertugas mengukur Total Dissolved Solid (TDS) dalam air, dan sebuah SEN0189 Turbidity Sensor bertugas mengukur tingkat kekeruhan dalam air. Data yang dibaca oleh ESP32 ini akan diteruskan dengan menggunakan protokol MQTT ke ESP32 server yang berfungsi untuk menerima dan mengirimkan data ke Thingsboard. Kedua ESP32 yang digunakan akan terkoneksi dengan internet melalui jaringan Wi-Fi. 

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
![image](https://github.com/frrp16/Water-Quality-Monitoring-System/assets/91055987/e0c0fa2d-d343-4554-8240-28eca46de661)


## Network infrastructure
* ESP32 yang berperan sebagai **master** membuat dua buah client koneksi Wi-Fi, yaitu untuk mengendalikan koneksi dari MQTT dan untuk mengendalikan koneksi ke Thingsboard. Perangkat akan melakukan koneksi ke Wi-Fi menggunakan WiFiSecure yang dilanjutkan dengan menjalankan task untuk koneksi ke MQTT setelah koneksi WiFi berhasil. Setelah koneksi MQTT berhasil dan telah berlangganan dengan topik yang dispesifikasikan, perangkat akan mengirimkan pesan pembuka dan siap untuk membaca data dari ESP32 sensor. Koneksi ke Thingsboard akan dijalankan pada fungsi loop dan akan melakukan reconnection bila koneksi terputus.
* ESP32 yang berperan sebagai **slave** (sensor) membuat satu buah client koneksi Wi-Fi, yaitu untuk mengirimkan data ke MQTT. Protokol koneksi Wi-Fi dan MQTT untuk sensor sama dengan master.

## Software implementation details
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

## Test results and performance evaluation
### Connection between hardware and software
ESP32 server

![image](https://github.com/frrp16/Water-Quality-Monitoring-System/assets/91055987/5ec65296-9b61-4f2a-9a0b-90811bd50c24)

ESP32 sensor reader

![image](https://github.com/frrp16/Water-Quality-Monitoring-System/assets/91055987/08d8a55f-ee3e-48da-ab6f-44f214209419)

Thingsboard devices

![image](https://github.com/frrp16/Water-Quality-Monitoring-System/assets/91055987/472b405f-5f55-4864-8ff5-269ea0395013)

### Connection between ESP32s and server-Thingsboard

Data sensor ke server

![image](https://github.com/frrp16/Water-Quality-Monitoring-System/assets/91055987/108ca38b-1a8d-4780-8064-93bc171aead5)

Data server ke Thingsboard

![image](https://github.com/frrp16/Water-Quality-Monitoring-System/assets/91055987/3e580e59-c138-46a2-ac19-9c99d8120b05)

### Water quality sensors to ESP32 sensor reader

Kasus pertama: kondisi air bersih pada suhu ruangan

![image](https://github.com/frrp16/Water-Quality-Monitoring-System/assets/91055987/c1d26fd2-9e92-4e03-8900-3871a456fd08)
![image](https://github.com/frrp16/Water-Quality-Monitoring-System/assets/91055987/6b5885f3-851b-4b2f-b127-f6b6738863a2)
![image](https://github.com/frrp16/Water-Quality-Monitoring-System/assets/91055987/4f2861ab-7df9-4efb-a240-0256ccbd8990)
![image](https://github.com/frrp16/Water-Quality-Monitoring-System/assets/91055987/d8665cb2-2fcd-461e-82d6-e74f6ba50e9f)


Kasus kedua: air keruh

![image](https://github.com/frrp16/Water-Quality-Monitoring-System/assets/91055987/fb92edcb-3668-474f-a2ad-fbdf89a88a06)
![image](https://github.com/frrp16/Water-Quality-Monitoring-System/assets/91055987/7151673b-fc84-4202-9a50-4bf5a172e26e)
![image](https://github.com/frrp16/Water-Quality-Monitoring-System/assets/91055987/e2763dc4-f42a-4780-8187-80ac2523dfcb)
![image](https://github.com/frrp16/Water-Quality-Monitoring-System/assets/91055987/2b04ebf7-cc7d-4e9a-80d7-76d2cc1e45fa)


Kasus ketiga: air panas

![image](https://github.com/frrp16/Water-Quality-Monitoring-System/assets/91055987/446ccc8e-7771-4e56-a67d-08d42e5b966f)
![image](https://github.com/frrp16/Water-Quality-Monitoring-System/assets/91055987/b299b657-0016-4c8f-a681-818dbbad0dad)
![image](https://github.com/frrp16/Water-Quality-Monitoring-System/assets/91055987/243f2c60-44b3-492c-9c6b-304cc692577d)
![image](https://github.com/frrp16/Water-Quality-Monitoring-System/assets/91055987/6833e8f8-ca87-4f95-80b7-5086f23c4e53)

### Evaluation
* Untuk tes pertama yang menguji koneksi antar hardware dan software, screenshot yang ditunjukkan menandakan bahwa koneksi ketiganya terjadi dengan sempurna. ESP32 yang membaca sensor mendapatkan pesan dari ESP32 master, sedangkan master terhubung ke MQTT dan ESP32 sensor serta Thingsboard. Platform tersebut mengonfirmasi hal ini dengan screenshot connected devices.
* Untuk tes kedua yang menguji pengiriman data dari ESP32 sensor ke Thingsboard, screenshot yang ditunjukkan menandakan bahwa pekerjaan tersebut terjadi dengan sempurna. ESP32 server menerima data dari ESP32 sensor, sedangkan Thingsboard menunjukkan data yang baru masuk pada grafiknya.
* Tes ketiga menguji kemampuan sensor. Untuk kasus pertama di mana air bersifat jernih dengan suhu normal, grafik berada di sisi tengah yang menandakan bahwa kondisi air adalah normal. Untuk kasus kedua di mana air bersifat keruh, berisi garam, dan bersuhu normal, grafik TDS dan kekeruhan mengalami peningkatan. Untuk kasus ketiga di mana air bersifat jernih dengan suhu panas, grafik suhu mengalami peningkatan. Ketiga kasus tersebut menunjukkan bahwa sensor berhasil mendeteksi keadaan air dengan akurat.

## Conclusion
* Jika air menjadi keruh, air tidak dapat digunakan. Oleh karena itu, sistem peringatan dan monitoring air keruh perlu dirancang, seperti Water Quality Monitoring System (WQMS).
* WQMS mengaplikasikan konsep IoT karena mengintegrasikan sistem monitoring jarak jauh dan menggunakan sistem benam khusus sebagai perangkat pengukur.
* Dengan WQMS, dua tujuan telah tercapai: mengaplikasikan IoT pada kebutuhan sehari-hari dan menyediakan solusi pengukuran kualitas air sederhana. WQMS menjadi solusi bagi mereka yang membutuhkan integrasi IoT dalam memantau kualitas air.

### Future work
Untuk pengembangan selanjutnya, interface yang dibuat sendiri perlu dirancang untuk mempermudah penambahan komponen kontrol dan pemantauan. Selain itu, dan yang paling utama, sensor yang diintegrasikan ke ESP32 pembaca sensor ditambahkan untuk menambah parameter pengukuran. Peraturan Menteri Kesehatan memberi 19 parameter wajib ditambah dengan parameter khusus untuk air minum.

