#include <iostream>
#include <string>
#include <curl/curl.h>
#include <regex>
#include <vector>

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    int depth;
    CURL *curl;
    CURLcode res;
    std::string rdBuffer;
    std::string input;
    std::vector <std::string> result;
    std::vector <std::string> current_list;
    std::vector <std::string> tmp_list;
    std::cout << "web site: " << '\n';
    std::cin >> input; 
    std::cout << "depth: " << '\n';
    std::cin >> depth;
    current_list.push_back(input);
    for(int i = 1; i <= depth; i++) {
        for(int j = 0; j < current_list.size(); j++) {
            curl = curl_easy_init();
            if(curl) {
                curl_easy_setopt(curl, CURLOPT_URL, current_list[j].c_str());
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rdBuffer);
                res = curl_easy_perform(curl);
                if(res != CURLE_OK) {
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                }
                std::regex r("http[s]?://[\\w\\.\\?\\$\\-\\+#/%=]+");
                std::smatch m;
                while (std::regex_search (rdBuffer, m, r)) {
                    tmp_list.push_back(m.str());
                    result.push_back(m.str());
                    rdBuffer = m.suffix().str();
                }
                curl_easy_cleanup(curl);
            }
            rdBuffer.clear();
        }
        current_list.clear();
        current_list.insert(current_list.end(), tmp_list.begin(), tmp_list.end());
        tmp_list.clear();
    }
    for(int k = 0; k < result.size(); k++) {
        std::cout << result[k] << std::endl;
    }
    return 0;
}
