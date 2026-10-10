#include "reassembler.hh"
#include <map>

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{
  // Your code here.
  if ( is_last_substring ) { 
    end_index_ = first_index + data.size(); 
    is_finished_ = true;
  }
  uint64_t first_unacceptable = next_byte_index_ + output_.writer().available_capacity();
  if ( first_index >= first_unacceptable ) { return; }
  if ( first_index < next_byte_index_ ) {
    if ( first_index + data.size() <= next_byte_index_ ) { return; }
    else { 
      data = data.substr( next_byte_index_ - first_index );
      first_index = next_byte_index_;
    }
  }
  if ( data.size() > first_unacceptable - first_index ) {
    data = data.substr( 0, first_unacceptable - first_index );
  }

  insert_helper( first_index, data, string_map );

  if ( string_map.contains(next_byte_index_) ) {
    string_map[first_index] = data;
    auto it = string_map.find(first_index);
    string push_data {};

    push_data = it->second;
    string_map.erase(it);
    output_.writer().push( push_data );
    next_byte_index_ += push_data.size();

  }

  if ( next_byte_index_ == end_index_ && is_finished_ ) { output_.writer().close(); }
}

uint64_t Reassembler::bytes_pending() const
{
  // Your code here.
  uint64_t pending_bytes_ {};
  for ( const auto& item : string_map ) {
    pending_bytes_ += item.second.size();
  }
  return pending_bytes_;
}

void insert_helper( uint64_t& first_index, string& data, map<uint64_t, string>& string_map ) {
  uint64_t insert_index = first_index;
  uint64_t last_data_index = first_index + data.size();
  auto it = string_map.begin();
  while ( it != string_map.end() ) {
    uint64_t last_it_index = it->first + it->second.size();

    if ( first_index >= it->first && last_data_index <= last_it_index ) {
      return;
    }

    else if ( first_index <= it->first && last_data_index >= last_it_index ) {
      it = string_map.erase(it);
      continue;
    }

    else if ( last_it_index >= first_index && it->first < first_index ) {
      data = it->second + data.substr( last_it_index - first_index );
      insert_index = it->first;
      it = string_map.erase(it);
      continue;
    }
    
    else if ( last_data_index >= it->first && first_index < it->first) {
      data = data + it->second.substr( last_data_index - it->first );
      last_data_index = insert_index + data.size();
      it = string_map.erase(it);
      continue;
    } 

    else { it++; }
  }
  string_map[insert_index] = data;
}
