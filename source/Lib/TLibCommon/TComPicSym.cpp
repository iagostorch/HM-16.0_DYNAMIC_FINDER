/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2014, ITU/ISO/IEC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the ITU/ISO/IEC nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/** \file     TComPicSym.cpp
    \brief    picture symbol class
*/

#include "TComPicSym.h"
#include "TComSampleAdaptiveOffset.h"
#include "TComSlice.h"
#include <math.h>   //IAGO

//! \ingroup TLibCommon
//! \{

// ====================================================================================================================
// Constructor / destructor / create / destroy
// ====================================================================================================================

TComPicSym::TComPicSym()
:m_uiWidthInCU(0)
,m_uiHeightInCU(0)
,m_uiMaxCUWidth(0)
,m_uiMaxCUHeight(0)
,m_uiMinCUWidth(0)
,m_uiMinCUHeight(0)
,m_uhTotalDepth(0)
,m_uiNumPartitions(0)
,m_uiNumPartInWidth(0)
,m_uiNumPartInHeight(0)
,m_uiNumCUsInFrame(0)
,m_apcTComSlice(NULL)
,m_uiNumAllocatedSlice (0)
,m_apcTComDataCU (NULL)
,m_iNumColumnsMinus1 (0)
,m_iNumRowsMinus1(0)
,m_puiCUOrderMap(0)
,m_puiTileIdxMap(NULL)
,m_puiInverseCUOrderMap(NULL)
,m_saoBlkParams(NULL)
{}

