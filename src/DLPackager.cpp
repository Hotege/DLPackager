//
// Created by dl on 2018/11/7.
//

#include "DLPackager.h"
#include <cstring>
#include <numeric>
#include <zlib.h>

void DLPackager::Pack(unsigned char** ppDst, unsigned int* pDstSz, const std::vector<PKG_FILE>& pkg)
{
	const char header[] = { 'D', 'L', 'P', 'K' };
	unsigned int nFileCount = pkg.size();
	std::vector<std::vector<unsigned char>> data;
	for (auto p : pkg)
	{
		uLongf nCmpSz = compressBound(p.nSourceSize);
		std::vector<unsigned char> vBlock(sizeof(unsigned int) * 3 + sizeof(int) + p.strFileName.size() + nCmpSz);
		unsigned int nFileNameSz = p.strFileName.size();
		memcpy((&vBlock[0] + 0), &nFileNameSz, sizeof(unsigned int));
		memcpy((&vBlock[0] + sizeof(unsigned int) * 1), p.strFileName.c_str(), nFileNameSz);
		memcpy((&vBlock[0] + sizeof(unsigned int) * 1 + nFileNameSz), &p.nSourceSize, sizeof(unsigned int));
		memcpy((&vBlock[0] + sizeof(unsigned int) * 2 + nFileNameSz), &p.nLevel, sizeof(int));
		auto pTmp = new unsigned char[nCmpSz];
		compress2(pTmp, &nCmpSz, p.pBufferInput, p.nSourceSize, p.nLevel);
		memcpy((&vBlock[0] + sizeof(unsigned int) * 2 + sizeof(int) + nFileNameSz), &nCmpSz, sizeof(unsigned int));
		memcpy((&vBlock[0] + sizeof(unsigned int) * 3 + sizeof(int) + nFileNameSz), pTmp, nCmpSz);
		vBlock.resize(sizeof(unsigned int) * 3 + sizeof(int) + nFileNameSz + nCmpSz);
		data.push_back(vBlock);
		delete[] pTmp; pTmp = nullptr;
	}
	*pDstSz = std::accumulate(data.begin(), data.end(), (unsigned int)0, [](unsigned int a, std::vector<unsigned char> t) { return a + t.size(); });
	*pDstSz += sizeof(unsigned int) * 2 + sizeof(header);
	*ppDst = new unsigned char[*pDstSz];
	memcpy(*ppDst, header, sizeof(header));
	memcpy(*ppDst + sizeof(header), pDstSz, sizeof(unsigned int));
	memcpy(*ppDst + sizeof(header) + sizeof(unsigned int), &nFileCount, sizeof(unsigned int));
	auto ptr = *ppDst + sizeof(header) + sizeof(unsigned int) * 2;
	for (auto p : data)
	{
		memcpy(ptr, &p[0], p.size());
		ptr += p.size();
	}
}

std::vector<PKG_FILE> DLPackager::Unpack(const unsigned char* pDst, const unsigned int& nDstSz)
{
	unsigned int nFileCount = *(unsigned int*)(pDst + 8);
	std::vector<PKG_FILE> result(nFileCount);
	auto ptr = pDst + 12;
	for (int i = 0; i < result.size(); i++)
	{
		unsigned int nFileNameSz = *(unsigned int*)ptr;
		ptr += sizeof(unsigned int);
		auto szFileName = new char[nFileNameSz + 1];
		memset(szFileName, 0, nFileNameSz + 1);
		memcpy(szFileName, ptr, nFileNameSz);
		result[i].strFileName = szFileName;
		ptr += nFileNameSz;
		result[i].nSourceSize = *(unsigned int*)ptr;
		ptr += sizeof(unsigned int);
		result[i].nLevel = *(int*)ptr;
		ptr += sizeof(int);
		result[i].nCompressedSize = *(unsigned int*)ptr;
		ptr += sizeof(unsigned int);
		result[i].pBufferInput = new unsigned char[result[i].nSourceSize + 1];
		memset(result[i].pBufferInput, 0, result[i].nSourceSize + 1);
		uncompress2(result[i].pBufferInput, (uLongf*)&result[i].nSourceSize, ptr, (uLong*)&result[i].nCompressedSize);
		ptr += result[i].nCompressedSize;
		delete[] szFileName; szFileName = nullptr;
	}
	return result;
}