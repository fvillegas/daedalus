#ifndef __HCM5883L_H_
#define __HCM5883L_H_

void hcm5883l_init();
void set_gain(uint8_t gain);
void set_mode(uint8_t new_mode);
void hcm5883l_get_heading(int16_t *x, int16_t *y, int16_t *z);

#endif