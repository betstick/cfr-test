#include "flver2.h"

//there has to be a more sane, but still memory saving, way to do this.
//this isn't compute or memory intensive... but just look at it! its spaghetti!

namespace cfr
{
	FLVER2::Header::Header(UMEM* src)
	{
		uread(this,sizeof(FLVER2::Header),1,src);

		if(memcmp(this->magic,"FLVER2",6) != 0)
			throw std::runtime_error("Failed to validate FLVER2!\n");
	};

	FLVER2::Dummy::Dummy(UMEM* src)
	{
		uread(this,sizeof(FLVER2::Dummy),1,src);

		if(this->unk38 + this->unk3C != 0)
			throw std::runtime_error("Failed to validate FLVER2::Dummy!\n");
	};

	FLVER2::Material::Material(UMEM* src, FLVER2* parent)
	{
		uread(&this->header,sizeof(Header),1,src);

		if(this->header.gxOffset != 0)
		{
			long start = utell(src);
			useek(src,this->header.gxOffset,SEEK_CUR);

			bool good = true;

			while(good)
			{
				GxItem* gxItem = new GxItem(src);

				if(parent->header.version >= 0x20010 && 
				gxItem->id != 0x7FFFFFFF && 
				gxItem->id != 0xFFFFFFFF)
				{
					this->gxItems.insert(this->gxItems.end(),gxItem);
				}
				else
				{
					good = false;
				}
			}

			useek(src,start,SEEK_SET);
		}
	};

	FLVER2::Material::GxItem::GxItem(UMEM* src)
	{
		uread(this,sizeof(int32_t),3,src);

		if(this->length > 12)
			uread(this->data,this->length-(sizeof(int32_t)*3),1,src);
	};

	FLVER2::Bone::Bone(UMEM* src)
	{
		uread(this,sizeof(FLVER2::Bone),1,src);
	};

	FLVER2::Mesh::Mesh(UMEM* src, FLVER2* parent)
	{
		uread(this,sizeof(Header),1,src);

		long pos = utell(src);

		if(this->header.boundingBoxOffset != 0)
		{
			uread(&this->boundingBoxMin,sizeof(cfr_vec3),2,src);

			if(parent->header.version >= 0x2001A)
				uread(&this->boundingBoxUnk,sizeof(cfr_vec3),1,src);
		}

		if(this->header.boneCount > 0)
		{
			useek(src,this->header.boneIndicesOffset,SEEK_SET);

			//hypothetically faster, but not worth the complexity
			//int32_t* boneIndices = (int32_t*)malloc(sizeof(int32_t)*this->header.boneCount);
			//uread(&boneIndices,sizeof(int32_t),this->header.boneCount,src);
			//this->boneIndices.insert(this->boneIndices.end(),&boneIndices[0],&boneIndices[this->header.boneCount]);

			for(int i = this->header.boneCount; i > 0; i--)
			{
				int32_t boneIndex = 0;
				uread(&boneIndex,sizeof(int32_t),1,src);
				this->boneIndices.insert(this->boneIndices.end(),boneIndex);
			}
		}

		useek(src,this->header.faceSetIndicesOffset,SEEK_SET);

		for(int i = this->header.faceSetCount; i > 0; i--)
		{
			int32_t faceSetIndex = 0;
			uread(&faceSetIndex,sizeof(int32_t),1,src);
			this->faceSetIndices.insert(this->faceSetIndices.end(),faceSetIndex);
		}

		useek(src,this->header.vertexBufferIndicesOffset,SEEK_SET);

		for(int i = this->header.vertexBufferCount; i > 0; i--)
		{
			int32_t vertBuffIndex = 0;
			uread(&vertBuffIndex,sizeof(int32_t),1,src);
			this->vertexBufferIndices.insert(this->vertexBufferIndices.end(),vertBuffIndex);
		}

		useek(src,0,pos);
	};

	FLVER2::Member::Member(UMEM* src, int startOffset)
	{
		uread(this,sizeof(Header),1,src);

		long pos = utell(src);

		useek(src,startOffset+this->header.dataOffset,SEEK_SET);

		char* tmp = (char*)malloc(sizeof(this->header.dataLength));
		uread(tmp,this->header.dataLength,1,src);
		this->data = uopenMem(tmp,this->header.dataLength);

		useek(src,pos,SEEK_SET);
	};

	FLVER2::EdgeIndices* edgeIndicesInit(MEM* src, int startOffset)
	{
		FLVER2::EdgeIndices* indices = (FLVER2::EdgeIndices*)malloc(sizeof(FLVER2::EdgeIndices));
		indices->header = (FLVER2::EdgeIndices::Header*)mtellptr(src);

		indices->members = (FLVER2::Member*)malloc(sizeof(FLVER2::Member*)*indices->header->memberCount);

		return indices;
	};

