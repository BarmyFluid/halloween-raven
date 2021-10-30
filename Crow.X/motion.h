#ifndef MOTION_H
#define	MOTION_H

#define NeckLeft msToTcy(1)
#define NeckCentre msToTcy(1.5)
#define NeckRight msToTcy(2)

void motionBegin();
void motionOpenMouth();
void motionCloseMouth();
void motionTurnHead(const unsigned long from, const unsigned long to);

#endif	/* MOTION_H */