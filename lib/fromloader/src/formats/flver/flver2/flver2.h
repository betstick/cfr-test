#pragma once

#include <stdexcept>

#include "../../stdafx.h"
//welcome to FLVER-town

//standard fromsoft model format from DSPTDE onwards
//as usual, built using SoulsTemplates! :)
//to help shorten this for now, it won't support non souls games

namespace cfr {
	enum VertexSemantic
	{
		//FLVER2_VERTEX_SEMANTIC
		FVS_POSITION     =  0,
		FVS_BONE_WEIGHTS =  1,
		FVS_BONE_INDICES =  2,
		FVS_NORMAL       =  3,
		FVS_UV           =  5,
		FVS_TANGENT      =  6,
		FVS_BITANGENT    =  7,
		FVS_VERTEX_COLOR = 10
	};

	enum VertexType
	{
		FVT_VEC2               = 0x01, //two floats
		FVT_VEC3               = 0x02, //three floats
		FVT_VEC4               = 0x03, //four floats
		FVT_UNK0               = 0x10, //unknown
		FVT_4_BYTE_0           = 0x11, //four bytes
		FVT_SHORT2             = 0x12, //two shorts
		FVT_4_BYTE_1           = 0x13, //four bytes
		FVT_UV                 = 0x15, //two shorts
		FVT_UV_PAIR            = 0x16, //four shorts
		FVT_SHORT_BONE_INDICES = 0x18, //four shorts
		FVT_4_SHORT_0          = 0x1A, //four shorts
		FVT_UNK1               = 0x2E, //unknown
		FVT_UNK2               = 0x2F, //unknown
		FVT_UNK3               = 0xF0, //unknown
	};
		
	class FLVER2 : public File
	{
		public:
		class Header
		{
			public:
			char magic[6]; //this was working with size 4 somehow. it shouldn't
			char endian[2];
			uint32_t version = 0;

			uint32_t dataOffset = 0;
			uint32_t dataLength = 0;

			int32_t dummyCount = 0;
			int32_t materialCount = 0;
			int32_t boneCount = 0;
			int32_t meshCount = 0;
			int32_t vertexBufferCount = 0;

			cfr_vec3 boundingBoxMin = {0.0f,0.0f,0.0f};
			cfr_vec3 boundingBoxMax = {0.0f,0.0f,0.0f};

			int32_t trueFaceCount  = 0; // Does not include shadow meshes or degenerate faces
			int32_t totalFaceCount = 0;

			char vertexIndexSize = '\0';
			char unicode = '\0';

			char unk4A = '\0';
			char unk4B = '\0';

			int32_t primitiveRestartConstant = 0; //guess, needs confirmation
			int32_t faceSetCount = 0;
			int32_t bufferLayoutCount = 0;
			int32_t textureCount = 0;

			char unk5C = '\0';
			char unk5D = '\0';
			char unk5E = '\0'; //assert(0)
			char unk5F = '\0'; //assert(0)

			int32_t unk60 = 0; //assert(0)
			int32_t unk64 = 0; //assert(0)
			int32_t unk68 = 0;
			int32_t unk6C = 0; //assert(0)
			int32_t unk70 = 0; //assert(0)
			int32_t unk74 = 0; //assert(0)
			int32_t unk78 = 0; //assert(0)
			int32_t unk7C = 0; //assert(0)

			Header();
			Header(UMEM* src);
		};

		class Dummy
		{
			public:
			cfr_vec3 position;

			uint8_t color[4];

			cfr_vec3 forward = {0.0f,0.0f,0.0f};

			int16_t referenceID;
			int16_t dummyBoneIndex;

			cfr_vec3 upward = {0.0f,0.0f,0.0f};

			int16_t attachBoneIndex;
			char unk2E;
			char useUpwardVector;

			int32_t unk30;
			int32_t unk34;
			int32_t unk38; //assert(0)
			int32_t unk3C; //assert(0)

			Dummy(UMEM* src);
		};

		class Material //unclear on how safe this is '_>', specifically gxOffset
		{
			public:
			struct Header
			{
				uint32_t nameOffset = 0;
				uint32_t mtdOffset = 0;

				int32_t textureCount = 0;
				int32_t textureIndex = 0;

				uint32_t flags = 0;
				uint32_t gxOffset = 0;

				int32_t unk18 = 0;
				int32_t unk1C = 0; //assert(0)
			};
			
			class GxItem //length INCLUDES this struct!
			{
				public:
				uint32_t id;
				int32_t unk04; //maybe assert(100)? if header.version < 0x20010
				int32_t length;

				char* data;

				GxItem(UMEM* src);
			};

			Header header;
			std::vector<GxItem*> gxItems;
			Material(UMEM* src, FLVER2* parent);
		};

		class Bone
		{
			public:
			cfr_vec3 translation = {0.0f,0.0f,0.0f};

			uint32_t nameOffset;

			cfr_vec3 rot = {0.0f,0.0f,0.0f};

			int16_t parentIndex;
			int16_t childIndex;

			cfr_vec3 scale = {0.0f,0.0f,0.0f};

			int16_t nextSiblingIndex;
			int16_t previousSiblingIndex;

			cfr_vec3 boundingBoxMin = {0.0f,0.0f,0.0f};
			
			int32_t unk3C = 0;

			cfr_vec3 boundingBoxMax = {0.0f,0.0f,0.0f};
			
			// 13 * 4 bytes of space
			char emptyJunk[52]; //potentially needed for spacing :/

			Bone(UMEM* src);
		};

		class Mesh
		{
			public:
			struct Header
			{
				int8_t dynamic; // 0 or 1
		
				int8_t unk01; //assert(0)
				int8_t unk02; //assert(0)
				int8_t unk03; //assert(0)
				
