#include "../lib/cfromreader/cfr.hpp"
#include <signal.h>
#include <errno.h>

using namespace cfr;

int main()
{
	std::unordered_map<std::string, int32_t> pathMap = createPathMap(getFileList("../src/ds_ptde.text",ds_ptde));

	std::vector<std::string> dvdbndPathList = {
		"../DATA/dvdbnd0.bhd5",
		"../DATA/dvdbnd1.bhd5",
		"../DATA/dvdbnd2.bhd5",
		"../DATA/dvdbnd3.bhd5"
	};

	std::unordered_map<int32_t, BHD5_1::FileHeader> hashMap;
	errno = 0;

	for(int i = 0; i < dvdbndPathList.size(); i++)
	{
		FILE* dvdbndPtr = v_fopen(dvdbndPathList[i].c_str(),"rb");

		BHD5_1* bhd5 = new BHD5_1(dvdbndPtr);
		hashMap.insert(bhd5->hashMap.begin(),bhd5->hashMap.end());

		fclose(dvdbndPtr);
		free(bhd5);
	}
	
	//printf("hashMap size: %i\n",hashMap.size());
	//printf("pathMap size: %i\n",pathMap.size());

	// gywn: /chr/c5370.chrbnd.dcx

	//printf("hash of /chr/c5370.chrbnd.dcx: %x\n",hash);

	BHD5_1::FileHeader gywnHeader = hashMap.at(pathMap.at("/chr/c5370.chrbnd.dcx"));

	BHD5_1::FileHeader pbndHeader = hashMap.at(pathMap.at("/paramdef/paramdef.paramdefbnd.dcx"));
	printf("size of paramdefbnd: %i\n",pbndHeader.fileSize);
	
	//BND3* bnd = openBND3("data/c5370.chrbnd.dcx");

	
	
	//BND3* paramdbnd = openBND3(parambnd.offset)

	//printf("offset of /chr/c5370.chrbnd.dcx: %lx\n",gywnHeader.offset);
	//printf("size of /chr/c5370.chrbnd.dcx: %lx\n",gywnHeader.fileSize);

	/*FILE* dvdbnd_bht  = v_fopen("../DATA/dvdbnd0.bdt","rb");
	if(dvdbnd_bht == NULL)
		throw std::runtime_error("failed to open bht!\n");*/

	//char* gwynChrBndDcxMem = (char*)malloc(gywnHeader.fileSize);

	//fseek(dvdbnd_bht,gywnHeader.offset,SEEK_SET);
	//size_t arraySize;
	//char* bndRaw = openDCX(dvdbnd_bht,gywnHeader.fileSize,&arraySize);
	//MEM* bndMem = mopen(bndRaw,arraySize);

	//int* fileCount;
	//BND3* bnd = openBND3(bndMem);

	//printf("files: %i\n",bnd->header->fileCount);
	//printf("file[0].rawFlags:%x\n",bnd->files[0].header->rawFlags);
	//printf("file[0].compressedSize:%x\n",bnd->files[0].header->compressedSize);
	//printf("file[0].dataStart:%x\n",bnd->files[0].data);

	//char magic[5];
	//memcpy(magic,bnd->files[0].data,5);
	//printf("file[0] magic: %s\n",magic);

	//memcpy(magic,&bndMem->src[bnd->files[0].header->dataOffset],5);
	//printf("file[0] magic: %s\n",magic);

	//mseek(bndMem,bnd->files[0].header->dataOffset,SEEK_SET);
	//FLVER2* flver = openFLVER2(bndMem);

	//this method seems to make sense, no messing with memory
	/*FLVER2* flver = openFLVER2(
		(void*)bnd->files[0].data,
		(size_t)bnd->files[0].uncompressedSize
	);*/

	//printf("flver.header.meshcount:%i\n",flver->header->meshCount);
	//printf("flver.dummy[8].color[0]:%i\n",flver->dummies[8].color[0]);
	//printf("flver.bones[0].rot_x[0]:%f\n",flver->bones[0].rot_x);

	//exportFLVER2(*flver,"flver.txt");
	//raise(SIGINT);

	//printFLVER2(flver);
};