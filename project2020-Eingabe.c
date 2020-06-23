#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <string.h>
#define MAX_LINE_SIZE 256

short funcEingabedateiTest(char *chDateiname);
short funcEingabe(char *chDateiname,short *npErstezeile,short *npZeit, short *npZielgeschwindigkeitvf);

int main()
{
	setbuf(stdout, NULL);

	char chDateiname[]="Eingabe.txt";

	short nTestergebnis =(funcEingabedateiTest(chDateiname)-1);
	short nArrErstezeile[5];/*Anfangsgeschwindigkeit ve //Anfangsgeschwindigkeit vf //Anfangsabstand //ART Status //Zielgeschwindigkeit ve //Zielgeschwindigkeit vf*/
	short nArrZeit[]={};
	short nArrZielgeschwindigkeitvf[]={};

	funcEingabe(chDateiname,nArrErstezeile,nArrZeit,nArrZielgeschwindigkeitvf);

	for(int i=0;i<5;i++){
		printf("%hd ",nArrErstezeile[i]);
	}

	printf("\n");

	for(int j=0;j<nTestergebnis;j++){
		printf ("%hd ",nArrZeit[j]);
		printf ("%hd ",nArrZielgeschwindigkeitvf[j]);
		printf("\n");
	}

	printf("%hd ",nArrErstezeile[-1]);
	printf("\n");
	printf("%hd ",nArrZeit[-1]);
	printf("\n");
	printf("%hd ",nArrZielgeschwindigkeitvf[-1]);


	getchar();
	return 0;
}

short funcEingabe(char *chDateiname,short *npErstezeile,short *npZeit, short *npZielgeschwindigkeitvf){
	FILE *fp;
	char line[MAX_LINE_SIZE];
	short nleerzeichen =0;
	short ni=0;
	if (funcEingabedateiTest(chDateiname)!=0){
		fp = fopen(chDateiname,"r");
		while (fgets(line, sizeof(line), fp))
		{
			if (line[0]!= '#')
			{
				nleerzeichen = 0;
				for (int i = 0; line[i] != '\n'; i++)
				{
					if(line[i]=='\n'|| line[i]=='\0'){
						break;
					}
					if (line[i] == ' '){
						nleerzeichen++;
					}
				}
				switch(nleerzeichen)
				{
				case 4:
					sscanf(line, "%hd %hd %hd %hd %hd",&npErstezeile[0],&npErstezeile[1],&npErstezeile[2],&npErstezeile[3],&npErstezeile[4]);
					printf("%hd %hd %hd %hd %hd",npErstezeile[0],npErstezeile[1],npErstezeile[2],npErstezeile[3],npErstezeile[4]);
					printf("\n");
					break;
				case 1:
					sscanf(line,"%hd %hd", &npZeit[ni],&npZielgeschwindigkeitvf[ni]);
					printf("%hd %hd", npZeit[ni],npZielgeschwindigkeitvf[ni]);
					printf("\n");
					ni++;
					break;
				}
			}
		}
	}
	fclose(fp);
	return 1;
}

short funcEingabedateiTest(char *chDateiname){
	char line[MAX_LINE_SIZE];
	short nKommentare;
	short nZeilen;
	short nleerzeichen =0;

	FILE *fp;
	fp = fopen(chDateiname,"r");
		if (fp == NULL){
			printf("Fehler1");
				return 0;
				fclose(fp);
		}
		else{
			nKommentare=0;
			nZeilen=0;
			while (fgets(line, sizeof(line), fp)){
				if (line[0] == '#'){
						nKommentare++;
						for (int x=1; x!=MAX_LINE_SIZE; x++){
							if(line[x]=='\n'|| line[x]=='\0'){
								break;
							}
							if  (line[x]== '#'){
								printf("Fehler2");
								fclose(fp);
								return 0;
							}
						}
					}
				else if (line[0]>=48 && line[0]<=57){
						nleerzeichen=0;
						for (int j = 0; j !=MAX_LINE_SIZE; j++){
							if(line[j]=='\n'|| line[j]=='\0'){
								if ((nZeilen==0 ) && nleerzeichen !=4){
									printf("Fehler3");
									fclose(fp);
									return 0;
								}
								if ((nZeilen!=0) && nleerzeichen !=1){
									printf("Fehler4");
									fclose(fp);
									return 0;
									}
								nZeilen++;
								break;
							}
							if (line[j]==' '){
								nleerzeichen++;
							}
							if (((line[j]<48 || line[j]>57) && line[j] !=32) && line[j]!='\0'){
								printf("Fehler5");
								fclose(fp);
								return 0;
							}
							}
					}
					else{
						printf("Fehler6");
						fclose(fp);
						return 0;
					}
				}
		}
return nZeilen;
}
