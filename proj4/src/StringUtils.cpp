#include "StringUtils.h"
#include <cctype>
#include <algorithm>

namespace StringUtils{

std::string Slice(const std::string &str, ssize_t start, ssize_t end) noexcept{
    //check bounds of start and end variables
    //return str.substr(start,str.length()-start);
    if (start >= 0 && static_cast<std::size_t>(start) < str.length() &&
        end >= start && static_cast<std::size_t>(end) <= str.length()) {
        //return the string minus the part specified between start and end
        return str.substr(start, end - start);
    }
    return "";
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

std::string LStrip(const std::string &str) noexcept{
    char space = ' ';
    int start = 0;
    for (size_t i = 0; i < str.length(); i++){
        if (str[i] == space){
            start++;
        } else {
            break;
        }
    }
    return str.substr(start);
}

std::string RStrip(const std::string &str) noexcept{
    char space = ' ';
    size_t end = str.length();
    int space_ends = 0;
    for (size_t i = end; i > 0; i--){
        if (str[i-1] == space){
            space_ends++;
        } else {
            break;
        }
    }
    return str.substr(0, end - space_ends);
}

std::string Strip(const std::string &str) noexcept {
    //combine LStrip and RStrip
    std::string Copy = str;
    char space = ' ';
    size_t start = 0;
    size_t end = Copy.length();
    size_t space_ends = 0;
    
    for (size_t i = 0; i < Copy.length(); i++) {
        if (Copy[i] == space) {
            start++;
        } else {
            break;
        }
    }
    for (size_t j = end; j > 0; j--) {
        if (Copy[j - 1] == space) {
            space_ends++;
        } else {
            break;
        }
    }

    if (space_ends > 0) {
        end -= space_ends;
    }

    return Copy.substr(start, end - start);
}

std::string Center(const std::string &str, int width, char fill) noexcept{
    int spaces = width - str.length();
    int rightSpace = spaces / 2; //this will output whole number
    int leftSpace = spaces - rightSpace; //we want more characters on the left if we have an uneven number
    std::string Left = "";
    std::string Right = "";
    for (int i = 0; i < leftSpace; i++){
        Left += fill;
    }
    for (int j = 0; j < rightSpace; j++){
        Right += fill;
    }
    return Left + str + Right;
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