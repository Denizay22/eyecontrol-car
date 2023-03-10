/*
ÇDTP / 25
Mekanik hareket kodlarını ve bluetooth bağlantılarını içerir.
İbrahim ERTEKİN tarafından yazılmıştır.
*/
#include <SoftwareSerial.h> // HC-06 bluetooth modülü kütüphanesi
SoftwareSerial bt_iletisim(6, 7); // 6. pin RX, 7. pin TX olarak ayarlandı.
//L298N Bağlantısı
const int in1 = 8; // sağ teker ileri
const int in2 = 9; // sağ teker geri
const int in3 = 10; // sol teker ileri
const int in4 = 11; // sol teker geri
int durmaLed = 13; // Durma hareketinin gerçekleştiğini göstermek için 13. pine bir adet led bağlantısı yapıldı.


void setup() {
    Serial.begin(9600); // 9600 baudrate hızında bir seri port açalım
    bt_iletisim.begin(9600); // 9600 BT baudrate hızı
    // motor bağlantıları çıkış olarak ayarlandı
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
    pinMode(durmaLed, OUTPUT);
}


void loop()
{
    if (bt_iletisim.available()) // Bluetooth iletişimi kontrol ediliyor.
    {
        char data = bt_iletisim.read(); // Bluetooth aracılığı ile gelen veri 'data' değişkenine yazılıyor.
        Serial.println(data); // Data değişkenindeki veri serial monitörden gözlemlenebilir.
        /***********************İleri****************************/
        //Gelen veri '1' ise araba ileri gider.
        //Aracın 1 saniye boyunca ileri yönlü hareket etmesini sağlayacak.
        if (data == '1') {
            analogWrite(in1, 255);
            analogWrite(in2, 0);
            analogWrite(in3, 255);
            analogWrite(in4, 0);
            delay(1000);
            digitalWrite(in1, LOW);
            digitalWrite(in3, LOW);
            digitalWrite(in2, LOW);
            digitalWrite(in4, LOW);
            delay(1000);
        }
        /***********************Geri****************************/
        //Gelen veri '2' ise araba geri gider.
        //Aracın 1 saniye boyunca geri yönlü hareket etmesini sağlayacak.
        else if (data == '2') {
            analogWrite(in1, 0);
            analogWrite(in2, 175); //Sol tekere 255 verilirken sağ kere 175 birim hız verilmesi tekerlerin hız farklarını dengelemek
            içindir.
            analogWrite(in3, 0);
            analogWrite(in4, 255);
            delay(1000);
            digitalWrite(in1, LOW);
            digitalWrite(in3, LOW);
            digitalWrite(in2, LOW);
            digitalWrite(in4, LOW);
            delay(1000);
        }
        /************************Stop*****************************/
        //Gelen veri '3' ise araba hareket etmez.
        else if (data == '3'){
            digitalWrite(durmaLed, HIGH);
            digitalWrite(in1, LOW);
            digitalWrite(in2, LOW);
            digitalWrite(in3, LOW);
            digitalWrite(in4, LOW);
            delay(1000);
            digitalWrite(durmaLed, LOW);
            delay(100);
        }
        /***************************Sol*****************************/
        //Gelen veri '4' ise araba sola gider.
        //Aracın 1 saniye boyunca sol yönlü hareket etmesini sağlayacak. (Açı hesabı, süre değiştirilerek yapılabilecek.)
        //Mevcut değerler baz alındığında dönüş açısı sol yöne 15 derece olarak hesaplanmıştır.
        //Sol yönlü hareket için sağ tekere ileri yönlü enerji veriyoruz. Sol tekere ise dönüşe yardımcı olması için geri yönlü enerji veriyoruz.
        else if (data == '4') {
            analogWrite(in1, 150); // sol yönlü hareket için sağ tekere ileri yönlü 150 birim güç gönderiyoruz
            analogWrite(in2, 0);
            analogWrite(in3, 0);
            analogWrite(in4, 100); // sol yönlü hareket için sol tekere ileri yönlü 100 birim güç gönderiyoruz
            delay(100); // hareket 100 ms yani 0.1 saniye boyunca gerçekleşiyor. Bu süre bize 15 derece dönüş açısı sağlıyor.
            digitalWrite(in1, LOW);
            digitalWrite(in3, LOW);
            digitalWrite(in2, LOW);
            digitalWrite(in4, LOW);
            delay(1000);
        }
        /***************************Sağ*****************************/
        //Gelen veri '5' ise araba sağa gider
        //Aracın 1 saniye boyunca sağ yönlü hareket etmesini sağlayacak. (Açı hesabı, süre değiştirilerek yapılabilecek.)
        //Mevcut değerler baz alındığında dönüş açısı sol yöne 15 derece olarak hesaplanmıştır.
        //Sağ yönlü hareket için sol tekere ileri yönlü enerji veriyoruz. Sağ tekere ise dönüşe yardımcı olması için geri yönlü enerji veriyoruz.
        else if (data == '5') {
            analogWrite(in1, 0);
            analogWrite(in2, 100); // sağ yönlü hareket için sağ tekere ileri yönlü 100 birim güç gönderiyoruz
            analogWrite(in3, 150); // sağ yönlü hareket için sol tekere ileri yönlü 150 birim güç gönderiyoruz
            analogWrite(in4, 0);
            delay(100); // hareket 100 ms yani 0.1 saniye boyunca gerçekleşiyor. Bu süre bize 15 derece dönüş açısı sağlıyor.
            digitalWrite(in1, LOW);
            digitalWrite(in3, LOW);
            digitalWrite(in2, LOW);
            digitalWrite(in4, LOW);
            delay(1000);
        }
    }
}