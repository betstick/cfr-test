#pragma once
#include "../stdafx.h"
#include <string>

namespace cfr {
	enum EditEnum
	{
		EDIT_WRAP = 1,
		EDIT_LOCK = 4,
	};

	class PARAMDEF : public File
	{
		private:
		//requires a fully manual read :(
		class i_Header //internal header
		{
			public:
			int32_t fileSize;
			uint16_t headerSize;
			uint16_t dataVersion;
			uint16_t fieldCount;
			uint16_t fieldSize;

			int32_t typePad1;
			int64_t paramTypeOffset;
			int64_t typePad2;
			int64_t typePad3;
			int32_t typePad4;

			char paramID[32];

			int8_t bigEndian;
			int8_t unicode;
			int16_t formatVersion;

			int64_t fieldsOffset;

			i_Header();
			i_Header(UMEM* src);
		};

		class i_Field //internal field
		{
			public:
			long displayNameOffset;
			//std::string displayName;
			char displayType[8];
			char displayFormat[8];
			float defaultValue;
			float minValue;
			float maxValue;
			float increment;
			int editFlags;
			int byteCount;
			//these should be internal to the reader
			/*long descriptionOffset;
			long internalTypeOffset;
			char internalType[32];
			long internalNameOffset;
			char internalName[32];*/
			int sortId;
			int unkB4;
			long offsetB8;
			long offsetC0;
			long offsetC8;
			std::wstring description;
			std::string unkB8;
			std::string unkC0;
			std::string unkC8;
			std::wstring displayName;
			std::string internalType;
			std::string internalName;

			i_Field();
			i_Field(UMEM* src, i_Header header);
		};

		public:
		class Field
		{
			public:
			std::wstring name;
			std::wstring description;
			std::wstring type;
			std::wstring format;
			int id;
			float defaultValue;
			float minValue;
			float maxValue;
			float increment;
		};

		std::vector<Field> fields;

		PARAMDEF();
		PARAMDEF(MEM* src);
	};
};