//IAGO
//SUMS THE TILE PARAMETERS IN THE SQUARE LIMITED BY THE INITIAL/FINAL ROWS/COLLUMN
float sumTilePartitions(double frameMatrix[100][100], int initialRow, int finalRow, int initialColumn, int finalColumn){	//FINAL COLUMN AND ROW ARE OPEN
    //printf("calculando limites %d-%d %d-%d\n", initialRow, finalRow, initialColumn, finalColumn);
    int i, j;
    float tileTotal = 0.0;
    for(i=initialRow; i<finalRow; i++){
    	for(j=initialColumn; j<finalColumn; j++){
        	tileTotal = tileTotal + frameMatrix[i][j];
        }
    }
    return tileTotal;
}   
//#########################POR TERMINAR###########################
void shiftLeftBoundaries(TComPPS *pps, UInt* optimalCols, UInt *optimalRows){       //SHIFTS EVERY BOUNDARY 1 CU TO THE LEFT
    int boundary;
    for(boundary=0; boundary < pps->getTileNumRowsMinus1(); boundary++){
        if((boundary==0 && optimalCols[boundary]>4) || (boundary>0 && optimalCols[boundary]-optimalCols[boundary-1]>4)){ //IF THE GIVEN TILE IS BIGGER THAN THE MINIMUM SIZE, MAKE IT 1 CU SMALLER
            optimalCols[boundary] = optimalCols[boundary]-1;
        }
    }
}
void extractOptimalPartitions(TComPPS *pps, double *time_CU, UInt* optimalCols, UInt* optimalRows, UInt widthInCU, UInt heightInCU){ 
    //double** frameMatrix = (double**) malloc(heightInCU*sizeof(double*));
    //for(i=0; i<widthInCU; i++)  frameMatrix[i] = (double*) malloc(widthInCU*sizeof(double*));
    //printf("EXTRAINDO OPTIMAL\n");
    int i, j, boundary;
    double frameMatrix[100][100];
    double total, average, current, currentDifference, previous, previousDifference;
    
    
    
    for(i=0; i<heightInCU; i++){        //INITIALISE THE FRAME MATRIX WITH THE ARRAY
        for(j=0; j<widthInCU; j++){
            frameMatrix[i][j] = time_CU[i*widthInCU+j];
            //printf("%d %f\n", i*widthInCU+j, time_CU[i*widthInCU+j]);
        }
    }
     
    /*  PRINTS FRAME MATRIX
    for(i=0; i<heightInCU; i++){
        for(j=0; j<widthInCU; j++){
            printf("%f ", frameMatrix[i][j]);
        }
        printf("\n");
    }
    //*/
    total = sumTilePartitions(frameMatrix, 0, heightInCU, 0, widthInCU);
    //printf("TOTAL %f\n", total);
    
    previous = 0;
    average = total/(pps->getTileNumRowsMinus1()+1);
    //printf("average rows %f\n", average);
    for(boundary = 0; boundary < pps->getTileNumRowsMinus1(); boundary++){
        for(i=0; i<heightInCU; i++){
            current =  sumTilePartitions(frameMatrix, 0, i, 0, widthInCU);
            if(current > (boundary+1)*average){
                currentDifference = fabs((boundary+1)*average - current);
                previousDifference = fabs((boundary+1)*average - previous);
                if(currentDifference > previousDifference)  //PREVIOUS IS BETTER
                    optimalRows[boundary] = i-1;
                else
                    optimalRows[boundary] = i;
                break;
            }
            previous = current;
        }
    }
    
    previous=0;
    average = total/(pps->getNumTileColumnsMinus1()+1);
    //printf("average columns %f\n", average);
    for(boundary = 0; boundary < pps->getNumTileColumnsMinus1(); boundary++){
        for(j=0; i<widthInCU; j++){
            current =  sumTilePartitions(frameMatrix, 0, heightInCU, 0, j);         //CALCULATE THE CURRENT PARTITION TIME
            if(current > (boundary+1)*average){                                 //COMPARES THE CURRENT WITH THE OPTIMAL DESIRED TIME, IF IT'S BIGGER THEN
                currentDifference = fabs((boundary+1)*average - current);       //COMPARES THE CURRENT PARTITION WITH THE PREVIOUS ONE AND SELECT THE BEST
                previousDifference = fabs((boundary+1)*average - previous);
                if((currentDifference > previousDifference) && ((boundary==0 && j>=5) || (boundary>0 && j-optimalCols[boundary-1]>=5))){  //PREVIOUS IS BETTER, COLUMN MUST BE 4 CUs WIDE OR MORE
                    optimalCols[boundary] = j-1;
                    break;
                }
                else if((boundary==0 && j>=4) || (boundary>0 && j-optimalCols[boundary-1]>=4)){                                           //COLUMN MUST BE 4 CUs WIDE OR MORE
                    optimalCols[boundary] = j;
                    break;
                }
            }
            previous = current;
        }
        if(j == widthInCU){     //IF J HIT THE LAST CU, THERE WASN'T A POSSIBLE PARTITION 
            printf("FALTOU ESPAÇO\n");
            shiftLeftBoundaries(pps, optimalCols, optimalRows); //SHIFTS EVERY BOUNDARY 1 CU TO THE LEFT, MAKING ROOM FOR THE NEXT TILE
            boundary--;                                         //RE-CALCULATE THE CURRENT BOUNDARY
        }
    }
    /*
    printf("COLUNAS/LINHAS DO EXTRACT\n");
    for(j=0; j<pps->getNumTileColumnsMinus1(); j++)
        printf("coluna %d\n", optimalCols[j]);
    for(i=0; i<pps->getTileNumRowsMinus1(); i++)
        printf("linha %d\n", optimalRows[i]);
    //*/
   // printf("LINHAS/COLUNAS NORMALIZADAS\n");
     
}

void extractUniformPartition(TComPPS *pps, UInt *optimalColumns, UInt *optimalRows, UInt widthInCU, UInt heightInCU){
    //CALCULAR AS PARTIÇÕES UNIFORMES USANDO  MESMO CALCULO DO PADRÃO
    //printf("EXTRAINDO UNIFORM\n"); 
    int i, j;
    for(i=0; i < pps->getTileNumRowsMinus1(); i++){
        optimalRows[i] = (i+1)*heightInCU/(pps->getTileNumRowsMinus1()+1);
    }
    for(j=0; j < pps->getNumTileColumnsMinus1(); j++){
        optimalColumns[j] = (j+1)*widthInCU/(pps->getNumTileColumnsMinus1()+1);
    }   
}



