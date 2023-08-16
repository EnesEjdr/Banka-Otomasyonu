#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include<time.h>

struct Islem
{
    int tarih;
    int tip; //0 para çekme, 1 para yatirma, 2 para gönderme, 3 para gelme
    float miktar;
    int hesapno;
};
struct Islem* islemyap(int tarihin,int tipin, float miktarin, int hesapnoin)  // yeni islem actim
{
    struct Islem* i = (struct Islem*)malloc(sizeof(struct Islem));
    i->tarih = tarihin;
    i->tip = tipin;
    i->miktar = miktarin;
    i->hesapno = hesapnoin;
    return i;
}

struct Hesap
{
    int no;
    float bakiye;
    struct Islem* hesapozeti[200];
    int islemsayisi;
};

struct Musteri
{
    char ad[50];
    char soyad[50];
    int no;
    int tip; //0 bireysel, 1 ticari
    struct Hesap* hesaplar[200];
    int hesapsayisi;
    int dogumtarihi;
    char dogumyeri[50];
    struct Hesap* kayitli[200];
    int kayitlisayisi;
};

struct Banka
{
    struct Musteri* bireysel[200];
    int bireyselsayi;
    struct Musteri* ticari[200];
    int ticarisayi;
    int hesapno; //her hesaba verilecek numara
    int musterino [500];
};

struct Banka* bankaac()
{
    struct Banka* b = (struct Banka*)malloc(sizeof(struct Banka));
    b->bireyselsayi = 0;
    b->ticarisayi = 0;
    b->hesapno = 0;
    return b;
};

void musterininhesaplariniyazdir(struct Musteri* m)
{
    int i;
    printf("----Hesaplar:\n");
    for(i = 0; i<m->hesapsayisi;i++)
    {
        printf(" ------ Hesap no: %d  Hesap bakiye: %.2f\n",m->hesaplar[i]->no,m->hesaplar[i]->bakiye);
    }
}
void kayitlihesaplariyazdir(struct Musteri* m)
{
    int i;
    printf("----Hesaplar:\n");
    for(i = 0; i<m->kayitlisayisi;i++)
    {
        printf(" ------ %d \n",m->kayitli[i]->no);
    }
}

void tummusterileriyazdir(struct Banka* ziraat)
{
    int i;
    printf("\nBireysel Musteriler (tip:0): \n");
    for(i=0;i< ziraat->bireyselsayi;i++)
    {
        printf(" %s %s Musteri No: %d \n",ziraat->bireysel[i]->ad,ziraat->bireysel[i]->soyad,ziraat->bireysel[i]->no);
        musterininhesaplariniyazdir(ziraat->bireysel[i]);
    }
    printf("Ticari Musteriler (tip:1): \n");
    for(i=0;i< ziraat->ticarisayi;i++)
    {
        printf(" %s %s Musteri No: %d \n",ziraat->ticari[i]->ad,ziraat->ticari[i]->soyad,ziraat->ticari[i]->no);
        musterininhesaplariniyazdir(ziraat->ticari[i]);
    }

}


int musterinovarmi(struct Banka* ziraat, int random)  //eger üretilen sayi öncekilerden farkliysa 1 dönderir
{
    int i;
    int sayi = ziraat->bireyselsayi + ziraat->ticarisayi;
    for(i=0; i<sayi; i++)
    {
        if(ziraat->musterino[i] == random)
        {
            return 0;
        }
    }
    return 1;

}

int musterinoolustur(struct Banka* ziraat)
{
    int tmp = 0;
    int var = 0;
    while (var == 0)
    {
        tmp = rand()%500;
        var = musterinovarmi(ziraat,tmp);
    }
    int sayi = ziraat->bireyselsayi + ziraat->ticarisayi;
    ziraat->musterino[sayi+1] = tmp;
    return tmp;
}

struct Hesap* hesapac(struct Banka* ziraat)
{
    struct Hesap* i = (struct Hesap*)malloc(sizeof(struct Hesap));
    i->no = ziraat->hesapno;
    ziraat->hesapno += 1;
    i->bakiye = 0;
    i->islemsayisi = 0;
    return i;
}

