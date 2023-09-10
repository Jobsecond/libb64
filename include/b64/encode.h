// :mode=c++:
/*
encode.h - c++ wrapper for a base64 encoding algorithm

This is part of the libb64 project, and has been placed in the public domain.
For details, see http://sourceforge.net/projects/libb64
*/
#ifndef BASE64_ENCODE_H
#define BASE64_ENCODE_H

#include <vector>
#include <iostream>

namespace base64
{
	extern "C"
	{
#include "cencode.h"
	}

	struct encoder
	{
		base64_encodestate _state;
		size_t _buffersize;

		encoder(size_t buffersize_in = BUFFERSIZE)
			: _buffersize(buffersize_in)
		{
			base64_init_encodestate(&_state);
		}

		char encode(char value_in)
		{
			return base64_encode_value(value_in);
		}

		std::streamsize encode(const char* code_in, const std::streamsize length_in, char* plaintext_out)
		{
			return base64_encode_block(code_in, static_cast<size_t>(length_in), plaintext_out, &_state);
		}

		size_t encode_end(char* plaintext_out)
		{
			return base64_encode_blockend(plaintext_out, &_state);
		}

		void encode(std::istream& istream_in, std::ostream& ostream_in)
		{
			base64_init_encodestate(&_state);
			//
			const size_t N = _buffersize;
			std::vector<char> plaintext(N);
			std::vector<char> code(2 * N);
			std::streamsize plainlength;
			std::streamsize codelength;

			do
			{
				istream_in.read(plaintext.data(), plaintext.size());
				plainlength = istream_in.gcount();
				//
				codelength = encode(plaintext.data(), plainlength, code.data());
				ostream_in.write(code.data(), codelength);
			} while(istream_in.good() && plainlength > 0);

			codelength = encode_end(code.data());
			ostream_in.write(code.data(), codelength);
			//
			base64_init_encodestate(&_state);
		}
	};

} // namespace base64

#endif // BASE64_ENCODE_H

