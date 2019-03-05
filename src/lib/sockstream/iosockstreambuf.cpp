#include "iosockstreambuf.hpp"

iosockstreambuf::iosockstreambuf(int socket, size_t buffer_size)
    : putBackSize(8),
      sendBuffer(buffer_size + 1),
      receiveBuffer(std::max(buffer_size, putBackSize) + putBackSize)
{
  this->socket = socket;
  auto receiveEnd = &this->receiveBuffer.front() + this->receiveBuffer.size();
  this->setg(receiveEnd, receiveEnd, receiveEnd);

  auto sendStart = &this->sendBuffer.front();
  this->setp(sendStart, sendStart + this->sendBuffer.size() - 1);
} //buffer managemn twork in buffer, parser just deals with stream ,

std::streambuf::int_type iosockstreambuf::overflow(std::streambuf::int_type c)
{
  if (c == traits_type::eof())
  {
    return c;
  }

  *this->pptr() = c;
  this->pbump(1);

  if (this->pptr() == this->epptr())
  {
    if (this->sync() == -1)
    {
      return traits_type::eof();
    }
  }

  return c;
}

int iosockstreambuf::sync() //synchronisessend buffer to target
{
  auto size = this->pptr() - this->pbase();
  if (size == 0)
  {
    return 0;
  }

  auto sent = send(this->socket, this->pbase(), size, 0);
  if (sent == -1)
  {
    return -1;
  }

  this->pbump(-sent);

  return sent;
}

std::streambuf::int_type iosockstreambuf::underflow()
{
  if (this->gptr() < this->egptr())
  {
    return traits_type::to_int_type(*this->gptr());
  }

  char *base = &this->receiveBuffer.front();
  char *start = base;

  if (this->eback() == base)
  {
    std::memmove(base, this->egptr() - this->putBackSize, this->putBackSize);
    start += this->putBackSize;
  }

  ssize_t read = recv(this->socket, start, this->receiveBuffer.size() - (start - base), 0);
  if (read <= 0)
  {
    return traits_type::eof();
  }

  this->setg(base, start, start + read);
  return traits_type::to_int_type(*this->gptr());
}
