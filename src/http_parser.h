#pragma once

#include "core.h"
using namespace core;

namespace http {
	class HttpParser {
	protected:
		int mode = 0;
		int subMode = 0, subMode2 = 0;
		bool isHttps = false;
		Buffer buffer;
		Str method = "";
		Str host = "";
		int port = 80;
	public:
		HttpParser();
		virtual void write(void *data, int size);
		virtual bool parse(Buffer &buffer);
		virtual bool parseMethod(Str &method);
		virtual bool parseHostPort(Str &host, Str &port);

		virtual Str getMethod();
		virtual Str getHost();
		virtual int getPort();
	};
}
