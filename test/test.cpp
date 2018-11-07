//
// Created by dl on 2018/11/7.
//

#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
#include <DLFileKit.h>
#include <DLPackager.h>

int main()
{
	std::string fn[] = { "button.vs", "button.fs", "cube.vs", "cube.fs", "mingliu.ttc", "RubiksCube.ico" };

	std::vector<PKG_FILE> vPkgFile(6);
	for (int i = 0; i < 6; i++)
	{
		auto pFile = new DLFileKit();
		pFile->ReadFile(fn[i].c_str());

		vPkgFile[i].strFileName = fn[i];
		vPkgFile[i].nSourceSize = (unsigned int)pFile->GetSize();
		vPkgFile[i].pBufferInput = new unsigned char[pFile->GetSize() + 1];
		memset(vPkgFile[i].pBufferInput, 0, pFile->GetSize() + 1);
		memcpy(vPkgFile[i].pBufferInput, pFile->GetData(), pFile->GetSize());
		vPkgFile[i].nLevel = i == 4 ? 0 : 9;

		delete pFile; pFile = nullptr;
	}
	unsigned char* pDst = nullptr;
	unsigned int nDstLen = 0;
	DLPackager::Pack(&pDst, &nDstLen, vPkgFile);
	printf("========\n");
	printf("%d\n", nDstLen);
	DLFileKit outCmp;
	outCmp.CreateFile("RubiksCube.dlp");
	outCmp.WriteFile(pDst, nDstLen, 1);

	std::vector<PKG_FILE> pfs = DLPackager::Unpack(pDst, nDstLen);
	printf("%d\n", pfs.size());

	delete[] pDst; pDst = nullptr;
	return 0;
}