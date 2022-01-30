#include "../lib/cfromreader/cfr.hpp"

using namespace cfr;

int main()
{
	
	std::unordered_map<std::string, int32_t> pathMap = createPathMap(getFileList("../src/ds_ptde.text",ds_ptde));

	std::vector<std::string> dvdbndPathList = {
		"../DATA/dvdbnd0.bhd5",
		/*"../DATA/dvdbnd1.bhd5",
		"../DATA/dvdbnd2.bhd5",
		"../DATA/dvdbnd3.bhd5"*/
	};

	FILE* dvdbndPtr;
	BHD5_1* bhd5;
	std::unordered_map<int32_t, BHD5_1::FileHeader> hashMap;

	for(int i = 0; i < dvdbndPathList.size(); i++)
	{
		dvdbndPtr = fopen(dvdbndPathList[i].c_str(),"rb");
		bhd5 = new BHD5_1(dvdbndPtr);
		hashMap.insert(bhd5->hashMap.begin(),bhd5->hashMap.end());
	}
	
	printf("hashMap size: %i\n",hashMap.size());
	printf("pathMap size: %i\n",pathMap.size());


	// gywn: /chr/c5370.chrbnd.dcx

	int32_t hash = pathMap.at("/chr/c5370.chrbnd.dcx");

	printf("hash of /chr/c5370.chrbnd.dcx: %x\n",hash);

	BHD5_1::FileHeader gywnHeader = hashMap.at((int32_t)0x8A67348D);
	printf("offset of /chr/c5370.chrbnd.dcx: %lx\n",gywnHeader.offset);
	printf("size of /chr/c5370.chrbnd.dcx: %lx\n",gywnHeader.fileSize);

	FILE* dvdbnd_bht  = fopen("../DATA/dvdbnd0.bdt","rb");
	if(dvdbnd_bht == NULL)
		throw std::runtime_error("failed to open bht!\n");

	char* gwynChrBndDcxMem = (char*)malloc(gywnHeader.fileSize);

	fseek(dvdbnd_bht,gywnHeader.offset,SEEK_SET);
	size_t arraySize;
	char* bndRaw = openDCX(dvdbnd_bht,gywnHeader.fileSize,&arraySize);

	//fread(gwynChrBndDcxMem,gywnHeader.fileSize,1,dvdbnd_bht);

	char magic[4];
	memcpy(magic,bndRaw,4);
	printf("magic: %s\n",magic);
};