	/*FLVER2::FaceSet* faceSetInit(MEM* src, int startOffset, FLVER2::Header* hdr)
	{
		FLVER2::FaceSet* set = (FLVER2::FaceSet*)malloc(sizeof(FLVER2::FaceSet));
		set->header = (FLVER2::FaceSet::Header*)mtellptr(src);
		mseek(src,sizeof(FLVER2::FaceSet::Header),SEEK_CUR);

		//printf("faceset: count: %i\n",set->header->vertexIndexCount);
		//printf("faceset: offset: 0x%x\n",set->header->vertexIndicesOffset);

		if(hdr->version >= 0x20009)
		{
			set->vertInfo = (FLVER2::FaceSet::VertInfo*)mtellptr(src);
			mseek(src,sizeof(FLVER2::FaceSet::VertInfo),SEEK_CUR);
		}

		set->vertexSize = hdr->vertexIndexSize == 0 ? set->vertexSize : hdr->vertexIndexSize;

		long pos = mtell(src);
		mseek(src,set->header->vertexIndicesOffset+hdr->dataOffset+startOffset,SEEK_SET);

		if(set->vertexSize == 8)
		{
			//TODO: fix this mess later :/
			/*set->vertexIndicesEdge = (FLVER2::EdgeIndices**)malloc(sizeof(FLVER2::EdgeIndices)*set->header->vertexIndexCount);

			for(int i = 0; i < set->header->vertexIndexCount; i++)
			{
				set->vertexIndicesEdge[i] = edgeIndicesInit(src, startOffset);
			}*/
		/*}
		else if(set->vertexSize == 16)
		{
			set->vertexIndicesShort = (uint16_t*)mtellptr(src);
		}
		else if(set->vertexSize == 32)
		{
			set->vertexIndicesInt = (uint32_t*)mtellptr(src);
		}

		mseek(src,pos,SEEK_SET);

		return set;
	};*/

	/*FLVER2::VertexBuffer* vertexBufferInit(MEM* src, int startOffset, FLVER2::Header* hdr)
	{
		FLVER2::VertexBuffer* buff = (FLVER2::VertexBuffer*)malloc(sizeof(FLVER2::VertexBuffer));
		buff->header = (FLVER2::VertexBuffer::Header*)mtellptr(src);
		mseek(src,sizeof(FLVER2::VertexBuffer::Header),SEEK_CUR);

		buff->vertices = (FLVER2::VertexBuffer::Vertex*)malloc(
				sizeof(FLVER2::VertexBuffer::Vertex)*buff->header->vertexCount
		);

		if(buff->header->unk10 == 0 && buff->header->unk14 == 0)
		{
			long pos = mtell(src);
			mseek(src,hdr->dataOffset+buff->header->bufferOffset+startOffset,SEEK_SET);

			for(int i = 0; i < buff->header->vertexCount; i++)
			{
				//printf("vertex[%i] loc: 0x%x\n",i,(mtell(src)-startOffset));
				buff->vertices[i].data = (char*)mtellptr(src);
				mseek(src,buff->header->vertexSize,SEEK_CUR);
			}

			mseek(src,pos,SEEK_SET);
		}

		return buff;
	};*/

	/*FLVER2::BufferLayout* bufferLayoutInit(MEM* src, int startOffset)
	{
		FLVER2::BufferLayout* layout = (FLVER2::BufferLayout*)malloc(sizeof(FLVER2::BufferLayout));
		layout->header = (FLVER2::BufferLayout::Header*)mtellptr(src);
		mseek(src,sizeof(FLVER2::BufferLayout::Header),SEEK_CUR);

		long pos = mtell(src);

		//is this needed?
		layout->members = (FLVER2::LayoutMember**)malloc(
			sizeof(FLVER2::LayoutMember)*layout->header->memberCount
		);

		for(int i = 0; i < layout->header->memberCount; i++)
		{
			layout->members[i] = (FLVER2::LayoutMember*)mtellptr(src);
			mseek(src,sizeof(FLVER2::LayoutMember),SEEK_CUR);
		}

		mseek(src,pos,SEEK_SET);

		return layout;
	};*/

	/*FLVER2::Texture* textureInit(MEM* src)
	{
		FLVER2::Texture* tex = (FLVER2::Texture*)mtellptr(src);
		mseek(src,sizeof(FLVER2::Texture),SEEK_CUR);

		return tex;
	};

	//TODO: these?
	FLVER2::VertexBoneWeights* vertexBoneWeightsInit(MEM* src, int startOffset)
	{
		return nullptr;
	};

	FLVER2::VertexBoneIndices* vertexBoneIndicesInit(MEM* src, int startOffset)
	{
		return nullptr;
	};

	FLVER2::VertexColor* vertexColorInit(MEM* src, int startOffset)
	{
		return nullptr;
	};

	FLVER2* openFLVER2(void* src, size_t size)
	{
		MEM* mem = mopen((char*)src,size);
		FLVER2* flver = openFLVER2(mem);
		mclose(mem);
		return flver;
	};

	FLVER2* openFLVER2(const char* path)
	{
		//load entire file into memory
		FILE* fp = v_fopen(path,"br");
		fseek(fp,0,SEEK_END);
		int size = ftell(fp);
		fseek(fp,0,SEEK_SET);
		char* data = (char*)malloc(size);
		fread(&data[0],size,1,fp);
		fclose(fp);

		return openFLVER2(data,size);
	};*/

