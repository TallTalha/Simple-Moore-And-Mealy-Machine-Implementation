#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <conio.h>
#define boyut 40

//Moore makinesinin implementasyonu, {a,b} sembol uzay� i�in uygulanm��t�r. 
//Text dosyalar�nda bulunan tablo girdilerine g�re moore makinesi implementasyonu yap�lmaktad�r.
//Sembol uzay�n� dinamik hale getirmek/geni�letmek i�in
// cInNext, dInNext... gibi yeni pointer olu�turmak yerine hepsini tutan bir pointer dizisi olu�tururdum. Ona g�re atamalar yapard�m.
// @dev Talha Burak Ayd�n - 191220095

char girdiler[boyut]; //Kullan�c�dan girilen girdileri saklar



typedef struct durum{
	char isim[boyut];		//qx / statein ismi tutulur. Ba�lang�� state'i q0'd�r.
	int state;		//  ��kt� de�erleri tutulur
	durum * aInNext; // a inputu girildi�inde sonraki durum/qx
	durum * bInNext; // b inputu girildi�inde sonraki durum/qx
	durum * next;
}durumlar;

durumlar * ilk = NULL;
durumlar * son = NULL;

durumlar * tahsis(char _isim[boyut]){
	durumlar *bilgi = (durumlar*)malloc(sizeof(durumlar));
	
	strcpy(bilgi->isim,_isim);
	bilgi->aInNext = NULL;
	bilgi->bInNext = NULL;
	
	return bilgi;
}


void ekle(durumlar *bilgi){ // q0,q1,q2 �eklinde sadece ba�l� liste yap�s�n� olu�turmak i�in yaz�lan ekleme kodu
	if (ilk==NULL){
		ilk = bilgi;		//a ve b (aInNext,bInNext) giri�leri ile ba�lant�s� yoktur sadece ba�l� liste yaps�n� korur.
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
	char afterA[boyut];
	char afterB[boyut];
	int _state;
	
	fp = fopen("mooreINPUT.txt", "r");
	
	printf("INPUT TXT OKUNUYOR...");
	while(!feof(fp)){			// DURUMLARIN �SM� B�LG�S� �LE BERABER MEMORYDE YER TAHS�S ED�LMES�
		fscanf(fp,"%s",_isim);
		bilgi = tahsis(_isim);
		ekle(bilgi);
		printf("\n INPUT %s",_isim);
	}
	printf("\nGECIS TABLOSU TXT OKUNUYOR...");
	printf("\nOLD STATE ->  After Input a  ::   Affter Input b ");
	fp = fopen("mooreGECISTABLOSU.txt", "r"); // A VE B G�R��LER�NDEN SONRA G�D�LECEK ADRESLER�N ATAMASI
	bilgi = ilk;
	while(!feof(fp)){
		fscanf(fp, "%s %s",afterA,afterB);
		bilgi->aInNext = ara(afterA);
		bilgi->bInNext = ara(afterB);
		printf("\n   %s     ->        %s       ::         %s",bilgi->isim,afterA,afterB);
		bilgi = bilgi->next;
		
	}
	
	fp = fopen("mooreOUTPUT.txt", "r"); // OUTPUT DOSYASININ OKUNMASI
	printf("\nOUTPUT TXT OKUNUYOR...\n");
	printf("\n||Output Table||\n");
	bilgi = ilk;
	while(!feof(fp)){
		
		fscanf(fp,"%d",&_state);
		
		bilgi->state = _state;
		printf("||%s output: %d||\n",bilgi->isim,_state);
		bilgi = bilgi->next;
		
	}
	printf("\nOKUMALAR BITTI!\n");
}

void moore(){ //txtOku k�sm�nda okunan kurallar sayesinde burada ko�ul bloklar�n� kullanarak istenilen 0 veya 1 ��kt�s� ekrana bast�r�rl�r.

	durumlar *gezici;
	gezici=ilk;
	
	for(int i=0; i<strlen(girdiler) ;i++){
		
		if(girdiler[i]=='a'){
			gezici = gezici->aInNext;
			printf("%d",gezici->state);
			
		}
		else{
			gezici = gezici->bInNext;
			printf("%d",gezici->state);
			
		}
	}
	
	//q1,q2,q1,q3,q2... �eklinde ekrana bast�rma yapmak i�in  kodu kopyalad�m
	gezici=ilk;
	printf("\nq0->");
	for(int i=0; i<strlen(girdiler) ;i++){
		
		if(girdiler[i]=='a'){
			gezici = gezici->aInNext;
			printf("%s->",gezici->isim);
			
		}
		else{
			gezici = gezici->bInNext;
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

//KURAL TABLOSUNA G�RE BU KEL�ME OKUNDU�UNDA EKRANA 1 BASACAK ::::: aab 
// �RNEK G�RD� :::: aaababbaabb
// �RNEK �IKTI :::: 00010000010
// q0 -> q1->q2->q2->q3->q1->q0->q0->q1->q2->q3->q0->

int main(){
	
	char secim; 
	printf("a veya b sembollerini iceren girdinizi yazin...\nOrnek girdi :: aaababbaabb\nSizin girdiniz... ");
	scanf("%s",girdiler);
	txtOku();
	printf("\nOkunan kurallar neticesinde ciktilar...\n");
	listele(girdiler);
	moore();
	
			
	return 0;
}
