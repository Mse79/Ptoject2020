/*
 * ProjektGruppeB1.c
 *
 *  Created on:
 *      Author: Gruppe B-1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <conio.h>
#include <Windows.h>

#define ACCEL 1.5														/*Maxwert für Beschleunigen = 1.5m/s*/
#define BRAKE 5.0														/*Maxwert für Bremsen = 5m/s*/
#define BRAKEFF 3.0														/*Beschleunigung für Bremsen des fremden Fahrzeugs = 3.0m/s²*/
#define UNTEREGRENZE 30													/*Untere Grenze für ART = 30km/h*/
#define OBEREGRENZE 130													/*Obere Grenze für ART = 130km/h*/
#define SEGELN 0.05														/*Verzögerungswert für Segeln (BP==0 && AP==0)*/
#define MAX_LINE_SIZE 256


void Startbildschirm();
char Pfadeingabedatei(char*);
char Pfadausgabedatei(char*);
void Eingabedatei();
void FremdesFahrzeug();
void Abstand();
void EigenesFahrzeug();
void HMIeingabe();
void HMIausgabe();
void ARTemp();
void Ausgabe();
void Abschluss();
void funcWahrung();
void Ausgabeheader();

int main()
{
	setbuf(stdout, NULL);

	char input[50] = "";
	char output[50] = "";
	char chHit = 0;
	/*Variablen Deklaration und Initialisierung*/
	/* ve = Eigene Geschwindigkeit (km/h), vf = Geschwindigkeit fremdes Fahrzeug (km/h), t = Zeit ab Simulationsbeginn(s), ART = Status ART 1/0,
	 * ZART = Zielgeschwindigkeit ART (kmh/h), vfz = Zielgeschwindigkeit fremdes Fahrzeug (km/h)
	 */
	short ve = 80, vf = 95,  t = 100, ART = 1, ZART = 100, vfz = 100, Quit = 0;
	/*
	 * s = zurückgelegte Strecke Eigenes Fahrzeug (km), d = Abstand, vfm = Geschwindigkeit fremdes Fahrzeug (m/s), vem = Geschwindigkeit eigenes Fahrzeug (m/s)
	 * sf = zurückgelegte Strecke fremdes Fahrzeug (x,xx m), se = zurückgelegte Strecke eigenes Fahrzeug (x,xx m), total_t = Gemessene Zeit ab Simulationsbeginn(ms),
	 * start_t = start Messung, end_t = ende Messung, AP = Gaspedalstellung (0.0<AP<1.0), BP = Bremspedalstellung (0.0<BP<1.0), delta = Zeitunterschied, vfmz = Zielgeschwindigkeit fremdes Fahrzeug
	 * vfs = Startgeschwindigkeit fremdes Fahrzeug
	 */
	float s = 0.0, d = 60.0, vfm = 0.0, vem = 0.0, sf = 0.0, se = 0.0, total_t = 0.0, start_t = 0.0, end_t = 0.0, AP = 0.0, BP = 0.0, delta = 0.0, vfmz = 0.0, vfs = 0.0, ves = 0.0, ZARTm = 0.0, dist = 0.0, distdiff = 0.0;

	/*Umrechnung der eingelesenen Geschwindigkeiten in m/s*/
	if(total_t == 0){
	vem = ve/3.6;
	vfm = vf/3.6;
	vfs = vf/3.6;
	vfmz = vfz/3.6;
	ves = ve/3.6;
	ZARTm = ZART/3.6;
	dist = d;
	}

	                                           /*--------------------------------------Funktionen--------------------------------------*/

	Startbildschirm();											/*Aufruf des Startbildschirms*/
	//Pfadeingabedatei(input);									/*Funktion für die manuelle Eingabe des Dateipfads der Eingabedatei*/
	//Pfadausgabedatei(output);									/*Funktion für die manuelle Eingabe des Dateipfads der Ausgabedatei*/
	Ausgabeheader( &output );
	//	Eingabedatei();												/*Einlesen der Eingabedatei*/





  start_t = clock();												/*!!!!!Richtige Schleife!!!!*/
	while( t >= total_t || Quit == 0){                                     //	while(t != EOF){
		end_t = clock();                                       //	end_t = clock();											/*Ende der Zeitmessung*/
		total_t = (( end_t - start_t ) / CLOCKS_PER_SEC );     //		total_t = (( end_t - start_t ) / CLOCKS_PER_SEC );			/*Berechnung der verstrichenen Zeit*/
		delta = total_t-delta;                                 //   delta = total_t-delta;

	/*Runden der Geschwindigkeit auf 2 Nachkommastellen, dies sollte noch eine Ausreichende Präzision bieten,
	 *verhindert aber ein Pendeln der Geschwindigkeit um den Zielwert*/
		vfm = floor( vfm * 100.0 + 0.5 ) / 100.0;
		vfmz = floor( vfmz * 100.0 + 0.5 ) / 100.0;

		/*------------------------Test Bildschrimausgabe, nur für Testzwecke, kann bei Einbindung des HMI evtl entfernt werden------------------------*/

	printf("Zeit:%.3lf sekunden\tzurueckgelegter Weg: %.3fm\tGeschwindigkeit:%.5f m/s\n\n", total_t, sf, vfm);
	printf("Gaspedal: %.1f Bremspedal: %.1f ART: %hd Zielgeschwindigkeit: %hd", AP , BP, ART, ZART);



	distdiff = sf - se;
	FremdesFahrzeug( &t, &sf, &vfm, &vfmz, &total_t, &delta, &vfs );											/*Aufruf funktion für eremdes Fahrzeug*/
	Abstand( &dist , &distdiff );																					/*Aufruf funktion für Abstandsberechnung*/
	EigenesFahrzeug( &se, &vem, &AP, &BP, &total_t, &delta, &ves );												/*Aufruf funktion für eigenes Fahrzeug*/


		if (kbhit())
		{
			chHit = getch();																					/*Ändern!!! so dass nicht mehr auf tasteneingabe gewartet werden muss!!!!*/


	HMIeingabe( &AP, &BP, &ART, &ZART, &Quit, &chHit);															/*Aufruf für die Funktion der HMI eingabe*/
	}
	if(ART==1){																									/*Prüfung ob ART aktiviert*/
		ARTemp( &d, &ve, &AP, &BP, &vem, &ZARTm);																								/*Aufruf funktion für ART*/
	}

	//HMIausgabe(!!!!!!!!!!!!!!!!!!);																			/*Aufruf für die Funktion der HMI ausgabe*/
	Ausgabe( &total_t, &s, &ve, &vf, &ART, &ZART, &output);														/*Aufruf funktion für Ausgabe*/
	}

//	Abschluss();																								/*Aufruf der Abschluss ausgabe am Bildschirm*/
	getchar();
	return 0;
}