Void TComPicSym::create  ( ChromaFormat chromaFormatIDC, Int iPicWidth, Int iPicHeight, UInt uiMaxWidth, UInt uiMaxHeight, UInt uiMaxDepth )
{
  UInt i;

  m_uhTotalDepth      = uiMaxDepth;
  m_uiNumPartitions   = 1<<(m_uhTotalDepth<<1);

  m_uiMaxCUWidth      = uiMaxWidth;
  m_uiMaxCUHeight     = uiMaxHeight;

  m_uiMinCUWidth      = uiMaxWidth  >> m_uhTotalDepth;
  m_uiMinCUHeight     = uiMaxHeight >> m_uhTotalDepth;

  m_uiNumPartInWidth  = m_uiMaxCUWidth  / m_uiMinCUWidth;
  m_uiNumPartInHeight = m_uiMaxCUHeight / m_uiMinCUHeight;

  m_uiWidthInCU       = ( iPicWidth %m_uiMaxCUWidth  ) ? iPicWidth /m_uiMaxCUWidth  + 1 : iPicWidth /m_uiMaxCUWidth;
  m_uiHeightInCU      = ( iPicHeight%m_uiMaxCUHeight ) ? iPicHeight/m_uiMaxCUHeight + 1 : iPicHeight/m_uiMaxCUHeight;

  m_uiNumCUsInFrame   = m_uiWidthInCU * m_uiHeightInCU;
  m_apcTComDataCU     = new TComDataCU*[m_uiNumCUsInFrame];

  if (m_uiNumAllocatedSlice>0)
  {
    for ( i=0; i<m_uiNumAllocatedSlice ; i++ )
    {
      delete m_apcTComSlice[i];
    }
    delete [] m_apcTComSlice;
  }
  m_apcTComSlice      = new TComSlice*[m_uiNumCUsInFrame];
  m_apcTComSlice[0]   = new TComSlice;
  m_uiNumAllocatedSlice = 1;
  for ( i=0; i<m_uiNumCUsInFrame ; i++ )
  {
    m_apcTComDataCU[i] = new TComDataCU;
    m_apcTComDataCU[i]->create( chromaFormatIDC, m_uiNumPartitions, m_uiMaxCUWidth, m_uiMaxCUHeight, false, m_uiMaxCUWidth >> m_uhTotalDepth
#if ADAPTIVE_QP_SELECTION
      , true
#endif
      );
  }

  m_puiCUOrderMap = new UInt[m_uiNumCUsInFrame+1];
  m_puiTileIdxMap = new UInt[m_uiNumCUsInFrame];
  m_puiInverseCUOrderMap = new UInt[m_uiNumCUsInFrame+1];

  for( i=0; i<m_uiNumCUsInFrame; i++ )
  {
    m_puiCUOrderMap[i] = i;
    m_puiInverseCUOrderMap[i] = i;
  }

  m_saoBlkParams = new SAOBlkParam[m_uiNumCUsInFrame];
}

Void TComPicSym::destroy()
{
  if (m_uiNumAllocatedSlice>0)
  {
    for (Int i = 0; i<m_uiNumAllocatedSlice ; i++ )
    {
      delete m_apcTComSlice[i];
    }
    delete [] m_apcTComSlice;
  }
  m_apcTComSlice = NULL;

  for (Int i = 0; i < m_uiNumCUsInFrame; i++)
  {
    m_apcTComDataCU[i]->destroy();
    delete m_apcTComDataCU[i];
    m_apcTComDataCU[i] = NULL;
  }
  delete [] m_apcTComDataCU;
  m_apcTComDataCU = NULL;

  delete [] m_puiCUOrderMap;
  m_puiCUOrderMap = NULL;

  delete [] m_puiTileIdxMap;
  m_puiTileIdxMap = NULL;

  delete [] m_puiInverseCUOrderMap;
  m_puiInverseCUOrderMap = NULL;

  if(m_saoBlkParams)
  {
    delete[] m_saoBlkParams; m_saoBlkParams = NULL;
  }
}

