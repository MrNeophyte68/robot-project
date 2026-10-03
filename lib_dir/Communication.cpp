#include "Communication.h"

Communication::Communication()
{
    UBRR0H = 0;

    UBRR0L = 0xCF;

    UCSR0A = (1 << UDRE0);

    UCSR0B = (1 << RXEN0) | (1 << TXEN0) ;

    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

Communication::~Communication() {}

void Communication::transmissionUART(uint8_t data) 
{

  /* Wait for empty transmit buffer */
  while ( !( UCSR0A & (1<<UDRE0)) )
  ;
  /* Put data into buffer, sends the data */
  UDR0 = data;

}

void Communication::resetRegisters()
{
    UCSR0A = 0;

    UCSR0B = 0;

    UCSR0C = 0;
}

void Communication::transmitString(const char *data)
{
    while (*data)
    {
        transmissionUART(*data++);
    }
}

uint8_t Communication::receive()
{
  while ( !( UCSR0A & (1 << RXC0)) );
  return UDR0;
}
