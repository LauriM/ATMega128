#include "draw.h"
#include "lcd.h"


void DrawBox(int x1, int y1, int x2, int y2);
{
	//MIIKA
}

void DrawFilledBox(int x1, int y1, int x2, int y2)
{
	//MIIKA
}

void DrawCircle(int x1, int y1, int r)
{
	//MIIKA
}

void DrawLine(int x1, int y1, int x2, int y2)//enemman tyota kuin odotin TODO: testaus
{
	//JP
	int deltaX = x2 - x1;//x suunnassa janan pituus
	int deltaY = y2 - y1;//y suunnassa janan korkeus
	
	//arvot voivatr olla negatiivisia joten joudutaan ottamaan niiden kahden potenssi
	if ((deltaX*deltaX) > (deltaY*deltaY)) //katsotaan kumpi pituus vai korkeus on suurempi ja incrementoidaan sita yhdella 
	{
		float incrementY = deltaY / deltaX;
		if (deltaY >= 0) //jos pituus on positiivinen
		{
			for (int i = 0; i < deltaX; i++) //mennaan pituuden verran
			{
				if (incrementY > 0) //jos korkeus on positiivinen
					int Y = (int)(incrementY * i + 0.5);//magiaa
				else //jos korkeus on negatiivinen
					int Y = (int)(incrementY * i - 0.5);//magiaa

				lcd_pixel(x1 + i, y1 + Y);
			}
		}
		else // jos pituus on negatiivinen
		{
			for (int i = 0; i > deltaX; i--)//mennaan pituuden verran
			{
				if (incrementY > 0) //jos korkeus on positiivinen
					int Y = (int)(incrementY * i + 0.5);//magiaa
				else //jos korkeus on negatiivinen
					int Y = (int)(incrementY * i - 0.5);//magiaa

				lcd_pixel(x1 + i, y1 + Y);
			}
		}
	}
	else //jos korkeus on suurempi kuin pituus
	{
		float incrementX = deltaX / deltaY;
		if (deltaX >= 0) // jos korkeus on positiivinen
		{
			for (int i = 0; i < deltaY; i++)//mennaan korkeuden verran
			{
				if (incrementX > 0) //jos pituus on positiivinen
					int X = (int)(incrementX * i + 0.5);//magiaa
				else //jos pituus on negatiivinen
					int X = (int)(incrementX * i - 0.5);//magiaa

				lcd_pixel(x1 + X, y1 + i);
			}
		}
		else // jos korkeus on negatiivinen
		{
			for (int i = 0; i > deltaX; i--) //mennaan korkeuden verran
			{
				if (incrementX > 0) //jos pituus on positiivinen
					int X = (int)(incrementX * i + 0.5);
				else //jos pituus on negatiivinen
					int X = (int)(incrementX * i - 0.5);

				lcd_pixel(x1 + X, y1 + i);
			}
		}

	}
}
/*
{1, 0, 0, 0, 0}
{0, 1, 0, 0, 0}
{0, 0, 1, 0, 0}
{0, 0, 0, 1, 0}
{0, 0, 0, 0, 1}
length = 5
height = 5
x = 30
y = 30
*/
void DrawBitmap(char* array2D, int lenght, int height, int x, int y)
{
	//JP
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < lenght; j++)
		{
			if (array2D[i][j] = '1')
				lcd_pixel(x + i, y + j);
		}
	}
}


