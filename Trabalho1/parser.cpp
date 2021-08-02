#include "parser.h"
#include "error.h"
#include <iostream>
#include <sstream>
#include <cctype>
using std::cin;
using std::cout;
using std::endl;
using std::stringstream;

void Parser::Program()
{
    // program -> math block
    if (!Match(Tag::MATH))
        throw SyntaxError(scanner.Lineno(), "\'math\' esperado");
    Block();
}

void Parser::Block()
{
    // block -> { decls stmts } 
    if (!Match('{'))
        throw SyntaxError(scanner.Lineno(), "\'{\' esperado");
    //else
        //cout << "{ ";    


    // nova tabela de símbolos para o bloco
    // ------------------------------------
    SymTable * saved = symtable;
    symtable = new SymTable(symtable);
    // ------------------------------------

    Decls();
    Stmts();

    if (!Match('}'))
        throw SyntaxError(scanner.Lineno(), "\'}\' esperado");
    //else
        //cout << "} ";
    

    // tabela do escopo envolvente volta a ser a tabela ativa
    // ------------------------------------------------------ 
    delete symtable;
    symtable = saved;
    // ------------------------------------------------------
}

void Parser::Decls()
{
    // decls -> decl decls
    //        | empty
    // decl  -> type id;

    while (lookahead->tag == Tag::TYPE)
    {
        string type {lookahead->toString()};
        Match(Tag::TYPE);

        string name {lookahead->toString()};
        Match(Tag::ID);

        Symbol s { name, type };
        
        // insere variável na tabela de símbolos
        if(!symtable->Insert(name, s))
        {
            // a inserção falha quando a variável já está na tabela
            stringstream ss;
            ss << "variável \"" << name << "\" já definida";
            throw SyntaxError(scanner.Lineno(), ss.str());    
        }

        if(!Match(';'))
        {
            stringstream ss;
            ss << "encontrado \'" << lookahead->toString() << "\' no lugar de ';'";
            throw SyntaxError { scanner.Lineno(), "esperado ;" };
        }
    }
}

void Parser::Stmts()
{
    // stmts -> stmt stmts
    //        | empty
    // stmt  -> block
    //        | fact;
    
    while(true)
    {
        switch(lookahead->tag)
        {
            // stmt -> block
            case '{': 
                Block(); 
                break;
            case '(': 
                Fact();
                if(!Match(';'))
                {
                    stringstream ss;
                    ss << "encontrado \'" << lookahead->toString() << "\' no lugar de ';'";
                    throw SyntaxError { scanner.Lineno(), ss.str() };
                } 
                cout << endl;
                break;
            // stmt -> fact; 
            case Tag::ID: 
                Fact();
                if(!Match(';'))
                {
                    stringstream ss;
                    ss << "encontrado \'" << lookahead->toString() << "\' no lugar de ';'";
                    throw SyntaxError { scanner.Lineno(), ss.str() };
                } 
                cout << endl;
                break;
            case Tag::INTEGRAL:
                Fact();
                if(!Match(';'))
                {
                    stringstream ss;
                    ss << "encontrado \'" << lookahead->toString() << "\' no lugar de ';'";
                    throw SyntaxError { scanner.Lineno(), ss.str() };
                } 
                cout << endl;
                break;
            case Tag::FLOATING:
                Fact();
                if(!Match(';'))
                {
                    stringstream ss;
                    ss << "encontrado \'" << lookahead->toString() << "\' no lugar de ';'";
                    throw SyntaxError { scanner.Lineno(), ss.str() };
                } 
                cout << endl;
                break;
            // stmts -> empty
            default: return;
        }
    }   
}

void Parser::Fact()
{
    // fact  -> term R
    // R -> + term R
    //       | - term R
    //       | empty

    Term();

    // função R()
    while (true)
    {

        // oper -> + term oper
        if (lookahead->tag == '+')
        {
            Match('+');
            Term();
            cout << '+';         
        }
        // oper -> - term oper
        else if (lookahead->tag == '-')
        {
            Match('-');
            Term();
            cout << '-';  
             
        }
        // oper -> empty
        else break; 
    }

    return;
}

void Parser::Term()
{
    // term -> tuts T
    // T -> * tuts T
    //       | / tuts T
    //       | empty

    Tuts();

    // função T()
    while (true)
    {

           // T -> * tuts T
        if (lookahead->tag == '*')
        {
            Match('*');
            Tuts();
            cout << '*';  
        } 
        // T -> / tuts T
        else if (lookahead->tag == '/')
        {
            Match('/');
            Tuts();
            cout << '/';
        }
        // calc -> empty
        else break;
    }

    return;
}


void Parser::Tuts()
{   /*
    if (!Match('('))
            throw SyntaxError(scanner.Lineno(), "\'(\' esperado");
        else
            cout << "( ";
        
    Fact();

    if (!Match(')'))
        throw SyntaxError(scanner.Lineno(), "\')\' esperado");
    else
        cout << ") ";
    */
    switch (lookahead->tag)
    {
    // factor -> (bool)
        case '(':   
        {  
            Match('(');
            Fact();
            if(!Match(')'))
            {
                stringstream ss;
                ss << "esperado ) no lugar de  \'" << lookahead->toString() << "\'";
                throw SyntaxError { scanner.Lineno(), ss.str() };
            }
            break;
        }
   
    case Tag::INTEGRAL:
    {
        cout << "(" << lookahead->toString() <<  ")";
        Match(Tag::INTEGRAL);
    }
        break;
    case Tag::FLOATING:
    {
        cout << "(" << lookahead->toString() <<  ")";
        Match(Tag::FLOATING);
    }
        break;
    case Tag::ID:
    {
        // verifica tipo da variável na tabela de símbolos
        Symbol * s = symtable->Find(lookahead->toString());
        if (!s)
        {
            stringstream ss;
            ss << "variável \"" << lookahead->toString() << "\" não declarada";
            throw SyntaxError{ scanner.Lineno(), ss.str() };
        }
        cout<<"(" << s->var << ':' << s->type << ") ";
        Match(Tag::ID);
    }
        break;
    default:
        stringstream ss;
        ss << '\'' << lookahead->toString() << "\' inválido na expressão";  
        throw SyntaxError{ scanner.Lineno(), ss.str() };
        break;
    }
    
    /*        
    if(lookahead->tag == Tag::INTEGRAL)
    {
        cout << lookahead->toString() << ':' << "int" << "; ";
        Match(Tag::INTEGRAL);
    }
    else if(lookahead->tag == Tag::FLOATING)
    {
        cout << lookahead->toString() << ':' << "float" << "; ";
        Match(Tag::FLOATING);
    }
    // fact -> id
    else if (lookahead->tag == Tag::ID)
    {      
        // verifica tipo da variável na tabela de símbolos
        Symbol * s = symtable->Find(lookahead->toString());
        if (!s)
        {
            stringstream ss;
            ss << "variável \"" << lookahead->toString() << "\" não declarada";
            throw SyntaxError{ scanner.Lineno(), ss.str() };
        }
        cout << s->var << ':' << s->type << "; ";
        Match(Tag::ID);
    }
    else
    {
        stringstream ss;
        ss << '\'' << lookahead->toString() << "\' inválido na expressão";  
        throw SyntaxError{ scanner.Lineno(), ss.str() };
    } */
}

bool Parser::Match(int tag)
{
    if (tag == lookahead->tag)
    {
        lookahead = scanner.Scan();
        return true;
    }
   
    return false;
}

Parser::Parser()
{
    lookahead = scanner.Scan();
    symtable = nullptr; 
}

void Parser::Start()
{
    Program();
}
