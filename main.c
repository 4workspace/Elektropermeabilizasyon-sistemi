#include <main.h>
#use delay(clock=4000000)
#fuses XT,NOWDT,NOPUT,NOLVP,NOCPD,NOPROTECT,NODEBUG,NOBROWNOUT,NOWRT
#define use_portb_lcd TRUE  // LCD  portB ye  baðlanýyor
#include <lcd.c>

#use fast_io(a)
#use fast_io(b)
#use fast_io(c)


#define desarj_button pin_a3;
#define button_800V pin_a4;
#define button_1200V pin_a5;
#define button_1500V pin_e0;


void volt_800();
void volt_1200();
void volt_1500();
void desarj();

#define kaplan pin_d0
#define MOD pin_a0
#define arttir pin_a1
#define azalt pin_a2
unsigned long int sayi, adet=10, i, duty=50, v2, v1, x, z=0;



void main()
{
   setup_adc_ports(NO_ANALOGS);
   setup_adc(ADC_OFF);
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   
   sayi=400;
   lcd_init();
   set_tris_a(0x07);
   set_tris_b(0x00);
   set_tris_d(0x00);
   set_tris_e(0x01);
   
   output_b(0x00);
   output_d(0x00);
   
   printf(lcd_putc, " \f   HOSGELDINIZ  \n");
   delay_ms(1000);

   
   printf(lcd_putc, "\fELEKTROPORATOR   \n");
   printf(lcd_putc, "BITIRME ODEVI ");
   delay_ms(1000);
   printf(lcd_putc, "\f");  

   
   while(1)
   {
      lcd_putc("\fBIR ISLEM SECIN!");

      if(input(pin_a4) & (!(input(pin_a5))) & (!(input(pin_e0))))
         volt_800();
      delay_ms(100);
      
      if(!(input(pin_a4)) & (input(pin_a5)) & (!(input(pin_e0))))
         volt_1200();
      delay_ms(100);
      
      if(!(input(pin_a4)) & (!(input(pin_a5))) & (input(pin_e0)))
         volt_1500();
      delay_ms(100);
      
      if(input(pin_a3))
         desarj();
     delay_ms(100); 
   }
}



void volt_800()
{
   output_low(pin_D6);
   
      while (TRUE)  // ANA PROGRAMA GÝRÝYOR
   {
      lcd_putc("\f800 VOLT AKTIF\n");
      delay_ms(500);
      lcd_putc("MOD ile set yap");
     while(input(MOD)) // PERÝYOD DEÐERÝ 100 MS VE ÜZERÝ
      {   

         if (input(arttir)&(sayi<1000))
         {
            sayi=sayi+50;
            while(input(arttir));
            printf(lcd_putc, "\f800V ICIN\nP. UZUNLUGU:%lums ",sayi);
         }
         
         if(input(arttir)&(sayi>=1000))
         {
            sayi=sayi+100;
            while(input(arttir));
            printf(lcd_putc,"\f800V ICIN\nP. UZUNLUGU: %lums",sayi); 
         }
         
         if(input(azalt)&(sayi!=100)&(sayi<1000))
         {
            sayi=sayi-50;
            while(input(azalt));
            printf(lcd_putc, "\f800V ICIN\nPERYOT SURESI: %lums",sayi);  
         }
         
         if(input(azalt)&(sayi!=100)&(sayi>=1000))
         {
            sayi=sayi-100;
            while(input(azalt));
            printf(lcd_putc,"\f800V ICIN\nP. UZUNLUGU:%lums",sayi);  
         }
      }
         
         
     delay_ms(500); // ADET BELIRLENECEK
     printf(lcd_putc,"\f800V ICIN\nPERYOT SAYISI:%lu", adet);
         
     while(input(MOD))// ADET DÖNGÜSÜ 1-20 ARASINDA
      {
            if(input(arttir)&(adet<1000))
            {
               adet=adet+10;
               while(input(arttir));
               printf(lcd_putc,"\f800V ICIN\nP. SAYISI:%lu",adet);  
            }
            
            if(input(azalt)&(adet!=0))
            {
               adet=adet-10;
               while(input(azalt));
               printf(lcd_putc,"\f800V ICIN\nP. SAYISI:%lu",adet);
            }
            
      }
         
      delay_ms(500); // DUTY BELÝRLENECEK
      printf(lcd_putc,"\f800V ICIN\nDUTY CYCLE:%lu%%",duty);
      
     while(input(MOD)) // DUTY 10-90 ARSINDA
      {
         if(input(arttir)&(duty<=90))
         {
            duty=duty+10;
            while(input(arttir));
            printf(lcd_putc,"\f800V ICIN\nDUTY CYCLE:%lu%%",duty);  
         }
         
         if(input(azalt)&(duty!=10))
         {
            duty=duty-10;
            while(input(azalt));
            printf(lcd_putc,"\f800V ICIN\nDUTY CYCLE:%lu%%",duty);         
         }
         
     }
     
     delay_ms(1000);
     printf(lcd_putc,"\fPERiOD=%lu ms\nQUAN=%lu DUTY=%lu",sayi,adet,duty);
     delay_ms(3000);
         
     x=sayi/100;    // toplam pulse süresi sadeleþtiriliyor
     v1=x*duty;     // pulse in aktif olma  süresi balirleniyor
     v2=sayi-v1;    // pulse in pasif olduðu süre
       
        
          while(z<5)
     {
            printf(lcd_putc,"\fDARBE YUKLENIYOR");
            delay_ms(200);
            z=z+1;
     }
     z=0;
     
     printf(lcd_putc,"\fDARBE UYGULANYOR!!");
     for(i=1;i<=adet;i++)
      {
            output_high(pin_D6);
            delay_us(v1);
            output_low(pin_D6);
            delay_us(v2);
      }
      
      printf(lcd_putc,"\fDARBE UYGULANDI");
      delay_ms(1000);
      break;
   }
}

