#ifndef EYES_H
#define	EYES_H

typedef unsigned char eye_state_t;
const eye_state_t EYE_ON = 0;
const eye_state_t EYE_OFF = 1;

void eyesBegin();
void eyesSet(eye_state_t left, eye_state_t right);
void eyesBlink();
void eyesWink(eye_state_t left, eye_state_t right);

#endif	/* EYES_H */