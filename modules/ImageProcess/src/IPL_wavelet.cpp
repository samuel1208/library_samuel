
#include "IPL_wavelet.h"

#include "stdio.h"
#include "string.h"
#include "float.h"
#include "math.h"


DLL_EXPORTS  void  DWT(float  *srcImg, int widthStep, int width, int height, int channel, int nLayer)
{

	if (srcImg)
	{
		if (channel == 1 &&
			((width >> nLayer) << nLayer) == width &&
			((height >> nLayer) << nLayer) == height)
		{
			int     i, x, y, n;
			float   fValue   = 0;
			float   fRadius  = sqrt(2.0f);
			int     nWidth   = width;
			int     nHeight  = height;
			int     nHalfW   = nWidth / 2;
			int     nHalfH   = nHeight / 2;
			float **pData    = new float*[height];
			float  *pRow     = new float[width];
			float  *pColumn  = new float[height];
			for (i = 0; i < height; i++)
			{
				pData[i] = srcImg + widthStep*i;
			}
			// ���С���任
			for (n = 0; n < nLayer; n++, nWidth /= 2, nHeight /= 2, nHalfW /= 2, nHalfH /= 2)
			{
				// ˮƽ�任
				for (y = 0; y < nHeight; y++)
				{
					// ��ż����
					memcpy(pRow, pData[y], sizeof(float) * nWidth);
					for (i = 0; i < nHalfW; i++)
					{
						x = i * 2;
						pData[y][i] = pRow[x];
						pData[y][nHalfW + i] = pRow[x + 1];
					}
					// ����С���任
					for (i = 0; i < nHalfW - 1; i++)
					{
						fValue = (pData[y][i] + pData[y][i + 1]) / 2;
						pData[y][nHalfW + i] -= fValue;
					}
					fValue = (pData[y][nHalfW - 1] + pData[y][nHalfW - 2]) / 2;
					pData[y][nWidth - 1] -= fValue;
					fValue = (pData[y][nHalfW] + pData[y][nHalfW + 1]) / 4;
					pData[y][0] += fValue;
					for (i = 1; i < nHalfW; i++)
					{
						fValue = (pData[y][nHalfW + i] + pData[y][nHalfW + i - 1]) / 4;
						pData[y][i] += fValue;
					}
					// Ƶ��ϵ��
					for (i = 0; i < nHalfW; i++)
					{
						pData[y][i] *= fRadius;
						pData[y][nHalfW + i] /= fRadius;
					}
				}
				// ��ֱ�任
				for (x = 0; x < nWidth; x++)
				{
					// ��ż����
					for (i = 0; i < nHalfH; i++)
					{
						y = i * 2;
						pColumn[i] = pData[y][x];
						pColumn[nHalfH + i] = pData[y + 1][x];
					}
					for (i = 0; i < nHeight; i++)
					{
						pData[i][x] = pColumn[i];
					}
					// ����С���任
					for (i = 0; i < nHalfH - 1; i++)
					{
						fValue = (pData[i][x] + pData[i + 1][x]) / 2;
						pData[nHalfH + i][x] -= fValue;
					}
					fValue = (pData[nHalfH - 1][x] + pData[nHalfH - 2][x]) / 2;
					pData[nHeight - 1][x] -= fValue;
					fValue = (pData[nHalfH][x] + pData[nHalfH + 1][x]) / 4;
					pData[0][x] += fValue;
					for (i = 1; i < nHalfH; i++)
					{
						fValue = (pData[nHalfH + i][x] + pData[nHalfH + i - 1][x]) / 4;
						pData[i][x] += fValue;
					}
					// Ƶ��ϵ��
					for (i = 0; i < nHalfH; i++)
					{
						pData[i][x] *= fRadius;
						pData[nHalfH + i][x] /= fRadius;
					}
				}
			}
			delete[] pData;
			delete[] pRow;
			delete[] pColumn;
		}
	}
}


DLL_EXPORTS  void  IDWT(float  *srcImg, int widthStep, int width, int height, int channel, int nLayer)
{
	// ִ������
	if (srcImg)
	{
		if (channel == 1 &&
			((width >> nLayer) << nLayer) == width &&
			((height >> nLayer) << nLayer) == height)
		{
			int     i, x, y, n;
			float   fValue   = 0;
			float   fRadius  = sqrt(2.0f);
			int     nWidth   = width >> (nLayer - 1);
			int     nHeight  = height >> (nLayer - 1);
			int     nHalfW   = nWidth / 2;
			int     nHalfH   = nHeight / 2;
			float **pData    = new float*[height];
			float  *pRow     = new float[width];
			float  *pColumn  = new float[height];
			for (i = 0; i < height; i++)
			{
				pData[i] = srcImg + widthStep * i;
			}
			// ���С���ָ�
			for (n = 0; n < nLayer; n++, nWidth *= 2, nHeight *= 2, nHalfW *= 2, nHalfH *= 2)
			{
				// ��ֱ�ָ�
				for (x = 0; x < nWidth; x++)
				{
					// Ƶ��ϵ��
					for (i = 0; i < nHalfH; i++)
					{
						pData[i][x] /= fRadius;
						pData[nHalfH + i][x] *= fRadius;
					}
					// ����С���ָ�
					fValue = (pData[nHalfH][x] + pData[nHalfH + 1][x]) / 4;
					pData[0][x] -= fValue;
					for (i = 1; i < nHalfH; i++)
					{
						fValue = (pData[nHalfH + i][x] + pData[nHalfH + i - 1][x]) / 4;
						pData[i][x] -= fValue;
					}
					for (i = 0; i < nHalfH - 1; i++)
					{
						fValue = (pData[i][x] + pData[i + 1][x]) / 2;
						pData[nHalfH + i][x] += fValue;
					}
					fValue = (pData[nHalfH - 1][x] + pData[nHalfH - 2][x]) / 2;
					pData[nHeight - 1][x] += fValue;
					// ��ż�ϲ�
					for (i = 0; i < nHalfH; i++)
					{
						y = i * 2;
						pColumn[y] = pData[i][x];
						pColumn[y + 1] = pData[nHalfH + i][x];
					}
					for (i = 0; i < nHeight; i++)
					{
						pData[i][x] = pColumn[i];
					}
				}
				// ˮƽ�ָ�
				for (y = 0; y < nHeight; y++)
				{
					// Ƶ��ϵ��
					for (i = 0; i < nHalfW; i++)
					{
						pData[y][i] /= fRadius;
						pData[y][nHalfW + i] *= fRadius;
					}
					// ����С���ָ�
					fValue = (pData[y][nHalfW] + pData[y][nHalfW + 1]) / 4;
					pData[y][0] -= fValue;
					for (i = 1; i < nHalfW; i++)
					{
						fValue = (pData[y][nHalfW + i] + pData[y][nHalfW + i - 1]) / 4;
						pData[y][i] -= fValue;
					}
					for (i = 0; i < nHalfW - 1; i++)
					{
						fValue = (pData[y][i] + pData[y][i + 1]) / 2;
						pData[y][nHalfW + i] += fValue;
					}
					fValue = (pData[y][nHalfW - 1] + pData[y][nHalfW - 2]) / 2;
					pData[y][nWidth - 1] += fValue;
					// ��ż�ϲ�
					for (i = 0; i < nHalfW; i++)
					{
						x = i * 2;
						pRow[x] = pData[y][i];
						pRow[x + 1] = pData[y][nHalfW + i];
					}
					memcpy(pData[y], pRow, sizeof(float) * nWidth);
				}
			}
			delete[] pData;
			delete[] pRow;
			delete[] pColumn;
		}
	}
}

