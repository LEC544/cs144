#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

bool Writer::is_closed() const
{
  return closed_;
}

void Writer::push( string data )
{
  string data_;
  if ( data.size() > available_capacity() ) {
    data_ = data.substr( 0, available_capacity() );
  } else {
    data_ = data;
  }

  buffer_ += data_;
  count_ += data_.size();
  return;
}

void Writer::close()
{
  closed_ = true;
}

uint64_t Writer::available_capacity() const
{
  uint64_t remain = capacity_ - buffer_.size();
  return remain;
}

uint64_t Writer::bytes_pushed() const
{
  return count_;
}

bool Reader::is_finished() const
{
  return closed_ and buffer_.empty();
}

uint64_t Reader::bytes_popped() const
{
  return count_ - buffer_.size();
}

string_view Reader::peek() const
{
  return buffer_;
}

void Reader::pop( uint64_t len )
{
  buffer_ = buffer_.substr( len, buffer_.size() );
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return buffer_.size();
}