Void TComPicSym::allocateNewSlice()
{
  assert ((m_uiNumAllocatedSlice + 1) <= m_uiNumCUsInFrame);
  m_apcTComSlice[m_uiNumAllocatedSlice ++] = new TComSlice;
  if (m_uiNumAllocatedSlice>=2)
  {
    m_apcTComSlice[m_uiNumAllocatedSlice-1]->copySliceInfo( m_apcTComSlice[m_uiNumAllocatedSlice-2] );
    m_apcTComSlice[m_uiNumAllocatedSlice-1]->initSlice();
  }
}

Void TComPicSym::clearSliceBuffer()
{
  UInt i;
  for (i = 1; i < m_uiNumAllocatedSlice; i++)
  {
    delete m_apcTComSlice[i];
  }
  m_uiNumAllocatedSlice = 1;
}

UInt TComPicSym::getPicSCUEncOrder( UInt SCUAddr )
{
  return getInverseCUOrderMap(SCUAddr/m_uiNumPartitions)*m_uiNumPartitions + SCUAddr%m_uiNumPartitions;
}

UInt TComPicSym::getPicSCUAddr( UInt SCUEncOrder )
{
  return getCUOrderMap(SCUEncOrder/m_uiNumPartitions)*m_uiNumPartitions + SCUEncOrder%m_uiNumPartitions;
}
//IAGO  the method was modified in order to receive the tiles' parameters from TEncGOP->compressGOP
Void TComPicSym::initTiles(TComPPS *pps, double *time_CU, int frame)  
{
  //set NumColumnsMinus1 and NumRowsMinus1
  setNumColumnsMinus1( pps->getNumTileColumnsMinus1() );
  setNumRowsMinus1( pps->getTileNumRowsMinus1() );

  const Int numCols = pps->getNumTileColumnsMinus1() + 1;
  const Int numRows = pps->getTileNumRowsMinus1() + 1;
  const Int numTiles = numRows * numCols;

  // allocate memory for tile parameters
  m_tileParameters.resize(numTiles);

  if( pps->getTileUniformSpacingFlag() )
  {
    //set width and height for each (uniform) tile
    for(Int row=0; row < numRows; row++)
    {
      for(Int col=0; col < numCols; col++)
      {
        const Int tileIdx = row * numCols + col;
        m_tileParameters[tileIdx].setTileWidth( (col+1)*getFrameWidthInCU()/numCols
                                              - (col*getFrameWidthInCU())/numCols );
        m_tileParameters[tileIdx].setTileHeight( (row+1)*getFrameHeightInCU()/numRows
                                               - (row*getFrameHeightInCU())/numRows );
      }
    }
    /*
    printf("altura: %d  %d\n",m_tileParameters[0].getTileHeight(), m_tileParameters[1].getTileHeight());
    printf("largura: %d  %d\n",m_tileParameters[0].getTileWidth(), m_tileParameters[1].getTileWidth());
    printf("altura: %d  %d\n",m_tileParameters[2].getTileHeight(), m_tileParameters[3].getTileHeight());
    printf("largura: %d  %d\n",m_tileParameters[2].getTileWidth(), m_tileParameters[3].getTileWidth());
  */
    }
  else
  {
      int i;
    //int cuIdx;
    //for(cuIdx = 0; time_CU[cuIdx]>0; cuIdx++) //TESTS ENTERED ARRAY
    //    printf("%d %f\n", cuIdx, time_CU[cuIdx]);
   
    UInt* optimalColumns =  (UInt*) malloc(pps->getNumTileColumnsMinus1()*sizeof(UInt));
    UInt* optimalRows =  (UInt*) malloc(pps->getTileNumRowsMinus1()*sizeof(UInt));
    for(i=0; i<pps->getNumTileColumnsMinus1(); i++)
        optimalColumns[i]=0;
    for(i=0; i<pps->getTileNumRowsMinus1(); i++)
        optimalRows[i]=0;
    if(frame)
        extractOptimalPartitions(pps, time_CU, optimalColumns, optimalRows, getFrameWidthInCU(), getFrameHeightInCU());
    else
        extractUniformPartition(pps, optimalColumns, optimalRows, getFrameWidthInCU(), getFrameHeightInCU());
    UInt* optimalColumns2 =  (UInt*) malloc(pps->getNumTileColumnsMinus1()*sizeof(UInt));
    UInt* optimalRows2 =  (UInt*) malloc(pps->getTileNumRowsMinus1()*sizeof(UInt));
    
    optimalColumns2[0] = optimalColumns[0];
    optimalRows2[0] = optimalRows[0];
    //for(i=1; optimalColumns[i] != 0; i++){
    for(i=1; i < pps->getNumTileColumnsMinus1(); i++){
        optimalColumns2[i] = optimalColumns[i] - optimalColumns[i-1];
    }
    //for(i=1; optimalColumns[i] != 0; i++){
    for(i=1; i < pps->getTileNumRowsMinus1(); i++){
        optimalRows2[i] = optimalRows[i] - optimalRows[i-1];
    }
    
    //printf("FRAME %d\n", frame);
    //for(i=0; i < pps->getNumTileColumnsMinus1(); i++)
    //    printf("coluna norm %d %d\n", i, optimalColumns2[i]);
    //for(i=0; i < pps->getTileNumRowsMinus1(); i++)
    //    printf("linha norm %d %d\n", i, optimalRows2[i]);
    
    
    std::vector<Int> OptimalColumnsList(optimalColumns2, optimalColumns2 + 10);   //ARRUMAR PARA FICAR ADEQUADO
    std::vector<Int> OptimalRowsList(optimalRows2, optimalRows2 + 10);         //ARRUMAR PARA FICAR ADEQUADO
    //*
    for(i=0; i < pps->getNumTileColumnsMinus1(); i++)
        printf("coluna list %d\n", OptimalColumnsList[i]);
    for(i=0; i < pps->getTileNumRowsMinus1(); i++)
        printf("linha list %d\n", OptimalRowsList[i]);
    
    pps->setTileColumnWidth(OptimalColumnsList);
    pps->setTileRowHeight(OptimalRowsList);
      //##########################################################################################

      
   //pps->setTileColumnWidth(larguras);    //sets the column/row size with the received parameters
  //pps->setTileRowHeight(alturas);
//  printf("SET WIDTH: %d\n",larguras[0]);
//  printf("SET HEIGHT: %d\n",alturas[0]);
  //printf("numRows: %d\n numColumns: %d\n", numRows, numCols);
  
    //set the width for each tile
    for(Int row=0; row < numRows; row++)
    {
      Int cumulativeTileWidth = 0;
      for(Int col=0; col < getNumColumnsMinus1(); col++)
      {
        m_tileParameters[row * numCols + col].setTileWidth( pps->getTileColumnWidth(col) );
        cumulativeTileWidth += pps->getTileColumnWidth(col);
      }
      m_tileParameters[row * numCols + getNumColumnsMinus1()].setTileWidth( getFrameWidthInCU()-cumulativeTileWidth );
    }

    //set the height for each tile
    for(Int col=0; col < numCols; col++)
    {
      Int cumulativeTileHeight = 0;
      for(Int row=0; row < getNumRowsMinus1(); row++)
      {
        m_tileParameters[row * numCols + col].setTileHeight( pps->getTileRowHeight(row) );
        cumulativeTileHeight += pps->getTileRowHeight(row);
      }
      m_tileParameters[getNumRowsMinus1() * numCols + col].setTileHeight( getFrameHeightInCU()-cumulativeTileHeight );
    }
  /*  
  printf("0 w h - %d %d\n", m_tileParameters[0].getTileWidth(), m_tileParameters[0].getTileHeight());
  printf("1 w h - %d %d\n", m_tileParameters[1].getTileWidth(), m_tileParameters[1].getTileHeight());
  printf("2 w h - %d %d\n", m_tileParameters[2].getTileWidth(), m_tileParameters[2].getTileHeight());
  printf("3 w h - %d %d\n", m_tileParameters[3].getTileWidth(), m_tileParameters[3].getTileHeight());
  printf("4 w h - %d %d\n", m_tileParameters[4].getTileWidth(), m_tileParameters[4].getTileHeight());
  printf("5 w h - %d %d\n", m_tileParameters[5].getTileWidth(), m_tileParameters[5].getTileHeight());
 */
  
 // printf("Height: %d\nWidth: %d\n", pps->getTileRowHeight(0), pps->getTileColumnWidth(0));
  }

#if TILE_SIZE_CHECK
  Int minWidth  = 1;
  Int minHeight = 1;
  const Int profileIdc = pps->getSPS()->getPTL()->getGeneralPTL()->getProfileIdc();
  if (  profileIdc == Profile::MAIN || profileIdc == Profile::MAIN10) //TODO: RExt - add more profiles...
  {
    if (pps->getTilesEnabledFlag())
    {
      minHeight = 64  / g_uiMaxCUHeight;
      minWidth  = 256 / g_uiMaxCUWidth;
    }
  }
  for(Int row=0; row < numRows; row++)
  {
    for(Int col=0; col < numCols; col++)
    {
      const Int tileIdx = row * numCols + col;
      assert (m_tileParameters[tileIdx].getTileWidth() >= minWidth);
      assert (m_tileParameters[tileIdx].getTileHeight() >= minHeight);
    }
  }
#endif

  //initialize each tile of the current picture
  for( Int row=0; row < numRows; row++ )
  {
    for( Int col=0; col < numCols; col++ )
    {
      const Int tileIdx = row * numCols + col;

      //initialize the RightEdgePosInCU for each tile
      Int rightEdgePosInCTU = 0;
      for( Int i=0; i <= col; i++ )
      {
        rightEdgePosInCTU += m_tileParameters[row * numCols + i].getTileWidth();
      }
      m_tileParameters[tileIdx].setRightEdgePosInCU(rightEdgePosInCTU-1);

      //initialize the BottomEdgePosInCU for each tile
      Int bottomEdgePosInCTU = 0;
      for( Int i=0; i <= row; i++ )
      {
        bottomEdgePosInCTU += m_tileParameters[i * numCols + col].getTileHeight();
      }
      m_tileParameters[tileIdx].setBottomEdgePosInCU(bottomEdgePosInCTU-1);

      //initialize the FirstCUAddr for each tile
      m_tileParameters[tileIdx].setFirstCUAddr( (m_tileParameters[tileIdx].getBottomEdgePosInCU() - m_tileParameters[tileIdx].getTileHeight() + 1) * getFrameWidthInCU() + 
                                                 m_tileParameters[tileIdx].getRightEdgePosInCU() - m_tileParameters[tileIdx].getTileWidth() + 1);
    }
  }

  Int  columnIdx = 0;
  Int  rowIdx = 0;

  //initialize the TileIdxMap
  for( Int i=0; i<m_uiNumCUsInFrame; i++)
  {
    for( Int col=0; col < numCols; col++)
    {
      if(i % getFrameWidthInCU() <= m_tileParameters[col].getRightEdgePosInCU())
      {
        columnIdx = col;
        break;
      }
    }
    for(Int row=0; row < numRows; row++)
    {
      if(i / getFrameWidthInCU() <= m_tileParameters[row*numCols].getBottomEdgePosInCU())
      {
        rowIdx = row;
        break;
      }
    }
    m_puiTileIdxMap[i] = rowIdx * numCols + columnIdx;
  }
}

