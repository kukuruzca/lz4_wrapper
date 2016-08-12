#include "LZ4_Wrapper.h"
#include "lz4hc.h"
#include "lz4.h"
#include <stdexcept>

const int COMPRESSION_LEVEL = 0;
const int WRAP_OFFSET_0 = 0;
const int WRAP_OFFSET_4 = sizeof(int32_t);
const int WRAP_OFFSET_8 = 2 * sizeof(int32_t);
const int WRAP_LENGTH = WRAP_OFFSET_8;



void LZ4_Wrapper::Poke4( uint8_t* buffer, size_t offset, uint32_t value)
{
	buffer[offset + 0] = (char)value;
	buffer[offset + 1] = (char)(value >> 8);
	buffer[offset + 2] = (char)(value >> 16);
	buffer[offset + 3] = (char)(value >> 24);
}

uint32_t LZ4_Wrapper::Peek4(const uint8_t* buffer, size_t offset)
{
	return
		((uint32_t)buffer[offset]) |
		((uint32_t)buffer[offset + 1] << 8) |
		((uint32_t)buffer[offset + 2] << 16) |
		((uint32_t)buffer[offset + 3] << 24);
}

std::shared_ptr<std::vector<uint8_t>> LZ4_Wrapper::Wrap(uint8_t* inputBuffer, size_t inputOffset, size_t inputLength, bool highCompression)
{
	std::shared_ptr<std::vector<uint8_t>> outputBuffer = std::make_shared<std::vector<uint8_t>>();
	//std::vector<uint8_t> outputBuffer;

	// if input lenght ==0 return empy buffer
	if (inputLength == 0)
		return outputBuffer;

	outputBuffer->resize(inputLength + WRAP_LENGTH+500);

	
//  	auto  CompressedLenght = highCompression
// 		? LZ4_compressHC_limitedOutput((char*)&inputBuffer[inputOffset], (char*)outputBuffer->data() + WRAP_LENGTH, inputLength,inputLength)
// 		: LZ4_compress_limitedOutput((char*)&inputBuffer[inputOffset], (char*)outputBuffer->data() + WRAP_LENGTH, inputLength, inputLength);

	auto  CompressedLenght = LZ4_compress((char*)&inputBuffer[inputOffset], (char*)outputBuffer->data() + WRAP_LENGTH, inputLength);

	Poke4(outputBuffer->data(), WRAP_OFFSET_0, inputLength);

	if (CompressedLenght >= inputLength || CompressedLenght <= 0)
	{
		Poke4(outputBuffer->data(), WRAP_OFFSET_4, inputLength);
		std::copy(inputBuffer + inputOffset, inputBuffer + inputOffset + inputLength, outputBuffer->begin() + WRAP_LENGTH);
		outputBuffer->resize(inputLength + WRAP_LENGTH);
	}
	else
	{	
		Poke4(outputBuffer->data(), WRAP_OFFSET_4, CompressedLenght);
		outputBuffer->resize(WRAP_LENGTH + CompressedLenght);
	}

	return outputBuffer;
}

std::shared_ptr<std::vector<uint8_t>> LZ4_Wrapper::Wrap(uint8_t* inputBuffer, size_t inputOffset, size_t inputLength)
{
	return Wrap(inputBuffer, inputOffset, inputLength, false);
}

std::shared_ptr<std::vector<uint8_t>> LZ4_Wrapper::Wrap(std::vector<uint8_t> vecBuf)
{
	return Wrap(vecBuf.data(), 0, vecBuf.size(), false);
}

std::shared_ptr<std::vector<uint8_t>> LZ4_Wrapper::WrapHC(uint8_t* inputBuffer, size_t inputOffset, size_t inputLength)
{
	return Wrap(inputBuffer, inputOffset, inputLength, true);
}

std::shared_ptr<std::vector<uint8_t>> LZ4_Wrapper::Unwrap(const uint8_t* inputBuffer, size_t buffLenght /*= 0*/)
{
	std::shared_ptr<std::vector<uint8_t>> outputBuffer = std::make_shared<std::vector<uint8_t>>();

	if (buffLenght < WRAP_LENGTH)
		throw new /*ArgumentException*/std::invalid_argument("LZ4_Wrapper::Unwrap inputBuffer size is invalid");

	auto outputLength = Peek4(inputBuffer, WRAP_OFFSET_0);
	auto inputLength = Peek4(inputBuffer,  WRAP_OFFSET_4);
	if (inputLength > buffLenght -  - WRAP_LENGTH)
		throw new /*ArgumentException*/std::invalid_argument("LZ4_Wrapper::Unwrap inputBuffer size is invalid or has been corrupted");
	

	if (inputLength >= outputLength)
	{
		outputBuffer->resize(inputLength);
		std::copy(inputBuffer + WRAP_LENGTH, inputBuffer + inputLength + WRAP_LENGTH, outputBuffer->begin());
	}
	else
	{
		outputBuffer->resize(outputLength);
		LZ4_uncompress( (char*)inputBuffer + WRAP_OFFSET_8, (char*)outputBuffer->data(), outputLength);
	}

	return outputBuffer;
}

std::shared_ptr<std::vector<uint8_t>> LZ4_Wrapper::Unwrap(std::vector<uint8_t> vecBuf)
{
	return Unwrap(vecBuf.data(), vecBuf.size());
}
