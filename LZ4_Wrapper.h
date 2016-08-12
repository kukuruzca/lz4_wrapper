#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include  "stdint.h"

class LZ4_Wrapper
{
private:


	/// <summary>Sets uint32 value in byte buffer.</summary>
	/// <param name="buffer">The buffer.</param>
	/// <param name="offset">The offset.</param>
	/// <param name="value">The value.</param>
	static void Poke4( uint8_t* buffer, size_t offset, uint32_t value);

	/// <summary>Gets uint32 from byte buffer.</summary>
	/// <param name="buffer">The buffer.</param>
	/// <param name="offset">The offset.</param>
	/// <returns>The value.</returns>
	static uint32_t Peek4(const uint8_t* buffer, size_t offset);

	static std::shared_ptr<std::vector<uint8_t>> Wrap(uint8_t* inputBuffer, size_t inputOffset, size_t inputLength, bool highCompression);
	//static std::vector<uint8_t> Wrap(uint8_t* inputBuffer, size_t inputOffset, size_t inputLength, bool highCompression);

public:

	static std::shared_ptr<std::vector<uint8_t>> Wrap(uint8_t* inputBuffer, size_t inputOffset, size_t inputLength);
	static std::shared_ptr<std::vector<uint8_t>> Wrap(std::vector<uint8_t> vecBuf);

	static std::shared_ptr<std::vector<uint8_t>> WrapHC(uint8_t* inputBuffer, size_t inputOffset, size_t inputLength);

	static std::shared_ptr<std::vector<uint8_t>> Unwrap(const uint8_t* inputBuffer, size_t inputLength);
	static std::shared_ptr<std::vector<uint8_t>> Unwrap(std::vector<uint8_t> vecBuf);

};

