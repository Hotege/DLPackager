//
// Created by dl on 2018/11/7.
//

#ifndef DLPKG_DLPACKAGER_H
#define DLPKG_DLPACKAGER_H


#include <vector>
#include <string>

struct PKG_FILE
{
	std::string strFileName;
	unsigned int nSourceSize;
	unsigned char* pBufferInput;
	int nLevel;
	unsigned int nCompressedSize;
	unsigned char* pBufferOutput;
};

class DLPackager
{
public:
	static void Pack(unsigned char** ppDst, unsigned int* pDstSz, const std::vector<PKG_FILE>& pkg);
	static std::vector<PKG_FILE> Unpack(const unsigned char* pDst, const unsigned int& nDstSz);
};


#endif //DLPKG_DLPACKAGER_H
