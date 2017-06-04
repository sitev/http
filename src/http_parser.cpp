#include "http.h"

namespace http {
	HttpParser::HttpParser() {
		
	}
	
	void HttpParser::write(void *data, int size) {
		buffer.write(data, size);
	}

	bool HttpParser::parse(Buffer &buffer) {
		this->buffer = buffer;
		if (mode == 0) {
			subMode = 0;
			method = "";
			bool flag = parseMethod(method);
			if (flag) mode = 1; 
			else return false;
		}
		if (mode == 1) {
			host = "";
			Str sp = "";
			bool flag = parseHostPort(host, sp);
			if (flag) {
				mode = 2;
				if (sp == "") {
					if (isHttps) port = 443;
					else port = 80;
				}
				else port = stoi(sp.to_string());
			}
			else return false;
		}
		return true;
	}

	bool HttpParser::parseMethod(Str &method) {
		int savePos = buffer.getPos();
		while (!buffer.eof()) {
			char ch;
			buffer.readChar(ch);
			if (ch >= 0 && ch <= 32) return true;
			method += ch;
		}
		buffer.setPos(savePos);
		return false;
	}
	bool HttpParser::parseHostPort(Str &host, Str &port) {
		int savePos = buffer.getPos();
		Str s = "";
		while (!buffer.eof()) {
			char ch;
			buffer.readChar(ch);
			s += ch;
			if (ch >= 0 && ch <= 32) {
				subMode = 0;
				subMode2 = 0;
				return true;
			}
			if (ch == ':') {
				if (subMode == 0) {
					if (s == "https:" || s == "http:") {
						host = "";
						subMode2 = 1;
						if (s == "https:") isHttps = true; else isHttps = false;
					}
					else {
						subMode = 1;
						subMode2 = 0;
					}
				}
			}
			else if (ch == '/') {
				if (subMode == 0) {
					if (subMode2 == 0) subMode = 1;
					else if (subMode2 == 2) {
						subMode = 0;
						subMode2 = 0;
						return true;
					}
				}
			}
			else {
				if (subMode == 0) {
					if (subMode2 == 1) subMode2 = 2;
					host += ch;
				}
				else if (subMode == 1) {
					if (ch >= 48 && ch <= 57) port += ch;
				}
			}
		}
		buffer.setPos(savePos);
		return false;
	}

	Str HttpParser::getMethod() {
		return method;
	}

	Str HttpParser::getHost() {
		return host;
	}

	int HttpParser::getPort() {
		return port;
	}

}
