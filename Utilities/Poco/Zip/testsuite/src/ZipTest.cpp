//
// ZipTest.cpp
//
// $Id$
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


#include "ZipTest.h"
#include "Poco/Zip/SkipCallback.h"
#include "Poco/Zip/ZipLocalFileHeader.h"
#include "Poco/Zip/ZipArchive.h"
#include "Poco/Zip/ZipStream.h"
#include "Poco/Zip/Decompress.h"
#include "Poco/Zip/ZipCommon.h"
#include "Poco/StreamCopier.h"
#include "Poco/File.h"
#include "Poco/URI.h"
#include "Poco/Path.h"
#include "Poco/Delegate.h"
#include "Poco/StreamCopier.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include <fstream>
#include <sstream>


using namespace Poco::Zip;


ZipTest::ZipTest(const std::string& name): CppUnit::TestCase(name)
{
}


ZipTest::~ZipTest()
{
}


void ZipTest::testSkipSingleFile()
{
	std::string testFile = getTestFile("test.zip");
	std::ifstream inp(testFile.c_str(), std::ios::binary);
	assert (inp.good());
	SkipCallback skip;
	ZipLocalFileHeader hdr(inp, false, skip);
	assert (ZipCommon::HS_FAT == hdr.getHostSystem());
	int major = hdr.getMajorVersionNumber();
	int minor = hdr.getMinorVersionNumber();
	assert (major <= 2);
	std::size_t hdrSize = hdr.getHeaderSize();
	assert (hdrSize > 30);
	ZipCommon::CompressionMethod cm = hdr.getCompressionMethod();
	assert (!hdr.isEncrypted());
	Poco::DateTime aDate = hdr.lastModifiedAt();
	Poco::UInt32 cS = hdr.getCompressedSize();
	Poco::UInt32 uS = hdr.getUncompressedSize();
	const std::string& fileName = hdr.getFileName();
}


void ZipTest::testDecompressSingleFile()
{
	std::string testFile = getTestFile("test.zip");
	std::ifstream inp(testFile.c_str(), std::ios::binary);
	assert (inp.good());
	ZipArchive arch(inp);
	ZipArchive::FileHeaders::const_iterator it = arch.findHeader("testfile.txt");
	assert (it != arch.headerEnd());
	ZipInputStream zipin (inp, it->second);
	std::ostringstream out(std::ios::binary);
	Poco::StreamCopier::copyStream(zipin, out);
	assert(!out.str().empty());
}


void ZipTest::testCrcAndSizeAfterData()
{
	std::string testFile = getTestFile("data.zip");
	std::ifstream inp(testFile.c_str(), std::ios::binary);
	assert (inp.good());
	Decompress dec(inp, Poco::Path());
	dec.EError += Poco::Delegate<ZipTest, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(this, &ZipTest::onDecompressError);
	dec.decompressAllFiles();
	dec.EError -= Poco::Delegate<ZipTest, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(this, &ZipTest::onDecompressError);
	assert (_errCnt == 0);
	assert (!dec.mapping().empty());
}


void ZipTest::testCrcAndSizeAfterDataWithArchive()
{
	std::string testFile = getTestFile("data.zip");
	std::ifstream inp(testFile.c_str(), std::ios::binary);
	assert (inp.good());
	Poco::Zip::ZipArchive zip(inp);
	inp.clear();
	inp.seekg(0);
	Poco::Zip::ZipArchive::FileHeaders::const_iterator it = zip.headerBegin();
	for ( ; it!=zip.headerEnd(); ++it)
	{
		Poco::Zip::ZipInputStream zipis(inp,it->second);
		Poco::Path path(it->second.getFileName());
		if (path.isFile())
		{
			std::ofstream os("test.dat");
			Poco::StreamCopier::copyStream(zipis,os);
		}
	}
}


std::string ZipTest::getTestFile(const std::string& testFile)
{
	Poco::Path root;
	root.makeAbsolute();
	Poco::Path result;
	while (!Poco::Path::find(root.toString(), "data", result))
	{
		root.makeParent();
		if (root.toString().empty() || root.toString() == "/")
			throw Poco::FileNotFoundException("Didn't find data subdir");
	}
	result.makeDirectory();
	result.setFileName(testFile);
	Poco::File aFile(result.toString());
	if (!aFile.exists() || (aFile.exists() && !aFile.isFile()))
		throw Poco::FileNotFoundException("Didn't find " + testFile);
	
	return result.toString();
}


void ZipTest::testDecompress()
{
	std::string testFile = getTestFile("test.zip");
	std::ifstream inp(testFile.c_str(), std::ios::binary);
	assert (inp.good());
	Decompress dec(inp, Poco::Path());
	dec.EError += Poco::Delegate<ZipTest, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(this, &ZipTest::onDecompressError);
	dec.decompressAllFiles();
	dec.EError -= Poco::Delegate<ZipTest, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(this, &ZipTest::onDecompressError);
	assert (_errCnt == 0);
	assert (!dec.mapping().empty());
}


void ZipTest::testDecompressFlat()
{
	std::string testFile = getTestFile("test.zip");
	std::ifstream inp(testFile.c_str(), std::ios::binary);
	assert (inp.good());
	Decompress dec(inp, Poco::Path(), true);
	dec.EError += Poco::Delegate<ZipTest, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(this, &ZipTest::onDecompressError);
	dec.decompressAllFiles();
	dec.EError -= Poco::Delegate<ZipTest, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(this, &ZipTest::onDecompressError);
	assert (_errCnt == 0);
	assert (!dec.mapping().empty());
}


void ZipTest::onDecompressError(const void* pSender, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>& info)
{
	++_errCnt;
}


void ZipTest::setUp()
{
	_errCnt = 0;
}


void ZipTest::tearDown()
{
}


CppUnit::Test* ZipTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ZipTest");

	CppUnit_addTest(pSuite, ZipTest, testSkipSingleFile);
	CppUnit_addTest(pSuite, ZipTest, testDecompressSingleFile);
	CppUnit_addTest(pSuite, ZipTest, testDecompress);
	CppUnit_addTest(pSuite, ZipTest, testDecompressFlat);
	CppUnit_addTest(pSuite, ZipTest, testCrcAndSizeAfterData);
	CppUnit_addTest(pSuite, ZipTest, testCrcAndSizeAfterDataWithArchive);
	return pSuite;
}
