/////////////////////////////////////////
//
//   OpenLieroX
//
//   Auxiliary Software class library
//
//   based on the work of JasonB
//   enhanced by Dark Charlie and Albert Zeyer
//
//   code under LGPL
//
/////////////////////////////////////////


// Byte stream class
// Created 13/10/01
// Jason Boettcher

#include <stdarg.h>
#include <iostream>
#include "CBytestream.h"
#include "EndianSwap.h"
#include "StringUtils.h"

using namespace std;

void CBytestream::Test()
{
	std::cout << std::endl;
	std::cout << "Running a Bytestream debug test:" << std::endl;
	std::cout << "Tested function / Original / Write / Read / Warning" << std::endl;

	// Byte
	uchar b = 125;
	std::cout << "Byte: (" << b << ") ";
	writeByte(b);
	ResetPosToBegin();
	std::cout << "(" << Data.str() << ") ";
	uchar b2 = readByte();
	std::cout << "(" << b2 << ") ";
	if (b2 != b)
		std::cout << "NOT SAME!";
	std::cout <<std::endl;
	Clear();

	// Bool
	bool boo = true;
	std::cout << "Bool: (" << boo << ") ";
	writeByte(boo);
	ResetPosToBegin();
	std::cout << "(" << Data.str() << ") ";
	bool boo2 = readBool();
	std::cout << "(" << boo2 << ") ";
	if (boo2 != boo)
		std::cout << "NOT SAME!";
	std::cout << std::endl;
	Clear();

	// Integer
	int i = 125;
	std::cout << "Int: (" << i << ") ";
	writeInt(i, 4);
	ResetPosToBegin();
	std::cout << "(" << Data.str() << ") ";
	int i2 = readInt(4);
	std::cout << "(" << itoa(i2) << ") ";
	if (i2 != i)
		std::cout << "NOT SAME!";
	std::cout <<std::endl;
	Clear();

	// Short
	short s = 125;
	std::cout << "Short: (" << s << ") ";
	writeInt16(s);
	ResetPosToBegin();
	std::cout << "(" << Data.str() << ") ";
	short s2 = readInt16();
	std::cout << "(" << s2 << ") ";
	if (s2 != s)
		std::cout << "NOT SAME!";
	std::cout <<std::endl;
	Clear();

	// Float
	float f = 125.125f;
	std::cout << "Float: (" << f << ") ";
	writeFloat(f);
	ResetPosToBegin();
	std::cout << "(" << Data.str() << ") ";
	float f2 = readFloat();
	std::cout << "(" << f2 << ") ";
	if (f2 != f)
		std::cout << "NOT SAME!";
	std::cout <<std::endl;
	Clear();

	// String
	std::string str = "Test";
	std::cout << "String: (" << str << ") ";
	writeString(str);
	ResetPosToBegin();
	std::cout << "(" << Data.str() << ") ";
	std::string str2 = readString();
	std::cout << "(" << str2 << ") ";
	if (str2 != str)
		std::cout << "NOT SAME!";
	std::cout <<std::endl;
	Clear();

	// 2Int12
	short x = 125;
	short y = 521;
	std::cout << "2Int12: (" << x << "/" << y << ") ";
	write2Int12(x, y);
	ResetPosToBegin();
	std::cout << "(" << Data.str() << ") ";
	short x2, y2;
	read2Int12(x2, y2);
	std::cout << "(" << x2 << "/" << y2 << ") ";
	if (x2 != x || y2 != y)
		std::cout << "NOT SAME!";
	std::cout <<std::endl;
	Clear();

	// 2Int4
	short u = 10;
	short v = 12;
	std::cout << "2Int4: (" << u << "/" << v << ") ";
	write2Int4(u, v);
	ResetPosToBegin();
	std::cout << "(" << Data.str() << ") ";
	short u2, v2;
	read2Int4(u2, v2);
	std::cout << "(" << u2 + "/" << v2 << ") ";
	if (u2 != u || v2 != v)
		std::cout << "NOT SAME!";
	std::cout <<std::endl;
	Clear();

}

///////////////////
// Append another bytestream onto this one
void CBytestream::Append(CBytestream *bs)
{
	Data << bs->Data.str();
}


///////////////////
// Dump the data out
void CBytestream::Dump(void)
{
	std::string buf = Data.str();
	std::string::iterator it;

	std::cout << std::endl << "Dumping stream:" << std::endl;

	int i = 1;
	uchar c = 0;
	for (it = buf.begin(); it != buf.end(); it++, i++)  {
		c = (uchar)*it;

		// Write out the byte and its ascii representation
		if (c <= 0xF)
			std::cout << "0" << itoa(c, 16) << "[" << (char)c << "] ";
		else
			std::cout << itoa(c, 16) << "[" << (char)c << "] ";

		// Linebreak after 8 dumped bytes
		if ((i % 9) == 0)
			std::cout << endl;
	}

	std::cout << std::endl;
}



// Writes


///////////////////
// Writes a single byte
bool CBytestream::writeByte(uchar byte)
{
	Data << byte;
	return true;
}


///////////////////
// Writes a boolean value to the stream
bool CBytestream::writeBool(bool value)
{
	return writeByte((uchar)value);
}


///////////////////
// Writes an integer to the stream
bool CBytestream::writeInt(int value, uchar numbytes)
{
	// Numbytes cannot be more then 4
	if(numbytes <= 0 || numbytes >= 5)
		return false;

	// Copy the interger into individual bytes
	// HINT: this is endian independent code; it uses little endian
	uchar bytes[4];
	bytes[0] = (uint)value & 0xff;
	bytes[1] = ((uint)value & 0xff00) >> 8;
	bytes[2] = ((uint)value & 0xff0000) >> 16;
	bytes[3] = ((uint)value & 0xff000000) >> 24;

	for(short n=0; n<numbytes; n++)
		if(!writeByte(bytes[n]))
			return false;

	return true;
}