UInt TComPicSym::xCalculateNxtCUAddr( UInt uiCurrCUAddr )
{
  UInt  uiNxtCUAddr;
  UInt  uiTileIdx;

  //get the tile index for the current LCU
  uiTileIdx = this->getTileIdxMap(uiCurrCUAddr);

  //get the raster scan address for the next LCU
  if( uiCurrCUAddr % m_uiWidthInCU == this->getTComTile(uiTileIdx)->getRightEdgePosInCU() && uiCurrCUAddr / m_uiWidthInCU == this->getTComTile(uiTileIdx)->getBottomEdgePosInCU() )
  //the current LCU is the last LCU of the tile
  {
    if(uiTileIdx == (m_iNumColumnsMinus1+1)*(m_iNumRowsMinus1+1)-1)
    {
      uiNxtCUAddr = m_uiNumCUsInFrame;
    }
    else
    {
      uiNxtCUAddr = this->getTComTile(uiTileIdx+1)->getFirstCUAddr();
    }
  }
  else //the current LCU is not the last LCU of the tile
  {
    if( uiCurrCUAddr % m_uiWidthInCU == this->getTComTile(uiTileIdx)->getRightEdgePosInCU() )  //the current LCU is on the rightmost edge of the tile
    {
      uiNxtCUAddr = uiCurrCUAddr + m_uiWidthInCU - this->getTComTile(uiTileIdx)->getTileWidth() + 1;
    }
    else
    {
      uiNxtCUAddr = uiCurrCUAddr + 1;
    }
  }

  return uiNxtCUAddr;
}

