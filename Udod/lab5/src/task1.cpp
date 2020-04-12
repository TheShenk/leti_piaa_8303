#include <iostream>
#include <map>
#include <vector>
#include <functional>

#define DEBUG

#ifdef DEBUG

    int globalIdCounter = 0;

#endif

// Функция разделения строки на подшаблоны
std::vector<std::pair<std::string, int>> split(std::string strToSplit, char delimeter)
{
    std::vector<std::pair<std::string, int>> splitted;

    std::string nowStr = "";

    for (int i=0; i<strToSplit.size(); i++){
        // Если встречаем разделитель и это не несколько шаблонов подряд, то есть нашли новый подшаблон
        if (strToSplit[i] == delimeter && !nowStr.empty()){
            splitted.emplace_back(nowStr, i-nowStr.size());
            nowStr = "";
        } else if(strToSplit[i] != delimeter){
            nowStr += strToSplit[i]; // Иначе добавляем текущий символ в текущий подшаблон
        }
    }

    // В результате выполнения цикла выше последний подшаблон мог быть не добавлен
    if (!nowStr.empty()){
        splitted.emplace_back(nowStr, strToSplit.size()-nowStr.size());
    }

    return splitted;
}

class BorNode{

private:
    int depth;

    BorNode *parent = nullptr;
    char parentThrough;
    BorNode *suffixPtr = nullptr;
    BorNode *terminalPtr = nullptr;

    bool _isTerminal = false;
    std::vector<int> _needles = {};

    std::map<char, BorNode*> children = {};


public:

    BorNode(): parentThrough(0), depth(0) {
#ifdef DEBUG
        id = globalIdCounter;
        globalIdCounter++;
#endif
    }
    explicit BorNode(BorNode *parent, char through): parent(parent), parentThrough(through), depth(parent->depth+1) {
#ifdef DEBUG
        id = globalIdCounter;
        globalIdCounter++;
#endif
    }

    // Добавление шаблона в бор
    void addNeedle(std::string &needle, int needleCount){

#ifdef DEBUG
        std::cout << std::endl;
        std::cout << "\tТекущая вершина: " << id << std::endl;
#endif

        // Если есть необработанные символы
        if (!needle.empty()) {

#ifdef DEBUG
            std::cout << "\tТекущий символ: " << needle[0] << std::endl;
#endif

            auto *next = children[needle[0]]; // Элемент, в который можем перейти по текущему символу

            // Если по этому пути еще не ходили
            if (next == nullptr) {

                next = new BorNode(this, needle[0]); // То создаем этот путь
                children[needle[0]] = next;
#ifdef DEBUG
                std::cout << "\tНет наследника по этому символу. Создаю. Его ID: " << next->id << std::endl;
#endif

            } else{
#ifdef DEBUG
                std::cout << "\tНаследник обнаружен. Его ID: " << next->id << std::endl;
#endif
            }

            // Переходим к следующему символу
            needle.erase(0, 1);
            next->addNeedle(needle, needleCount);

        } else{

#ifdef DEBUG
            std::cout << "\tСтрока пустая. Значит, вершина - терминальная." << std::endl;
#endif

            // Так как дошли до конца строки, то отмечаем вершину терминальной
            _isTerminal = true;
            _needles.push_back(needleCount);

        }

    }

    // Получение суффикс-ссылки. Реализована лениво.
    BorNode *getSuffixPtr(){

#ifdef DEBUG
        std::cout << std::endl;
        std::cout << "\tРассчет суфикс-ссылки. ID вершины: " << id << std::endl;
#endif

        // Если текущая верщина - корень, то возвращаем его в качестве суффикса
        if (parent == nullptr){
#ifdef DEBUG

            std::cout << "\tТекущая вершина - корень" << std::endl;

#endif
            return this;
        }

        // Если на уровень ниже корня, то возвращаем корень
        if (parent->parent == nullptr){
#ifdef DEBUG

            std::cout << "\tТекущая вершина - сын корня." << std::endl;

#endif
            return parent;
        }

        // Если суффикс-ссылка еще не вычислена, то вычисляем
        if (suffixPtr == nullptr){
#ifdef DEBUG
            std::cout << "\tСуффикс ссылка еще не вычислялась. Переход к суффикс-ссылке родителя. ID родителя: " << parent->id << std::endl;
#endif

            // Переходим по суффикс-ссылке родителя и смотрим, можно ли из нее пройти по символу, по которому попали сюда
            suffixPtr = parent->getSuffixPtr()->deltaPtr(parentThrough);
        }else{
#ifdef DEBUG

            std::cout << "\tСуффикс-ссылка уже вычислялась." << std::endl;

#endif
        }

#ifdef DEBUG

        std::cout << "\tСуффикс-ссылка вычеслена." << std::endl;

#endif

        return suffixPtr;

    };

