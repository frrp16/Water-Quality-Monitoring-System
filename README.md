# Water-Quality-Monitoring-System

# Deskripsi
Water Quality Monitoring System adalah sebuah proyek IoT yang bertujuan untuk mengukur dan memantau kualitas air minum, yaitu suhu dan kekeruhan air. Suhu dan kekeruhan air adalah dua parameter penting yang dapat menunjukkan tingkat kemurnian dan kesegaran air. Sistem ini menggunakan modul WiFi ESP32, sensor TDS, sensor turbidity, sensor suhu DS18B20, dan PCB.

# Cara Kerja
Sistem ini bekerja dengan cara berikut:
Sensor turbidity mengukur jumlah partikel terlarut dalam air dengan cara mengirimkan sinyal AC ke probe elektroda dan menerima sinyal analog yang berbanding lurus dengan kekeruhan air.
Sensor suhu DS18B20 mengukur suhu air dengan cara mengirimkan sinyal digital ke modul ESP32 melalui pin data yang dihubungkan dengan resistor pull-up 4.7K.
Modul ESP32 membaca nilai analog dari sensor turbidity dan nilai digital dari sensor suhu, lalu mengubahnya menjadi satuan ppm (part per million) untuk kekeruhan dan derajat Celcius untuk suhu.
Modul ESP32 juga mengirimkan data kekeruhan dan suhu ke server IoT Thingspeak melalui koneksi WiFi, sehingga data dapat dipantau secara online dari mana saja.
Data kekeruhan dan suhu juga ditampilkan pada layar OLED 0.96" yang terhubung dengan modul ESP32 melalui protokol I2C.

Komponen
Komponen yang dibutuhkan untuk membuat sistem ini adalah sebagai berikut:
- Modul WiFi ESP321
- Sensor turbidity2
- Sensor TDS
- Sensor suhu DS18B203
- Resistor 4.7K
- Kabel jumper
- Breadboard
