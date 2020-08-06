#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <math.h>
#include <crtdbg.h>
#define M_PI 3.14159265


//sizeof(int)=4=sizeof(int*)=sizeof(int**)
//sizeof(double)=8,sizeof(double*)=4

struct obraz
{
	int ok;
	int wysokosc;
	int	szerokosc;
	int glebia;
	char standard[3];
	char nazwa[50];
	int** pixele;
	//ok
};

void skopiujObrazBP(struct obraz src, struct obraz* dest)
{


	dest->glebia = src.glebia;
	dest->wysokosc = src.wysokosc;
	dest->szerokosc = src.szerokosc;
	strcpy(dest->standard, src.standard);
	dest->pixele = malloc(dest->wysokosc * sizeof(int*));
	if (dest->pixele != NULL)
	{
		for (int i = 0; i < dest->wysokosc; i++)
		{
			dest->pixele[i] = malloc(dest->szerokosc * sizeof(int));
			if (dest->pixele[i] == NULL)
			{
				for (int j = 0; j < i; j++)
				{
					free(dest->pixele[j]);
				}
				free(dest->pixele);
				printf("B³¹d allokacji pamiêci!\nNaciœnij dwolny klawisz aby kontynuowaæ...\n");
				getch();
				dest->ok = -1;
				return;

			}

		}
	}
	else
	{
		printf("B³¹d allokacji pamiêci!\nNaciœnij dwolny klawisz aby kontynuowaæ...\n");
		getch();
		dest->ok = -1;
		return;

	}


}

void zwolnijObraz(struct obraz* obrazDoZwolnienia)
{
		for (int i = 0; i < obrazDoZwolnienia->wysokosc; i++)
	{
		free(obrazDoZwolnienia->pixele[i]);
	}
	free(obrazDoZwolnienia->pixele);
}

struct obraz wczytajObraz()
{

