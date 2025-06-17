#ifndef IO_H
# define IO_H

# include "types.h"


void send_byte_to_port(uint16_t port, uint8_t value);
uint8_t read_byte_from_port(uint16_t port);

#endif