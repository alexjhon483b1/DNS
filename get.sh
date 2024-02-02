#!/bin/bash

# Function to convert IP parts to integer
ip_to_int() {
  local a b c d
  IFS=. read -r a b c d <<< "$1"
  echo $((a * 256 ** 3 + b * 256 ** 2 + c * 256 + d))
}

# Function to convert integer to IP
int_to_ip() {
  local num=$1
  echo "$((num >> 24 & 255)).$((num >> 16 & 255)).$((num >> 8 & 255)).$((num & 255))"
}

# Function to iterate through IP range
iterate_ip_range() {
  local start_ip end_ip
  start_ip=$(ip_to_int $1)
  end_ip=$(ip_to_int $2)

  for ((i = start_ip; i <= end_ip; i++)); do
    echo $(int_to_ip $i)
  done
}

# Example usage: iterate_ip_range <start_ip> <end_ip>
start_ip="103.89.255.188"
end_ip="104.78.247.255"

# Create a temporary file for the IP list
tmp_ip_list=$(mktemp)

# Run the command in parallel with 120 IP addresses at a time
iterate_ip_range "$start_ip" "$end_ip" | parallel -j 100 ./domain {}
# Clean up the temporary file
rm -f "$tmp_ip_list"
