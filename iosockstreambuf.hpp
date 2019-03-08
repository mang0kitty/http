#pragma once
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <sys/socket.h>
#include <vector>

class iosockstreambuf : public std::streambuf
{
public:
  explicit iosockstreambuf(int socket, size_t buffer_size);

protected:
  virtual std::streambuf::int_type overflow(std::streambuf::int_type c); //setter(Writer) write buffer full flushes to underlying stream WRITE SIDE
  virtual std::streambuf::int_type underflow();
  virtual int sync();
  //read character from stream and returns character reading virtual int sync();
  //sends basket(write buffer) WRITE SIDE
  //5TB file, dont wan tto read into memory, small array , ask for one byte at a time
private:
  iosockstreambuf(const iosockstreambuf &);
  iosockstreambuf &operator=(const iosockstreambuf &);

  int socket;
  size_t putBackSize;
  std::vector<char> receiveBuffer;
  std::vector<char> sendBuffer;
};