	//Cast a FLVER2 from a position in memory
	/*FLVER2* openFLVER2(MEM* src)
	{
		long startOffset = mtell(src);
		FLVER2* flver = (FLVER2*)malloc(sizeof(FLVER2));
		flver->header = (FLVER2::Header*)mtellptr(src);
		mseek(src,sizeof(FLVER2::Header),SEEK_CUR);

		flver->dummies = (FLVER2::Dummy*)malloc(flver->header->dummyCount*sizeof(FLVER2::Dummy));
		flver->materials = (FLVER2::Material*)malloc(flver->header->materialCount*sizeof(FLVER2::Material));
		flver->bones = (FLVER2::Bone*)malloc(flver->header->boneCount*sizeof(FLVER2::Bone));
		flver->meshes = (FLVER2::Mesh*)malloc(flver->header->meshCount*sizeof(FLVER2::Mesh));
		flver->faceSets = (FLVER2::FaceSet*)malloc(flver->header->faceSetCount*sizeof(FLVER2::FaceSet));
		flver->vertexBuffers = (FLVER2::VertexBuffer*)malloc(flver->header->vertexBufferCount*sizeof(FLVER2::VertexBuffer));
		flver->bufferLayouts = (FLVER2::BufferLayout*)malloc(flver->header->bufferLayoutCount*sizeof(FLVER2::BufferLayout));
		flver->textures = (FLVER2::Texture*)malloc(flver->header->textureCount*sizeof(FLVER2::Texture));

		for(int32_t i = 0; i < flver->header->dummyCount; i++)
			flver->dummies[i] = *dummyInit(src,startOffset);

		for(int32_t i = 0; i < flver->header->materialCount; i++)
			flver->materials[i] = *materialInit(src,startOffset,flver->header,i);
		
		for(int32_t i = 0; i < flver->header->boneCount; i++)
			flver->bones[i] = *boneInit(src,startOffset,i);

		for(int32_t i = 0; i < flver->header->meshCount; i++)
			flver->meshes[i] = *meshInit(src,startOffset,flver->header);

		for(int32_t i = 0; i < flver->header->faceSetCount; i++)
			flver->faceSets[i] = *faceSetInit(src,startOffset,flver->header);

		for(int32_t i = 0; i < flver->header->vertexBufferCount; i++)
			flver->vertexBuffers[i] = *vertexBufferInit(src,startOffset,flver->header);

		for(int32_t i = 0; i < flver->header->bufferLayoutCount; i++)
			flver->bufferLayouts[i] = *bufferLayoutInit(src,startOffset);

		for(int32_t i = 0; i < flver->header->textureCount; i++)
			flver->textures[i] = *textureInit(src);

		return flver;
	};

	void exportFLVER2(FLVER2 flver, const char* filename)
	{
		FILE* out = v_fopen(filename,"w");

		char magicString[] = "magic: xxxxxx\n";
		memcpy(&magicString[7],flver.header->magic,5); //cut off the null char
		fwrite(magicString,14,1,out);

		fclose(out);
	};*/

	/*void printFLVER2(FLVER2* flver)
	{
		printf("header:\n");
		printf("\tdummyCount:        %4i\n",flver->header->dummyCount);
		printf("\tmaterialCount:     %4i\n",flver->header->materialCount);
		printf("\tboneCount:         %4i\n",flver->header->boneCount);
		printf("\tmeshCount:         %4i\n",flver->header->meshCount);
		printf("\tvertexBufferCount: %4i\n",flver->header->vertexBufferCount);
		printf("\ttrueFaceCount:     %4i\n",flver->header->trueFaceCount);
		printf("\ttotalFaceCount:    %4i\n",flver->header->totalFaceCount);
		printf("\tfaceSetCount:      %4i\n",flver->header->faceSetCount);
		printf("\tbufferLayoutCount: %4i\n",flver->header->bufferLayoutCount);
		printf("\ttextureCount:      %4i\n",flver->header->textureCount);
		printf("\n");

		for(int i = 0; i < flver->header->faceSetCount; i++)
		{
			printf("faceSet[%4i]\n",i);
			printf("\tvertexIndexCount:      %12i\n",flver->faceSets[i].header->vertexIndexCount);
			printf("\tvertexIndicesOffset: 0x%12x\n",flver->faceSets[i].header->vertexIndicesOffset);
			printf("\n");
		}

		for(int i = 0; i < flver->header->vertexBufferCount; i++)
		{
			printf("vertexBuffer[%4i]:\n",i);
			printf("\tlayoutIndex: %4i\n",flver->vertexBuffers[i].header->layoutIndex);
			for(int j = 0; j < flver->vertexBuffers[i].header->vertexCount; j ++)
			{
				printf("\tvertex[%4i]: ",j);
				for(int k = 0; k < flver->vertexBuffers[i].header->vertexSize; k++)
				{
					uint8_t c = 'x';
					memcpy(&c,&flver->vertexBuffers[i].vertices[j].data[k],1);
					printf("%3u, ", c);
				}
				printf("\n");
			}
			printf("\n");
		}*/
//	};
};