    // Вычисление терминальной ссылке. Реализована лениво.
    BorNode *getTerminalPtr(){

#ifdef DEBUG
        std::cout << std::endl;
        std::cout << "\tРасчет терминальной ссылки. ID вершины: " << id << std::endl;

#endif

        // Если корень, то возврашаем его
        if (isRoot()){
#ifdef DEBUG

            std::cout << "\tТекущая вершина - корень" << std::endl;

#endif
            return this;
        }

        // Если еще не вычисляли
        if (terminalPtr == nullptr){

#ifdef DEBUG
            std::cout << "\tТерминальная ссылка еще не вычислялась." << std::endl;
#endif

            if (getSuffixPtr()->_isTerminal){
#ifdef DEBUG
                std::cout << "\tВершина по суффикс-ссылке терминальная." << std::endl;
#endif
                terminalPtr = getSuffixPtr();
            } else if (getSuffixPtr()->isRoot()){
#ifdef DEBUG
                std::cout << "\tВершина по суффикс-ссылке - корень." << std::endl;
#endif
                terminalPtr = getSuffixPtr();
            } else{
#ifdef DEBUG
                std::cout << "\tПереход по терминальной ссылке суффикс-ссылки."<< std::endl;
#endif
                terminalPtr = getSuffixPtr()->getTerminalPtr();
            }

        } else{
#ifdef DEBUG
            std::cout << "\tТерминальная ссылка уже вычислялась." << std::endl;
#endif
        }

        return terminalPtr;

    }

    // Вычисление ссылки, в которую нужно перейти по переданному символу.
    // Данная функция учитывает, что если прямого перехода по символу нет, то требуется перейти по суффикс ссылке
    BorNode *deltaPtr(char c){

#ifdef DEBUG

        std::cout << "\tРассчет перехода по символу: " << c << std::endl;
        std::cout << "\tID текущей вершины: " << id << std::endl;

#endif

        // Проверяем возможность прямого перехода
        BorNode *child = children[c];

        if (child == nullptr){
#ifdef DEBUG

            std::cout << "\tНет сына по этому символу" << std::endl;

#endif
            if (isRoot()){
#ifdef DEBUG

                std::cout << "\tПопали в корень. Так как из него нет вершины, то остаемся тут." << std::endl;

#endif
                return this;
            }

#ifdef DEBUG

            std::cout << "\tИспользуем суффикс-ссылку в качестве промежуточной вершины." << std::endl;

#endif
            // Если нет прямого перехода, то переходим по суффикс-ссылке
            return getSuffixPtr()->deltaPtr(c);

        }

#ifdef DEBUG

        std::cout << "\tОбнаружен прямой путь. ID вершины: " << child->id << std::endl;

#endif

        return child;

    }

    // Является ли текущая вершина - терминальной
    bool isTerminal(){ return _isTerminal; }
    // Является ли текущая вершина еорнем
    bool isRoot(){ return parent == nullptr; }

    int getDepth(){ return depth; }
    char getParentThrough(){ return parentThrough; }
    std::map<char, BorNode*> &getChildren(){ return children; }
    std::vector<int> &getNeedles(){ return _needles; }

    void forEach(std::function<void(BorNode*)> f){

        f(this);
        for (auto child: children){
            if (child.second != nullptr)
                child.second->forEach(f);
        }

    }

#ifdef DEBUG
    int id;
#endif
};

