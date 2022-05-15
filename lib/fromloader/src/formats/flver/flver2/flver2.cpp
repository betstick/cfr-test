#include "flver2.h"

//there has to be a more sane, but still memory saving, way to do this.
//this isn't compute or memory intensive... but just look at it! its spaghetti!

namespace cfr
{
	FLVER2::Header::Header(){};

	FLVER2::Header::Header(UMEM* src)
	{
		uread(this,sizeof(FLVER2::Header),1,src);

#ifdef VALIDATE_ALL
		if(strncmp(this->magic,"FLVER\0",6) != 0)
			throw std::runtime_error("Failed to validate FLVER2!\n");
#endif
	};

	FLVER2::Dummy::Dummy(UMEM* src)
	{
		uread(this,sizeof(FLVER2::Dummy),1,src);

#ifdef VALIDATE_ALL
		assertMsg(this->unk38,"FLVER2::Dummy.unk38 is not 0!\n");
		assertMsg(this->unk3C,"FLVER2::Dummy.unk38 is not 0!\n");
#endif
	};

	FLVER2::Material::Material(UMEM* src, FLVER2* parent)
	{
		uread(&this->header,sizeof(Header),1,src);

		if(this->header.gxOffset != 0)
		{
			long start = utell(src);
			useek(src,this->header.gxOffset,SEEK_SET);

			bool good = true;

			while(good)
			{
				GxItem* gxItem = new GxItem(src);

				if(parent->header.version >= 0x20010 && 
					gxItem->id != 0x7FFFFFFF && 
					gxItem->id != 0xFFFFFFFF)
				{
					this->gxItems.push_back(gxItem);
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

#ifdef VALIDATE_ALL
		assertMsg(this->Header.unk1C,"FLVER2::Material.Header.unk1C is not 0!\n");
#endif

		if(this->length > 12)
			uread(this->data,this->length-12,1,src);
	};

	FLVER2::Bone::Bone(UMEM* src)
	{
		uread(this,sizeof(FLVER2::Bone),1,src);
	};

	FLVER2::Mesh::Mesh(UMEM* src, FLVER2* parent)
	{
		uread(&this->header,sizeof(Header),1,src);

#ifdef VALIDATE_ALL
		assertMsg(this->Header.unk01,"FLVER2::Mesh.Header.unk01 is not 0!\n");
		assertMsg(this->Header.unk02,"FLVER2::Mesh.Header.unk02 is not 0!\n");
		assertMsg(this->Header.unk03,"FLVER2::Mesh.Header.unk03 is not 0!\n");

		assertMsg(this->Header.unk08,"FLVER2::Mesh.Header.unk08 is not 0!\n");
		assertMsg(this->Header.unk0C,"FLVER2::Mesh.Header.unk0C is not 0!\n");
#endif

		long pos = utell(src);

		if(this->header.boundingBoxOffset != 0)
		{
			useek(src,this->header.boundingBoxOffset,SEEK_SET);
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

			int32_t boneIndex = 0;
			for(int i = this->header.boneCount; i > 0; i--)
			{
				uread(&boneIndex,sizeof(int32_t),1,src);
				this->boneIndices.push_back(boneIndex);
			}
		}

		useek(src,this->header.faceSetIndicesOffset,SEEK_SET);
		
		int32_t faceSetIndex = 0;
		for(int i = this->header.faceSetCount; i > 0; i--)
		{
			uread(&faceSetIndex,sizeof(int32_t),1,src);
			this->faceSetIndices.push_back(faceSetIndex);
		}

		useek(src,this->header.vertexBufferIndicesOffset,SEEK_SET);

		int32_t vertBuffIndex = 0;
		for(int i = this->header.vertexBufferCount; i > 0; i--)
		{
			uread(&vertBuffIndex,sizeof(int32_t),1,src);
			this->vertexBufferIndices.push_back(vertBuffIndex);
		}

		useek(src,pos,SEEK_SET);
	};

	FLVER2::Member::Member(UMEM* src, long startOffset)
	{
		uread(&this->header,sizeof(Header),1,src);

#ifdef VALIDATE_ALL
		assertMsg(this->Header.unk08,"FLVER2::Member.Header.unk08 is not 0!\n");
		assertMsg(this->Header.unk0C,"FLVER2::Member.Header.unk0C is not 0!\n");

		assertMsg(this->Header.unk20,"FLVER2::Member.Header.unk20 is not 0!\n");
		assertMsg(this->Header.unk24,"FLVER2::Member.Header.unk24 is not 0!\n");
		assertMsg(this->Header.unk28,"FLVER2::Member.Header.unk28 is not 0!\n");
		assertMsg(this->Header.unk2C,"FLVER2::Member.Header.unk2C is not 0!\n");

		assertMsg(this->Header.unk3C == -1,"FLVER2::Member.Header.unk3C is not -1!\n");
#endif

		long pos = utell(src);

		useek(src,startOffset+this->header.dataOffset,SEEK_SET);

		char* tmp = (char*)malloc(sizeof(this->header.dataLength));
		uread(tmp,this->header.dataLength,1,src);
		this->data = uopenMem(tmp,this->header.dataLength);

		useek(src,pos,SEEK_SET);
	};

	FLVER2::EdgeIndices::EdgeIndices(UMEM* src)
	{
		long pos = utell(src);

		uread(&this->header,sizeof(Header),1,src);

#ifdef VALIDATE_ALL
		assertMsg(this->Header.unk08,"FLVER2::EdgeIndices.Header.unk08 is not 0!\n");
#endif

		for(int i = this->header.memberCount; i > 0; i--)
		{
			this->members.push_back(new Member(src,pos));
		}
	};

	FLVER2::FaceSet::FaceSet(UMEM* src, FLVER2* parent)
	{
		uread(&this->header,sizeof(Header),1,src);

		if(parent->header.version >= 0x20009)
			uread(&this->vertInfo,sizeof(vertInfo),1,src);

		if(parent->header.vertexIndexSize == 0)
			this->vertexSize = this->vertInfo.vertexIndexSize;
		else
			this->vertexSize = parent->header.vertexIndexSize;
		
		long pos = utell(src);
		useek(src,parent->header.dataOffset + this->header.vertexIndicesOffset,SEEK_SET);

		/*switch(this->vertexSize)
		{
			case  8:
			{
				this->vertexIndicesEdge = new EdgeIndices(src);
			}
			case 16:
			{
				uint16_t tmp = 0;
				for(int i = this->header.vertexIndexCount; i > 0; i--)
				{
					uread(&tmp,sizeof(uint16_t),1,src);
					this->vertexIndicesShort.push_back(tmp);
				}
			}
			case 32:
			{
				uint32_t tmp = 0;
				for(int i = this->header.vertexIndexCount; i > 0; i--)
				{
					uread(&tmp,sizeof(uint16_t),1,src);
					this->vertexIndicesInt.push_back(tmp);
				}
			}
			default:
			{
				throw std::runtime_error("FLVER2::FaceSet.VertexSize is unkown!\n");
			}
		}*/

		if(this->vertexSize == 8)
		{
			throw std::runtime_error("FLVER2::FaceSet.VertexSize not implemented (8)!\n");
		}
		else if(this->vertexSize == 16)
		{
			uint16_t tmp = 0;
			for(int i = this->header.vertexIndexCount; i > 0; i--)
			{
				uread(&tmp,sizeof(uint16_t),1,src);
				this->vertexIndicesShort.push_back(tmp);
			}
		}
		else if(this->vertexSize == 32)
		{
			throw std::runtime_error("FLVER2::FaceSet.VertexSize not implemented (32)!\n");
		}

		useek(src,pos,SEEK_SET);
	};

	FLVER2::VertexBuffer::VertexBuffer(UMEM* src, FLVER2* parent)
	{
		uread(&this->header,sizeof(Header),1,src);
		long dataSize = this->header.vertexCount*this->header.vertexSize;

#ifdef VALIDATE_ALL
		assertMsg(this->Header.unk10,"FLVER2::EdgeIndices.Header.unk10 is not 0!\n");
		assertMsg(this->Header.unk14,"FLVER2::EdgeIndices.Header.unk14 is not 0!\n");
#endif

		if(this->header.unk10 == 0 && this->header.unk14 == 0)
		{
			long pos = utell(src);

			char* tmp = (char*)malloc(dataSize);
			uread(tmp,this->header.vertexCount*this->header.vertexSize,1,src);
			this->data = uopenMem(tmp,this->header.vertexCount*this->header.vertexSize);

			useek(src,pos,SEEK_SET);
		}
	};

	FLVER2::LayoutMember::LayoutMember(UMEM* src)
	{
		uread(this,sizeof(LayoutMember),1,src);

#ifdef VALIDATE_ALL
		//assertMsg(this->header.unk04,"FLVER2::LayoutMember.unk00 not from 0 to 2!\n");
#endif
	};

	FLVER2::BufferLayout::BufferLayout(UMEM* src)
	{
		uread(&this->header,sizeof(Header),1,src);

#ifdef VALIDATE_ALL
		assertMsg(this->header.unk04,"FLVER2::BufferLayout.header.unk04 not 0!\n");
		assertMsg(this->header.unk08,"FLVER2::BufferLayout.header.unk04 not 0!\n");
#endif

		long pos = utell(src);
		useek(src,this->header.membersOffset,SEEK_SET);

		for(int i = this->header.memberCount; i > 0; i--)
		{
			this->members.push_back(new LayoutMember(src));
		}

		useek(src,pos,SEEK_SET);
	};

	FLVER2::Texture::Texture(UMEM* src)
	{
		uread(this,sizeof(Texture),1,src);

#ifdef VALIDATE_ALL
		assertMsg(this->unk12,"FLVER2::Texture.unk12 not 0!\n");
#endif
	};

	//TODO: VALIDATE THESE
	FLVER2::VertexBoneWeights::VertexBoneWeights(UMEM* src)
	{
		uread(this,sizeof(VertexBoneWeights),1,src);

#ifdef VALIDATE_ALL
		assertMsg((this->length == 4),"FLVER2::VertexBoneWeights.length not 4!\n");
#endif
	};

	FLVER2::VertexBoneIndices::VertexBoneIndices(UMEM* src)
	{
		uread(this,sizeof(VertexBoneIndices),1,src);

#ifdef VALIDATE_ALL
		assertMsg((this->length == 4),"FLVER2::VertexBoneIndices.length not 4!\n");
#endif
	};

	FLVER2::FLVER2(UMEM* src) : File(src)
	{
		useek(src,0,SEEK_SET);

		this->header = Header(src);
		this->data = src;

		for(int i = this->header.dummyCount; i > 0; i--)
			this->dummies.push_back(new Dummy(src));
		
		for(int i = this->header.materialCount; i > 0; i--)
			this->materials.push_back(new Material(src,this));
		
		for(int i = this->header.boneCount; i > 0; i--)
			this->bones.push_back(new Bone(src));

		for(int i = this->header.meshCount; i > 0; i--)
			this->meshes.push_back(new Mesh(src,this));

		for(int i = this->header.faceSetCount; i > 0; i--)
			this->faceSets.push_back(new FaceSet(src,this));

		for(int i = this->header.vertexBufferCount; i > 0; i--)
			this->vertexBuffers.push_back(new VertexBuffer(src,this));

		for(int i = this->header.bufferLayoutCount; i > 0; i--)
			this->bufferLayouts.push_back(new BufferLayout(src));

		for(int i = this->header.textureCount; i > 0; i--)
			this->textures.push_back(new Texture(src));

		useek(src,0,SEEK_SET);
	};
};