#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

// read the file as string
static bool read_all(const std::string& path, std::string& out){
    std::ifstream f(path);
    if(!f.is_open()) return false;
    out.assign((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    return true;
}


static bool extract_array(const std::string& s, const std::string& key, std::string& out){
    std::string k = "\"" + key + "\"";
    size_t p = s.find(k);
    if(p==std::string::npos) return false;
    p = s.find('[', p);
    if(p==std::string::npos) return false;
    int depth = 0;
    size_t q = p;
    for(; q < s.size(); ++q){
        if(s[q]=='[') depth++;
        else if(s[q]==']'){
            depth--;
            if(depth==0){ q++; break; }
        }
    }
    if(depth!=0) return false; 
    out = s.substr(p+1, q-p-2); 
    return true;
}


static int count_occurs(const std::string& s, const std::string& pat){
    int cnt = 0;
    size_t pos = 0;
    while(true){
        pos = s.find(pat, pos);
        if(pos==std::string::npos) break;
        cnt++;
        pos += pat.size();
    }
    return cnt;
}

int main(){
    std::string text;
    if(!read_all("../ASCII/scene.json", text)){ 
        std::cerr << "Failed to open ../ASCII/scene.json\n";
        return 1;
    }


    bool has_cam = (text.find("\"camera\"") != std::string::npos);
    std::cout << (has_cam ? "Camera found " : "No camera ") << "\n";


    std::string arr;

    int nLights = 0;
    if(extract_array(text, "lights", arr))
        nLights = count_occurs(arr, "\"type\"");
    std::cout << "Lights:  " << nLights << "\n";

    int nSpheres = 0;
    if(extract_array(text, "spheres", arr))
        nSpheres = count_occurs(arr, "\"name\"");
    std::cout << "Spheres: " << nSpheres << "\n";

    int nCubes = 0;
    if(extract_array(text, "cubes", arr))
        nCubes = count_occurs(arr, "\"name\"");
    std::cout << "Cubes:   " << nCubes << "\n";

    int nPlanes = 0;
    if(extract_array(text, "planes", arr))
        nPlanes = count_occurs(arr, "\"name\"");
    std::cout << "Planes:  " << nPlanes << "\n";

    return 0;
}