				int32_t materialIndex;
				
				int32_t unk08; //assert(0)
				int32_t unk0C; //assert(0)
				
				int32_t defaultBoneIndex;
				int32_t boneCount;

				uint32_t boundingBoxOffset = 0;
				uint32_t boneIndicesOffset = 0;
				
				int32_t faceSetCount;
				uint32_t faceSetIndicesOffset;

				int32_t vertexBufferCount;
				uint32_t vertexBufferIndicesOffset;
			};
			
			Header header;

			//only if boundingBoxOffset != 0
			cfr_vec3 boundingBoxMin = {0.0f,0.0f,0.0f};
			cfr_vec3 boundingBoxMax = {0.0f,0.0f,0.0f};

			//only if header.version >= 0x2001A
			cfr_vec3 boundingBoxUnk = {0.0f,0.0f,0.0f};

			std::vector<int32_t> boneIndices; //size of boneCount
			std::vector<int32_t> faceSetIndices; //size of faceSetCount
			std::vector<int32_t> vertexBufferIndices; //size of vertexBufferCount

			Mesh(UMEM* src, FLVER2* parent);
		};

		struct Member
		{
			struct Header
			{
				int32_t dataLength;
				int32_t dataOffset;

				int32_t unk08; //assert(0)
				int32_t unk0C; //assert(0)
				int16_t unk10;
				int16_t unk12;

				int16_t baseIndex;

				int16_t unk16;
				int32_t unk18;
				int32_t unk1C;
				int32_t unk20; //assert(0)
				int32_t unk24; //assert(0)
				int32_t unk28; //assert(0)
				int32_t unk2C; //assert(0)

				//edgeGeomSpuConfigInfo? not sure what this means
				int16_t unk30;
				int16_t unk32;
				int16_t unk34;
				int16_t unk36;
				int16_t unk38;

				int16_t indexCount;

				int32_t unk3C; //assert(-1)
			};

			Header header;
			UMEM* data; //size of dataLength

			Member(UMEM* src, long startOffset);
		};

		class EdgeIndices
		{
			public:
			struct Header
			{
				int16_t memberCount = 0;
				int16_t unk02 = 0;
				int32_t unk04 = 0;
				int32_t unk08 = 0; //assert(0)
				int32_t unk0C = 0;
			};

			Header header;
			std::vector<Member*> members; //size of memberCount

			EdgeIndices(UMEM* src);
		};

		class FaceSet
		{
			public:
			struct Header
			{
				uint32_t flags;
				bool triangleStrip;
				bool cullBackFaces;
				char unk06;
				char unk07;
				int32_t vertexIndexCount; //number of indices
				uint32_t vertexIndicesOffset;
			};

			//only if header.version >= 0x20009
			struct VertInfo
			{
				int32_t vertexIndicesLength; //i don't know
				int32_t unk14; //assert(0)
				int32_t vertexIndexSize; //byte size of indices
				int32_t unk1C; //assert(0)
			};
			
			Header header;
			VertInfo vertInfo;
			EdgeIndices* vertexIndicesEdge = nullptr;
			std::vector<uint16_t> vertexIndicesShort = {}; //vertexindexcount
			std::vector<uint32_t> vertexIndicesInt = {}; //vertexindexcount
			int vertexSize = 0; //more accurate

			FaceSet(UMEM* src, FLVER2* parent);
		};

		class VertexBuffer
		{
			public:
			struct Header
			{
				int32_t bufferIndex = 0;
				int32_t layoutIndex = 0;
				int32_t vertexSize  = 0;
				int32_t vertexCount = 0;

				int32_t unk10 = 0; //assert(0)
				int32_t unk14 = 0; //assert(0)

				uint32_t verticesLength = 0; //0 in version 20005, non 0 in 20008
				uint32_t bufferOffset   = 0;
			};
			
			Header header;
			UMEM* data = nullptr; //vertexCount * vertexSize

			VertexBuffer(UMEM* src, FLVER2* parent);
		};

		class LayoutMember
		{
			public:
			int32_t unk00; //0, 1, or 2
			int32_t structOffset; //unused?
			uint32_t type;
			uint32_t semantic;
			int32_t index;

			LayoutMember(UMEM* src);
		};

		class BufferLayout
		{
			public:
			struct Header
			{
				int32_t memberCount;

				int32_t unk04; //assert(0)
				int32_t unk08; //assert(0)

				uint32_t membersOffset;
			};

			Header header;
			std::vector<LayoutMember*> members; //size of memberCount

			BufferLayout(UMEM* src);
		};

		class Texture
		{
			public:
			uint32_t pathOffset;
			uint32_t typeOffset;

			cfr_vec2 scale;

			char unk10;
			char unk11;
			int16_t unk12; //assert(0)

			float unk14;
			float unk18;
			float unk1C;

			Texture(UMEM* src);
		};

		class VertexBoneWeights
		{
			public:
			float a,b,c,d;
			int32_t length; //always 4

			VertexBoneWeights(UMEM* src);
		};

		class VertexBoneIndices
		{
			public:
			int32_t a,b,c,d;
			int32_t length; //always 4

			VertexBoneIndices(UMEM* src);
		};

		struct VertexColor
		{
			float a,r,g,b;
		};

		Header header;
		std::vector<Dummy*> dummies;
		std::vector<Material*> materials;
		std::vector<Bone*> bones;
		std::vector<Mesh*> meshes;
		std::vector<FaceSet*> faceSets;
		std::vector<VertexBuffer*> vertexBuffers;
		std::vector<BufferLayout*> bufferLayouts;
		std::vector<Texture*> textures;

		FLVER2(const char* path);
		FLVER2(UMEM* src);
	};
};