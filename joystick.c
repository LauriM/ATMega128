#include <avr/io.h>

/****************************************************************************
void ADC_Init()

AD-muuntimen alustaminen 
****************************************************************************/
void ADC_Init()
{
	ADCSRA |= 1<<ADPS2; 			//Esijakaja 64 -> Taajuus sopivaksi AD-muuntimelle
	ADCSRA |= 1<<ADPS1;
	
	ADMUX |= 1<<ADLAR;				// AD-muunnoksen tulos vasemmalle vieritetty
	
	ADMUX |= 1<<REFS0;
	ADMUX &= ~(1<<REFS1);			//Avcc(+5v) muuntimen referenssijännitteeksi
	
	//ADCSRA |= 1<<ADIE;			//Sallitaan AD-muunnnoksen tuottama keskeytys
	ADCSRA |= 1<<ADEN;				//Otetaan AD-muunnin käyttöön
	//sei();						//Sallitaan keskeytykset
}

/****************************************************************************
void ADC_Read(unsigned char ch)

AD-muuntimen lukeminen ja tuloksen palauttaminen
****************************************************************************/
short ADC_Read(unsigned char ch)
{
	short ADCresult = 0;

	ADMUX &= (~0x1F);					//Nollataan rekisterin kanavanvalintabitit
	ADMUX |= ch;						//otetaan haluttu kanava käyttöön
	
	ADCSRA |= 1<<ADSC;							//Aloitetaan uusi muunnos
    while(!(ADCSRA & (1<<ADIF)));				//Odotetaan että muunnos on valmis
	
	uint8_t theLowADC = (ADCL>>6);	// Luetaan AD-muuntimelta tuleva LSB ja bittien siirto
	uint8_t theHighADC = ADCH;		// Luetaan AD-muuntimelta tuleva MSB

	ADCresult = theLowADC | (theHighADC<<2);	//Yhdistetään AD-muuntimen LSB ja MSB ja bittien siirto
	ADCresult = ADCresult & 0x03FF;				//Tuloksen maskaus	
	
	return ADCresult;
}
