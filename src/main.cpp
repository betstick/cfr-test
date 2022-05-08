#include "../lib/cfromreader/cfr.hpp"
#include <signal.h>
#include <errno.h>

using namespace cfr;

int main()
{
	size_t size = 0;
	size_t nothing = 0;
	FILE* f = v_fopen("../DATA/c5370.chrbnd.dcx","rb");
	char* rawbnd = openDCX(f,0,&size);
	printf("size of uncompressed: %i\n",size);
	BND3* bnd = openBND3(rawbnd,size);

	for(int i = 0; i < bnd->header->fileCount; i++)
	{
		printf("id %i\n",bnd->files[i].id);
	}

	return 0;
};