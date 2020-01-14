#include <iostream>
#include <SFML/Network.hpp>
#include <string>
#include <sstream>
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define END "\033[0m"

static bool port_open(const std::string &addr, int port)
{
    return (sf::TcpSocket().connect(addr, port) == sf::Socket::Done);
}

template<typename T>
static void swap_vals(T &x, T &y)
{
    x ^= y ^= x;
}

template <typename T>
static std::vector<T> port_range(T min, T max)
{
    if (min > max)
        swap_vals(min, max);
    else if (min == max)
        return (std::vector<T>(1, min));
    std::vector<T> ports;
    for (; min <= max; ++min)
        ports.push_back(min);
    return (ports);
}

static std::vector<std::string> split_str(const std::string &s,
        char del = ' ', bool allow_empty = false)
{
    std::vector<std::string> tokens;
    std::stringstream sstream(s);
    std::string token;
    while (std::getline(sstream, token, del))
    {
        if (allow_empty || token.size() > 0)
            tokens.push_back(token);
    }
    return (tokens);
}

static int str_to_int(const std::string &s)
{
    std::stringstream sstream(s);
    int i;
    sstream >> i;
    return (i);
}

static std::vector<int> get_ports(const std::string &range)
{
    std::vector<int> ports;
    for (const std::string &token : split_str(range, ','))
    {
        std::vector<std::string> srange = split_str(token, '-');
        switch(srange.size())
        {
            case 0:
                ports.push_back(str_to_int(token));
                break;
            case 1:
                ports.push_back(str_to_int(srange[0]));
                break;
            default:
            {
                int min = str_to_int(srange[0]);
                int max = str_to_int(srange[srange.size() - 1]);
                // we can do just ports = port_range(min, max)
                for (int port : port_range(min, max))
                    ports.push_back(port);
                break;
            }
        }
    }
    return (ports);
}

int main()
{
    std::string address;
    std::string ports_lst;
    std::vector<int> open_ports;
    std::vector<int> ports;
    std::cout << "Address: " << std::flush;
    std::getline(std::cin, address);
    std::cout << "Port: " << std::flush;
    std::getline(std::cin, ports_lst);
    ports = get_ports(ports_lst);
    std::cout << "Scanning for open ports... " << std::flush;
    for (int port : ports)
    {
        std::cout << "Port " << port << " at address " << address << " is ";
        if (port_open(address, port))
        {
            std::cout << GREEN << "OPEN" << END << std::endl;
            open_ports.push_back(port);
        }
        else
            std::cout << RED << "CLOSED" << END << std::endl;
    }
    if (!open_ports.empty())
    {
        if (open_ports.size() == 1)
            std::cout << GREEN << "Open port found: " << open_ports[0] << END << std::endl;
        else
        {
            std::cout << GREEN << "Open ports found: " ;
            for (const int opport : open_ports)
            {
                std::cout << opport;
                if (opport != open_ports[open_ports.size() - 1])
                    std::cout << " , ";
            }
            std::cout << END << std::endl;
        }
    }
    else
        std::cout << RED << "No open ports found." << END << std::endl;
    std::cout << std::flush;
    return (0);
}