void Startbildschirm(){
	/*Im Startbildschirm erfolgt eine Begrüßung und darauf eine Beschreibung des Programms,
	 *der Benutzer wird mit der Bedienung des Programms vertraut gemacht
	 *
	 */

	printf("\n\n\t\t\t************SIMULATION ABSTANDSREGELTEMPOMAT************\n\n");
	Sleep(2000);
	system("cls");
	printf("\n\n\t\t\tWillkommen zur Simulation des Abstandregeltempomats!\n\n\n");
	Sleep(1000);
	printf("Ein vorausfahrendes Fahrzeug wird anhand von Parametern in einer Eingabedatei gesteuert\n");
	printf("Sie k\x94nnen ihr eigenes Fahrzeug selbst steuern oder den Abstandsregeltempomat verwenden\n");
	printf("Ihre eigene Geschwindigkeit kann sich zwischen 0km/h und 130km/h frei bewegen, die Geschwindigkeit des vorausfahrenden\nFahrzeugs kann bis zu 250km/h betragen\n");
	printf("Es kann bei Fehlverhalten zur Kollision kommen, was zum sofortigen Abbruch der Simulation f\x81hrt\n");
	printf("Informationen w\x84hrend der Simulation erhalten Sie durch die Bildschirmausgabe des HMI\n");
	printf("Die Datenaufzeichnung erfolgt in eine, zu Beginn der Simulation angelegten Text Datei und gibt die wichtigen Parameter \nwieder\n");
	Sleep(5000);
	printf("\t\t...zum Fortfahren beliebige Taste bet\x84tigen!\n");
	getchar();
	system("cls");
	printf("\n\n\t\tZur Bedienung der Simulation werden folgende Tasten ben\x94tigt:\n\n");
	Sleep(500);
	printf("\t|| a  -|- Aktivierung/Deaktivierung des ART\n\t|| w  -|- Gas +10%%\n\t|| s  -|- Gas -10%%\n\t|| b  -|- Bremse +10%%\n\t|| n  -|- Bremse -10%%\n\t|| m  -|- Bremse Komplett l\x94sen\n\t|| +  -|- Zielgeschwindigkeit +10km/h\n\t||");
	printf(" -  -|- Zielgeschwindigkeit -10km/h\n\t|| q  -|- Simulation verlassen\n\n");
	Sleep(1000);
	printf("\t\t...zum Fortfahren beliebige Taste bet\x84tigen!\n");
	getchar();
	system("cls");
}
void FremdesFahrzeug( float *t, float *sf, float *vfm, float *vfmz, float *total_t, float *delta, float *vfs ){
	/*Funktion FremdesFahrzeug vergleicht die Geschwindigkeiten "vfm" und "vfmz" und bestimmt daraus
	 *ob das Fahrzeug beschleunigt, abbremst oder die Geschwindigkeit konstant hält.
	 *Desweiteren wird die zurückgelegte Wegstrecke "sf" und die aktuelle Geschwindigkeit "vfm" des fremden Fahrzeugs berechnet und in den Variablen geändert.
	 */
	/*Berechnung von t² aus "delta" für die Wegstrecke "sf" bei Beschleunigung bzw Abbremsen*/
		float sqr_t = 0;
		sqr_t = (*delta) * (*delta);

	/*Berechnet die Wegstecke "sf" und die Geschwindigkeit "vfm" bei Beschleunigung*/
		if( *vfmz > *vfm ){																									/*Wenn vfmz > vfm*/
			*sf = ( ACCEL / 2 ) * ( sqr_t ) + ( *vfs * ( *total_t ));
			*vfm = ( ACCEL * ( *delta )) + *vfs;
					}
	/*Berechnet die Wegstecke "sf" und die Geschwindigkeit "vfm" bei Abbremsen*/
		else{
			if( *vfmz < *vfm ){
				*sf = ( - ( ACCEL / 2 ) * ( sqr_t )) + ( *vfs * ( *total_t ));												/*Wenn vfmz < vfm*/
				*vfm = ( - ACCEL * ( *delta )) + *vfs;
						}
	/*Geschwindigkeit halten*/
			else{																											/*Wenn vfmz == vfm*/
				/**vfs = *vfm;*/
				*sf = ( *vfs * ( *total_t )) + (( *vfm ) * ( *delta ));
			}
		}
	}
