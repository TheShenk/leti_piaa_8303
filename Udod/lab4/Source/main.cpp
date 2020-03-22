#include <vector>
#include <string>
#include <iostream>
#include <sstream>

//#define DEBUG

std::vector<int> prefix(std::string str){

#ifdef DEBUG

    std::cout << std::endl;
    std::cout << "\tCalculating prefix-function: " << std::endl;

#endif

    std::vector<int> pref(str.length(), 0);
    for (int i=1; i<str.length(); i++){

#ifdef DEBUG

    std::cout << "\tNow symbol: " << str[i] << " with index " << i << std::endl;

#endif

        int now_pref = pref[i-1];

#ifdef DEBUG
        std::cout << "\tPrevious symbol's prefix function: " << now_pref << std::endl;
#endif
        // До тех пора, пока не нашли префикс, который можно расширить
        while (now_pref > 0 && str[i] != str[now_pref]){

#ifdef DEBUG

            std::cout << "\tstr[i] != str[now_pref] <=> " << str[i] << " != " << str[now_pref] << now_pref << std::endl;
            std::cout << "\tGo to symbol with index " << now_pref-1 << now_pref << std::endl;

#endif

            // Переходи к предыдущему префиксу. Т.е. префиксу, который расширил текущий префикс
            now_pref = pref[now_pref-1];

#ifdef DEBUG
            std::cout << "\tHis prefix-function: " << now_pref << std::endl;
#endif

        }
        // Если в результате цикла выше нашли префикс, то расширяем его
        if (str[i] == str[now_pref]){
#ifdef DEBUG
            std::cout << "\tEqual symbols founded" << std::endl;
#endif
            now_pref++;
        } else{

#ifdef DEBUG
            std::cout << "\tEqual symbols not founded" << std::endl;
#endif

        }
        // Созраняем результат префикс функции
        pref[i] = now_pref;
#ifdef DEBUG
        std::cout << "\tResult prefix-function: " << now_pref << std::endl;
#endif
    }

    return pref;

}

int char_prefix(std::string &str, std::vector<int> &pref, char c, int prev_char_pref){

    int now_pref = prev_char_pref;

#ifdef DEBUG

    std::cout << std::endl;
    std::cout << "\tCalculating prefix-function for symbol " << c << std::endl;
    std::cout << "\tPrevious symbol's prefix function: " << prev_char_pref << std::endl;

#endif

    // До тех пора, пока не нашли префикс, который можно расширить
    while (now_pref > 0 && c != str[now_pref]){

#ifdef DEBUG

        std::cout << "\tc != str[now_pref] <=> " << c << " != " << str[now_pref] << now_pref << std::endl;
        std::cout << "\tGo to symbol with index " << now_pref-1 << now_pref << std::endl;

#endif

        // Переходи к предыдущему префиксу. Т.е. префиксу, который расширил текущий префикс
        now_pref = pref[now_pref-1];

#ifdef DEBUG
        std::cout << "\tHis prefix-function: " << now_pref << std::endl;
#endif

    }
    // Если в результате цикла выше нашли префикс, то расширяем его
    if (c == str[now_pref]){
#ifdef DEBUG
        std::cout << "\tEqual symbols founded" << std::endl;
#endif
        now_pref++;
    } else{
#ifdef DEBUG
        std::cout << "\tEqual symbols not founded" << std::endl;
#endif
    }
#ifdef DEBUG
    std::cout << "\tResult prefix-function: " << now_pref << std::endl << std::endl;
#endif
    // Созраняем результат префикс функции
    return now_pref;

}

std::vector<int> find(std::string &needle, std::basic_istream<char> &haystack){

#ifdef DEBUG
    std::cout << "Searching " << needle << " in haystack" << std::endl;
#endif

    // Массив индексов, по которым расположены вхождения подстроки в строку
    std::vector<int> indexes = {};

    // Длина искомой подстроки
    int needle_length = needle.length();
    // Уникальный символ. Нужен, что бы префикс всегда соответсвовал искомой подстроки
    needle += "@";

    // Префикс-функция искомой подстроки
    auto pref = prefix(needle);

#ifdef DEBUG
    std::cout << "Needle's prefix-function: ";
    for (auto p: pref) std::cout << p << " ";
    std::cout << std::endl;
#endif

    // Префикс-функция от предыдущего символа. Так как это всегда уникальный символ, то инициализируем нулем
    int prev_pref = 0;

    char now_c;
    // Индекс текущего символа в строке
    int index = 0;
    // Чтение текущего символа
    while (haystack >> now_c){
#ifdef DEBUG
        std::cout << "Read symbol: " << now_c << std::endl;
#endif
        // Вычисления префикс-функци для него
        prev_pref = char_prefix(needle, pref, now_c, prev_pref);

#ifdef DEBUG
        std::cout << "Read symbol's prefix-function: " << prev_pref << std::endl;
#endif

        // Если ее значение равна длине, то это одно из вхождений
        if (prev_pref == needle_length){
#ifdef DEBUG
            std::cout << "Now prefix-function equal needle length. New enter founded" << std::endl;
#endif
            indexes.push_back(index-needle_length+1);
        }

        // Переходим к следующему символу
        index++;
    }

    return indexes;

}

int main(int argc, char* argv[]){

    if (argc == 1){
        std::cout << "No file provided" << std::endl;
        return 0;
    }

    freopen(argv[1], "r", stdin);

    std::string p;
    std::getline(std::cin, p);

    auto indexes = find(p, std::cin);

    if (indexes.size()){
        std::cout << indexes[0];
        for (int i=1; i<indexes.size(); i++){

            std::cout << "," << indexes[i];

        }
    } else{
        std::cout << -1;
    }
    std::cout << std::endl;

}