void volt_1200()
{
   output_low(pin_D5);
      while (TRUE)  // ANA PROGRAMA GIRIYOR
   {
      lcd_putc("\f1200 VOLT AKTIF\n");
      delay_ms(500);
      lcd_putc("MOD ile set yap");

     while(input(MOD)) // PERÝYOD DEÐERÝ 100 MS VE ÜZERÝ
      {   

         if (input(arttir)&(sayi<1000))
         {
            sayi=sayi+50;
            while(input(arttir));
            printf(lcd_putc, "\f1200V ICIN\nP. UZUNLUGU:%lums ",sayi);
         }
         
         if(input(arttir)&(sayi>=1000))
         {
            sayi=sayi+100;
            while(input(arttir));
            printf(lcd_putc,"\f1200V ICIN\nP. UZUNLUGU: %lums",sayi); 
         }
         
         if(input(azalt)&(sayi!=100)&(sayi<1000))
         {
            sayi=sayi-50;
            while(input(azalt));
            printf(lcd_putc, "\f1200V ICIN\nPERYOT SURESI: %lums",sayi);  
         }
         
         if(input(azalt)&(sayi!=100)&(sayi>=1000))
         {
            sayi=sayi-100;
            while(input(azalt));
            printf(lcd_putc,"\f1200V ICIN\nP. UZUNLUGU:%lums",sayi);  
         }
      }
         
         
     delay_ms(500); // ADET BELIRLENECEK
     printf(lcd_putc,"\f1200V ICIN\nPERYOT SAYISI:%lu", adet);
         
     while(input(MOD))// ADET DÖNGÜSÜ 1-20 ARASINDA
      {
            if(input(arttir)&(adet<1000))
            {
               adet=adet+10;
               while(input(arttir));
               printf(lcd_putc,"\f1200V ICIN\nP. SAYISI:%lu",adet);  
            }
            
            if(input(azalt)&(adet!=0))
            {
               adet=adet-10;
               while(input(azalt));
               printf(lcd_putc,"\f1200V ICIN\nP. SAYISI:%lu",adet);
            }
            
      }
         
      delay_ms(500); // DUTY BELÝRLENECEK
      printf(lcd_putc,"\f1200V ICIN\nDUTY CYCLE:%lu%%",duty);
      
     while(input(MOD)) // DUTY 10-90 ARSINDA
      {
         if(input(arttir)&(duty<=90))
         {
            duty=duty+10;
            while(input(arttir));
            printf(lcd_putc,"\f1200V ICIN\nDUTY CYCLE:%lu%%",duty);  
         }
         
         if(input(azalt)&(duty!=10))
         {
            duty=duty-10;
            while(input(azalt));
            printf(lcd_putc,"\f1200V ICIN\nDUTY CYCLE:%lu%%",duty);         
         }
         
     }
     
     delay_ms(1000);
     printf(lcd_putc,"\fPERiOD=%lu ms\nQUAN=%lu DUTY=%lu",sayi,adet,duty);
     delay_ms(3000);
         
     x=sayi/100;    // toplam pulse süresi sadeleþtiriliyor
     v1=x*duty;     // pulse in aktif olma  süresi balirleniyor
     v2=sayi-v1;    // pulse in pasif olduðu süre
       
        
          while(z<5)
     {
            printf(lcd_putc,"\fDARBE YUKLENIYOR");
            delay_ms(200);
            z=z+1;
     }
     z=0;
     
     printf(lcd_putc,"\fDARBE UYGULANIYOR");
     for(i=1;i<=adet;i++)
      {     
            output_high(pin_D5);
            delay_us(v1);           
            output_low(pin_D5);
            delay_us(v2);
      }
      printf(lcd_putc,"\fDARBE UYGULANDI");
      delay_ms(1000);
      break;
   }
}