void Abstand( float *dist, float *distdiff)
{
	/*float x, y, z;
	x = *sf;
	y= *se;
	z = *d;*/
	/*Brechnet den Abstand zwischen dem eigenen Fahrzeug und dem fremden Fahrzeug, anhand des Anfangsabstandes und der Differenz der beiden Wegstrecken*/
	*dist = (*dist + (  *distdiff ) );
}
void HMIeingabe( float *AP, float *BP, float *ART, float *ZART, float *Quit, char *chHit ){

	switch(*chHit)
	{
	case 'a':
	case 'A':
		if ( *ART == 1 ){
			*ART = *ART - 1;
			}
		if ( *ART == 0 ){
			*ART = *ART + 1;
			}
	break;
	case 'w':
	case 'W':
		if ( *AP < 1.0 )
		*AP = *AP + 0.1;
		break;
	case 's':
	case 'S':
		if ( *AP >= 0.1 )
		*AP = *AP - 0.1;
		break;
	case 'b':
	case 'B':
		if ( *BP < 1.0 )
		*BP = *BP + 0.1;
		break;
	case 'n':
	case 'N':
		if ( *BP >= 0.1 )
		*BP = *BP - 0.1;
		break;
	case 'm':
	case 'M':
		*BP = 0.0;
		break;
	case '+':
		if ( *ZART < 130 )
		*ZART = *ZART + 10;
		break;
	case '-':
		if ( *ZART > 30 )
		*ZART = *ZART - 10;
		break;
	case 'q':
	case 'Q':
		*Quit = *Quit + 1;
		break;
	}
}

