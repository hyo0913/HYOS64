/*
 * ScreenClear.c
 *
 *  Created on: 2022. 3. 2.
 *      Author: root
 */

void SceenClear()
{
	int i = 0;
	char* mem = (char*)0xB800;
	
	while (1) {
		mem[i] = 0;
		mem[i+1] = 0x0A;

		i += 2;

		if(i >= 80 * 25 * 2) {
			break;
		}
	}
}
