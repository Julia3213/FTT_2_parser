#include <iostream>
#include <vector>
#include <memory>
#include <array>
#include <variant>
#include <unordered_map>
#include <cctype>


struct OpeningBracket { std::string value = "("; };

struct ClosingBracket { std::string value = ")"; };

struct Number {
    int value;
};

struct UnknownToken {
    std::string value = "un";
};

struct MinToken { std::string value = "min"; };

struct MaxToken { std::string value = "max"; };

struct AbsToken { std::string value = "abs"; };

struct Plus { std::string value = "+"; };

struct Minus { std::string value = "-"; };

struct Multiply { std::string value = "*"; };

struct Modulo { std::string value = "%"; };

struct Divide { std::string value = "/"; };

struct Sqr { std::string value = "sqr"; };

struct Sqrt { std::string value = "sqrt"; };

struct Comma { std::string value = ","; };

using Token = std::variant<OpeningBracket, ClosingBracket, Number, UnknownToken, MinToken, MaxToken, AbsToken, Plus, Minus, Multiply, Divide, Modulo, Sqr, Sqrt, Comma>;


const std::unordered_map<char, Token> kSymbol2Token {
    {'+', Plus{}}, {'-', Minus{}}, {'*', Multiply{}}, {'/', Divide{}}, {'%', Modulo{}}, {')', ClosingBracket{}}, {'(', OpeningBracket{}}, {',', Comma{}} };

const std::unordered_map<std::string, Token> string2Token {
    {"min", MinToken{}}, {"max", MaxToken{}}, {"abs", AbsToken{}}, {"sqr", Sqr{}}, {"sqrt", Sqrt{}} };

int ToDigit(unsigned char symbol) {
    return symbol - '0';
}

bool IsLetter(unsigned char symbol) {
    return (symbol >= 'A' && symbol <= 'Z') || (symbol >= 'a' && symbol <= 'z');
}

Number ParseNumber(const std::string& input, size_t& pos) {
    int value = 0;
    auto symbol = static_cast<unsigned char>(input[pos]);
    while (std::isdigit(symbol)) {
        value = value * 10 + ToDigit(symbol);
        if (pos == input.size() - 1) {
            break;
        }
        symbol = static_cast<unsigned char>(input[++pos]);
    }
    return Number{ value };
}

Token ParseName(const std::string& input, size_t& pos) {
    auto symbol = input[pos];
    std::string str = "";
    while (IsLetter(symbol)) {
        str += symbol;
        if (pos == input.size() - 1) {
            break;
        }
        symbol = input[++pos];
    }
    auto it = string2Token.find(str);
    if (it == string2Token.end()) {
        ++pos;
        return UnknownToken{};
    }
    return (it->second);
}

std::vector<Token> Tokenize(const std::string& input) {
    std::vector<Token> tokens;
    const size_t size = input.size();
    size_t pos = 0;
    while (pos < size) {
        const auto symbol = static_cast<unsigned char>(input[pos]);
        if (std::isspace(symbol)) {
            ++pos;
        }
        else if (std::isdigit(symbol)) {
            tokens.emplace_back(ParseNumber(input, pos));
        }
        else if (auto it = kSymbol2Token.find(symbol); it != kSymbol2Token.end()) {
            tokens.emplace_back(it->second);
            ++pos;
        }
        else {
            tokens.emplace_back(ParseName(input, pos));
        }
    }
    return tokens;
}

int main() {
    std::string exp = "(167 + 26) * 3 / 4 + 5 * (6 - 7) + sqrt(4) * max(sqr(3), abs(5)) + x";
    auto res = Tokenize(exp);
    
    for (const auto& s : res) {
        std::visit([](const auto& st) {
            std::cout << st.value << ", ";
        }, s);
    }
    return 0;
}