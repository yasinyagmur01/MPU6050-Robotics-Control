# MPU6050 Projesi: Adım Adım Gelişim, Arka Plan Konseptleri ve Araştırma Rehberi

Bu belge, Arduino UNO ve MPU6050 (İvmeölçer ve Jiroskop) sensörü ile yaptığımız projenin **adım adım nasıl geliştiğini**, arka planda çalışan **matematiksel/fiziksel konseptleri** ve eksik kalabileceğiniz noktalar için **internet araştırmalarını** içermektedir. Evde okuyup projenin mantığını tam olarak kavrayabilmeniz için bir rehber olarak hazırlanmıştır.

---

## Aşama 1: Başlangıç ve Kötü Donanım Gerçeği
**Ne Yaptık?**
Sensörü Arduino'ya bağladık ve Serial Monitor üzerinden ivme (X, Y, Z) ve açısal hız verilerini okumaya başladık. Ancak ortada büyük bir fiziksel sorun vardı: **Sensör çok titriyordu (gürültülüydü) ve kablonuz kısaydı.**

**Arka Planda Ne Oluyordu? (İnternet Araştırması: Sensör Gürültüsü)**
MPU6050 iki ana sensörden oluşur:
1. **İvmeölçer (Accelerometer):** Yerçekimini ölçerek yönü (aşağısı neresi) çok iyi bilir. Ancak harekete ve titreşime karşı aşırı hassastır. Masaya vursanız bile değerler zıplar (Buna "Gürültü / Noise" denir).
2. **Jiroskop (Gyroscope):** Dönüş hızını ölçer. Kısa süreli hareketleri çok akıcı ve kusursuz ölçer. Ancak zaman geçtikçe ölçümünde hata birikir ve durduğu yerde bile dönüyormuş gibi değer üretir (Buna "Drift / Kayma" denir).

---

## Aşama 2: Filtreleme ve Uçak Kokpiti (Görselleştirici)
**Ne Yaptık?**
Sensörden gelen o berbat, titreyen veriyi doğrudan kullanmak yerine bir filtre yazıp `visualizer.html` adında bir 3D Uçak Kokpiti (Yapay Ufuk) arayüzüne bağladık. Web Serial API sayesinde tarayıcının direkt Arduino ile konuşmasını sağladık.

**Arka Planda Ne Oluyordu? (İnternet Araştırması: Kalman Filtresi - EKF)**
Sensörü düzgün okumak için **Extended Kalman Filter (EKF)** algoritmasını kullandık. Kalman filtresi "Sensör Füzyonu" yapar:
* **Tahmin Et (Predict):** Jiroskopa bakarak uçağın bir sonraki an nerede olacağını tahmin eder (çünkü jiroskop anlık değişimlerde harikadır).
* **Düzelt (Update):** İvmeölçere (yerçekimine) bakarak jiroskopun zamanla yaptığı "kayma" hatasını düzeltir.
* **Sonuç:** Ekranda gördüğünüz uçak, eliniz titrese bile oldukça pürüzsüz dönüyordu.

---

## Aşama 3: Oyunlaştırma (Savaş Pilotu Oyunu)
**Ne Yaptık?**
Sensörle sadece ufku izlemek yerine bunu bir oyun koluna çevirdik. `combat_flight.html` adında bir WebGL uzay/savaş oyunu yaptık.

**Arka Planda Ne Oluyordu?**
Sensörünüz kötüyken ve hareket alanınız darkken "hassas nişan alma" oyunları oynanmaz. Bu yüzden arka planda kodlarken **Hitbox'ları (Düşman uçaklarının ve sizin vuruş alanlarınızı) devasa boyutta** ayarladık. Ayrıca otomatik ateş etme ekledik. Böylece sensör titrese bile oyundan keyif almanız ve hedefleri vurabilmeniz garanti altına alındı.

---

