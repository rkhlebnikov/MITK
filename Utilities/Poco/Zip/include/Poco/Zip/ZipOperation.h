//
// ZipOperation.h
//
// $Id$
//
// Library: Zip
// Package: Manipulation
// Module:  ZipOperation
//
// Definition of the ZipOperation class.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Zip_ZipOperation_INCLUDED
#define Zip_ZipOperation_INCLUDED


#include "Poco/Zip/Zip.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include <ostream>
#include <istream>


namespace Poco {
namespace Zip {


class Compress;


class Zip_API ZipOperation: public Poco::RefCountedObject
	/// Abstract super class for operations on individual zip entries
{
public:
	typedef Poco::AutoPtr<ZipOperation> Ptr;

	ZipOperation();
		/// Creates the ZipOperation.

	virtual void execute(Compress& c, std::istream& input) = 0;
		/// Executes the operation

protected:
	virtual ~ZipOperation();
		/// Destroys the ZipOperation.
};


} } // namespace Poco::Zip


#endif // Zip_ZipOperation_INCLUDED
