#include <iostream>
#include <fstream>
#include <string>
//  Исходными данные для программы : test.cpp - в папке

// однострочный коммент

bool handleLineComment(std::ifstream & in)
{
    char c;
    while (in.get(c))
        if (c == '\n')
            return false;
    return true;
}

// многострочный комментарий

bool handleMultilineComment(std::ifstream & in)
{
    char c;
    while (in.get(c))
    {
        if (c == '*')
        {
            if (!in.get(c))
                break;
            if (c == '/')
                return false;
        }
    }
    return true;
}

//  >> содержимое строки в файловый поток

void flushLine(std::string & line, std::ofstream & out)
{
    static bool prevEmptyLine = false;
    
    for (size_t i = 0; i < line.length(); ++i)
    {
        if (!isspace(line[i]))
        {
            //  >> строку в файл
            prevEmptyLine = false;
            out.write(line.c_str(), line.length());
            line = "";
            return;
        }
    }
    // иначе если предыдущая строка не была пустой
    if (!prevEmptyLine)
    {
        // скидываем новую строку в файл и запоминаем
        // что мы скинули пустую строку
        out.put('\n');
        prevEmptyLine = true;
    }
    line = "";
}

// обрабатываем файл

void handleFile(std::string fileName)
{
    std::string line;
    
    // открываем исходный файл

    std::ifstream in(fileName.c_str());
    if (!in) return;
    
    
     // открываем выходной файл

    fileName += ".nocomments.cpp";
    std::ofstream out(fileName.c_str());
    if (!out) return;
    
    char c;
    bool inString = false, inChar = false;
    while (in.get(c))
    {
        if (inChar || inString || c != '/')
        {
            line += c;
            if (c == '\n') flushLine(line, out);
            else if (c == '\"') inString = !inString;
            else if (c == '\'') inChar = !inChar;
            continue;
        }
        
        if (!in.get(c))
            break;
        
        bool end = true;
        if (c == '/')
        {
            // обрабатываем однострочный комментарий
            /*—ÎÓÊÌ˚È ÍÓÏÂÌÚ‡ËÈ* */
            end = handleLineComment(in);
            line += '\n';
            flushLine(line, out);
        }
        else if (c == '*')
        {
            // обрабатываем многострочный комментарий
            end = handleMultilineComment(in);
            // добавляем пробел вместо многострочного комента
            line += ' ';
        }
        else
        {
            line += '/';
            line += c;
            continue;
        }
        
        if (end) break;
    }
    flushLine(line, out);
    
    // закрвывает открытые ранее файлы
    in.close();
    out.close();
}

//  точка программы

int main (int argc, char **argv)
{
    handleFile("test.cpp");
    return 0;
}