void volt_1500()
{
   output_low(pin_D4);
   
      while (TRUE)  // ANA PROGRAMA GÝRÝYOR
   {
      lcd_putc("\f1500 VOLT AKTIF\n");
      delay_ms(500);
      lcd_putc("MOD ile set yap");
     while(input(MOD)) // PERÝYOD DEÐERÝ 100 MS VE ÜZERÝ
      {   

         if (input(arttir)&(sayi<1000))
         {
            sayi=sayi+50;
            while(input(arttir));
            printf(lcd_putc, "\f1500V ICIN\nP. UZUNLUGU:%lums ",sayi);
         }
         
         if(input(arttir)&(sayi>=1000))
         {
            sayi=sayi+100;
            while(input(arttir));
            printf(lcd_putc,"\f1500V ICIN\nP. UZUNLUGU: %lums",sayi); 
         }
         
         if(input(azalt)&(sayi!=100)&(sayi<1000))
         {
            sayi=sayi-50;
            while(input(azalt));
            printf(lcd_putc, "\f1500V ICIN\nPERYOT SURESI: %lums",sayi);  
         }
         
         if(input(azalt)&(sayi!=100)&(sayi>=1000))
         {
            sayi=sayi-100;
            while(input(azalt));
            printf(lcd_putc,"\f1500V ICIN\nP. UZUNLUGU:%lums",sayi);  
         }
      }
         
         
     delay_ms(500); // ADET BELIRLENECEK
     printf(lcd_putc,"\f1500V ICIN\nPERYOT SAYISI:%lu", adet);
         
     while(input(MOD))// ADET DÖNGÜSÜ 1-20 ARASINDA
      {
            if(input(arttir)&(adet<1000))
            {
               adet=adet+10;
               while(input(arttir));
               printf(lcd_putc,"\f1500V ICIN\nP. SAYISI:%lu",adet);  
            }
            
            if(input(azalt)&(adet!=0))
            {
               adet=adet-10;
               while(input(azalt));
               printf(lcd_putc,"\f1500V ICIN\nP. SAYISI:%lu",adet);
            }
            
      }
         
      delay_ms(500); // DUTY BELÝRLENECEK
      printf(lcd_putc,"\f1500V ICIN\nDUTY CYCLE:%lu%%",duty);
      
     while(input(MOD)) // DUTY 10-90 ARSINDA
      {
         if(input(arttir)&(duty<=90))
         {
            duty=duty+10;
            while(input(arttir));
            printf(lcd_putc,"\f1500V ICIN\nDUTY CYCLE:%lu%%",duty);  
         }
         
         if(input(azalt)&(duty!=10))
         {
            duty=duty-10;
            while(input(azalt));
            printf(lcd_putc,"\f1500V ICIN\nDUTY CYCLE:%lu%%",duty);         
         }
         
     }
     
     delay_ms(1000);
     printf(lcd_putc,"\fPERIOD = %lu ms\nQUAN=%lu DUTY=%lu",sayi,adet,duty);
     delay_ms(3000);
         
     x=sayi/100;    // toplam pulse süresi sadeleþtiriliyor
     v1=x*duty;     // pulse in aktif olma  süresi balirleniyor
     v2=sayi-v1;    // pulse in pasif olduðu süre
       
        
          while(z<5)
     {
            printf(lcd_putc,"\fDARBE YUKLENIYOR");
            delay_ms(200);
            z=z+1;
     }
     z=0;
     
     printf(lcd_putc,"\fDARBE UYGULANIYOR");
     for(i=1;i<=adet;i++)
      {     
            output_low(pin_D4);
            delay_us(v2);
            output_high(pin_D4);
            delay_us(v1);
      }
      printf(lcd_putc,"\fDARBE UYGULANDI.");
      delay_ms(1000);
      break;
   }
}

void desarj()
{
   lcd_putc("\fDESARJ BASLADI\nLUTFEN BEKLEYIN!");
   output_high(pin_D7);
   delay_ms(3000);
   output_low(pin_D7);
   lcd_putc("\fKAPASITORLAR\nDESARJ EDILDI...");
   delay_ms(2000);
   break;
}
