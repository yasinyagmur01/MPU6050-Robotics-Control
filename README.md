# MPU6050 Projesi: Sensörden Simülasyona

Bu proje, titreyen ve kısa kablolu ("kötü setup") bir MPU6050 ivmeölçer/jiroskop sensörünün verilerini alıp, matematiksel filtreler (EKF, EMA, DEMA, LERP) kullanarak profesyonel, pürüzsüz ve eğlenceli arayüzlere dönüştürdüğümüz bir serüvendir.

## 🔌 Donanım Kurulumu (Devreyi Evde Tekrar Kurmak İçin)
Evde devreyi tekrar kurduğunuzda kabloları şu şekilde bağlayın:
* **VCC:** Arduino `5V`
* **GND:** Arduino `GND`
* **SDA:** Arduino `A4`
* **SCL:** Arduino `A5`

## 🚀 Proje Aşamaları ve Yaptıklarımız

Proje boyunca 3 ana konsept geliştirdik. Bunları çalıştırmak için Arduino'yu takın, VS Code'u açın ve terminale `python -m http.server 8080` yazıp tarayıcıdan linklere tıklayın.

### 1. Uçak Kokpiti ve 3D Görselleştirici (`visualizer.html`)
**Ne Yaptık?** Sensörden gelen ham verileri "Extended Kalman Filter (EKF)" ile süzüp, ekranda 3 boyutlu bir uçağı kontrol ettik.
**Özellikler:** Gerçekçi yapay ufuk (Artificial Horizon), Roll (Yatış) ve Pitch (Yunuslama) göstergeleri, anlık sıcaklık takibi.

### 2. Savaş Pilotu Oyunu (`combat_flight.html`)
**Ne Yaptık?** Sensörü bir oyun kolu (joystick) gibi kullandık. Sensör çok titrediği ve kablo kısa olduğu için hitbox'ları (vuruş alanlarını) çok geniş tuttuk.
**Özellikler:** Düz yeşil ova, otomatik ateş eden lazerler, size doğru gelen ve ateş eden düşman uçakları, kaçmanız gereken dev meteorlar. Sensörün burnunu kaldırıp indirerek uçağı yönetiyorsunuz.

### 3. Ters Kinematik (IK) Robot Kolu Simülasyonu (`robot_arm.html`)
**Ne Yaptık?** "Robot kolu ama yumuşak ve matematiksel olsun" isteği üzerine, saf mühendislik algoritmaları yazdık.
**Kullanılan Matematik ve Özellikler:**
* **DEMA (Double Exponential Moving Average):** Sensördeki titremeleri %100 yok etmek için 2 katmanlı aşırı ağırlaştırılmış sinyal süzgeci. Sensörde deprem olsa robot milim oynamaz.
* **Türev (Hız) Algılaması:** Hangi eksende hareket ettiğinizi anlık hızınızı hesaplayarak bulur ve arayüzde o eksenin "Neon" ışığını yakar (X-Z veya Y ekseni).
* **Inverse Kinematics (Ters Kinematik - Trigonometri):** Sensörü hedef (Target XYZ) olarak alır. Robot kolunun o hedefe ulaşması için omuz ve dirseğinin (`θ1, θ2, θ3`) kaç derece bükülmesi gerektiğini Kosinüs teoremiyle otomatik hesaplar.
* **LERP (Linear Interpolation):** Hesaplanan açılara aniden zıplamaz, t=0.015 gibi ağır bir katsayı ile fiziksel kütlesi varmış gibi yumuşacık süzülerek hedefe ulaşır.

---

## 🛠️ Nasıl Çalıştırılır?

1. Arduino USB kablosunu bilgisayara takın.
2. Açık olan **tüm Serial Monitor'leri kapatın** (Port meşgul olmasın).
3. Klasörde terminal (CMD/PowerShell/VS Code Terminal) açıp şu komutu girin: 
   `python -m http.server 8080`
4. Tarayıcınızı açın ve hangi projeyi görmek istiyorsanız adres satırına yazın:
   * **Robot Kolu:** `http://localhost:8080/robot_arm.html`
   * **Savaş Oyunu:** `http://localhost:8080/combat_flight.html`
   * **Uçak Kokpiti:** `http://localhost:8080/visualizer.html`
5. Sayfa açıldığında sol üstteki **SYS.CONNECT()** (veya Sensöre Bağlan) tuşuna basıp `COM` portunuzu seçin.
6. İyi eğlenceler!

*Not: Eğer koda müdahale edip tekrar yüklemek isterseniz, yükleme işleminden önce tarayıcıdaki o sekmeyi mutlaka kapatın, yoksa "COM Port Erişim Engellendi" hatası alırsınız.*
