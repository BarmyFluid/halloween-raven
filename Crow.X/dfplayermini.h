#ifndef DFPLAYERMINI_H
#define	DFPLAYERMINI_H

#include "common.h"

void dfpBegin();
void dfpSetVolume(uint8_t volume);
void dfpPlay(uint8_t folder, uint8_t file);
void dfpExecute(uint8_t command, uint8_t param1, uint8_t param2);

#endif	/* DFPLAYERMINI_H */