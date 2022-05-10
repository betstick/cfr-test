#include "../lib/cfromreader/cfr.hpp"
#include <signal.h>
#include <errno.h>

using namespace cfr;

int main()
{
	size_t size = 0;
	FILE* f = v_fopen("../DATA/c5370.chrbnd.dcx","rb");
	char* rawbnd = openDCX(f,0,&size);
	printf("size of uncompressed: %i\n",size);
	BND3* bnd = openBND3(rawbnd,size);

	for(int i = 0; i < bnd->header->fileCount; i++)
	{
		printf("id %i\n",bnd->files[i].id);
		printf("file size %i\n",bnd->files[i].header->compressedSize);
	}

	FLVER2* flver = openFLVER2(mopen(bnd->files[0].data,bnd->files[0].uncompressedSize));

	printf("flver bone count: %i\n",flver->header->boneCount);

	return 0;
};