struct Musteri* musteriac(struct Banka* ziraat,char adin[50],char soyadin[50], int tipin, int dogumtarihiin, char dyeri[50])
{
    struct Musteri* m = (struct Musteri*)malloc(sizeof(struct Musteri));
    m->no = musterinoolustur(ziraat);
    int i;
    for(i=0;i<50;i++)
    {
        m->ad[i] = adin[i];
        m->soyad[i] = soyadin[i];
        m->dogumyeri[i] = dyeri[i];
    }
    m->dogumtarihi = dogumtarihiin;
    m->tip = tipin;
    if(tipin == 0)
    {
        ziraat->bireysel[ziraat->bireyselsayi] = m;
        ziraat->bireyselsayi += 1;
    }
    if(tipin == 1)
    {
        ziraat->ticari[ziraat->ticarisayi] = m;
        ziraat->ticarisayi += 1;
    }
    m->hesaplar[0] = hesapac(ziraat);
    m->hesapsayisi = 1;
    m->kayitlisayisi = 0;
    return m;
}

void musteriyehesapac(struct Banka* ziraat, struct Musteri* m)
{
    m->hesaplar[m->hesapsayisi] = hesapac(ziraat);
    m->hesapsayisi += 1;
}

void kayitliekle(struct Banka* ziraat, struct Musteri* m, struct Hesap* eklenecek)
{
    m->kayitli[m->kayitlisayisi] = eklenecek;
    m->kayitlisayisi += 1;
}

void hesaptanparacek(int tarihin,struct Hesap* h,float miktarin)
{
    struct Islem* i = islemyap(tarihin,0,miktarin,-1);
    h->bakiye -= miktarin;
    h->hesapozeti[h->islemsayisi] = i;
    h->islemsayisi += 1;
}
void MusteriParaCek(int tarih, struct Musteri* m, struct Hesap* h, float mik)
{
    if(h->bakiye>mik)
    {
        hesaptanparacek(tarih,h,mik);
        printf("Para Cekildi\n%d Nolu hesabin yeni bakiyesi: %.2f\n\n",h->no,h->bakiye);
    } else{
        float kalan = mik - h->bakiye;
        hesaptanparacek(tarih,h,h->bakiye);
        int i;
        for(i=0;i<m->hesapsayisi;i++)
        {
            if(m->hesaplar[i]->bakiye > kalan)
            {
                hesaptanparacek(tarih,m->hesaplar[i],kalan);
                kalan = 0;
                break;
            }else{
                kalan = kalan - m->hesaplar[i]->bakiye;
                hesaptanparacek(tarih,m->hesaplar[i],m->hesaplar[i]->bakiye);
            }
        }
        if(kalan == 0)
        {
            printf("Para Cekme Islemi Gerceklesti\nYeni Bakiyeler:\n");
        }else{
            printf("Para Cekme Islemi Gerceklesti\nBakiye Yetersiz Oldugu Icin Istediginiz Miktarin %.2f TL'si Cekilemedi\nYeni Bakiyeler:\n",kalan);
        }
        musterininhesaplariniyazdir(m);
    }
}

void hesabaparayatir(int tarihin,struct Hesap* h,float miktarin)
{
    struct Islem* i = islemyap(tarihin,1,miktarin,-1);
    h->bakiye += miktarin;
    h->hesapozeti[h->islemsayisi] = i;
    h->islemsayisi += 1;
}

void hesabaparagonder(int tarihin,struct Hesap* gonderen,struct Hesap* alan,float miktarin,int gonderentipi)
{
    float gidecek =miktarin;
    if(gonderentipi == 0)
    {
        gidecek = (miktarin * 98)/100;
    }
    struct Islem* gonderici = islemyap(tarihin,2,miktarin,alan);
    struct Islem* alici = islemyap(tarihin,3,gidecek,gonderen);
    gonderen->bakiye -= miktarin;
    alan->bakiye += miktarin;
    gonderen->hesapozeti[gonderen->islemsayisi] = gonderici;
    alan->hesapozeti[alan->islemsayisi] = alici;
    gonderen->islemsayisi += 1;
    alan->islemsayisi += 1;

    printf("Yeni hesap bakiyesi: %.2f\n",gonderen->bakiye);
}