## Aşama 4: Matematiksel Transformasyon (Sinyali Dijitale Çevirme)
**Ne Yaptık?**
Oyun mantığından çıkıp işi mekaniğe dökmek istediniz. "Robot kolu bu kadar yatınca sağa dönsün, şöyle olunca füze atsın" dediniz.

**Arka Planda Ne Oluyordu? (Deadzone ve Hysteresis Kavramları)**
Kısa kablolu analog bir sensörü "Dijital bir butona" çevirmek zorundaydık:
1. **Deadzone (Ölü Bölge):** Sensör merkezdeyken (-5 ile +5 derece arası) oluşan tüm titremeleri kod içinde "Sıfır" kabul ettik. Eliniz titrese bile robot duruyordu.
2. **Histerezis (Schmitt Trigger):** Sensörü 15 derece yatırana kadar hiçbir şey olmadı, 15 dereceyi geçince aniden "Tık!" diye **ROBOT SAĞA DÖN** komutu ateşlendi. Geri döndürürken de 15 derecede değil, 5 dereceye düşene kadar dönmeye devam etti. Bu sayede eşik değerinde "Aç-Kapat-Aç-Kapat" gibi motoru yakacak titremelerin (bouncing) önüne geçtik.

---

## Aşama 5: Ters Kinematik ve Robot Kolu Simülasyonu
**Ne Yaptık?**
"Dijital küt küt hareketler istemiyorum, soft geçişli matematiksel ve fiziksel göstergeleri olan bir mühendislik arayüzü istiyorum" dediniz. Bunun üzerine `robot_arm.html` arayüzünü yaptık ve Arduino'dan saniyede yüzlerce veri akacak şekilde kodladık.

**Arka Planda Ne Oluyordu? (İnternet Araştırması: İleri Robotik)**

1. **DEMA (Double Exponential Moving Average):**
   Sensörü "ultra soft" yapmak için veri üstüne veri süzgeci (DEMA) ekledik. Bu süzgeç, sizin ani hareketlerinizi bile sanki tonlarca ağırlığında çelik bir robot kolu hareket ediyormuş gibi ağır ve akıcı bir hale getirdi.

2. **Ters Kinematik (Inverse Kinematics - IK):**
   Robotikte iki yöntem vardır:
   * *İleri Kinematik (FK):* Omuz açısını 30, dirseği 45 derece yaparsam robotun ucu nereye gider? (Hesaplaması kolaydır).
   * *Ters Kinematik (IK):* Robotun ucunun tam olarak X=10, Y=15 koordinatına gitmesini istiyorum; omuz ve dirsek **kaç derece olmalı?** (Hesaplaması çok zordur).
   Biz projenin son aşamasında arayüze bir IK motoru gömdük. Sizin sensörünüz sadece X ve Y hedefini veriyordu, arka planda trigonometri (Kosinüs teoremi) çalışarak omuz (`Theta 2`) ve dirsek (`Theta 3`) açılarını hesaplayıp ekranda robotu o noktaya büküyordu.

3. **LERP (Linear Interpolation - Çizgisel Yumuşatma):**
   Açılar hesaplandıktan sonra robot o açıya aniden (ışınlanarak) geçmesin diye LERP formülü kullandık. Robot her saniye hedef açının %5'i kadar ilerledi, bu da o muazzam süzülme (soft geçiş) hissiyatını yarattı.

---

## Özet ve Sonuç
Fiziksel şartları (donanımı) değiştiremediğimiz durumlarda, yazılımın gücüyle (Filtreler, Histerezis, Kinematik algoritmalar) bu fiziksel hataları nasıl maskeleyip kusursuz sistemler yaratabileceğimizi adım adım deneyimledik. 

Bu rehberi okuduktan sonra `README.md` dosyasındaki çalıştırma talimatlarını uygulayarak tüm bu arayüzleri evinizde rahatça çalıştırıp inceleyebilirsiniz.
