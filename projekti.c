#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "projekti.h"


// 1. Lisää maan tietokantaan. Alkutilassa maalla on kutakin mitalia 0 kappaletta.
struct Maa* lisaamaa(struct Maa* lista, const char* str, int lkm) {
	char A; // A-kirjain
	struct Maa* uusi_lista = realloc(lista, (lkm + 1) * sizeof(struct Maa));
	uusi_lista[lkm].kulta = 0;
	uusi_lista[lkm].hopea = 0;
	uusi_lista[lkm].pronssi = 0;
	sscanf(str, "%c %s", &A, uusi_lista[lkm].maanimi);
	return uusi_lista;
}


// 2. Lisää annetulle maalle annetun määrän mitaleita.
struct Maa* paivitamitalit(struct Maa* lista, const char* str, int lkm) {
	//char M; // M-kirjain
	char maa[50];
	int n = 0;
	int p_kulta;
	int p_hopea;
	int p_pronssi;
	sscanf(str, "M %s %d %d %d", maa, &p_kulta, &p_hopea, &p_pronssi);
	//selvitetään monesko kyseinen maa on listassa
	//jos maata ei löydy listasta tulostetaan virheilmoitus

	while (strcmp(maa,lista[n].maanimi)!=0) {	
		n++;
		if (n == lkm+1){
			printf("Maata ei loydy listasta.\n");
			printf("Yrita uudelleen!\n");
			return lista;
		}		
		
	}

	//päivitetään mitalien lkm
	lista[n].kulta = lista[n].kulta + p_kulta;
	lista[n].hopea = lista[n].hopea + p_hopea;
	lista[n].pronssi = lista[n].pronssi + p_pronssi;
	printf("Paivitettiin onnistuneesti!\n");
	return lista;
}



int cmpfunc(const void * a, const void * b) {
	if ((((*(struct Maa*)a).kulta) - ((*(struct Maa*)b).kulta))==0){
		if ((((*(struct Maa*)a).hopea) - ((*(struct Maa*)b).hopea))==0){
			return (((*(struct Maa*)a).pronssi) - ((*(struct Maa*)b).pronssi));	
		}
		else{
			return (((*(struct Maa*)a).hopea) - ((*(struct Maa*)b).hopea));
		}
	}
	else{
		return (((*(struct Maa*)a).kulta) - ((*(struct Maa*)b).kulta));
	}
}


// 3. funktio tulostaa maat ja ansaitut mitalit riveittäin seuraavalla tavalla määrätyssä järjestyksessä
// 1. eniten kultamitaleita, 2. eniten hopeamitaleita (jos kultaa sama lkm), 3. eniten pronssimitaleita (jos kultaa ja hopeaa sama lkm)

void tulostamitalitaulukko(struct Maa* lista, int j) {
	qsort(lista, j, sizeof(struct Maa), cmpfunc);
	j--;
	while (j >= 0) {
		printf("%s %d %d %d", lista[j].maanimi, lista[j].kulta, lista[j].hopea, lista[j].pronssi);
		printf("\n");
		j--;
	}
}


// 4. Tallentaa mitallitaulukon tiedostoon annetulla tiedostonnimellä
void tallennataulukko(struct Maa* lista, const char* str, int i) {
	char W; // W-kirjain
	char tiedosto[20];
	int n;
	sscanf(str, "%c %s", &W, tiedosto);
	FILE* file = fopen(tiedosto, "w");
	if (file == NULL) {
		printf("Virhe tiedoston avaamisessa.\n");
		exit(1);
	}
	else {
		for (n = 0; n < i; n++) {
			fprintf(file, "%s %d %d %d\n", lista[n].maanimi, lista[n].kulta, lista[n].hopea, lista[n].pronssi);
		}
	}
	fclose(file);
}



// 5. Lataa mitallitaulukon tiedostosta, korvaten muistissa aiemmin olleen taulukon
struct Maa* lataataulukko(struct Maa* lista, const char* str, int i) {
	char O; //O-kirjain
	char tiedosto[20];
	int n = 0;
	free(lista);
	printf("Vanha lista vapautettiin.\n");
	struct Maa* uusi_lista = malloc(sizeof(struct Maa));

	sscanf(str, "%c %s", &O, tiedosto);
	FILE* file = fopen(tiedosto, "r");
	
	if (file == NULL){
		printf("Virhe tiedoston avaamisessa.\n");
		exit(1);
	}
	
	else{
		while (fscanf(file, "%s %d %d %d\n", uusi_lista[n].maanimi, &uusi_lista[n].kulta, &uusi_lista[n].hopea, &uusi_lista[n].pronssi)==4) {
			struct Maa* uusi_lista = realloc(uusi_lista, (n + 1) * sizeof(struct Maa));		
			n++;
			printf("n = %d\n", n);
		}
	}
	//uusi_lista[n].maanimi = NULL;  en osannut laittaa oikein
	return uusi_lista;
}




// 6. Poistuu ohjelmasta ja vapauttaa käytetyn muistin.
void poistuohjelmasta(struct Maa* lista) {
	free(lista);
	exit(0);
	
}


int main() {

	char str[100];
	struct Maa* lista = malloc(sizeof(struct Maa));

	int i = 0; //tietokantaan merkittyjen maiden lkm.

	while (fgets(str, 100, stdin) != NULL) {

		printf("syote:  %s", str); 
		if (str[0] == 'A') {
			lista = lisaamaa(lista, str, i);
			i++;
			printf("Lisattiin onnistuneesti!\n");
		}

		else if (str[0] == 'M') {
			lista = paivitamitalit(lista, str, i);
		}

		else if (str[0] == 'L') {
			tulostamitalitaulukko(lista, i);
			printf("Mitalitaulukko tulostettiin onnistuneesti!\n");
		}

		else if (str[0] == 'W') {
			tallennataulukko(lista, str, i);
			printf("Taulukko tallennettiin onnistuneesti!\n");
		}
		
		else if (str[0] == 'O') {
			lista = lataataulukko(lista, str, i);
			//lasketaan uusi maiden lkm eli i
			int i = 0;
			while (lista[i].maanimi!=NULL) {
				i++;
			}
			//täällä ei lasketa oikein listan alkioiden lkm...
			printf("Taulukko ladattiin onnistuneesti!\n");
		}

		else if (str[0] == 'Q') {
			printf("Ohjelmasta poistutaan!\n");
			poistuohjelmasta(lista);
		}
		else {
			printf("Tapahtui virhe!");
		}
		printf("\n");
	}
	return 0;
}
