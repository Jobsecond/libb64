// :mode=c++:
/*
decode.h - c++ wrapper for a base64 decoding algorithm

This is part of the libb64 project, and has been placed in the public domain.
For details, see http://sourceforge.net/projects/libb64
*/
#ifndef BASE64_DECODE_H
#define BASE64_DECODE_H

#include <vector>
#include <iostream>

namespace base64
{
	extern "C"
	{
		#include "cdecode.h"
	}

	struct decoder
	{
		base64_decodestate _state;
		size_t _buffersize;

		decoder(size_t buffersize_in = BUFFERSIZE)
		: _buffersize(buffersize_in)
		{
			base64_init_decodestate(&_state);
		}

		int decode(char value_in)
		{
			return base64_decode_value(value_in);
		}

		std::streamsize decode(const char* code_in, const std::streamsize length_in, char* plaintext_out)
		{
			return base64_decode_block(code_in, static_cast<size_t>(length_in), plaintext_out, &_state);
		}

		void decode(std::istream& istream_in, std::ostream& ostream_in)
		{
			base64_init_decodestate(&_state);
			//
			const size_t N = _buffersize;

			std::vector<char> code(N);
			std::vector<char> plaintext(N);
			std::streamsize codelength;
			std::streamsize plainlength;

			do
			{
				istream_in.read(code.data(), code.size());
				codelength = istream_in.gcount();
				plainlength = decode(code.data(), codelength, plaintext.data());
				ostream_in.write(plaintext.data(), plainlength);
			}
			while (istream_in.good() && codelength > 0);
			//
			base64_init_decodestate(&_state);
		}
	};

} // namespace base64



#endif // BASE64_DECODE_H

