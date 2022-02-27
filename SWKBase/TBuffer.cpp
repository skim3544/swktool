#include "pch.h"
#include "TBuffer.h"

namespace swktool
{
	std::string TBuffer::PullString(size_t Len)
	{
		BufferContainer Head;
		std::string Output;

		size_t CopySize = std::min<size_t>(Len, Buffer.size());


		{
			std::lock_guard<std::mutex> lock(buffer_lock);

			typename BufferItr BeginItr = Buffer.begin();
			typename BufferItr EndItr = Buffer.begin();

			// Find data range,
			std::advance(EndItr, CopySize);

			// copy
			Output.assign(BeginItr, EndItr);

			// remove out of buffer
			Buffer.erase(Buffer.begin(), Buffer.begin() + CopySize);
		}

		return Output;
	}

	char TBuffer::PullChar() {
		unsigned char Data;

		{
			std::lock_guard<std::mutex> lock(buffer_lock);
			typename BufferItr BeginItr = Buffer.begin();
			Data = *BeginItr;

			Buffer.erase(BeginItr);
		}

		return Data;
	}

	int TBuffer::PullInt()
	{
		// pull out int data
		unsigned int data;
		{
			std::lock_guard<std::mutex> lock(buffer_lock);
			data = GetLittleEndianIntegerFromByteArray(&Buffer[0], 0);
			typename BufferItr BeginItr = Buffer.begin();

			Buffer.erase(BeginItr, BeginItr + sizeof(int));
		}

		return data;
	}

	short TBuffer::PullShort()
	{
		// pull out int data
		unsigned short data;

		{
			std::lock_guard<std::mutex> lock(buffer_lock);
			data = GetLittleEndianShortFromByteArray(&Buffer[0], 0);
			typename BufferItr BeginItr = Buffer.begin();

			Buffer.erase(BeginItr, BeginItr + sizeof(short));
		}

		return data;
	}

	long long TBuffer::PullLongLong()
	{
		unsigned long long data;

		{
			std::lock_guard<std::mutex> lock(buffer_lock);
			data = GetLittleEndianLongLongFromByteArray(&Buffer[0], 0);
			typename BufferItr BeginItr = Buffer.begin();

			Buffer.erase(BeginItr, BeginItr + sizeof(long long));
		}


		return data;
	}

	void TBuffer::PutString(std::string s)
	{

		{
			std::lock_guard<std::mutex> lock(buffer_lock);

			// get the raw memory pointer, convert to vector
			const unsigned char* raw_memory =
				reinterpret_cast<const unsigned char*>(s.c_str());

			//
			std::vector<unsigned char> ddd(raw_memory, raw_memory + s.size());

			// shove append vector
			Buffer.insert(Buffer.end(), ddd.begin(), ddd.end());
		}

	}


	void TBuffer::PutChar(char C)
	{

		{
			std::lock_guard<std::mutex> lock(buffer_lock);
			Buffer.insert(Buffer.end(), C);
		}

	}

	void TBuffer::PutShort(short val)
	{
		{
			std::lock_guard<std::mutex> lock(buffer_lock);

			unsigned char convert_buffer[2] = { 0 };

			GetLittleEndianToCharArray(&convert_buffer[0], val, sizeof(short));
			Buffer.insert(Buffer.end(), &convert_buffer[0], &convert_buffer[2]);
		}
	}

	void TBuffer::PutInt(int val)
	{

		{
			std::lock_guard<std::mutex> lock(buffer_lock);

			unsigned char convert_buffer[4] = { 0 };

			GetLittleEndianToCharArray(&convert_buffer[0], val, sizeof(int));
			Buffer.insert(Buffer.end(), &convert_buffer[0], &convert_buffer[4]);
		}
	}



	void TBuffer::PutLongLong(long long val)
	{

		{
			std::lock_guard<std::mutex> lock(buffer_lock);
			unsigned char convert_buffer[8] = { 0 };
			GetLittleEndianToCharArray(&convert_buffer[0], val, sizeof(long long));
			Buffer.insert(Buffer.end(), &convert_buffer[0], &convert_buffer[8]);
		}


	}

	void TBuffer::PutArray(char* pNewBuffer, size_t t) {
		{
			std::lock_guard<std::mutex> lock(buffer_lock);
			Buffer.insert(Buffer.end(), &pNewBuffer[0], &pNewBuffer[t]);
		}
	}


	void TBuffer::PutFloat(float val)
	{
		{
			std::lock_guard<std::mutex> lock(buffer_lock);

			FloatPack pack = { 0 };
			pack.FloatData = val;

			Buffer.insert(Buffer.end(), &pack.bytearray[0], &pack.bytearray[4]);
		}
	}

	void TBuffer::PutDouble(double val)
	{
		{
			std::lock_guard<std::mutex> lock(buffer_lock);

			DoublePack pack = { 0 };
			pack.DoubleData = val;

			Buffer.insert(Buffer.end(), &pack.bytearray[0], &pack.bytearray[8]);
		}
	}

	float TBuffer::PullFloat() {
		float val = 0.0f;
		{
			
			std::lock_guard<std::mutex> lock(buffer_lock);

			FloatPack pack = { 0 };

			memcpy(&pack.bytearray[0], &Buffer[0], sizeof(FloatPack));
			val = pack.FloatData;

			Buffer.erase(Buffer.begin(), Buffer.begin() + sizeof(FloatPack));			
		}
		return val;
	}


	double TBuffer::PullDouble() {
		double val = 0.0;
		{
			std::lock_guard<std::mutex> lock(buffer_lock);

			DoublePack pack = { 0 };

			memcpy(&pack.bytearray[0], &Buffer[0], sizeof(DoublePack));
			val = pack.DoubleData;

			Buffer.erase(Buffer.begin(), Buffer.begin() + sizeof(DoublePack));
		}
		return val;
	}



	
}