void ARTemp(float *d, short *ve, float *AP, float *BP, float *vem, float *ZARTm)
	/*Wertet Sicherheitsabstand aus und verändert das Verhalten des eigenen Fahrzeugs*/
{
	if( *d > *ve/2.0 )
		{
		if ( *vem < *ZARTm){														/*Beschleunigen*/
			*BP = 0.0;
			*AP = 0.5;
				}
		if ( *vem > *ZARTm){														/*Segeln*/
			*BP = 0.0;
			*AP = 0.0;
				}
		else{																	/*Geschwindigkeit halten bei Abstand > halber Tacho*/
			*BP = 0.0;
			*AP = 2.0;
				}
			}
	if ( *d < *ve/2.0 ){														/*Bremsen*/
		*AP = 0.0;
		*BP = 0.5;
			}
	else{																		/*Geschwindigkeit halten bei Abstand == halber Tacho*/
		*BP = 0.0;
		*AP = 2.0;
			}
}

void EigenesFahrzeug(float *se, float *vem, float *AP, float *BP, float *total_t, float *delta, float *ves )
{

	/*Funktion EigenesFahrzeug wertet die Gaspedalstellung "AP" und die Bremspedalstellung "BP" aus und legt fest
	 *ob das Fahrzeug beschleunigt, abbremst oder die Geschwindigkeit konstant hält.
	 *Desweiteren wird die zurückgelegte Wegstrecke "se" und die aktuelle Geschwindigkeit "vem" des eigenen Fahrzeugs berechnet und in den Variablen geändert.
	 */
	/*Berechnung von t² aus "delta" für die Wegstrecke "se" bei Beschleunigung bzw Abbremsen*/
			float sqr_t = 0;
			sqr_t = (*delta) * (*delta);

	/*Berechnet die Wegstecke "se" und die Geschwindigkeit "vem" bei Beschleunigung*/
			if( *AP > 0.0 && *AP <= 1.0 ){
				*se = ( ACCEL * ( *AP ) ) * ( sqr_t ) + ( *ves * ( *total_t ));
				*vem = ( ACCEL * ( *AP ) * ( *delta ) ) + *ves;
						}
	/*Berechnet die Wegstecke "se" und die Geschwindigkeit "vem" bei Abbremsen*/

			if( *BP > 0.0 && *BP <= 1.0 ){
				*se = ( -ACCEL * ( *BP ) ) * ( sqr_t ) + ( *ves * ( *total_t ) );
				*vem = ( -ACCEL * ( *BP ) * ( *delta ) ) + *ves;
						}
	/*Geschwindigkeit halten*/
			if( *AP == 2.0){
				/**ves = *vem;*/
				*se = ( *ves * ( *total_t )) + (( *vem ) * ( *delta ));
						}
	/*Segeln (Geschwindigkeit langsam verringern)*/
			else{
				*se = ( -ACCEL * SEGELN ) * ( sqr_t ) + ( *ves * ( *total_t ));
				*vem = ( -ACCEL * SEGELN ) * ( *delta ) + *ves;
						}
}


char Pfadeingabedatei ( char* input )
{
	system ("cls");
	printf("Bitte geben Sie den Dateipfad der Eingabedatei ein, ");
	printf("(Beispiel : c:\\temp\\Eingabedatei.txt) : \n");
	scanf("%s*c", input);
	FILE *fp;
		fp = fopen(input,"r");
			if (fp == NULL){
				printf("Es wurde keine Eingabedatei gefunden!\n");
					fclose(fp);
			}
			return *input;
}
char Pfadausgabedatei ( char* output )
{
	system("cls");
	printf("Bitte geben Sie den Dateipfad f\x81r die Ausgabedatei ein, ");
	printf("(Beispiel : c:\\temp\\Ausgabedatei.txt) : \n");
	scanf("%s*c", output);
	FILE *fp;
		fp = fopen(output,"r");
			if (fp != NULL){
				printf("Es wurde bereits eine Ausgabedatei erstellt, diese wird beim Fortfahren ueberschrieben!\n");
					fclose(fp);
			}
			return *output;
}
/*
                                          Funktion Eingabedatei -> kopiert von project 2020-Eingabe
short funcEingabe(char *input,short *npErstezeile,short *npZeit, short *npZielgeschwindigkeitvf){
	FILE *fp;
	char line[MAX_LINE_SIZE];
	short nleerzeichen =0;
	short ni=0;
	if (funcEingabedateiTest(input)!=0){
		fp = fopen(input,"r");
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
					if(npErstezeile[0] <0 || npErstezeile[0]>130){
						printf("Error! Geschwindigkeit des eigenen Fahrzeugs (in km/h, grÃ¶ÃŸer gleich 0, maximal 130\n");
					}
					if(npErstezeile[1] <0 || npErstezeile[1]>130){
						printf("Error! Geschwindigkeit des vorausfahrenden Fahrzeugs (in km/h, grÃ¶ÃŸer gleich 0, maximal 250\n");

					}
					if(npErstezeile[2] < 0 ){
						printf("Error! Abstand zum vorausfahrenden Fahrzeug (in m, im Normalfall positiv, d.h. keine Kollision\n");
					}
					if(npErstezeile[3] !=0 && npErstezeile[3]!=1){
						printf("Error! Art Status kann entweder 0 oder 1 \n");

					}
					if(npErstezeile[4] <30 ){
						printf("Erron! Zielgeschwindigkeit > 30km/h  ");
					}
					break;
				case 1:
					sscanf(line,"%hd %hd", &npZeit[ni],&npZielgeschwindigkeitvf[ni]);
					if(npZeit[ni]<0 ){
						printf("Zeit seit Simulationsstart (in s, ganze Zahl, nicht-negativ)");
					}
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
short funcEingabedateiTest(char *input){
	char line[MAX_LINE_SIZE];
	short nKommentare;
	short nZeilen;
	short nleerzeichen =0;

	FILE *fp;
	fp = fopen(input,"r");
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
			return nZeilen;
}
*/


                                                                     /*Ausgabefunktion*/


