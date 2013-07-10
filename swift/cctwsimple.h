/*
 * cctwsimple.h
 *
 *  Created on: Jul 9, 2013
 *      Author: wozniak
 */

#ifndef CCTWSIMPLE_H
#define CCTWSIMPLE_H

int CctwMaxRequisites(void);

void CctwSimpleTransform(int osx, int osy, int osz,
                         void **inputBlobs, int inputCount,
                         void **outputBlobs);

#endif
