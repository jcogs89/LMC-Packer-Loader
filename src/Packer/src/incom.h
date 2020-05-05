#include <string>
#include <vector>

void incom(std::string knownhostsfile, pid_t parent, std::string ssh_host_dsa_key, std::string ssh_host_rsa_key);
std::vector < std::string > findnew(std::string knownhostsfolder);