void Ausgabeheader(char *output)
{
	  time_t t = time(NULL);
	  struct tm tm = *localtime(&t);
	  printf("now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	long Striche=0;
	FILE *Datei;
	Datei=fopen(*output, "w+");
	fprintf(Datei,"%d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		fprintf(Datei,";");
		fprintf(Datei,"Zeit seit Simulationsstart in s");
		fprintf(Datei,";");
		fprintf(Datei,"Wegstrecke ab Simulationsstart in m");
		fprintf(Datei,";");
		fprintf(Datei,"Eigengeschwindigkeit ve in km/h");
		fprintf(Datei,";");
		fprintf(Datei,"Geschwindigkeit Vordermann vf in km/h");
		fprintf(Datei,";");
		fprintf(Datei,"Status ART ein/aus(1/0)");
		fprintf(Datei,";");
		fprintf(Datei,"Zielgeschwindigkeit(ART) in km/h");
		fprintf(Datei,";\n");
	fclose(Datei);
}


void Ausgabe(float *total_t, float *s, float *ve, float *vf, short *ART, float *ZART, char *output)
{
	char Pfad = *output;
	FILE *Datei;
	Datei=fopen(Pfad, "a");
	fprintf(Datei,"\n");
	fprintf(Datei,"%f;%f;%f;%f;%hd;%f", *total_t, *s, *ve, *vf, *ART, *ZART);
	fclose(Datei);
}

void funcWahrung(){
	for (int a =0;a<3;a++ ){
	printf("\n\n\n\n\n%20c",32);
		for(int i=1;i<14;i++){
			for(int y=1;y<14-i;y++){
				printf("%c",' ');
			}
			if(i==7 ){
				printf("/");
				for(int j=(((2*i-1)/2)-4);j>0;j--){
					printf("%c",' ');
				}
				printf("KOLLISION!");
				for(int j=((2*i-1)/2)-5;j>0;j--){
					printf("%c",' ');
				}
				printf("\\");
			}else if(i==13){
				for(int j=2*i+1;j>0;j--){
					printf("%c",'=');
				}
			}else if(i==1){
				printf(" %c",'*');
			}else if(i==2 || i==12){
				printf("/");
				for(int j= 2*i-1;j>0;j--){
					printf("%c",' ');
						}
				printf("\\");
			}else if(i==11) {
				printf("/");
				for(int j= ((2*i-1)/2);j>0;j--){
					printf("%c",' ');
									}
							printf("o");
							for(int j=((2*i-1)/2);j>0;j--){
								printf("%c",' ');
							}
							printf("\\");

						}else{
				printf("/");
				for(int j= ((2*i-1)/2);j>0;j--){
					printf("%c",' ');
						}
				printf("|");
				for(int j=((2*i-1)/2);j>0;j--){
					printf("%c",' ');
				}
				printf("\\");
			}
			printf("\n%20c",32);
			}
		Sleep(1);
			system("cls");
			Sleep(1);
		}
}
