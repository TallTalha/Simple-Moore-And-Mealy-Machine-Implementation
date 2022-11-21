#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <conio.h>
#define boyut 40

//Mealet makinesinin implementasyonu, {a,b} sembol uzay� i�in uygulanm��t�r. 
//Text dosyalar�nda bulunan tablo girdilerine g�re mealey makinesi implementasyonu yap�lmaktad�r.
//Sembol uzay�n� dinamik hale getirmek/geni�letmek i�in
// cInNext, dInNext... gibi yeni pointer olu�turmak yerine hepsini tutan bir pointer dizisi olu�tururdum. Ona g�re atamalar yapard�m.
// @dev Talha Burak Ayd�n - 191220095

char girdiler[boyut]; //Kullan�c�dan girilen girdileri saklar



typedef struct durum{
	char isim[boyut];			//qx / state'in ismi tutulur. Ba�lang�� state'i q0'd�r.
	int stateIn1;		 	//1 girildi�inde ��kt� nedir bilgisini tutar
	int stateIn0;			//0 girildi�inde ��kt� nedir bilgisini tutar
	durum * in0Next; 	// 0 inputu girildi�inde sonraki durum/qx
	durum *in1Next; 	// 1 inputu girildi�inde sonraki durum/qx
	durum * next;
}durumlar;

durumlar * ilk = NULL;
durumlar * son = NULL;

durumlar * tahsis(char _isim[boyut]){
	durumlar *bilgi = (durumlar*)malloc(sizeof(durumlar));
	
	strcpy(bilgi->isim,_isim);
	bilgi->in0Next = NULL;
	bilgi->in1Next = NULL;
	
	return bilgi;
}


void ekle(durumlar *bilgi){ // q0,q1,q2 �eklinde sadece ba�l� liste yap�s�n� olu�turmak i�in yaz�lan ekleme kodu
	if (ilk==NULL){
		ilk = bilgi;		//0 ve 1 (in0Next,in1Next) giri�leri ile ba�lant�s� yoktur sadece ba�l� liste yaps�n� korur.
		son = bilgi;	
	}
	else{
		son->next = bilgi;
		son = bilgi;
	}
}

durumlar * ara(char _isim[boyut]) { //parametre olarak ismi verilen state'i arar ve return eder.
	durumlar *gecici,*onceki;
	gecici = ilk;
	onceki=NULL;
	do{
		if(strcmp(gecici->isim, _isim) == 0) break;
		onceki=gecici;
		gecici=gecici->next;
		
	}
	while(gecici!=ilk);
	
	return gecici;
}

void txtOku(){	//Tablo okunur ve bu tablodaki kurallar ile 0 ve 1 outputlar� belirlenir.
	FILE* fp;
	
	durumlar *bilgi;
	char _isim[boyut];
	char after0[boyut];
	char after1[boyut];
	int _stateIn0,_stateIn1;
	
	fp = fopen("mealyINPUT.txt", "r");
	
	printf("INPUT TXT OKUNUYOR...");
	while(!feof(fp)){			// DURUMLARIN �SM� B�LG�S� �LE BERABER MEMORYDE YER TAHS�S� YAPILIR
		fscanf(fp,"%s",_isim);
		bilgi = tahsis(_isim);
		ekle(bilgi);
		printf("\n INPUT %s",_isim);
	}
	printf("\nGECIS TABLOSU TXT OKUNUYOR...");
	fp = fopen("mealyGECISTABLOSU.txt", "r"); // 0 VE 1 G�R��LER�NDEN SONRA G�D�LECEK ADRESLER�N ATAMASI
	bilgi = ilk;
	printf("\n	 ----------------     AFTER INPUT 0          ||             AFTER INPUT 1");
	printf("\n	 OLD STATE->  NEW  STATE    ::    OUTPUT     ||       NEW STATE   ::        OUTPUT");
	while(!feof(fp)){
		fscanf(fp, "%s	%d,%s	%d",after0,&_stateIn0,after1,&_stateIn1);
			
		bilgi->stateIn0 = _stateIn0;
		bilgi->stateIn1 = _stateIn1;
		bilgi->in0Next = ara(after0);
		bilgi->in1Next = ara(after1);
		printf("\n	   %s 	  ->  %s            ::      %d        ||       %s          ::          %d",bilgi->isim,after0,_stateIn0,after1,_stateIn1);
		bilgi = bilgi->next;
		
	}
	
	printf("\nOKUMALAR BITTI!\n");
}

void moore(){ //txtOku k�sm�nda okunan kurallar sayesinde burada ko�ul bloklar�n� kullanarak istenilen 0 veya 1 ��kt�s� ekrana bast�r�rl�r.

	durumlar *gezici;
	gezici=ilk;
	
	for(int i=0; i<strlen(girdiler) ;i++){
		
		if(girdiler[i]=='0'){
			printf("%d",gezici->stateIn0);
			gezici = gezici->in0Next;
			
			
		}
		else{
			printf("%d",gezici->stateIn1);
			gezici = gezici->in1Next;
			
			
		}
	}
	
	//q1,q2,q1,q3,q2... �eklinde ekrana bast�rma yapmak i�in  kodu kopyalad�m
	gezici=ilk;
	printf("\nq0->");
	for(int i=0; i<strlen(girdiler) ;i++){
		
		if(girdiler[i]=='0'){
			gezici = gezici->in0Next;
			printf("%s->",gezici->isim);
			
		}
		else{
			gezici = gezici->in1Next;
			printf("%s->",gezici->isim);
			
		}
	}
	
	

}


void sifirla(char _dizi[boyut]){ //Parametre olarak girilen diziyi sifirlar
	for(int a=0; a<=strlen(_dizi);a++){
		_dizi[a] = NULL;
	}
}
void listele(char _dizi[boyut]){ //Parametre olarak girilen dizinin elemanlar�n� ekrana basar

	for(int a=0; a<=strlen(_dizi);a++){
		printf("%c",_dizi[a]);
	}
	printf("\n");
}

//KURAL TABLOSUNA G�RE DURUMLARIN BA�LANTISI VE 0,1 G�RD�S�NE G�RE �IKTILARI BEL�RLEND� 
// �RNEK G�RD� :::: 110010110
// �RENK �IKTI :::: 111011110
// q0->q2->q1->q2->q3->q1->q2->q1->q0->q2->

int main(){
	
	char secim; 
	printf("0 veya 1 sembolunu iceren girdinizi yazin...\nOrnek girdi :: 00000111011\nSizin girdiniz... ");
	scanf("%s",girdiler);
	txtOku();
	printf("\nOkunan kurallar neticesinde ciktilar...\n");
	printf("Girdikleriniz::");
	listele(girdiler);
	printf("Ciktilar.....::");
	moore();
			
	return 0;
}
