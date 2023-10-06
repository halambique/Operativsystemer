#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#define THREADS 4

// Matrisemultiplikasjon: Beregner produktet C = AB

#define MAX 64			// Maksimalt antall rader/kolonner

int A[MAX][MAX],		// De tre matrisene som skal brukes
  B[MAX][MAX],			// deklareres globalt, slik at de er
  C[MAX][MAX];			// tilgjengelige for alle funksjoner

int N;				// Antall rader og kolonner

char *filnavn_A = "A.txt",	// Navn pÃ¥ de tre filene
  *filnavn_B = "B.txt",		// som brukes til Ã¥
  *filnavn_C = "C.txt";		// lagre matrisene

// Leser inn N og dataene for A og B fra filer
void
les_AB ()
{
  FILE *fil_A, *fil_B;
  int tmp, i, j;

  fil_A = fopen (filnavn_A, "r");
  fil_B = fopen (filnavn_B, "r");
  if (fil_A == NULL || fil_B == NULL)
    {
      printf ("Feil: Kan ikke Ã¥pne fil\n");
      exit (-1);
    }

  fscanf (fil_A, "%d", &N);
  if (N > MAX)
    {
      printf ("Feil: N > %d\n", MAX);
      exit (-1);
    }
  fscanf (fil_B, "%d", &tmp);
  if (N != tmp)
    {
      printf ("Feil: Matrisene har ulike dimensjoner\n");
      exit (-1);
    }

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      {
	fscanf (fil_A, "%d", &A[i][j]);
	fscanf (fil_B, "%d", &B[i][j]);
      }

  fclose (fil_A);
  fclose (fil_B);
}


// Skriver resultatet C = AB ut til fil
void
skriv_C ()
{
  FILE *fil_C;
  int i, j;

  fil_C = fopen (filnavn_C, "w");
  if (fil_C == NULL)
    {
      printf ("Feil: Kan ikke Ã¥pne fil\n");
      exit (-1);
    }

  fprintf (fil_C, "%d\n", N);
  for (i = 0; i < N; i++)
    {
      for (j = 0; j < N; j++)
	fprintf (fil_C, "%d ", C[i][j]);
      fprintf (fil_C, "\n");
    }

  fclose (fil_C);
}

// UtfÃ¸rer hele matrisemultiplikasjonen
void *
multipliser_AB_threaded (void *arg)
{

  int i, j, k;
  long start = (long) arg;
  long end = start + N / THREADS;

  for (i = start; i < end; i++)
    for (j = 0; j < N; j++)
      {
	C[i][j] = 0;
	for (k = 0; k < N; k++)
	  C[i][j] += A[i][k] * B[k][j];
      }
}

void
multipliser_AB ()
{
  pthread_t threads[THREADS];
  for (long i = 0; i < THREADS; i++)
    {
      pthread_create (&threads[i], NULL, multipliser_AB_threaded,
		      (void *) (i * N / THREADS));
    }
  for (int i = 0; i < THREADS; i++)
    {
      pthread_join (threads[i], NULL);
    }
}

// Hovedprogram
int
main ()
{
  les_AB ();
  multipliser_AB ();
  skriv_C ();
}
