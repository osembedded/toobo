#ifndef __EEPROM_H__
#define __EEPROM_H__

typedef enum {
  EE_PAGE1 = 0x50,
  EE_PAGE2,
  EE_PAGE3,
  EE_PAGE4,
  EE_PAGE5,
  EE_PAGE6,
  EE_PAGE7,
  EE_PAGE8
}EE_PAGE;


void clear_24lc16b_eeprom(int file);

unsigned int read_eeprom(int file, 
			 int dev_addr, 
			 unsigned char addr, /* only b/w 0-15 */
			 unsigned char *buf,
			 int num_bytes);

unsigned char write_eeprom(int file, 
			   int dev_addr, 
			   unsigned char addr, /* only b/w 0-15 */
			   unsigned char *buf,
			   int num_bytes);

#endif /* __EEPROM_H__ */