	struct obraz nowyObraz;
	char nazwa[50] = "";
	char sciezka[50]= "";
	printf("Podaj nazwê obrazu który chcesz wczytaæ:  (bez rozszerzenia) \n");
	scanf("%s", nazwa);
	strcpy(sciezka, nazwa);
	strcat(sciezka, ".pgm");
	FILE* plik = fopen(sciezka, "r"); 
//	while(!plik)
	if (plik)
	{
		strcpy(nowyObraz.nazwa, nazwa);
		printf("nazwa:  %s", nowyObraz.nazwa);
		int linia = 0;
		int n = 0;
		char znak;
		while (1)
		{
				if (linia == 0)
				{	
					fgets(nowyObraz.standard, 3, plik);
				
					linia++;

					printf("%s\n", nowyObraz.standard);
					if (strcmp(nowyObraz.standard, "P2") != 0)
					{
						printf("Nie obs³ugujê takiego pliku!\nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
						getch();
						nowyObraz.ok = -1;
						fclose(plik);
						return nowyObraz;

					}
				
				}
				else if (linia == 1)
				{
					while (fscanf(plik, "%d", &nowyObraz.szerokosc) != 1)
					{
						while(fgetc(plik)!='\n');
					}	
					while (fscanf(plik, "%d",&nowyObraz.wysokosc) != 1)
					{
						while(fgetc(plik)!='\n');
					}
					printf("%d %d\n", nowyObraz.szerokosc, nowyObraz.wysokosc);
					linia++;
				}
				else if (linia == 2)
				{
					while (fscanf(plik, "%d", &nowyObraz.glebia) != 1)
					{
						while (fgetc(plik) != '\n');
					}
					linia++;
					printf("%d\n", nowyObraz.glebia);
					break;
				}
			
			
		
		}
		nowyObraz.pixele = malloc(nowyObraz.wysokosc * sizeof(int*));	
		if (nowyObraz.pixele != NULL)
		{
			for (int i = 0; i < nowyObraz.wysokosc; i++)
			{
				nowyObraz.pixele[i] = malloc(nowyObraz.szerokosc * sizeof(int));
				if (nowyObraz.pixele[i] == NULL)
				{
					for(int j=0;j<i;j++)
					{ 
						free(nowyObraz.pixele[i]);
					}
					free(nowyObraz.pixele);
					printf("B³¹d alokacji pamiêci!\nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
					getch();
					nowyObraz.ok = -1;
					fclose(plik);
					return nowyObraz;
				}

			}
		}
		else
		{
			printf("B³¹d alokacji pamiêci!\nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
			getch();
			nowyObraz.ok = -1;
			fclose(plik);
			return nowyObraz;
		}


		
		//wczytywanie

		for (int i = 0; i < nowyObraz.wysokosc; i++)
		{
		
			for (int j = 0; j < nowyObraz.szerokosc; j++)
			{
				while (fscanf(plik, "%d", &nowyObraz.pixele[i][j]) != 1)
				{
					while (fgetc(plik) != '\n');
				}
			}
		}
		fclose(plik);
		return nowyObraz;
	}
	else
	{
		printf("B³¹d otwierania pliku! \nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
		getch();
		nowyObraz.ok = -1;
		return nowyObraz;
	}
} //ok

void zapiszObraz(struct obraz obrazDoZapisu)
{
	char sciezka[50] = "";
    //strcat(sciezka,"przykladowe\\");
	strcat(sciezka, obrazDoZapisu.nazwa);
	strcat(sciezka, ".pgm");
	FILE* plik=fopen(sciezka,"w");
	if (plik)
	{
		fprintf(plik, "%s\n", obrazDoZapisu.standard);
		fprintf(plik, "%d %d\n", obrazDoZapisu.szerokosc, obrazDoZapisu.wysokosc);
		fprintf(plik, "%d\n", obrazDoZapisu.glebia);

		for (int i = 0; i < obrazDoZapisu.wysokosc; i++)
		{
			for (int j = 0; j < obrazDoZapisu.szerokosc; j++)
			{
				fprintf(plik, "%d ", obrazDoZapisu.pixele[i][j]);
			}
			fprintf(plik, "\n");
		}
		fclose(plik);
	}
	else
	{
		printf("B³¹d otwarcia pliku!\nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
		getch();
		return;
	}

} //legit

void dodajObrazDoListy(struct obraz** listaObrazow, int* ilosc, struct obraz obrazDoDodania)

{
	if (obrazDoDodania.ok == -1)
	{
		return;
	}
	if (*listaObrazow != NULL)
	{
		*ilosc += 1;
		struct obraz* temp = realloc(*listaObrazow, (*ilosc) * sizeof(struct obraz));
		if (temp == NULL)
		{
			printf("B³¹d allokacji pamiêci dla nowego obrazu!\nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
			getch();
			return;
		}
		*listaObrazow = temp;
		*(*listaObrazow + *ilosc - 1) = obrazDoDodania;
		//printf("nazwa ob: %s \n", obrazDoDodania.nazwa);

	}
}

void usunObrazZTablicy(struct obraz** listaObrazow, int* ilosc, int* aktiv)
{
	if (*ilosc < 1)
	{
		printf("Nie ma obrazu do usuniêcia.\nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
		getch();
		return;
	}
	if (*ilosc > 1)
	{
		struct obraz tp = *(*listaObrazow+*aktiv);
		
		for (int i = 0; i < *ilosc - *aktiv - 1; i++)
		{
			*(*listaObrazow + *aktiv + i) = *(*listaObrazow + *aktiv + i + 1);
		}
		struct obraz* temp = realloc(*listaObrazow, (*ilosc - 1) * sizeof(struct obraz));
		if (temp == NULL)
		{
			for (int i = *ilosc - 1; i > *aktiv; i--)
			{
				*(*listaObrazow + i) = *(*listaObrazow + i - 1);
			}
			*(*listaObrazow + *aktiv) = tp;
			zwolnijObraz(&tp);
			printf("B³¹d allokacji pamiêci!\nNie usuniêto obrazu!\nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
			getch();
			return;
		}
		else
		{
			zwolnijObraz(&tp);
			*ilosc -= 1;
			*listaObrazow = temp;
			if (*aktiv > *ilosc-1 )
			{
				*aktiv-=1;
			}
		}
	}
	else
	{
		zwolnijObraz(*listaObrazow);
		*ilosc -= 1;
		*aktiv = 0;
	}
}//TODO: sprawdzic czy ok

void wyswietlListeObrazow(struct obraz* listaObrazow, int ilosc,int aktiv)
{
	printf("Wyœwietlam listê obrazów:    Aktualnie aktywny jest obraz: %d -> \"%s\" \n",aktiv,listaObrazow[aktiv].nazwa);
	printf("Index | Nazwa \n");
	for (int i = 0; i < ilosc; i++)
	{
		printf("  %d    \"%s\" \n", i, listaObrazow[i].nazwa);
	}

}//ok

void wybierzAktiv(int* aktiv, int ilosc)
{
	int temp;
	printf("Podaj indeks obrazu na którym chcesz pracowaæ: \n");
	temp = scanfInt();
	if (temp<0 || temp>ilosc - 1)
	{
		printf("Nie ma takiego obrazu!\nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
		getch();
		return;
	}
	*aktiv = temp;
}

struct obraz zaszumObraz(struct obraz obrazDoZaszumienia)
{
	struct obraz obrazZaszumiony;
	skopiujObrazBP(obrazDoZaszumienia, &obrazZaszumiony);
	if (obrazZaszumiony.ok == -1)
	{
		return obrazZaszumiony;
	}
	for (int i = 0; i < obrazDoZaszumienia.wysokosc; i++)
	{
		for (int j = 0; j < obrazDoZaszumienia.szerokosc; j++)
		{
			if (rand() % 101 > 98)
			{
				if (rand() % 101 > 50)
					obrazZaszumiony.pixele[i][j] = obrazDoZaszumienia.glebia;
				else
					obrazZaszumiony.pixele[i][j] = 0;
			}
			else
				obrazZaszumiony.pixele[i][j] = obrazDoZaszumienia.pixele[i][j];
		}
	}
	printf("Podaj nazwê dla zaszumionego obrazu: \n");
	scanf("%s", obrazZaszumiony.nazwa);
	return obrazZaszumiony;
}

void stworzIZapiszHistogram(struct obraz obrazHist)
{
	int* tabHist = calloc(obrazHist.glebia+1,sizeof(int));
	if (tabHist == NULL)
	{
		printf("B³¹d allokacji pamiêci!\nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
		getch();
		return;
	}
	for (int i = 0; i < obrazHist.wysokosc; i++)
	{
		for (int j = 0; j < obrazHist.szerokosc; j++)
		{
			if (obrazHist.pixele[i][j] > obrazHist.glebia)
			{
				printf("Coœ jest nie tak z obrazem!\nNie stworzono histogramu.\nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
				free(tabHist);
				getch();
				return;
			}
			tabHist[obrazHist.pixele[i][j]] += 1;
		}
	}
	char nazwa[50]="";
	printf("Podaj nazwê pliku do którego zapisaæ histogram: (bez rozszerzenia) \n");
	scanf("%s", nazwa);
	strcat(nazwa, ".csv");
	FILE* plik = fopen(nazwa, "w");
	if (plik)
	{
		for (int i = 0; i < obrazHist.glebia+1; i++)
			fprintf(plik, "%d;%d\n", i, tabHist[i]);
		
		fclose(plik);
		free(tabHist);
	}
	else
	{
		free(tabHist);
		printf("B³¹d otwarcia pliku!\nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
		getch();
	}
	
}//raczej ok

struct obraz obrocObrazWPrawo(struct obraz obrazDoObrocenia) //dzia³a
{//TODO: rekurencja/uproszczenie
	int k;
	printf("Podaj ile razy obróciæ obraz: \n");
	k = scanfInt();
	while (k < 0)
	{
		printf("Liczba obrotów musi byæ nieujemna!\nSpróbuj ponownie:\n");
		k = scanfInt();
	}
	k = k % 4;

	struct obraz temp;
	strcpy(temp.standard, "P2");

	switch (k)
	{
	case 0:
		return obrazDoObrocenia;
		break;
	case 1:
		temp.glebia = obrazDoObrocenia.glebia;
		temp.szerokosc = obrazDoObrocenia.wysokosc;
		temp.wysokosc = obrazDoObrocenia.szerokosc;
		temp.pixele = malloc(temp.wysokosc * sizeof(int));
		if (temp.pixele != NULL)
		{
			for (int i = 0; i < temp.wysokosc; i++)
			{
				temp.pixele[i] = malloc(temp.szerokosc * sizeof(int));
				if (temp.pixele[i] == NULL)
				{
					for (int j = 0; j < i; j++)
					{
						free(temp.pixele[j]);
					}
					printf("B³¹d alokacji pamiêci!\nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
					getch();
					temp.ok = -1;
					return temp;
				}

			}
		}
		else
		{
			printf("B³¹d alokacji pamiêci!\nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
			getch();
			temp.ok = -1;
			return temp;
		}
		for (int i = 0; i < temp.wysokosc; i++)
		{
			for (int j = 0; j < temp.szerokosc; j++)
			{
				temp.pixele[i][temp.szerokosc - 1 - j] = obrazDoObrocenia.pixele[j][i];
			}
		}

		printf("Podaj nazwê dla obróconego obrazu: \n");
		scanf("%s", temp.nazwa);
		return temp;


		break;
	case 2:

		temp.glebia = obrazDoObrocenia.glebia;
		temp.szerokosc = obrazDoObrocenia.szerokosc;
		temp.wysokosc = obrazDoObrocenia.wysokosc;
		temp.pixele = malloc(temp.wysokosc * sizeof(int));
		if (temp.pixele != NULL)
		{
			for (int i = 0; i < temp.wysokosc; i++)
			{
				temp.pixele[i] = malloc(temp.szerokosc * sizeof(int));
				if (temp.pixele[i] == NULL)
				{
					for (int j = 0; j < i; j++)
					{
						free(temp.pixele[j]);
					}
					printf("B³¹d alokacji pamiêci!\nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
					getch();
					temp.ok = -1;
					return temp;
				}

			}
		}
		else
		{
			printf("B³¹d alokacji pamiêci!\nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
			getch();
			temp.ok = -1;
			return temp;
		}
		for (int i = 0; i < temp.wysokosc; i++)
		{
			for (int j = 0; j < temp.szerokosc; j++)
			{
				temp.pixele[i][j] = obrazDoObrocenia.pixele[temp.wysokosc-1-i][temp.szerokosc-1-j];
			}
		}

		printf("Podaj nazwê dla obróconego obrazu: \n");
		scanf("%s", temp.nazwa);
		return temp;

		break;

	case 3:
		temp.glebia = obrazDoObrocenia.glebia;
		temp.szerokosc = obrazDoObrocenia.wysokosc;
		temp.wysokosc = obrazDoObrocenia.szerokosc;
		temp.pixele = malloc(temp.wysokosc * sizeof(int));
		if (temp.pixele != NULL)
		{
			for (int i = 0; i < temp.wysokosc; i++)
			{
				temp.pixele[i] = malloc(temp.szerokosc * sizeof(int));
				if (temp.pixele[i] == NULL)
				{
					for (int j = 0; j < i; j++)
					{
						free(temp.pixele[j]);
					}
					printf("B³¹d alokacji pamiêci!\nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
					getch();
					temp.ok = -1;
					return temp;
				}

			}
		}
		else
		{
			printf("B³¹d alokacji pamiêci!\nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
			getch();
			temp.ok = -1;
			return temp;
		}
		for (int i = 0; i < temp.wysokosc; i++)
		{
			for (int j = 0; j < temp.szerokosc; j++)
			{
				temp.pixele [temp.wysokosc-1-i][j] = obrazDoObrocenia.pixele[j][i];
			}
		}

		printf("Podaj nazwê dla obróconego obrazu: \n");
		scanf("%s", temp.nazwa);
		return temp;


		break;

	}
	

}

struct obraz odbijObraz(struct obraz obrazDoOdbicia)
{//TODO: pion i poziom
	struct obraz temp;
	skopiujObrazBP(obrazDoOdbicia, &temp);
	if (temp.ok == -1)
	{
		return temp;
	}
	
		for (int i = 0; i < temp.wysokosc; i++)
		{
			for (int j = 0; j < temp.szerokosc; j++)
			{
				temp.pixele[i][j] = obrazDoOdbicia.pixele[i][temp.szerokosc - 1 - j];
			//	printf("%d ", temp.pixele[i][j]);
			}
		//	printf("\n");
		}
	
		printf("Podaj nazwê dla odbitego obrazu: \n");
		scanf("%s", temp.nazwa);
		
	    return temp;

}

struct obraz konwolucja(struct obraz obrazDoKonw, int rozmiarFiltra,int *filtr,int nazwij,int wyrownanie)
{
	struct obraz temp;
	skopiujObrazBP(obrazDoKonw, &temp);
	if (obrazDoKonw.ok == -1)
	{
		return temp;
	}
	int dzielnik=0;
	for (int i = 0; i < rozmiarFiltra; i++)
	{
		for (int j = 0; j < rozmiarFiltra; j++)
		{
			dzielnik += *(filtr+i*rozmiarFiltra+j);
		//	printf(" %d ", *(filtr + i * rozmiarFiltra + j));
		}
	}
	printf(" dzielnik : %d\n", dzielnik);
	dzielnik = fabs(dzielnik);
	long int pf = rozmiarFiltra / 2;

	for (int i = 0; i < temp.wysokosc; i++)
	{
		for (int j = 0; j < temp.szerokosc; j++)
		{
			unsigned int suma = 0;
	
			for (int k = -1*pf; k <= pf; k++)
			{
				for (int l = -1 * pf; l <= pf; l++)
				{
					int a = k + i; 
					int b = l + j;
					//odbicie lustrzane
					if (a < 0)
					{
						a *= -1;
					}
					if (b < 0)
					{
						b *= -1;
					}
					if (a >= temp.wysokosc)
					{
						a = 2 * temp.wysokosc - a - 1;
					}
					if (b >= temp.szerokosc)
					{
						b = 2 * temp.szerokosc - b - 1;
					}
					long int tsuma= obrazDoKonw.pixele[a][b] * *(filtr + (k + pf) * rozmiarFiltra + (l + pf));
					suma += tsuma;    //(*(filtr + (k + pf) * rozmiarFiltra + l + pf));  filtr[k+pf][l+pf];

				}
			}
			long int dsuma = suma;// dzielnik;
			//dsuma = fabs(dsuma);
		//	if (dsuma > obrazDoKonw.glebia)
			//	dsuma = obrazDoKonw.glebia;
			dsuma = fabs(dsuma);
			dzielnik = fabs(dzielnik);
		
			if (dzielnik == 0)
				dzielnik = 1;
			temp.pixele[i][j] = dsuma/dzielnik;
			if (wyrownanie == 1)
			{
				if (temp.pixele[i][j] > temp.glebia)
					temp.pixele[i][j] == temp.glebia;
			}

			/// TODO: if kiedy potrzebuje to sprawdzac a kiedy nie, np do gradientow, dla dynamicznej tez rozwa¿yc
		
			dsuma = 0;
		}
	}
	if (nazwij == 1)
	{
		printf("Podaj nazwê dla obrazu po konwolucji:\n");
		scanf("%s", temp.nazwa);
	}
	return temp;
}

struct obraz konwolucjaDynamicznie(struct obraz obrazDoKonw, int rozmiarFiltra, int** filtr, int nazwij,int wyrownanie)
{
	struct obraz temp;
	skopiujObrazBP(obrazDoKonw, &temp);
	if (obrazDoKonw.ok == -1)
	{
		return temp;
	}
	int dzielnik = 0;
	for (int i = 0; i < rozmiarFiltra; i++)
	{
		for (int j = 0; j < rozmiarFiltra; j++)
		{
			dzielnik += filtr[i][j];
			//printf(" %d ",filtr[i][j]);
		}
	}
	//printf(" dzielnik : %d\n", dzielnik);
	dzielnik = fabs(dzielnik);
	long int pf = rozmiarFiltra / 2;

	for (int i = 0; i < temp.wysokosc; i++)
	{
		for (int j = 0; j < temp.szerokosc; j++)
		{
			long int suma = 0;

			for (int k = -1 * pf; k <= pf; k++)
			{
				for (int l = -1 * pf; l <= pf; l++)
				{
					int a = k + i;
					int b = l + j;
					//odbicie lustrzane
					if (a < 0)
					{
						a *= -1;
					}
					if (b < 0)
					{
						b *= -1;
					}
					if (a >= temp.wysokosc)
					{
						a = 2 * temp.wysokosc - a - 1;
					}
					if (b >= temp.szerokosc)
					{
						b = 2 * temp.szerokosc - b - 1;
					}
					long int tsuma = obrazDoKonw.pixele[a][b] * filtr[k + pf][l + pf];
					suma += tsuma;    //(*(filtr + (k + pf) * rozmiarFiltra + l + pf));  filtr[k+pf][l+pf];

				}
			}
			long int dsuma = suma;// dzielnik;
			//dsuma = fabs(dsuma);
		//	if (dsuma > obrazDoKonw.glebia)
			//	dsuma = obrazDoKonw.glebia;
			dsuma = fabs(dsuma);
			dzielnik = fabs(dzielnik);
		

			if (dzielnik == 0)
				dzielnik = 1;
			temp.pixele[i][j] = dsuma / dzielnik;
			if (wyrownanie == 1)
			{

				if (temp.pixele[i][j] > obrazDoKonw.glebia)
					temp.pixele[i][j] = obrazDoKonw.glebia;
			}
			dsuma = 0;
		}
	}
	if (nazwij == 1)
	{
		printf("Podaj nazwê dla obrazu po konwolucji:\n");
		char nazwa[50] = "";
		scanf("%s",nazwa);
		strcpy(temp.nazwa, nazwa);
	}
	return temp;
}

struct obraz prostyGauss(struct obraz obrazGauss)
{
	int rozmiarOkna;
	printf("UWAGA! Nie nale¿y przesadzaæ z wielkoœci¹ filtra! (bezpiecznie->max 19)\n");
	printf("Podaj rozmiar okna: (nieparzysty)\n");
	rozmiarOkna = scanfInt();
	while (rozmiarOkna % 2 == 0)
	{
		printf("Poda³eœ liczbê parzyst¹, spróbuj ponownie: \n");
		rozmiarOkna = scanfInt();

	}
	struct obraz temp;
	skopiujObrazBP(obrazGauss, &temp);
	if (temp.ok == -1)
	{
		return temp;
	}


	int** okno = NULL;
	okno = malloc(rozmiarOkna * sizeof(int));
	if (okno != NULL)
	{
		for (int i = 0; i < rozmiarOkna; i++)
		{
			okno[i] = malloc(rozmiarOkna * sizeof(int));
			if (okno[i] == NULL)
			{
				for (int j = 0; j < i; j++)
				{
					free(okno[j]);
				}
				free(okno);
				printf("B³¹d allokacji pamiêci!\nNaciœnij dwolny klawisz aby kontynuowaæ...\n");
				getch();
				temp.ok = -1;
				return temp;
			}

		}
	}
	else
	{
		printf("B³¹d allokacji pamiêci!\nNaciœnij dwolny klawisz aby kontynuowaæ...\n");
		getch();
		temp.ok = -1;
		return temp;
	}
	int n;
	long int dzielnik = 0;
	//wyplenianie okna potêgami 2
	for (int i = 0; i < rozmiarOkna; i++)
	{
		for (int j = 0; j < rozmiarOkna; j++)
		{
			if (i <= rozmiarOkna / 2)
			{
				if (j <= rozmiarOkna / 2)
				{
					n = j + i;
				}
				else
				{
					n = rozmiarOkna-j-1+i; //       DZIA£A NIE RUSZAC
				}
			}
			else
			{
				if (j <= rozmiarOkna / 2)
				{
					n = rozmiarOkna-i+j-1;
				}
				else
				{
					n = rozmiarOkna  - j-1+rozmiarOkna-i-1;
				}
			}
			okno[i][j] = pow(2, n);
			dzielnik += okno[i][j];
		}
	}
	//for (int i = 0; i < rozmiarOkna;i++)
	//{
	//	for (int j = 0; j < rozmiarOkna; j++)
	//	{
	//		printf("%d ", okno[i][j]);
	//	}
	//	printf("\n");
	//}
	//printf("%d ", dzielnik);

	///

	
	long int suma=0;
	int pf = rozmiarOkna / 2;
	for (int i = 0; i < temp.wysokosc ; i++)
	{
		for (int j = 0; j < temp.szerokosc; j++)
		{
			for (int k = -1*pf; k <=  pf; k++)
			{				
				for (int l =  -1*pf; l <= pf; l++)
				{
					int a = k + i;
					int b = l + j;
					//odbicie lustrzane
					if (a < 0)
					{
						a *= -1;
					}
					if (b < 0)
					{
						b *= -1;
					}
					if (a >= temp.wysokosc)
					{
						a = 2 * temp.wysokosc - a - 1;
					}
					if (b >= temp.szerokosc)
					{
						b = 2 * temp.szerokosc - b - 1;
					}
					long int gsuma = obrazGauss.pixele[a][b] * okno[k + pf][l + pf];
					suma += gsuma;
				}
				
			}
			temp.pixele[i][j] = suma / dzielnik;
			suma = 0;
			//printf("asd1\n");
		}
	}
	printf("Podaj nazwê dla przefiltrowanego obrazu:\n");
	scanf("%s", temp.nazwa);
	for (int i = 0; i < rozmiarOkna; i++)
	{
		free(okno[i]);
		
	}
	free(okno);
	return temp;

}

struct obraz kindaCannyEdgeDetect(struct obraz obrazCanny)
{
	//UWAGA FUNCKJA NIEBEZPIECZNA U¯YWASZ NA W£ASN¥ ODPOWIEDZIALNOŒÆ
	//TODO: przepisaæ to i ulepszyæ

	int x = 3;
	int y = 3;
	int Gx[3][3] = //dynamicznie
	{
		{-1,0,1},
		{-2,0,2},
		{-1,0,1}
	};
	int Gy[3][3] =
	{
		{1,2,1},
		{0,0,0},
		{-1,-2,-1}
	};
	struct obraz tempX;
	strcpy(tempX.standard, "P2");
	tempX.glebia = obrazCanny.glebia;
	tempX.szerokosc = obrazCanny.szerokosc - 2;
	tempX.wysokosc = obrazCanny.wysokosc - 2;
	tempX.pixele = malloc(tempX.wysokosc * sizeof(int));
	if (tempX.pixele != NULL)
	{
		for (int i = 0; i < tempX.wysokosc; i++)
		{
			tempX.pixele[i] = malloc(tempX.szerokosc * sizeof(int));
			if (tempX.pixele[i] == NULL)
			{
				//tutaj wychodzic
			}

		}
	}
	struct obraz tempY;
	strcpy(tempY.standard, "P2");
	tempY.glebia = obrazCanny.glebia;
	tempY.szerokosc = obrazCanny.szerokosc - 2;
	tempY.wysokosc = obrazCanny.wysokosc - 2;
	tempY.pixele = malloc(tempY.wysokosc * sizeof(int));
	if (tempY.pixele != NULL)
	{
		for (int i = 0; i < tempY.wysokosc; i++)
		{
			tempY.pixele[i] = malloc(tempY.szerokosc * sizeof(int));
			if (tempY.pixele[i] == NULL)
			{
				//tutaj wychodzic
			}

		}
	}

	struct obraz temp;
	strcpy(temp.standard, "P2");
	temp.glebia = obrazCanny.glebia;
	temp.szerokosc = obrazCanny.szerokosc - 2;
	temp.wysokosc = obrazCanny.wysokosc - 2;
	temp.pixele = malloc(temp.wysokosc * sizeof(int));
	if (temp.pixele != NULL)
	{
		for (int i = 0; i < temp.wysokosc; i++)
		{
			temp.pixele[i] = malloc(temp.szerokosc * sizeof(int));
			if (temp.pixele[i] == NULL)
			{
				//tutaj wychodzic
			}

		}
	}


	int a = -1;
	int b = -1;
	int suma = 0;
	for (int i = 1; i < obrazCanny.wysokosc - 1; i++)
	{
		for (int j = 1; j < obrazCanny.szerokosc - 1; j++)
		{
			for (int k = i - 1; k <= i + 1; k++)
			{
				if (a < x - 1)
				{
					a++;
				}
				else
				{
					a = 0;
				}
				for (int l = j - 1; l <= j + 1; l++)
				{
					if (b < y - 1)
					{
						b++;
					}
					else
					{
						b = 0;
					}
					suma += obrazCanny.pixele[k][l] * Gx[a][b];
				}
			}
			tempX.pixele[i - 1][j - 1] = suma;

			suma = 0;
			
		}
	}


	 a = -1;
	 b = -1;
	 suma = 0;
	for (int i = 1; i < obrazCanny.wysokosc - 1; i++)
	{
		for (int j = 1; j < obrazCanny.szerokosc - 1; j++)
		{
			for (int k = i - 1; k <= i + 1; k++)
			{
				if (a < x - 1)
				{
					a++;
				}
				else
				{
					a = 0;
				}
				for (int l = j - 1; l <= j + 1; l++)
				{
					if (b < y - 1)
					{
						b++;
					}
					else
					{
						b = 0;
					}
					suma += obrazCanny.pixele[k][l] * Gy[a][b];
				}
			}
			tempY.pixele[i - 1][j - 1] = suma;

			suma = 0;

		}
	}
	int maxGrad = 0;
	for (int i = 0; i < temp.wysokosc; i++)
	{
		for (int j = 0; j < temp.szerokosc; j++)
		{
			temp.pixele[i][j] =sqrt(pow(tempX.pixele[i][j],2) + pow(tempY.pixele[i][j],2));
			if (temp.pixele[i][j] > maxGrad)
			{
				maxGrad = temp.pixele[i][j];
			}
		}
	}
	//zapiszObraz(temp);
	int progmin = 0.125 * maxGrad;
	int progmax = 0.9 * maxGrad;

	double** edgeDirection;
	edgeDirection = malloc(temp.wysokosc * sizeof(double));
	if (edgeDirection != NULL)
	{
		for (int i = 0; i < temp.wysokosc; i++)
		{
			edgeDirection[i] = malloc(temp.szerokosc * sizeof(double));
			if (edgeDirection[i] == NULL)
			{
				printf("blad\n");
				//tu wychodzic
			}

		}
	}
	else
	{
		printf("blad2\n");
	}
	printf("ok\n");
	for (int i = 0; i < temp.wysokosc ; i++)
	{
		for (int j = 0; j < temp.szerokosc; j++)
		{
			if (tempX.pixele[i][j] == 0)
			{
				if (tempY.pixele[i][j] == 0)
				{
					edgeDirection[i][j] = 0;
				}
				else
				{
					edgeDirection[i][j] = 90;
				}
			}
			else
			{
				edgeDirection[i][j] = atan(tempY.pixele[i][j] / (double)tempX.pixele[i][j]) * 180 / M_PI;

				if (edgeDirection[i][j] <= -67.5 || edgeDirection[i][j] >= 67.5) //zolty
				{
					edgeDirection[i][j] = 0;
				}
				else if (edgeDirection[i][j] > -67.5 && edgeDirection[i][j] <= -22.5)//zielony
				{
					edgeDirection[i][j] = 45;
				}
				else if (edgeDirection[i][j] > -22.5 && edgeDirection[i][j] <= 22.5) //niebieski
				{
					edgeDirection[i][j] = 90;
				}
				else //czerwony
				{
					edgeDirection[i][j] = 135;
				}

			}
		//	printf("%lf ", edgeDirection[i][j]);
		}
	//	printf("\n");
	}
	struct obraz edges;
	strcpy(edges.standard, "P2");
	edges.glebia = 1;
	edges.szerokosc = obrazCanny.szerokosc - 2;
	edges.wysokosc = obrazCanny.wysokosc - 2;
	edges.pixele = malloc(edges.wysokosc * sizeof(int));
	if (edges.pixele != NULL)
	{
		for (int i = 0; i < edges.wysokosc; i++)
		{
			edges.pixele[i] = malloc(edges.szerokosc * sizeof(int));
			if (edges.pixele[i] == NULL)
			{
				//tutaj wychodzic
			}

		}
	}
	for (int i = 1; i < edges.wysokosc-1; i++)
	{
		for (int j = 1; j < edges.szerokosc-1; j++)
		{
			if (edgeDirection[i][j] == 0)
			{
				if (temp.pixele[i][j-1] < temp.pixele[i][j] && temp.pixele[i][j+1] < temp.pixele[i][j]&&temp.pixele[i][j]>progmin && temp.pixele[i][j] < progmax)
				{
					temp.pixele[i][j] = 0;
				}
				else
					temp.pixele[i][j] = 1;
			}
			else if(edgeDirection[i][j] == 45)
			{
				if (temp.pixele[i+1][j + 1] < temp.pixele[i][j] && temp.pixele[i+1][j + 1] < temp.pixele[i][j] && temp.pixele[i][j] > progmin && temp.pixele[i][j] < progmax)
				{
					temp.pixele[i][j] = 0;
				}
				else
					temp.pixele[i][j] = 1;
			}
			else if (edgeDirection[i][j] == 90)
			{
				if (temp.pixele[i -1][j] < temp.pixele[i][j] && temp.pixele[i+1][j] < temp.pixele[i][j] && temp.pixele[i][j] > progmin && temp.pixele[i][j] < progmax)
				{
					temp.pixele[i][j] = 0;
				}
				else
					temp.pixele[i][j] = 1;
			}
			else if (edgeDirection[i][j] == 135)
			{
				if (temp.pixele[i - 1][j + 1] < temp.pixele[i][j] && temp.pixele[i + 1][j - 1] < temp.pixele[i][j] && temp.pixele[i][j] > progmin && temp.pixele[i][j] < progmax)
				{
					temp.pixele[i][j] = 0;
				}
				else
					temp.pixele[i][j] = 1;
			}
		}
	}
	temp.glebia = 1;
	return temp;


}

struct obraz legitCannyEdgeDetect(struct obraz obrazCanny)	
{
	// https://www.massey.ac.nz/~mjjohnso/notes/59731/presentations/img_proc.PDF
	// http://www.mif.pg.gda.pl/homepages/marcin/AG2015-16/wyklad5.pdf
	int gauss[5][5] =
	{
		{2,4,5,4,2},
		{4,9,12,9,4},
		{5,12,15,12,5},
		{4,9,12,9,4},
		{2,4,5,4,2 }

	};
	int Gx[3][3] = //operatory Sobela
	{
		{-1,0,1},
		{-2,0,2},
		{-1,0,1}
	};
	int Gy[3][3] =
	{
		{ 1, 2, 1},
		{ 0, 0, 0},
		{-1,-2,-1}
	};

	obrazCanny = konwolucja(obrazCanny, 5, gauss,-1,0);
	struct obraz obrazGx= konwolucja(obrazCanny, 3, Gx,-1,0);
	struct obraz obrazGy= konwolucja(obrazCanny, 3, Gy,-1,0);
	struct obraz obrazG;
	
	skopiujObrazBP(obrazCanny, &obrazG); 
	if (obrazCanny.ok == -1 || obrazGx.ok == -1 || obrazGy.ok == -1 || obrazG.ok == -1)
	{
		return obrazG;
	}
	int maxG=0;
	//kopiowac dane do obrazg
	for (int i = 0; i < obrazCanny.wysokosc; i++)
	{
		for (int j = 0; j < obrazCanny.szerokosc; j++)
		{
			//wartosc gradientu w punkcie
			obrazG.pixele[i][j] = sqrt(pow(obrazGx.pixele[i][j], 2) + pow(obrazGy.pixele[i][j], 2)); 
			if (obrazG.pixele[i][j] > maxG)
			{
				maxG = obrazG.pixele[i][j];

			}
		}
	}

	int highT = 0.2 * maxG;
	int lowT = 0.1*maxG;


	//strcpy(obrazG.nazwa, "grad");
	//zapiszObraz(obrazG);

	int** edgeDirection;
	edgeDirection = malloc(obrazG.wysokosc * sizeof(int));
	if (edgeDirection != NULL)
	{
		for (int i = 0; i < obrazG.wysokosc; i++)
		{
			edgeDirection[i] = malloc(obrazG.szerokosc * sizeof(int));
			if (edgeDirection[i] == NULL)
			{
				for (int j = 0; j < i; j++)
				{
					free(edgeDirection[j]);
				}
				free(edgeDirection);
				printf("B³¹d allokacji pamiêci!\nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
				getch();
				obrazCanny.ok = -1;
				return obrazCanny;
			}

		}
	}
	else
	{
		printf("B³¹d allokacji pamiêci!\nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
		getch();
		obrazCanny.ok = -1;
		return obrazCanny;
	}


	for (int i = 0; i < obrazG.wysokosc; i++)
	{
		for (int j = 0; j < obrazG.szerokosc; j++)
		{
			if (obrazGx.pixele[i][j] == 0)
			{
				if (obrazGy.pixele[i][j] == 0)
				{
					edgeDirection[i][j] = 0;
				}
				else
				{
					edgeDirection[i][j] = 90;
				}
			}
			else
			{
				edgeDirection[i][j] = atan(obrazGy.pixele[i][j] / (double)obrazGy.pixele[i][j]) * 180 / M_PI;

				if (edgeDirection[i][j] <= -67.5 || edgeDirection[i][j] >= 67.5) //zolty
				{
					edgeDirection[i][j] = 0;
				}
				else if (edgeDirection[i][j] > -67.5 && edgeDirection[i][j] <= -22.5)//zielony
				{
					edgeDirection[i][j] = 45;
				}
				else if (edgeDirection[i][j] > -22.5 && edgeDirection[i][j] <= 22.5) //niebieski
				{
					edgeDirection[i][j] = 90;
				}
				else //czerwony
				{
					edgeDirection[i][j] = 135;
				}

			}
			//	printf("%lf ", edgeDirection[i][j]);
		}
		//	printf("\n");
	}

	// nms

	struct obraz nms;
	skopiujObrazBP(obrazCanny, &nms);
	if (nms.ok == -1)
	{ 
		return nms;
		}
	//nms.glebia = 1;
	//TODO: nazwa
	//strcpy(nms.nazwa, "nms");

	for (int i = 1; i < nms.wysokosc-1 ; i++)
	{
		for (int j = 1; j < nms.szerokosc - 1; j++)
		{
		
			if (edgeDirection[i][j] == 0)
			{
				if (obrazG.pixele[i][j - 1] < obrazG.pixele[i][j] && obrazG.pixele[i][j + 1] < obrazG.pixele[i][j])//&&obrazG.pixele[i][j]>=lowT)
				{
					nms.pixele[i][j] = obrazG.pixele[i][j];
				}
				else
					nms.pixele[i][j] = 0;
			}
			else if (edgeDirection[i][j] == 45)
			{
				if (obrazG.pixele[i + 1][j + 1] < obrazG.pixele[i][j] && obrazG.pixele[i - 1][j - 1] < obrazG.pixele[i][j])// &&obrazG.pixele[i][j] >= lowT)
				{
					nms.pixele[i][j] = obrazG.pixele[i][j];
				}
				else
					nms.pixele[i][j] = 0;
			}
			else if (edgeDirection[i][j] == 90)
			{
				if (obrazG.pixele[i - 1][j] < obrazG.pixele[i][j] && obrazG.pixele[i + 1][j] < obrazG.pixele[i][j])// && obrazG.pixele[i][j] >= lowT)
				{
					nms.pixele[i][j] = obrazG.pixele[i][j];
				}
				else
					nms.pixele[i][j] = 0;
			}
			else if (edgeDirection[i][j] == 135)
			{
				if (obrazG.pixele[i - 1][j + 1] < obrazG.pixele[i][j] && obrazG.pixele[i + 1][j - 1] < obrazG.pixele[i][j])// && obrazG.pixele[i][j] >= lowT)
				{
					nms.pixele[i][j] = obrazG.pixele[i][j];
				}
				else
					nms.pixele[i][j] = 0;
			}
		}
	}
	//obrazG.glebia = 1;
	strcpy(obrazG.nazwa, "nms");
	//zapiszObraz(nms);
		

	//histereza
	int iedges[400];
		int jedges[400];
	int n = 1;
	int m = 1;
	struct obraz histereza;
	skopiujObrazBP(obrazG, &histereza);
	for (int i = 0; i < histereza.wysokosc; i++)
	{
		for (int j = 0; j < histereza.szerokosc; j++)
		{
			histereza.pixele[i][j] = 0;
		}
	}
	for (int i = 0; i < histereza.wysokosc; i++)
	{
		for (int j = 0; j < histereza.szerokosc; j++)
		{
			if (nms.pixele[i][j] >= highT)// &&histereza.pixele[i][j]==0) //jeœli pixel mocny 
			{
				histereza.pixele[i][j] = histereza.glebia;

				int lkraw = 1;
				iedges[0] = i;
				jedges[0] = j;
				do
				{
					lkraw--;
					int a = iedges[lkraw];
					int b = jedges[lkraw];
		/*			if (a < 0)
					{
						a = 1;
					}
					if (b < 0)
					{
						b = 1;
					}
					if (a > histereza.wysokosc - 1)
					{
						a = histereza.wysokosc - 2;
					}if (b > histereza.szerokosc - 1)
					{
						b = histereza.szerokosc - 2;
					}*/
					int indexSasiad[8][2] =
					{
						{a - 1,b},
						{a - 1,b + 1},
						{a,    b + 1},
						{a + 1,b + 1},
						{a+1,  b },
						{a + 1,b - 1},
						{a ,b-1},
						{a - 1,b - 1}

					};
					for (int h = 0; h < 8; h++)
					{
						if (indexSasiad[h][0] < 0)
						{
							indexSasiad[h][0] *= -1;
						}
						if (indexSasiad[h][1] < 0)
						{
							indexSasiad[h][1] *= -1;
						}
						if (indexSasiad[h][0] >= nms.wysokosc)
						{
							indexSasiad[h][0] = 2 * nms.wysokosc - indexSasiad[h][0] - 1;
						}
						if (indexSasiad[h][1] >=nms.szerokosc)
						{
							indexSasiad[h][0] = 2 * nms.szerokosc - indexSasiad[h][1] - 1;
						}
					}
		/*			int sasiedzi[8] = {
						{obrazG.pixele[i - 1][j] },
						{obrazG.pixele[i - 1][j + 1]},
						{obrazG.pixele[i][j + 1]},
						{obrazG.pixele[i + 1][j + 1]},
						{obrazG.pixele[i][j + 1]},
						{obrazG.pixele[i - 1][j + 1]},
						{obrazG.pixele[i - 1][j]},
						{obrazG.pixele[i - 1][j - 1]},
					};
					*/
					int mocnySasiad = 0;
					//printf("i: %d, j: %d \n", i, j);
					for (int k = 0; k < 8; k++)
					{
						//printf("k: %d\n", k);
						//printf("nms indexy: %d,%d\n", indexSasiad[k][0],indexSasiad[k][1]);
						//printf("adres :%p , %p\n",nms.pixele,histereza.pixele);
						//printf( "nms.pixele: %d \n", nms.pixele[indexSasiad[k][0]][indexSasiad[k][1]]);
						if (nms.pixele[indexSasiad[k][0]][indexSasiad[k][1]] > lowT && histereza.pixele[indexSasiad[k][0]][indexSasiad[k][1]] == 0)
						{
							printf("ho ");
							//printf("adres1 :%p , %p\n", nms.pixele, histereza.pixele);
							histereza.pixele[indexSasiad[k][0]][indexSasiad[k][1]] = histereza.glebia;
							
						//	printf("adres2 :%p , %p\n", nms.pixele, histereza.pixele);
							iedges[lkraw] = indexSasiad[k][0];
						//	printf("adres3 :%p , %p\n", nms.pixele, histereza.pixele);
							jedges[lkraw] = indexSasiad[k][1];
						//	printf("adres4 :%p , %p\n", nms.pixele, histereza.pixele);
							lkraw++;
							
						}
					//	printf("adres5 :%p , %p\n", nms.pixele, histereza.pixele);
					}
				//printf("lkraw: %d\n", lkraw);
				} while (lkraw > 0);
		}
		
		}
	}
	printf("gut\n");
	for (int i = 0; i < obrazG.wysokosc; i++)
	{
		free(edgeDirection[i]);
	}
	free(edgeDirection);
	zwolnijObraz(&obrazGx);
	zwolnijObraz(&obrazGy);
	
	zwolnijObraz(&nms);
	zwolnijObraz(&obrazCanny);
	printf("Podaj nazwê dla obrazu po detekcji krawêzi:\n");
	scanf("%s", histereza.nazwa);
	zwolnijObraz(&obrazG);
	return histereza;

}

struct obraz dowolnaMediana(struct obraz obrazMed)
{
	struct obraz temp;
	skopiujObrazBP(obrazMed, &temp);
	int rozmOkna=0;
	printf("Podaj rozmiar nieparzysty rozmiar okna: \n");
	rozmOkna = scanfInt();
	while (rozmOkna <= 0 || rozmOkna%2 == 0)
	{
		printf("Poda³eœ nieprawid³ow¹ liczbê!\nSpróbuj ponownie: \n");
		rozmOkna = scanfInt();
	}
	int indeksDanych = 0;
	int* dane = malloc(rozmOkna *rozmOkna* sizeof(int));
	if (dane == NULL)
	{
		printf("B³¹d allokacji pamiêci!\nNaciœnij dwolny klawisz aby kontynuowaæ...\n");
		getch();
		temp.ok = -1;
		return temp;
	}
	int pf = rozmOkna / 2;
	for (int i = 0; i < temp.wysokosc; i++)
	{
		for (int j = 0; j < temp.szerokosc; j++)
		{
			indeksDanych = 0;
			for (int k = -1 * pf; k <= pf; k++)
			{
				for (int l = -1 * pf; l <= pf; l++)
				{
					int a = k + i;
					int b = l + j;
					//odbicie lustrzane
					if (a < 0)
					{
						a *= -1;
					}
					if (b < 0)
					{
						b *= -1;
					}
					if (a >= temp.wysokosc)
					{
						a = 2 * temp.wysokosc - a - 1;
					}
					if (b >= temp.szerokosc)
					{
						b = 2 * temp.szerokosc - b - 1;
					}
					dane[indeksDanych] = obrazMed.pixele[a][b];
					indeksDanych++;
				}
			}
			
			for (int r = 0; r <( rozmOkna*rozmOkna) - 1; r++)
			{
				for (int f = 0; f < (rozmOkna*rozmOkna)-r-1; f++)
				{
					if (dane[f] > dane[f + 1])
					{
						int tempD = dane[f];
						dane[f] = dane[f + 1];
						dane[f + 1] = tempD;
					}
				}
			}
			int srodek = rozmOkna * rozmOkna / 2;
			temp.pixele[i][j] = dane[srodek];


		}
	}
	printf("Podaj nazwê dla przefiltrowanego obrazu: \n");
	scanf("%s", temp.nazwa);
	free(dane);

	return temp;

}

struct obraz dowolnaKonwolucja(struct obraz obrazDoKonw)
{
	struct obraz temp;
	
	int rozmOkna = 0;
	printf("Podaj rozmiar nieparzysty rozmiar okna: (>1) \n");
	rozmOkna = scanfInt();
	while (rozmOkna <= 1 || rozmOkna%2 == 0)
	{
		printf("Poda³eœ nieprawid³ow¹ liczbê!\nSpróbuj ponownie: \n");
		rozmOkna = scanfInt();
	}
	int** okno;
	okno = malloc(rozmOkna * sizeof(int));
	if (okno != NULL)
	{
		for (int i = 0; i < rozmOkna; i++)
		{
			okno[i] = malloc(rozmOkna * sizeof(int));
			if (okno[i] == NULL)
			{
				for (int j = 0; j <= i; j++)
				{
					free(okno[i]);

				}
				temp.ok = -1;
				printf("B³¹d allokacji pamiêci!\nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
				getch();
				return temp;
			}
		}
	}
	else
	{
		temp.ok = -1;
		printf("B³¹d allokacji pamiêci!\nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
		getch();
		return temp;
	}
	for (int i = 0; i < rozmOkna; i++)
	{
		for (int j = 0; j < rozmOkna; j++)
		{
			printf("[ ]");
		}
		printf("\n");
	}

	printf("Uzupe³nij filtr do konwolucji liczbami ca³kowitymi:\n");

	for (int i = 0; i < rozmOkna; i++)
	{
		for (int j = 0; j < rozmOkna; j++)
		{
			
			int x = scanfInt();
			okno[i][j] = x;
			system("cls");
			printf("Uzupe³nij filtr do konwolucji liczbami ca³kowitymi:\n");
			for (int k = 0; k < rozmOkna; k++)
			{
				for (int l = 0; l < rozmOkna; l++)
				{
					if (okno[k][l]>-999999)
					{
						printf("[%d]", okno[k][l]);
					}
					else
					{
						printf("[ ]");
					}
				}
				printf("\n");
			}
			
		
			//printf("%d ", okno[i][j]);
		}
		printf("\n");
	}

	
	temp = konwolucjaDynamicznie(obrazDoKonw, rozmOkna, okno, 1,1);
	for (int i = 0; i < rozmOkna; i++)
	{
		free(okno[i]);
	}
	free(okno);
	return temp;

}

struct obraz zmienRozmiarObrazu(struct obraz obrazDoZmienienia)
{
	//brak zabezpieczenia przed double
	printf("Podaj skalê do zmienienia wielkoœci obrazu:\n");
	double skala;
	scanf("%lf", &skala);
	struct obraz temp;
	temp.glebia = obrazDoZmienienia.glebia;
	strcpy(temp.standard, "P2");
	temp.wysokosc =(int)( obrazDoZmienienia.wysokosc * skala);
	temp.szerokosc =(int)( obrazDoZmienienia.szerokosc * skala);
	temp.pixele = malloc(temp.wysokosc * sizeof(int*));
	if (temp.pixele != NULL)
	{
		for (int i = 0; i < temp.wysokosc; i++)
		{
			temp.pixele[i] = malloc(temp.szerokosc * sizeof(int));
			if (temp.pixele[i] == NULL)
			{
				for (int j = 0; j < i; j++)
				{
					free(temp.pixele[i]);
				}
				free(temp.pixele);
				printf("B³¹d allokacji pamiêci!\nNaciœnij dwolny klawisz aby kontynuowaæ...\n");
				getch();
				temp.ok = -1;
				return temp;
			}
		}
	}
	else
	{
		printf("B³¹d allokacji pamiêci!\nNaciœnij dwolny klawisz aby kontynuowaæ...\n");
		getch();
		temp.ok = -1;
		return temp;
	}
	//if (skala <= 1)
	//{
		for (int i = 0; i < temp.wysokosc; i++)
		{
			for (int j = 0; j < temp.szerokosc; j++)
			{
				temp.pixele[i][j] = obrazDoZmienienia.pixele[(int)(i / skala)][(int)(j / skala)];
			}
		}
		printf("Podaj nazwê dla obrazu o zmienionych wymiarach: \n");
		scanf("%s", temp.nazwa);
		return temp;
	//}
	//else
	//{
	//	for (int i = 0; i < obrazDoZmienienia.wysokosc; i++)
	//	{
	//		for (int j = 0; j < obrazDoZmienienia.szerokosc; j++)
	//		{
	//			temp.pixele[(int)(i*skala)][(int)(j*skala)] = obrazDoZmienienia.pixele[i][j];
	//		}
	//	}
	//	int maska[2][2] =
	//	{
	//		{1,1},
	//	{1,1}
	//	};
	//	int pf = 3 / 2;
	//	int suma = 0;
	//	double ii = 0;
	//	double jj = 0;
	//	for (int i = 0; i < obrazDoZmienienia.wysokosc; i++)
	//	{
	//		ii = i + 1;
	//		for (int j = 0; j < obrazDoZmienienia.szerokosc; j++)
	//		{
	//			jj = j + 1;
	//			for (int k = -1 * pf; k < pf; k++)
	//			{
	//				for (int l = -1 * pf; l < pf; l++)
	//				{
	//					int a = k + i;
	//					int b = l + j;
	//					//odbicie lustrzane
	//					if (a < 0)
	//					{
	//						a *= -1;
	//					}
	//					if (b < 0)
	//					{
	//						b *= -1;
	//					}
	//					if (a >= obrazDoZmienienia.wysokosc)
	//					{
	//						a = 2 * obrazDoZmienienia.wysokosc - a - 1;
	//					}
	//					if (b >= obrazDoZmienienia.szerokosc)
	//					{
	//						b = 2 * obrazDoZmienienia.szerokosc - b - 1;
	//					}
	//					long int tsuma = obrazDoZmienienia.pixele[a][b] * maska[k + pf][l + pf];
	//					suma += tsuma;

	//				}
	//			}
	//			if (i < obrazDoZmienienia.wysokosc - 1 && j < obrazDoZmienienia.szerokosc - 1)
	//			{
	//				if((int)(i/skala)%1!=0)
	//				temp.pixele[(int)(1 + i * skala)][(int)(1 + j * skala)] = suma / 4;
	//			}
	//			suma = 0;
	//		}
	//	}
	/*	printf("Podaj nazwê dla obrazu o zmienionych wymiarach: \n");
		scanf("%s", temp.nazwa);
		return temp;
	}*/
}

struct obraz wyrownajHistogram(struct obraz obrazDoWyrow)
{

	// https://pl.wikipedia.org/wiki/Wyr%C3%B3wnanie_histogramu <- Ÿród³o
	struct obraz temp;
	skopiujObrazBP(obrazDoWyrow, &temp);
	int* tabHist = calloc(obrazDoWyrow.glebia + 1, sizeof(int));
	if (tabHist == NULL)
	{
		printf("B³¹d allokacji pamiêci!\nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
		getch();
		temp.ok = -1;
		return temp;
	}
	for (int i = 0; i < obrazDoWyrow.wysokosc; i++)
	{
		for (int j = 0; j < obrazDoWyrow.szerokosc; j++)
		{
			tabHist[obrazDoWyrow.pixele[i][j]] += 1;
		}
	}
	int tempsum = 0;
	int znaleziono = 0;
	double d0;
	int k = obrazDoWyrow.glebia;
	double* dystr = calloc((obrazDoWyrow.glebia + 1) , sizeof(double));
	int sum = obrazDoWyrow.szerokosc * obrazDoWyrow.wysokosc;
	for (int i = 0; i < obrazDoWyrow.glebia+1; i++)
	{
		tempsum += tabHist[i];
		dystr[i] =tempsum / (double)sum;
		if (dystr[i] > 0 && znaleziono != 1)
		{
			d0 = dystr[i];
			znaleziono = 1;
		}
	}
	for (int i = 0; i < temp.wysokosc; i++)
	{
		for (int j = 0; j < temp.szerokosc; j++)
		{
			temp.pixele[i][j] = ((dystr[obrazDoWyrow.pixele[i][j]] - d0) * (double)(k - 1)) / (double)(1 - d0);
		}
	}
	printf("Podaj nazwê dla obrazu z wyrównanym histogramem: \n");
	char nazwa[50] = "";
	scanf("%s",nazwa);
	strcpy(temp.nazwa, nazwa);
	free(tabHist);
	free(dystr);
	return temp;

}

void zmienNazweObrazu(struct obraz* obrazDoZmianyNazwy)
{
	printf("Podaj now¹ nazwê dla obrazu:\n");
	scanf("%s", obrazDoZmianyNazwy->nazwa);

}

int scanfInt()
{
	int liczba;
	while (1)
	{
		if (!scanf("%d", &liczba))
		{
			while (getchar() != '\n');
			printf("B³¹d!,wprowadŸ liczbê ca³kowit¹:\n");
		}
		else
		{
			while (1)
			{
				char znak = getchar();
				if (znak == '\n')
					break;
			}
			return liczba;
		}
	}
}

void zwolnijPamiec(struct obraz** listaObrazow, int ilosc)
{
	for (int i = 0;i<ilosc;i++)
	{
		printf("%d ", i);
		for (int j = 0; j < (*listaObrazow)[i].wysokosc; j++)
		{
			free((*listaObrazow+i)->pixele[j]);
		}
		free((*listaObrazow+i)->pixele);
	
	}
	free(*listaObrazow);
	//dzia³a
	//TODO: dodac ustawianie nazw gdzie trzeba
}

struct obraz dodajObraz()
{
	struct obraz utworzonyObraz;
	strcpy(utworzonyObraz.standard, "P2");
	printf("Podaj wysokoœæ obrazu:\n");
	utworzonyObraz.wysokosc = scanfInt();
	while (utworzonyObraz.wysokosc < 1)
	{
		printf("Podaj liczbê dodatni¹!\n");
		utworzonyObraz.wysokosc = scanfInt();
	}
	
	printf("Podaj szerokoœæ obrazu:\n");
	utworzonyObraz.szerokosc = scanfInt();
	while (utworzonyObraz.szerokosc < 1)
	{
		printf("Podaj liczbê dodatni¹!\n");
		utworzonyObraz.szerokosc = scanfInt();
	}

	printf("Podaj g³êbiê szaroœci obrazu:\n");
	utworzonyObraz.glebia = scanfInt();
	while (utworzonyObraz.glebia < 1)
	{
		printf("Podaj liczbê dodatni¹!\n");
		utworzonyObraz.glebia = scanfInt();
	}
	utworzonyObraz.pixele = malloc(utworzonyObraz.wysokosc * sizeof(int*));
	if (utworzonyObraz.pixele != NULL)
	{
		for (int i = 0; i < utworzonyObraz.wysokosc; i++)
		{
			utworzonyObraz.pixele[i] = malloc(utworzonyObraz.szerokosc * sizeof(int));
			if (utworzonyObraz.pixele[i] == NULL)
			{
				for (int j = 0; j <= i; j++)
				{
					free(utworzonyObraz.pixele[i]);
				}
				free(utworzonyObraz.pixele);
				printf("B³¹d alokacji pamiêci!\nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
				getch();
				utworzonyObraz.ok = -1;
				return utworzonyObraz;
			}

		}
	}
	else
	{
		printf("B³¹d alokacji pamiêci!\nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
		getch();
		utworzonyObraz.ok = -1;
		return utworzonyObraz;
	}
	printf("Podaj nazwê dla obrazu: \n");
	scanf("%s", utworzonyObraz.nazwa);
	for (int i = 0; i < utworzonyObraz.wysokosc; i++)
	{
		for (int j = 0; j < utworzonyObraz.szerokosc; j++)
		{
			utworzonyObraz.pixele[i][j] = rand() % (utworzonyObraz.glebia + 1);
		}
	}
	return utworzonyObraz;

}



int main()
{
	srand(time(NULL));
	int aktiv=0;
	setlocale(LC_ALL, "polish_poland");
	int ilosc = 0;
	struct obraz* listaObrazow = malloc(sizeof(struct obraz));
	if (listaObrazow == NULL)
	{
		printf("Krytyczny b³¹d allokacji pamiêci!\nNaciœnij dowolny klawisz aby zakoñczyæ dzia³anie programu...\n");
		getch();
		return 0;
	}
	int isOn = 1;
	int is2, is1;
	int wybor1, wybor2, wybor3;
	while (isOn==1)
	{
		system("cls");
		printf("Wszytkie dzia³ania wykonywane s¹ na aktualnie \"aktywnym\" obrazie.\n");
		printf("0-> Wybierz aktywny obraz. \n");
		printf("1-> Wczytaj obraz z pliku.\n");
		printf("2-> Zapisz obraz. \n");
		printf("3-> Usuñ obraz z listy.\n");
		printf("4-> Zmieñ nazwê obrazu.\n");
		printf("5-> Menu przetwarzania obrazów.\n");
		printf("6-> Menu przetwarzania obrazów +.\n");
		printf("7-> Utwórz obraz z losowymi wartoœciami\n");
		printf("33-> Zapisz wszystkie obrazy z listy.\n");
		printf("Aby zakoñczyæ dzia³anie programu wprowadŸ dowoln¹ inn¹ liczbê.\n");

		if (ilosc > 0) wyswietlListeObrazow(listaObrazow, ilosc, aktiv);	
		wybor1 = scanfInt();
		switch (wybor1)
		{
		case 1:
			dodajObrazDoListy(&listaObrazow, &ilosc, wczytajObraz());
			break;

		case 2:
			if (ilosc < 1)
			{
				printf("Nie ma obrazu do zapisania.\nNaciœnij dowolny klawisz aby kontynuowaæ"); 
				getch();
				break;
			}
			zapiszObraz(listaObrazow[aktiv]);
			break;

		case 0:
			wybierzAktiv(&aktiv, ilosc);
			break;

		case 3:
			usunObrazZTablicy(&listaObrazow, &ilosc, &aktiv);
			break;
		case 4:
			if (ilosc < 1)
			{
				printf("Nie ma obrazu.\nNaciœnij dowolny klawisz aby kontynuowaæ...\n");
				getch();
				break;
					
			}
			zmienNazweObrazu(&listaObrazow[aktiv]);
			break;
		case 5:
			is1 = 1;
			system("cls");
			if (ilosc == 0)
			{
				printf("Lista obrazów jest pusta!\nNaciœnij dowolny klawisz aby kontynuowaæ..\n");
				getch();
				break;
			}
			while (is1 == 1)
			{
				system("cls");
				printf("Wszytkie dzia³ania wykonywane s¹ na aktualnie \"aktywnym\" obrazie.\n");
				printf("0-> Wybierz aktywny obraz. \n");
				printf("1-> Obróæ obraz o wielokrotnoœæ 90st w prawo.\n");
				printf("2-> Stwórz histogram i zapisz do pliku.\n");
				printf("3-> Odbij obraz wzglêdem osi Y.\n");
				printf("4-> Dodaj szum typu pieprz/sól do obrazu.\n");
				printf("5-> Filtr Gaussa z dowolnym rozmiarem okna.\n");
				printf("Aby powróciæ wprowad¿ dowoln¹ liczbê.\n");

				if (ilosc > 0) wyswietlListeObrazow(listaObrazow, ilosc, aktiv);
				wybor2 = scanfInt();
				switch (wybor2)
				{
				case 0:
					wybierzAktiv(&aktiv, ilosc);
					is1 = 1;
					break;
				case 1:
					dodajObrazDoListy(&listaObrazow, &ilosc, obrocObrazWPrawo(listaObrazow[aktiv]));
					is1 = 0;
					break;
				case 2:
					stworzIZapiszHistogram(listaObrazow[aktiv]); 
					is1 = 0;

					break;
				case 3:
					dodajObrazDoListy(&listaObrazow, &ilosc, odbijObraz(listaObrazow[aktiv]));
					is1 = 0;

					break;
				case 4:
					dodajObrazDoListy(&listaObrazow, &ilosc, zaszumObraz(listaObrazow[aktiv]));
					is1 = 0;

					break;
				case 5:
					dodajObrazDoListy(&listaObrazow, &ilosc, prostyGauss(listaObrazow[aktiv]));
					is1 = 0;

					break;
				default:
					is1 = 0;
					break;
				}

			}
			break;

		case 6:
			is2 = 1;
			system("cls");
			if (ilosc == 0)
			{
				printf("Lista obrazów jest pusta!\nNaciœnij dowolny klawisz aby kontynuowaæ..\n");
				getch();
				break;
			}
			while (is2 == 1)
			{
				system("cls");
				printf("Wszytkie dzia³ania wykonywane s¹ na aktualnie \"aktywnym\" obrazie.\n");
				printf("0-> Wybierz aktywny obraz. \n");
				printf("1-> Filtr medianowy o dowolnym rozmiarze okna\n");
				printf("2-> Dowolna konwolucja z dowolnym rozmiarem okna.\n");
				printf("3-> Algorytm Canny`ego do detekcji krawêdzi.\n");
				printf("4-> Wyrównanie histogramu obrazu.\n");
				printf("5-> Zmiana rozmiaru obrazu.\n");
				printf("Aby powróciæ wprowad¿ dowoln¹ liczbê.\n");
				if (ilosc > 0) wyswietlListeObrazow(listaObrazow, ilosc, aktiv);
				wybor3 = scanfInt();
				switch (wybor3)
				{
				case 0:
					wybierzAktiv(&aktiv, ilosc);
					is2 = 1;
					break;
				case 1:
					dodajObrazDoListy(&listaObrazow, &ilosc, dowolnaMediana(listaObrazow[aktiv]));
					is2 = 0;
					break;
				case 2:
					dodajObrazDoListy(&listaObrazow, &ilosc, dowolnaKonwolucja(listaObrazow[aktiv]));
					is2 = 0;
					break;
				case 3:
					dodajObrazDoListy(&listaObrazow, &ilosc, legitCannyEdgeDetect(listaObrazow[aktiv]));
					is2 = 0;
					break;
				case 4:
					dodajObrazDoListy(&listaObrazow, &ilosc, wyrownajHistogram(listaObrazow[aktiv]));
					is2 = 0;
					break;	
				case 5:
					dodajObrazDoListy(&listaObrazow, &ilosc, zmienRozmiarObrazu(listaObrazow[aktiv]));
					is2 = 0;
					break;
				default:
					is2 = 0;
					break;
				}
				

			}
			break;
			
		case 7:
			dodajObrazDoListy(&listaObrazow, &ilosc, dodajObraz());
			break;


		case 33:
			for (int i = 0; i < ilosc; i++)
			{
				zapiszObraz(listaObrazow[i]);
			}
			break;

		default:
			isOn = -1;
			break;
		}

	}

	zwolnijPamiec(&listaObrazow, ilosc);
	_CrtDumpMemoryLeaks();
	
}