int main()
{
    int gun = 0;
    struct Banka* garanti = bankaac();
    while(gun != -1)
    {
        printf("***%d. Gun Banka Acildi***\n-Islem seciniz: \n1- Musteri Ekle\n2- Para Yatir \n3- Para Cek\n4- Kayitli islemler / Para Gonder\n5- Hesap Ozeti\n6- Hesap Kapat\n7-Cikis\n\n",gun);
        int secim;
        scanf("%d",&secim);

		char isim[50], soyisim[50], dyeri[50];
        int tip,dtarihi;
		int durdur = 1;
		int durdur2 = 1;
		int durdur3 = 1;
		int durdur4 = 1;
        int durdur4in =1;
        int durdur5 = 1;
        int durdur6 = 1;
        FILE *bir;
        FILE *tic;

        switch (secim)
        {
            case 1 : //Musteri ac


                while(durdur ==1)
                {
                    printf("\nSadece isim giriniz: "); scanf("%s",isim);
                    printf("Soyisim giriniz: "); scanf("%s",soyisim);
                    printf("Dogum yeri giriniz: "); scanf("%s",dyeri);
                    printf("Dogum tarihi giriniz(yil): "); scanf("%d",&dtarihi);
                    printf("Musteri tipi giriniz (Bireysel = 0, Ticari = 1): "); scanf("%d",&tip);

                    struct Musteri* yenibirey = musteriac(garanti,isim,soyisim,tip,dtarihi,dyeri);

                    printf("\n!!!!Musteri eklendi ve Hesap Acildi!!!!\n\nIslem seciniz:\n\n1-Yeni Musteri Ekle\n\n2-Ayni Musteriye Hesap Ekle\n\n3-Cikis\n\n");
                    scanf("%d",&durdur);
                    while(durdur == 2)
                    {
                        musteriyehesapac(garanti, yenibirey);
                        printf("%d Nolu Hesap Acildi\n",yenibirey->hesaplar[yenibirey->hesapsayisi-1]->no);
                        printf("!!!Musteri eklendi ve Hesap acildi!!!\n\nIslem seciniz:\n\n1-Yeni Musteri Ekle\n\n2-Ayni Musteriye Hesap Ekle\n\n3-Cikis\n\n");
                        scanf("%d",&durdur);
                    }
                }
            case 2 : //Para Yatir

                while(durdur2 ==1)
                {
                tummusterileriyazdir(garanti);
                printf("Para Yatiracaginiz Musteri nosunu giriniz\n");
                int musNo,mustip;
                scanf("%d",&musNo);
                printf("Para Yatiracaginiz Musteri tipini giriniz\n");
                scanf("%d",&mustip);
                if(mustip == 0) //bireysele para atar
                {
                    int i;
                    int bul = 0;
                    for(i=0;i<garanti->bireyselsayi;i++)
                    {
                        if(musNo == garanti->bireysel[i]->no)  //müsteriler arasinda müsteri no ile ariyor
                        {
                            bul = 1;
                            printf("Secilen Musteri Hesap Bilgileri Listenmektedir...\n\n");
                            musterininhesaplariniyazdir(garanti->bireysel[i]);

                            int hesno,j;
                            int bul2 = 0;
                            printf("Yatirilacak Hesap Numarasini Seciniz\n");
                            scanf("%d",&hesno);
                            for(j=0;j<garanti->bireysel[i]->hesapsayisi;j++)
                            {
                                if(hesno == garanti->bireysel[i]->hesaplar[j]->no)
                                {
                                    bul2 = 1;
                                    printf("Tutar Giriniz:\n");
                                    float mik;
                                    scanf("%f",&mik);
                                    hesabaparayatir(gun,garanti->bireysel[i]->hesaplar[j],mik);
                                    printf("Yeni Hesap Bakiyesi: %.2f\n\n",garanti->bireysel[i]->hesaplar[j]->bakiye);
                                    break;
                                }
                            }
                            if(bul2 == 0)
                            {
                                printf("Girdiginiz Deger Bulunamadi!!!!!!!!\n");
                            }
                        break;
                        }
                    }
                    if(bul==0)
                    {
                        printf("Girdiginiz Deger Bulunamadi!!!!!!!!\n");
                    }
                }
                if(mustip == 1)// ticariye para atar
                {
                        int i;
                        int bul = 0;
                        for(i=0;i<garanti->ticarisayi;i++)
                        {
                            if(musNo == garanti->ticari[i]->no)  //müsteriler arasinda müsteri no ile ariyor
                            {
                                bul = 1;
                                printf("Secilen Musteri Hesap Bilgileri Listenmektedir...\n");
                                musterininhesaplariniyazdir(garanti->ticari[i]);
                                printf("Yatirilacak Hesap Numarasini Seciniz\n");
                                int hesno,j;
                                int bul2 = 0;
                                scanf("%d",&hesno);
                                for(j=0;j<garanti->ticari[i]->hesapsayisi;j++)
                                {
                                    if(hesno == garanti->ticari[i]->hesaplar[j]->no)
                                    {
                                        bul2 = 1;
                                        printf("Tutar Giriniz:\n");
                                        float mik;
                                        scanf("%f",&mik);
                                        hesabaparayatir(gun,garanti->ticari[i]->hesaplar[j],mik);
                                        printf("Yeni Hesap Bakiyesi: %.2f\n",garanti->ticari[i]->hesaplar[j]->bakiye);
                                        break;
                                    }
                                }
                                if(bul2 == 0)
                                {
                                    printf("Girdiginiz Deger Bulunamadi!!!!!!!!\n");
                                }
                                break;
                            }
                        }
                        if(bul==0)
                        {
                            printf("Girdiginiz Deger Bulunamadi!!!!!!!!\n");
                        }
                    }
                printf("\nYeni Islem Giriniz:\n\n1- Yeniden Para Yatir\n\n2-Cikis\n\n");
                scanf("%d",&durdur2);

                }

            case 3 : // Para Cek

                while(durdur3 ==1)
                {
                    tummusterileriyazdir(garanti);
                    printf("Para Cekeceginiz Musteri nosunu giriniz\n");
                    int musNo,mustip;
                    scanf("%d",&musNo);
                    printf("Para Cekeceginiz Musteri tipini giriniz\n");
                    scanf("%d",&mustip);
                    if(mustip == 0) //bireyselden para ceker
                    {
                        int i;
                        int bul = 0;
                        for(i=0;i<garanti->bireyselsayi;i++)
                        {
                            if(musNo == garanti->bireysel[i]->no)  //müsteriler arasinda müsteri no ile ariyor
                            {
                                bul = 1;
                                printf("Secilen Musteri Hesap Bilgileri Listenmektedir...\n");
                                musterininhesaplariniyazdir(garanti->bireysel[i]);

                                int hesno,j;
                                int bul2 = 0;
                                printf("Cekilecek Hesap Numarasini Seciniz\n");
                                scanf("%d",&hesno);
                                for(j=0;j<garanti->bireysel[i]->hesapsayisi;j++)
                                {
                                    if(hesno == garanti->bireysel[i]->hesaplar[j]->no)
                                    {
                                        bul2 = 1;
                                        printf("Tutar Giriniz:\n");
                                        float mik;
                                        scanf("%f",&mik);
                                        if(mik<=750)
                                        {
                                            MusteriParaCek(gun, garanti->bireysel[i], garanti->bireysel[i]->hesaplar[j], mik);
                                        } else{
                                            printf("Maksimum Cekilebilecek Limit 750 TL\n\n");
                                        }
                                        break;
                                    }
                                }
                                if(bul2 == 0)
                                {
                                    printf("Girdiginiz Deger Bulunamadi!!!!!!!!\n");
                                }
                                break;
                            }
                        }
                        if(bul==0)
                        {
                            printf("Girdiginiz Deger Bulunamadi!!!!!!!!\n");
                        }
                    }
                    if(mustip == 1)// ticariye para atar
                    {
                        int i;
                        int bul = 0;
                        for(i=0;i<garanti->ticarisayi;i++)
                        {
                            if(musNo == garanti->ticari[i]->no)  //müsteriler arasinda müsteri no ile ariyor
                            {
                                bul = 1;
                                printf("Secilen Musteri Hesap Bilgileri Listenmektedir...\n");
                                musterininhesaplariniyazdir(garanti->ticari[i]);
                                printf("Cekilecek Hesap Numarasini Seciniz\n");
                                int hesno,j;
                                int bul2 = 0;
                                scanf("%d",&hesno);
                                for(j=0;j<garanti->ticari[i]->hesapsayisi;j++)
                                {
                                    if(hesno == garanti->ticari[i]->hesaplar[j]->no)
                                    {
                                        bul2 = 1;
                                        printf("Tutar Giriniz:\n");
                                        float mik;
                                        scanf("%f",&mik);
                                        if(mik<=1500)
                                        {
                                            MusteriParaCek(gun, garanti->ticari[i], garanti->ticari[i]->hesaplar[j], mik);
                                        } else{
                                            printf("\nMaksimum Cekilebilecek Limit 1500 TL\n\n");
                                        }
                                        break;
                                    }
                                }
                                if(bul2 == 0)
                                {
                                    printf("Girdiginiz Deger Bulunamadi!!!!!!!!\n");
                                }
                                break;
                            }
                        }
                        if(bul==0)
                        {
                            printf("Girdiginiz Deger Bulunamadi!!!!!!!!\n");
                        }
                    }
                    printf("Yeni Islem Giriniz :\n\n1- Yeniden Para Cek\n\n2-Cikis\n\n");
                    scanf("%d",&durdur3);

                }

            case 4 : // Havale


                while(durdur4in ==1)
                {
                    printf("Kalici Alici Eklemek Ister misiniz?(1-Evet  2-Hayir) :");
                    scanf("%d",&durdur4in);
                    if(durdur4in == 2)
                    {
                        break;
                    }
                    tummusterileriyazdir(garanti);
                    printf("Islem Yaptiracaginiz Musteri nosunu giriniz\n");
                    int musNo;
                    scanf("%d",&musNo);
                    printf("Kayit Edeceginiz Musterinin nosunu giriniz\n");
                    int musNo2;
                    scanf("%d",&musNo2);
                    struct Musteri* a = NULL;
                    struct Musteri* b = NULL;
                    int i;
                    for(i=0;i<garanti->bireyselsayi;i++)
                    {
                        if(musNo == garanti->bireysel[i]->no)
                        {
                            a = garanti->bireysel[i];
                        }
                        if(musNo2 == garanti->bireysel[i]->no)
                        {
                            b = garanti->bireysel[i];
                        }
                    }
                    for(i=0;i<garanti->ticarisayi;i++)
                    {
                        if(musNo == garanti->ticari[i]->no)
                        {
                            a = garanti->ticari[i];
                        }
                        if(musNo2 == garanti->ticari[i]->no)
                        {
                            b = garanti->ticari[i];
                        }
                    }
                    if(a== NULL || b== NULL)
                    {
                        printf("Gecersiz Giris!!!\n");
                    } else {
                        musterininhesaplariniyazdir(b);
                        printf("Kayit edeceginiz hesabi secin:\n");
                        struct Hesap* kayitet = NULL;
                        int c;
                        scanf("%d",&c);
                        for(i=0;i<b->hesapsayisi;i++)
                        {
                            if(c==b->hesaplar[i]->no)
                            {
                                kayitet = b->hesaplar[i];
                            }
                        }
                        if(kayitet == NULL)
                        {
                            printf("Gecersiz Giris!!!\n");
                        } else {
                            kayitliekle(garanti,a,kayitet);
                            printf("Hesap Eklendi\n\n");
                        }
                    }
                }

                while (durdur4 =1)
                {
                    printf("Para Gonderecek miniz?(1-Evet  2-Hayir) :");
                    scanf("%d",&durdur4);
                    if(durdur4 == 2)
                    {
                        break;
                    }
                    tummusterileriyazdir(garanti);
                    printf("Islem Yaptiracaginiz Musteri nosunu giriniz\n");
                    int musNo;
                    scanf("%d",&musNo);
                    struct Musteri* a = NULL;
                    int i;
                    for(i=0;i<garanti->bireyselsayi;i++)
                    {
                        if(musNo == garanti->bireysel[i]->no)
                        {
                            a = garanti->bireysel[i];
                        }
                    }
                    for(i=0;i<garanti->ticarisayi;i++)
                    {
                        if(musNo == garanti->ticari[i]->no)
                        {
                            a = garanti->ticari[i];
                        }
                    }
                    if(a== NULL)
                    {
                        printf("Gecersiz Giris!!!\n");
                    } else {
                        musterininhesaplariniyazdir(a);
                        printf("Para cekilecek hesabinizin nosunu giriniz\n");
                        int gonderen;
                        struct Hesap* g = NULL;
                        scanf("%d",&gonderen);
                        for(i=0;i<a->hesapsayisi;i++)
                        {
                            if(gonderen == a->hesaplar[i]->no)
                            {
                                g = a->hesaplar[i];
                            }
                        }
                        kayitlihesaplariyazdir(a);
                        printf("Para gondereceginiz hesabin nosunu giriniz\n");
                        int gonder;
                        struct Hesap* al = NULL;
                        scanf("%d",&gonder);
                        for(i=0;i<a->kayitlisayisi;i++)
                        {
                            if(gonder == a->kayitli[i]->no)
                            {
                                al = a->kayitli[i];
                            }
                        }
                        if(al == NULL || g == NULL)
                        {
                            printf("Gecersiz Giris!!!\n");
                        } else {
                            printf("Tutar giriniz:\n");
                            float tutar;
                            scanf("%f",&tutar);
                            if(tutar>g->bakiye)
                            {
                                printf("Bakiye Yetersiz!!!\n");
                            } else{
                                hesabaparagonder(gun,g,al,tutar,a->tip);
                                printf("islem gerceklesti....\n");
                            }
                        }

                    }

                }

            case 5 :


                while(durdur5 == 1)

                    {
                        printf("Dekont ister miniz?(1-Evet  2-Hayir) :");
                        scanf("%d",&durdur5);
                        if(durdur5 == 2)
                        {
                            break;
                        }
                        tummusterileriyazdir(garanti);
                        printf("Islem Yaptiracaginiz Musteri nosunu giriniz\n");
                        int musNo;
                        scanf("%d",&musNo);
                        struct Musteri* a = NULL;
                        int i;
                        for(i=0;i<garanti->bireyselsayi;i++)
                        {
                            if(musNo == garanti->bireysel[i]->no)
                            {
                                a = garanti->bireysel[i];
                            }
                        }
                        for(i=0;i<garanti->ticarisayi;i++)
                        {
                            if(musNo == garanti->ticari[i]->no)
                            {
                                a = garanti->ticari[i];
                            }
                        }
                        if(a== NULL)
                        {
                            printf("Gecersiz Giris!!!\n");
                        } else {
                            musterininhesaplariniyazdir(a);
                            printf("Hesap ozeti yazdirilacak hesabinizin nosunu giriniz :\n");
                            int gonderen;
                            struct Hesap* g = NULL;
                            scanf("%d",&gonderen);
                            for(i=0;i<a->hesapsayisi;i++)
                            {
                                if(gonderen == a->hesaplar[i]->no)
                                {
                                    g = a->hesaplar[i];
                                }
                            }
                            if(g == NULL)
                            {
                                printf("Gecersiz Giris!!!\n");
                            } else {
                                printf("Hangi gunleri istiyorsunuz\n banka 0. gunde acildi\n mevcut gun %d\n",gun);
                                int bas;
                                int son;
                                printf("baslangic giriniz: ");
                                scanf("%d",&bas);
                                printf("bitis giriniz: ");
                                scanf("%d",&son);
                                int j;
                                FILE *dek;
                                dek=fopen("dekont.txt","w");
                                for(j=0;j<g->islemsayisi;j++)
                                {
                                    if(g->hesapozeti[j]->tarih >= bas && g->hesapozeti[j]->tarih <= son)
                                    {
                                        fputs("****Islem*******\n",dek);
                                        fprintf(dek,"gun = %d\n",g->hesapozeti[j]->tarih);
                                        fprintf(dek,"miktar = %.2f\n",g->hesapozeti[j]->miktar);

                                        printf("****Islem*******\ngun = %d\nmiktar = %.2f\n",g->hesapozeti[j]->tarih,g->hesapozeti[j]->miktar);

                                        if(g->hesapozeti[j]->tip == 0)
                                        {
                                            fputs("Para Cekildi\n",dek);
                                            printf("Para Cekildi\n");
                                        }
                                        if(g->hesapozeti[j]->tip == 1)
                                        {
                                            fputs("Para Yatirildi\n",dek);
                                            printf("Para Yatirildi\n");
                                        }
                                        if(g->hesapozeti[j]->tip == 2)
                                        {
                                            fprintf(dek,"hesap no = %d\n",g->hesapozeti[j]->hesapno);
                                            fputs("Para Gonderildi\n",dek);
                                            printf("hesap no = %d\nPara Gonderildi\n",g->hesapozeti[j]->hesapno);
                                        }
                                        if(g->hesapozeti[j]->tip == 3)
                                        {
                                            fprintf(dek,"hesap no = %d\n",g->hesapozeti[j]->hesapno);
                                            fputs("Para Geldi\n",dek);
                                            printf("hesap no = %d\nPara Geldi\n",g->hesapozeti[j]->hesapno);
                                        }
                                    }
                                }
                                fclose( dek );
                            }
                        }
                    }

            case 6 :


                while(durdur6 ==1)
                {
                    printf("Hesap kapatmak ister miniz?(1-Evet  2-Hayir) :");
                    scanf("%d",&durdur6);
                    if(durdur6 == 2)
                    {
                        break;
                    }
                    tummusterileriyazdir(garanti);
                    printf("Islem Yaptiracaginiz Musteri nosunu giriniz\n");
                    int musNo;
                    scanf("%d",&musNo);
                    struct Musteri* a = NULL;
                    int i;
                    for(i=0;i<garanti->bireyselsayi;i++)
                    {
                        if(musNo == garanti->bireysel[i]->no)
                        {
                            a = garanti->bireysel[i];
                        }
                    }
                    for(i=0;i<garanti->ticarisayi;i++)
                    {
                        if(musNo == garanti->ticari[i]->no)
                        {
                            a = garanti->ticari[i];
                        }
                    }
                    if(a== NULL)
                    {
                        printf("Gecersiz Giris!!!\n");
                    } else {
                        musterininhesaplariniyazdir(a);
                        printf("Kapatilacak hesabinizin nosunu giriniz\n");
                        int gonderen;
                        struct Hesap* g = NULL;
                        scanf("%d",&gonderen);
                        for(i=0;i<a->hesapsayisi;i++)
                        {
                            if(gonderen == a->hesaplar[i]->no)
                            {
                                g = a->hesaplar[i];
                            }
                        }
                        if(g == NULL)
                        {
                            printf("Gecersiz Giris!!!\n");
                        } else {
                        	if(g->bakiye >0)
                        	{
                        		printf("Hesabinizda para var kapatamazsiniz\n");
							} else{

	                            struct Hesap* yenihesaplar[200];
	                            int j=0;
	                            for(i=0;i<a->hesapsayisi;i++)
	                            {
	                                if(g != a->hesaplar[i])
	                                {
	                                    yenihesaplar[j]=a->hesaplar[i];
	                                    j+=1;
	                                }
	                                a->hesaplar[i] == NULL;
	                            }
	                            a->hesapsayisi = j;
	                            for(i=0;i<a->hesapsayisi;i++)
	                            {
	                                a->hesaplar[i] = yenihesaplar[i];
	                            }
	                            printf("Hesap Kapatildi\n");
	                            musterininhesaplariniyazdir(a);
                            }
                        }
                    }
                }


            default:

                bir=fopen("bireysel.txt","w");
                int i;
                fprintf(bir,"\nBireysel Musteriler (tip:0): \n");
                for(i=0;i<garanti->bireyselsayi;i++)
                {
                    fprintf(bir,"ad: %s soyad: %s dogumyeri: %s dogumtarihi: %d tip %d no:%d\n",garanti->bireysel[i]->ad,garanti->bireysel[i]->soyad,garanti->bireysel[i]->dogumyeri,garanti->bireysel[i]->dogumtarihi,garanti->bireysel[i]->tip,garanti->bireysel[i]->no);
                    fprintf(bir,"----Hesaplar:\n");
                    int j;
                    for(j = 0; j<garanti->bireysel[i]->hesapsayisi;j++)
                    {
                        fprintf(bir," ------ Hesap no: %d  Hesap bakiye: %.2f\n",garanti->bireysel[i]->hesaplar[j]->no,garanti->bireysel[j]->hesaplar[j]->bakiye);
                    }
                }
                fclose( bir );



                tic=fopen("ticari.txt","w");

                fprintf(bir,"\nTicari Musteriler (tip:0): \n");
                for(i=0;i<garanti->ticarisayi;i++)
                {
                    fprintf(tic,"ad: %s soyad: %s dogumyeri: %s dogumtarihi: %d tip %d no:%d\n",garanti->ticari[i]->ad,garanti->ticari[i]->soyad,garanti->ticari[i]->dogumyeri,garanti->ticari[i]->dogumtarihi,garanti->ticari[i]->tip,garanti->ticari[i]->no);
                    fprintf(tic,"----Hesaplar:\n");
                    int j;
                    for(j = 0; j<garanti->ticari[i]->hesapsayisi;j++)
                    {
                        fprintf(tic," ------ Hesap no: %d  Hesap bakiye: %.2f\n",garanti->ticari[i]->hesaplar[j]->no,garanti->ticari[i]->hesaplar[j]->bakiye);
                    }
                }
                fclose( tic );
                gun +=1;
                if(secim == 7)
                {
                    gun = -1;
                }
        }



    }
    return 0;
}














































































