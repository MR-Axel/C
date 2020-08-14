#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // usleep
#include <sys/io.h> // ioperm unsigneds, outb
#include <string.h>
#define P 0x70
#define R 0x00
#define PA 0x378

int fmenu ()
{
	int thisrta;
	printf("Choose an option:\n");
	printf("1) Show registers 0-13.\n");
	printf("2) Set Alarm.\n");
	printf("3) Pulses at 4Hz.\n");
	printf("4) Send RTC hour with paralel port, and show it.\n");
	printf("5) Exit.\n");
	scanf ("%d", &thisrta);
	return thisrta;
}

void per (unsigned long reg, unsigned long cant, int sudo)
{
    if (ioperm(reg, cant, sudo))
		{
				perror("ioperm \n");
        exit(1);
    }
}

void freg ()
{
	unsigned char regi, regj=0, bi, remain=0;
	int j, bina [8];
	per(P, 2, 1);
	per(R, 14, 1);
	char vec [15][14];
	strcpy (vec[0], "Seconds");
	strcpy (vec[1], "Seconds Alarm");
	strcpy (vec[2], "Minutes");
	strcpy (vec[3], "Minutes Alarm");
	strcpy (vec[4], "Hours	");
	strcpy (vec[5], "Hours Alarms");
	strcpy (vec[6], "Day of week");
	strcpy (vec[7], "Day of month");
	strcpy (vec[8], "Month	");
	strcpy (vec[9], "Year	");
	strcpy (vec[10], "Reg. A	");
	strcpy (vec[11], "Reg. B	");
	strcpy (vec[12], "Reg. C	");
	strcpy (vec[13], "Reg. D	");
	printf("Description  Register   Binary   Hex Type\n");
	for (int i = 0; i < 14; i++)				//recorro los registros para mostrarlos
	{
		outb (R+i, P);		// apunto en el 0x70 el registro 0x00 + i
		regi = inb(P+1);	//valor que contiene 0x71 en ese momento (0x00+i)
		bi = regi;		//otra aux para poder pasarlo a hex modificando la misma var
		printf("%s      %x  ", vec[i], regj);	//muestra registros y binario
		for (j = 0; j<8; j++)
    {
  		remain = bi % 2;
			bina[7-j] = remain;
  	  bi = bi / 2;
		}
		for (j = 0; j<8; j++) printf ("%u", bina[j]);		// Muestra el valor Binario
		printf (" 0x%02hhx", regi);			//muestra valor hex
		regj++;
		if (regi & 0x01 ) printf("  Unpair\n");		//comparo que el último sea 1
			else printf(" Pair\n");
	}
	per(P, 2, 0);					//Quito Permisos
	per(R, 14, 0);
}

void falarm ()
{
	unsigned char h, m, s, md, u=0, regc;
	printf ("Please, set alarm (HH:MM:SS): ");
	scanf ("%hhx:%hhx:%hhx", &h, &m, &s);
	printf ("\nNumber of alarms (one every minute): ");
	scanf ("%hhx", &md);
	per(P, 2, 1);
	outb (0x01, P);	// Seteo hora, mins y segs
	outb (s, P+1);
	outb (0x03, P);
	outb (m, P+1);
	outb (0x05, P);
	outb (h, P+1);
	for (int z=1; z<=md; z++)		//voy desde 1ra alarma a cantidad deseadas
	{
		outb (0x0C, P);
		regc = inb (P+1); 						//digo que regc es todo el 0x0c (registro C)
	  printf ("Waiting alarm...\n");
		for (int i = 0; i < 61; i++)		//polling del reg C (pregunta si hay alarma)
		{ 																						//Espera 50 segundos en total
			regc = inb (P+1);
	    if (regc & 0x20) 												//Verif bit 5 (AF) con 0010 0000
	    {
	   		printf ("Ring! Ring!\n");
	   		break;
	   	}
	   	usleep(1000000);   												//Esperar 1000ms -> 1 segundo
	  }
		printf ("Alarm %hhx \n", m);					//muestro los minutos actuales
		if ((m+z)>59)						//verifico que no sea min 00
			{
				h++;						//si lo es, pongo nueva hora en alarma
				outb (0x05, P);
				outb (h, P+1);
				m=00;				//reinicio los minutos a 0
				u=0;			//reinicio el contador a 0
			}
		outb (0x03, P);
		outb (m+u, P+1);		//guardo la nueva alarma minuto + u
		u++;
	}
	per(P, 2, 0);
}

void fpulse ()
{
	unsigned char regc, rega;
	int tim;
	per (P, 2, 1);
	outb (0x0A, P);			// marco el registro A
	rega = inb (P+1);
	rega = ((rega & 0xF0) | 0x0E);
	outb (rega, P+1);		// configuro para freq de 4hz, xxxx 1110 en el A
	outb (0x0C, P);
	printf("0---+---1---+---2---+---3---+---4---+---5---+---6---+---7---+---8---+---9---+---10 \n");
	for (int i = 0; i < 81; i++)
	{
		regc = inb (P+1);
		if ((regc & 0x40)==0x40) printf("|"); //comparo con bit 6 (PF) 0100 0000
			else printf ("_");
		fflush (stdout);
		usleep (124500);		//125 ms, va a pasar 10 segundos
	}
	per (P, 2, 0);
}

void fsendhour ()
{
	unsigned char paso;
	per(P, 2, 1);
	per(PA, 3, 1);
	outb (0x04, P);		//marco la hora
	paso = inb(P+1);	//variable con la hora a pasar
	printf("Value written on parallel port 0x%hhx. \n", paso);
	paso = ~paso;	//niego el valor para pasarlo ya que del otro lado se invierte
	outb (paso, PA);	// Valor que debería verse del otro lado
	printf("Value of the other side	0x%hhx. \n", paso);
	per(PA, 3, 0);
	per(P, 2, 0);
}

int main ()
{
	printf("Axel Rosso\n");
	printf("File: 7858\n");
	int rta;
	do
	{
		rta = fmenu();
		switch (rta)
		{
			case 1:
			printf("\n");
			freg();
			break;

			case 2:
			printf("\n");
			falarm();
			break;

			case 3:
			printf("\n");
			fpulse();
			break;

			case 4:
			printf("\n");
			fsendhour();
			break;

			case 5:
			break;

			default:
			printf("\n");
			printf("Wrong answer.\n");
			break;
		}
		printf("\n \n");
	} while (rta!=5);
return 0;
}
