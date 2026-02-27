#include <iostream>
#include <vector>

typedef std::vector<std::pair<std::string, int>> pairlist_t;
typedef std::string str;

int main(){
    
    pairlist_t pairtlist;
    str name = "Sucker";

    return 0;
}

/*
typedef is something used to name coplicated data types or simple as you want but it is used mainly for complicated ones like :

"typedef std::vector<std::pair<std::string, int>> pairlist_t;"


you can use something else called using like

using std::stirng = str
unisg std::vector<std::pair<std::string, int>> = pairlist_t
........
*/