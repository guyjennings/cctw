/*
 * cctwsimple.h
 *
 *  Created on: Jul 9, 2013
 *      Author: wozniak
 */

#ifndef CCTWSIMPLE_H
#define CCTWSIMPLE_H

int CctwMaxRequisites(void);

int CctwRequisites(int max_x, int max_y, int max_z,
                   int osx, int osy, int osz, int* output);

void CctwSimpleTransform(int osx, int osy, int osz,
                         void **inputBlobs, int inputCount,
                         void **outputBlobs);

#endif
