#pragma once

#include <vector>
#include <string>
#include <iterator>
#include <iomanip>
#include <algorithm>  
#include <mutex>

namespace swktool {

	typedef std::vector<unsigned char> BufferContainer;
	typedef std::vector<unsigned char>::iterator BufferItr;

	class TBuffer {
	protected:
		BufferContainer Buffer;
		int GetIndex = 0;

		std::mutex  buffer_lock;
	public:
		TBuffer() { ; }
		TBuffer(std::string str) :
			Buffer(str.begin(), str.end())
		{
		}
		TBuffer(int nSize)
		{
			Buffer.reserve(nSize);
		}

		TBuffer(const TBuffer& Other)
		{
			Buffer = Other.Buffer;
		}

		TBuffer(TBuffer&& Other) noexcept
		{
			Buffer = std::move(Other.Buffer);
			GetIndex = Other.GetIndex;
		}
		virtual ~TBuffer() { ; }

		size_t Size() const
		{
			return Buffer.size();
		}

		std::string PullString(size_t Len);
		char  PullChar();
		int   PullInt();
		short PullShort();
		long long PullLongLong();
		float PullFloat();
		double PullDouble();

		void PutString(std::string s);
		void PutChar(char C);
		void PutInt(int val);
		void PutShort(short val);
		void PutLongLong(long long val);
		void PutArray(char* pNewBuffer, size_t t);
		void PutFloat(float val);
		void PutDouble(double val);
		
		unsigned char& operator[](const size_t i) noexcept
		{
			return Buffer[i];
		}

		std::vector<unsigned char>& GetVector() {
			return Buffer;
		}

	protected:
		unsigned int GetBigEndianIntegerFromByteArray(unsigned char data[], int startIndex)
		{
			unsigned int b0 = (data[startIndex] & 0xff) << 24;
			unsigned int b1 = (data[startIndex + 1] & 0xff) << 16;
			unsigned int b2 = (data[startIndex + 2] & 0xff) << 8;
			unsigned int b3 = (data[startIndex + 3] & 0xff);

			unsigned int Final = (b0 | b1 | b2 | b3);

			return Final;
		}

		unsigned int GetLittleEndianIntegerFromByteArray(unsigned char data[], int startIndex = 0) {
			unsigned int b0 = (data[startIndex] & 0xff);
			unsigned int b1 = (data[startIndex + 1] & 0xff);
			unsigned int b2 = (data[startIndex + 2] & 0xff);
			unsigned int b3 = (data[startIndex + 3] & 0xff);

			unsigned int Final = (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;

			return Final;
		}

		unsigned short GetLittleEndianShortFromByteArray(unsigned char data[], int startIndex = 0) {
			unsigned short b0 = (data[startIndex] & 0xff);
			unsigned short b1 = (data[startIndex + 1] & 0xff);

			unsigned short Final = ((b1 << 8) | b0) & 0xffff;

			return Final;
		}

		unsigned long long GetLittleEndianLongLongFromByteArray(unsigned char data[], int startIndex = 0) {
#pragma warning( disable : 4293 6297 26451 26452)
			unsigned long long b0 = (data[startIndex + 0] & 0xff);
			unsigned long long b1 = (data[startIndex + 1] & 0xff) << 8;
			unsigned long long b2 = (data[startIndex + 2] & 0xff) << 16;
			unsigned long long b3 = (data[startIndex + 3] & 0xff) << 24;
			unsigned long long b4 = (data[startIndex + 4] & 0xff) << 32;
			unsigned long long b5 = (data[startIndex + 5] & 0xff) << 40;
			unsigned long long b6 = (data[startIndex + 6] & 0xff) << 48;
			unsigned long long b7 = (data[startIndex + 7] & 0xff) << 56;

			unsigned long long Final = (b7 | b6 | b5 | b4 | b3 | b2 | b1 | b0);

			return  Final;
		}


		int GetBigEndianToCharArray(char* buffer, int in, size_t BYTES_PER_INT = sizeof(int))
		{
			for (size_t i = 0; i < BYTES_PER_INT; i++) {
				size_t shift = 8 * (BYTES_PER_INT - 1 - i);
				buffer[i] = (in >> shift) & 0xff;
			}
			return 0;
		}

		int GetLittleEndianToCharArray(unsigned char* buffer, long long in, size_t BYTES_PER_INT)
		{
			size_t shift = 8;

			for (size_t i = 0; i < BYTES_PER_INT; i++) {
				buffer[i] = (in) & 0xff;

				//std::cout << std::hex << (short)buffer[i] << std::endl;
				in = (in >> 8);
			}
			return 0;
		}

		// use union to data layer the byte array on top of Float data type
		// whatever you stored in float, you can get byte aray representation of the data
		union FloatPack {
			char  bytearray[sizeof(float)];
			float FloatData;
		};


		union DoublePack {
			char  bytearray[sizeof(double)];
			double DoubleData;
		};
	};
};


