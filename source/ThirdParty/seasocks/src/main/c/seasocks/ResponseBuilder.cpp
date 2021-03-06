// Copyright (c) 2013-2016, Matt Godbolt
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
// 
// Redistributions of source code must retain the above copyright notice, this 
// list of conditions and the following disclaimer.
// 
// Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation 
// and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE.

#include "seasocks/ResponseBuilder.h"

#include "internal/ConcreteResponse.h"

namespace seasocks {

ResponseBuilder::ResponseBuilder(ResponseCode code) :
        _code(code),
        _contentType("text/plain"),
        _keepAlive(true),
        _stream(new std::ostringstream) {

}

ResponseBuilder& ResponseBuilder::asHtml() {
    return withContentType("text/html");
}

ResponseBuilder& ResponseBuilder::asText() {
    return withContentType("text/plain");
}

ResponseBuilder& ResponseBuilder::asJson() {
    return withContentType("application/json");
}

ResponseBuilder& ResponseBuilder::withContentType(const std::string& contentType) {
    _contentType = contentType;
    return *this;
}

ResponseBuilder& ResponseBuilder::keepsConnectionAlive() {
    _keepAlive = true;
    return *this;
}

ResponseBuilder& ResponseBuilder::closesConnection() {
    _keepAlive = false;
    return *this;
}

ResponseBuilder& ResponseBuilder::withLocation(const std::string& location) {
    return withHeader("Location", location);
}

ResponseBuilder& ResponseBuilder::setsCookie(const std::string& cookie, const std::string& value) {
    return addHeader("Set-Cookie", cookie + "=" + value);
}

ResponseBuilder& ResponseBuilder::withHeader(const std::string& name, const std::string& value) {
    _headers.erase(name);
    _headers.insert(std::make_pair(name, value));
    return *this;
}

ResponseBuilder& ResponseBuilder::addHeader(const std::string& name, const std::string& value) {
    _headers.insert(std::make_pair(name, value));
    return *this;
}

std::shared_ptr<Response> ResponseBuilder::build() {
    return std::shared_ptr<Response>(new ConcreteResponse(_code, _stream->str(), _contentType, _headers, _keepAlive));
}

}
