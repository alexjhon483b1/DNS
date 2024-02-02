import socket
import argparse

def get_domain(ip_address, dns_servers):
    for dns_server in dns_servers:
        try:
            print(ip_address)
            socket.setdefaulttimeout(5)  # Set a timeout for the entire socket operation
            domain = socket.gethostbyaddr(ip_address)
            return domain[0]
        except (socket.herror, socket.timeout):
            print(f"Failed to resolve using DNS server {dns_server}. Trying another DNS server.")

    return "Unable to find domain for the given IP address."

def main():
    parser = argparse.ArgumentParser(description="Find domain for a given IP address.")
    parser.add_argument("ip_address", help="IP address to find the domain for")
    args = parser.parse_args()

    ip_address = args.ip_address

    # List of DNS servers to try
    dns_servers = ["8.8.8.8", "8.8.4.4", "208.67.222.222", "208.67.220.220"]

    domain = get_domain(ip_address, dns_servers)
    print(f"{domain}")

if __name__ == "__main__":
    main()