int main() {

    std::string text;
    std::string needle;
    // Найденные совпадения в формате <индекс начала, индекс конца>
    std::vector<std::pair<int, int>> ans;

    auto *root = new BorNode();

    std::cin >> text;

    int needleCount;
    std::cin >> needleCount;

#ifdef DEBUG
    std::cout << "Построение бора: " << std::endl;
#endif
    for (int needleIndex=0; needleIndex<needleCount; needleIndex++){
        std::cin >> needle;
#ifdef DEBUG
        std::cout << "Добавление строки " << needle << std::endl;
#endif
        root->addNeedle(needle, needleIndex);
    }

#ifdef DEBUG
    std::cout << "Вывод построенного бора: " << std::endl;
    root->forEach([](BorNode *now){
        std::cout << std::endl;
        std::cout << "Вершина с ID: " << now->id << std::endl;
        std::cout << "\tТерминальная: " << now->isTerminal() << std::endl;
        std::cout << "\tМожно попасть в: " << std::endl;
        for (auto child: now->getChildren()){
            std::cout << "\t\t" << "В вершину с ID " << child.second->id << " через символ " << child.first << std::endl;
        }

    });
#endif

#ifdef DEBUG

    std::cout << "Поиск шаблона в тексте: " << std::endl;

#endif

    BorNode *now = root;
    // Проходим по бору в зависимости от приходящих символов
    for (int i=0; i<text.size(); i++){

        char c = text[i];

#ifdef DEBUG
        std::cout << std::endl;
        std::cout << "Текущий символ: " << c << std::endl;

#endif

        // Получаем вершину, в которую надо перейти
        now = now->deltaPtr(c);

#ifdef DEBUG

        std::cout << "Текущая вершина: " << now->id << std::endl;

#endif

        // Если она терминальня, то нашли одно из совпадений
        if (now->isTerminal()){

            for (auto needleIndex: now->getNeedles()) {
#ifdef DEBUG
                std::cout << "Обнаружено совпадение: ";
#endif
                std::cout << i - now->getDepth() + 2 << " " << needleIndex + 1 << std::endl;
                ans.emplace_back(i - now->getDepth() + 2, i);
            }

#ifdef DEBUG

            std::cout << "Попали в терминальную вершину. Начинаем обход терминальных ссылок." << std::endl;

#endif

        }

        // Просмотр терминальных ссылок
        BorNode *terminalNext = now->getTerminalPtr();
        // До тех пор, пока не попадем в корень
        while (!terminalNext->isRoot()){

#ifdef DEBUG

            std::cout << "Найдено подслово по терминальной ссылке. ID терминальной вершины: " << terminalNext->id << std::endl;

#endif
            for (auto needleIndex: terminalNext->getNeedles()) {
#ifdef DEBUG
                std::cout << "Обнаружено совпадение: ";
#endif
                std::cout << i - terminalNext->getDepth() + 2 << " " << needleIndex + 1 << std::endl;
                ans.emplace_back(i - terminalNext->getDepth() + 2, i);
            }
            terminalNext = terminalNext->getTerminalPtr();

        }

    }

    // Поиск максимального числа выходящих ребер бора
    int maxOut = 0;

    root->forEach([&maxOut](BorNode *now){

        int nowOut = 0;
        for (auto child: now->getChildren()){
            if (child.second != nullptr) nowOut++;
        }

        if (maxOut < nowOut) maxOut = nowOut;

    });

    std::cout << "Максимальное число исходящие ребер: " << maxOut << std::endl;

    // Удаление вхождений. Так как могут быть пересечения, то отмечаем все символы, который должны быть удалены
    for (auto founded: ans){
        for (int i=founded.first-1; i<=founded.second; i++){
            text[i] = '#'; // Это символа нет в алфавите
        }
    }

    std::string erasedText;
    for (auto c: text){

        if (c != '#')
            erasedText += c;

    }

    std::cout << "Текст с вырезанными подстроками: " << erasedText << std::endl;


    return 0;
}