Void TComPicSym::deriveLoopFilterBoundaryAvailibility(Int ctu,
                                                      Bool& isLeftAvail,
                                                      Bool& isRightAvail,
                                                      Bool& isAboveAvail,
                                                      Bool& isBelowAvail,
                                                      Bool& isAboveLeftAvail,
                                                      Bool& isAboveRightAvail,
                                                      Bool& isBelowLeftAvail,
                                                      Bool& isBelowRightAvail
                                                      )
{

  isLeftAvail      = (ctu % m_uiWidthInCU != 0);
  isRightAvail     = (ctu % m_uiWidthInCU != m_uiWidthInCU-1);
  isAboveAvail     = (ctu >= m_uiWidthInCU );
  isBelowAvail     = (ctu <  m_uiNumCUsInFrame - m_uiWidthInCU);
  isAboveLeftAvail = (isAboveAvail && isLeftAvail);
  isAboveRightAvail= (isAboveAvail && isRightAvail);
  isBelowLeftAvail = (isBelowAvail && isLeftAvail);
  isBelowRightAvail= (isBelowAvail && isRightAvail);

  Bool isLoopFiltAcrossTilePPS = getCU(ctu)->getSlice()->getPPS()->getLoopFilterAcrossTilesEnabledFlag();

  {
    TComDataCU* ctuCurr  = getCU(ctu);
    TComDataCU* ctuLeft  = isLeftAvail ?getCU(ctu-1):NULL;
    TComDataCU* ctuRight = isRightAvail?getCU(ctu+1):NULL;
    TComDataCU* ctuAbove = isAboveAvail?getCU(ctu-m_uiWidthInCU):NULL;
    TComDataCU* ctuBelow = isBelowAvail?getCU(ctu+m_uiWidthInCU):NULL;
    TComDataCU* ctuAboveLeft  = isAboveLeftAvail ? getCU(ctu-m_uiWidthInCU-1):NULL;
    TComDataCU* ctuAboveRigtht= isAboveRightAvail? getCU(ctu-m_uiWidthInCU+1):NULL;
    TComDataCU* ctuBelowLeft  = isBelowLeftAvail ? getCU(ctu+m_uiWidthInCU-1):NULL;
    TComDataCU* ctuBelowRight = isBelowRightAvail? getCU(ctu+m_uiWidthInCU+1):NULL;

    {
      //left
      if(ctuLeft != NULL)
      {
        isLeftAvail = (ctuCurr->getSlice()->getSliceCurStartCUAddr() != ctuLeft->getSlice()->getSliceCurStartCUAddr())?ctuCurr->getSlice()->getLFCrossSliceBoundaryFlag():true;
      }
      //above
      if(ctuAbove != NULL)
      {
        isAboveAvail = (ctuCurr->getSlice()->getSliceCurStartCUAddr() != ctuAbove->getSlice()->getSliceCurStartCUAddr())?ctuCurr->getSlice()->getLFCrossSliceBoundaryFlag():true;
      }
      //right
      if(ctuRight != NULL)
      {
        isRightAvail = (ctuCurr->getSlice()->getSliceCurStartCUAddr() != ctuRight->getSlice()->getSliceCurStartCUAddr())?ctuRight->getSlice()->getLFCrossSliceBoundaryFlag():true;
      }
      //below
      if(ctuBelow != NULL)
      {
        isBelowAvail = (ctuCurr->getSlice()->getSliceCurStartCUAddr() != ctuBelow->getSlice()->getSliceCurStartCUAddr())?ctuBelow->getSlice()->getLFCrossSliceBoundaryFlag():true;
      }
      //above-left
      if(ctuAboveLeft != NULL)
      {
        isAboveLeftAvail = (ctuCurr->getSlice()->getSliceCurStartCUAddr() != ctuAboveLeft->getSlice()->getSliceCurStartCUAddr())?ctuCurr->getSlice()->getLFCrossSliceBoundaryFlag():true;
      }
      //below-right
      if(ctuBelowRight != NULL)
      {
        isBelowRightAvail = (ctuCurr->getSlice()->getSliceCurStartCUAddr() != ctuBelowRight->getSlice()->getSliceCurStartCUAddr())?ctuBelowRight->getSlice()->getLFCrossSliceBoundaryFlag():true;
      }


      //above-right
      if(ctuAboveRigtht != NULL)
      {
        Int curSliceStartEncOrder  = ctuCurr->getSlice()->getSliceCurStartCUAddr();
        Int aboveRigthtSliceStartEncOrder = ctuAboveRigtht->getSlice()->getSliceCurStartCUAddr();

        isAboveRightAvail = (curSliceStartEncOrder == aboveRigthtSliceStartEncOrder)?(true):
          (
          (curSliceStartEncOrder > aboveRigthtSliceStartEncOrder)?(ctuCurr->getSlice()->getLFCrossSliceBoundaryFlag())
          :(ctuAboveRigtht->getSlice()->getLFCrossSliceBoundaryFlag())
          );          
      }
      //below-left
      if(ctuBelowLeft != NULL)
      {
        Int curSliceStartEncOrder  = ctuCurr->getSlice()->getSliceCurStartCUAddr();
        Int belowLeftSliceStartEncOrder = ctuBelowLeft->getSlice()->getSliceCurStartCUAddr();

        isBelowLeftAvail = (curSliceStartEncOrder == belowLeftSliceStartEncOrder)?(true):
          (
          (curSliceStartEncOrder > belowLeftSliceStartEncOrder)?(ctuCurr->getSlice()->getLFCrossSliceBoundaryFlag())
          :(ctuBelowLeft->getSlice()->getLFCrossSliceBoundaryFlag())
          );
      }        
    }

    if(!isLoopFiltAcrossTilePPS)
    {
      isLeftAvail      = (!isLeftAvail      ) ?false:(getTileIdxMap( ctuLeft->getAddr()         ) == getTileIdxMap( ctu ));
      isAboveAvail     = (!isAboveAvail     ) ?false:(getTileIdxMap( ctuAbove->getAddr()        ) == getTileIdxMap( ctu ));
      isRightAvail     = (!isRightAvail     ) ?false:(getTileIdxMap( ctuRight->getAddr()        ) == getTileIdxMap( ctu ));
      isBelowAvail     = (!isBelowAvail     ) ?false:(getTileIdxMap( ctuBelow->getAddr()        ) == getTileIdxMap( ctu ));
      isAboveLeftAvail = (!isAboveLeftAvail ) ?false:(getTileIdxMap( ctuAboveLeft->getAddr()    ) == getTileIdxMap( ctu ));
      isAboveRightAvail= (!isAboveRightAvail) ?false:(getTileIdxMap( ctuAboveRigtht->getAddr()  ) == getTileIdxMap( ctu ));
      isBelowLeftAvail = (!isBelowLeftAvail ) ?false:(getTileIdxMap( ctuBelowLeft->getAddr()    ) == getTileIdxMap( ctu ));
      isBelowRightAvail= (!isBelowRightAvail) ?false:(getTileIdxMap( ctuBelowRight->getAddr()   ) == getTileIdxMap( ctu ));
    }
  }

}


TComTile::TComTile()
: m_uiTileWidth         (0)
, m_uiTileHeight        (0)
, m_uiRightEdgePosInCU  (0)
, m_uiBottomEdgePosInCU (0)
, m_uiFirstCUAddr       (0)
{
}

TComTile::~TComTile()
{
}
//! \}