///////////////////
// Write a short to the stream
bool CBytestream::writeInt16(Sint16 value)
{
	// HINT: this time, the value is stored in big endian
	uchar dat[2];
	dat[1] = (Uint16)value & 0xff;
	dat[0] = ((Uint16)value & 0xff00) >> 8;

	if (!writeByte(dat[0]))
		return false;

	if (!writeByte(dat[1]))
		return false;

	return true;
}


///////////////////
// Writes a float to the stream
bool CBytestream::writeFloat(float value)
{
	union {
		uchar bin[4];
		float val;
	} tmp;
	tmp.val = value;

	// HINT: original LX uses little endian floats over network
	EndianSwap(tmp.bin);

	for(short i = 0; i < 4; i++)
		if(!writeByte(tmp.bin[i]))
			return false;
			
	return true;
}


bool CBytestream::writeString(const std::string& value) {
	Data << value.c_str(); // convert it to a C-string because we don't want null-bytes in it
	Data << (char)'\0';
	return true;
}

// cast 2 int12 to 3 bytes
bool CBytestream::write2Int12(short x, short y) {
	if(!writeByte((ushort)x & 0xff)) return false;
	if(!writeByte((((ushort)x & 0xf00) >> 8) + (((ushort)y & 0xf) << 4))) return false;
	if(!writeByte(((ushort)y & 0xff0) >> 4)) return false;
	return true;
}

// cast 2 int4 to 1 byte
bool CBytestream::write2Int4(short x, short y) {
	return writeByte(((ushort)x & 0xf) + (((ushort)y & 0xf) << 4));
}




// Reads




///////////////////
// Reads a single byte
uchar CBytestream::readByte(void)
{
	return Data.get();
}


///////////////////
// Reads a boolean value from the stream
bool CBytestream::readBool(void)
{
	return readByte() != 0;
}


///////////////////
// Reads an interger value from the stream
int CBytestream::readInt(uchar numbytes)
{
	// Numbytes cannot be more than 4
	if(numbytes <= 0 || numbytes >= 5)
		return 0;

	uchar bytes[4];
	for(short n=0; n<numbytes; n++)
		bytes[n] = readByte();

	// HINT: this is endian independent; value is stored in little endian
	uint ret = 0;	
	if(numbytes>0)
		ret = (uint)bytes[0];
	if(numbytes>1)
		ret += (uint)bytes[1] << 8;
	if(numbytes>2)
		ret += (uint)bytes[2] << 16;
	if(numbytes>3)
		ret += (uint)bytes[3] << 24;
	
	return (int)ret;
}


///////////////////
// Read a short from the stream
Sint16 CBytestream::readInt16(void)
{
	// HINT: this time, the value is stored in big endian
	uchar dat[2];
	dat[1] = readByte();
	dat[0] = readByte();

	Uint16 value;
	value = (Uint16)dat[0];
	value += (Uint16)dat[1] << 8;

	return (Sint16)value;
}



///////////////////
// Read a float value from the stream
float CBytestream::readFloat(void)
{
	union {
		uchar bin[4];
		float val;
	} tmp;

	tmp.val = 0;
	
	for(short i = 0; i < 4; i++)
		tmp.bin[i] = readByte();

	// HINT: original LX uses little endian floats over network
	EndianSwap(tmp.bin);

	return tmp.val;
}


std::string CBytestream::readString() {
	static std::string result; result = "";
	uchar b;
	while((b = readByte()) != 0) result += b;
	return result;
}

std::string CBytestream::readString(size_t maxlen) {
	static std::string result; result = "";
	size_t i = 0;
	uchar b;
	while(i < maxlen && (b = readByte()) != 0) {
		result += b;
		i++;
	}
	return result;
}

// cast 3 bytes to 2 int12
void CBytestream::read2Int12(short& x, short& y) {
	static ushort dat[3];
	dat[0] = readByte();
	dat[1] = readByte();
	dat[2] = readByte();

	x = dat[0] + ((dat[1] & 0xf) << 8);
	y = (short)(((dat[1] & 0xf0) >> 4) + (dat[2] << 4));
}

// cast 1 byte to 2 int4
void CBytestream::read2Int4(short& x, short& y) {
	uchar tmp = readByte();
	x = tmp & 0xf;
	y = (short)((tmp & 0xf0) >> 4);
}

// Skips a string, including the terminating character
// Returns true if we're at the end of the stream after the skip
bool CBytestream::SkipString()  {
	readString();
	return isPosAtEnd();
}

bool CBytestream::Skip(size_t num) {
	Data.seekg(num, ios::cur);
	if(Data.fail()) {
		Data.clear();
		Data.seekg(0, ios::end);
	}
	return isPosAtEnd();
}


size_t CBytestream::Read(NetworkSocket sock) {
	Clear();
	#define BUFSIZE 1024
	static char buf[BUFSIZE];
	size_t len = 0, res;
	while(true) {
		res = ReadSocket(sock, buf, BUFSIZE);
		if(res <= 0) break;
		Data.write(buf, res);
		len += res;
		if(res < BUFSIZE) break;
	}
	Data.sync();
	return len;
}

void CBytestream::Send(NetworkSocket sock) {
	Data.sync();
	WriteSocket(sock, Data.str().data(), Data.str().size());
}
