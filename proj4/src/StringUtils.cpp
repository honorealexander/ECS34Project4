#include "StringUtils.h"
#include <cctype>
#include <algorithm>

namespace StringUtils{

    std::string Slice(const std::string &str, ssize_t start, ssize_t end) noexcept {
        ssize_t length = static_cast<ssize_t>(str.length());

        if (start < 0) {
            start = std::max(length + start, static_cast<ssize_t>(0));
        }

        if (end < 0) {
            end = std::max(length + end, static_cast<ssize_t>(0));
        }

        if (start < length) {
            // Adjust the condition to handle the case when only the start index is provided
            if (end == -1 || (end <= length && start <= end)) {
                return str.substr(start, (end == -1) ? std::string::npos : end - start);
            }
        }

        return "";  // Return an empty string for invalid input
    }

    std::string Capitalize(const std::string &str) noexcept{
        auto Copy = str;
        for (size_t i = 1; i < Copy.length(); i++){
            Copy[i] = tolower(Copy[i]);
        }

        Copy[0] = toupper(str[0]);
        return Copy;
    }

    std::string Upper(const std::string &str) noexcept{
        auto Copy = str;
        for (size_t i = 0; i < Copy.length(); i++){
            Copy[i] = toupper(Copy[i]);
        }
        return Copy;
    }

    std::string Lower(const std::string &str) noexcept{
        auto Copy = str;
        for (size_t i = 0; i < Copy.length(); i++){
            Copy[i] = tolower(Copy[i]);
        }
        return Copy;
    }

    std::string LStrip(const std::string &str) noexcept {
        std::string Copy = str;

        // Trim leading spaces
        size_t start = 0;
        while (start < Copy.length() && std::isspace(Copy[start])) {
            ++start;
        }

        return Copy.substr(start);
    }

    std::string RStrip(const std::string &str) noexcept {
        std::string Copy = str;

        // Trim trailing spaces
        size_t end = Copy.length();
        while (end > 0 && std::isspace(Copy[end - 1])) {
            --end;
        }

        return Copy.substr(0, end);
    }

    std::string Strip(const std::string &str) noexcept {
        std::string Copy = str;

        // Trim leading spaces
        size_t start = 0;
        while (start < Copy.length() && std::isspace(Copy[start])) {
            ++start;
        }

        // Trim trailing spaces
        size_t end = Copy.length();
        while (end > start && std::isspace(Copy[end - 1])) {
            --end;
        }

        return Copy.substr(start, end - start);
    }

    std::string Center(const std::string &str, int width, char fill) noexcept {
        int spaces = width - str.length();
        if (spaces > 0) {
            int leftSpace = spaces / 2;
            int rightSpace = spaces - leftSpace;
            std::string Left(leftSpace, fill);
            std::string Right(rightSpace, fill);
            return Left + str + Right;
        } else {
            return str;
        }
    }

    std::string LJust(const std::string &str, int width, char fill) noexcept{
        //like center function but only on left
        int spaces = width - str.length();
        std::string Right = "";
        for (int i = 0; i < spaces; i++){
            Right += fill;
        }
        return str + Right;
    }

    std::string RJust(const std::string &str, int width, char fill) noexcept{
        int spaces = width - str.length();
        std::string Left = "";
        for (int i = 0; i < spaces; i++){
            Left += fill;
        }
        return Left + str;
    }

    std::string Replace(const std::string &str, const std::string &old, const std::string &rep) noexcept{
        auto Copy = str;
        size_t pos = Copy.find(old);
        while (pos != std::string::npos){ //only continue to iterate while replacement string is in the string
            Copy.replace(pos, old.length(), rep);
            pos = Copy.find(old, pos + rep.length()); //iterate through to the next positions
        }        
        return Copy;
    }

    std::vector< std::string > Split(const std::string &str, const std::string &splt) noexcept{
        std::vector<std::string> res; //creating a vector to store split substrings
        size_t start = 0;
        size_t end = str.find(splt); //use find to find the first occurence and split from there
        while (end != std::string::npos){ //loop continues to go through looking for split characters
            res.push_back(str.substr(start, end - start));
            start = end + splt.length();
            end = str.find(splt, start);
        }
        res.push_back(str.substr(start)); //we have to add final part of the string to our result vector 
        return res;
    }

    std::string Join(const std::string &str, const std::vector< std::string > &vect) noexcept{
        std::string res = "";
        auto JoinCopy = str;
        for (const std::string &s : vect){
            res += s + JoinCopy;
        }

        //get rid of trailing character(s)
        if (!res.empty()) {
            res = res.substr(0, res.size() - JoinCopy.size());
        }
        return res;
    }

    std::string ExpandTabs(const std::string &str, int tabsize) noexcept{
        std::string Copy;

        for(char ch : str){
            if(ch == '\t'){ 
                int spaces = tabsize - (Copy.length() % tabsize); 
                Copy.append(spaces, ' ');
            } else {
                Copy.push_back(ch);
            }
        }
        return Copy;
    }


    int EditDistance(const std::string &left, const std::string &right, bool ignorecase) noexcept{
        std::string Left = left;
        std::string Right = right;

        if (ignorecase){ //if ignorecase == true transform our words so that they are lower case
            std::transform(Left.begin(), Left.end(), Left.begin(), ::tolower);
            std::transform(Right.begin(), Right.end(), Right.begin(), ::tolower);
        }

        int m = Left.length(); //get lengths
        int n = Right.length();

        //constructing matrix
        std::vector<std::vector<int>> matrix(m + 1, std::vector<int>(n + 1));
        for (int i = 1; i <= m; i++) {
            matrix[i][0] = i;
        }
        for (int j = 1; j <= n; j++) {
            matrix[0][j] = j;
        }
        //filling in matrix
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (Left[i-1] == Right[j-1]){
                    matrix[i][j] = matrix[i - 1][j - 1];
                } else {
                    matrix[i][j] = std::min({matrix[i - 1][j - 1], matrix[i - 1][j], matrix[i][j - 1]}) + 1;
                }                           //replace               delete          insert
            }
        }
        return matrix[m][n];
    }
};