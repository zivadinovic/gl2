

/*
generic language (2), version 1;

written by zivadinovic;
gl2 host compiler;
h1 5/2026 h2 5/2026

filename - main.cc
*/

#include <iostream>
#include <string>
#include <string_view>
#include <cstdlib>
#include <vector>
#include <cctype>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <array>
#include <cstdint>

static void error(std::string_view msg, std::string_view add = "") noexcept
{
    add.empty() ? std::cerr << "error: " << msg << '\n' : std::cerr << "error: " << msg << ", with " << add << '\n';
    std::exit(1);
}

enum class tokentype
{
    identifier,
    number,
    string,
    assign,
    neg,
    equals,
    less,
    greater,
    noteq,
    lesseq,
    greatereq,
    lparen,
    rparen,
    lbrace,
    rbrace,
    lbracket,
    rbracket,
    comma,
    semi,
    plus,
    minus,
    star,
    slash,
    at,
    kw_fun,
    kw_i64,
    kw_if,
    kw_while,
    kw_return,
    kw_asm
};

struct token
{
    tokentype type;
    std::string value;
};

static std::vector<token> tokenize(const std::string &content) noexcept
{
    std::vector<token> tokens;
    std::size_t index = 0;

    while (index < content.size())
    {
        if (std::isspace(static_cast<unsigned char>(content[index])))
        {
            index++;
            continue;
        }
        else if (std::isdigit(static_cast<unsigned char>(content[index])))
        {
            std::string number;

            while (std::isdigit(static_cast<unsigned char>(content[index])))
            {
                number.push_back(content[index]);
                index++;
            }

            tokens.push_back({ tokentype::number, number });
        }
        else if (content[index] == '\"')
        {
            std::string string;

            index++;
            while (content[index] != '\"')
            {
                string.push_back(content[index]);
                index++;
            }
            index++;

            tokens.push_back({ tokentype::string, string });
        }
        else if (std::isalpha(static_cast<unsigned char>(content[index])) || content[index] == '_')
        {
            std::string identifier;

            while (std::isalnum(static_cast<unsigned char>(content[index])) || content[index] == '_')
            {
                identifier.push_back(content[index]);
                index++;
            }

            if (identifier == "i64")
            {
                tokens.push_back({ tokentype::kw_i64, identifier });
            }
            else if (identifier == "fun")
            {
                tokens.push_back({ tokentype::kw_fun, identifier });
            }
            else if (identifier == "asm")
            {
                tokens.push_back({ tokentype::kw_asm, identifier });
            }
            else if (identifier == "return")
            {
                tokens.push_back({ tokentype::kw_return, identifier });
            }
            else if (identifier == "if")
            {
                tokens.push_back({ tokentype::kw_if, identifier });
            }
            else if (identifier == "while")
            {
                tokens.push_back({ tokentype::kw_while, identifier });
            }
            else
            {
                tokens.push_back({ tokentype::identifier, identifier });
            }
        }
        else if (static_cast<unsigned char>(content[index]) == '=')
        {
            if (index + 1 < content.size() && static_cast<unsigned char>(content[index + 1] == '='))
            {
                tokens.push_back({ tokentype::equals, "==" });
                index += 2;
            }
            else
            {
                tokens.push_back({ tokentype::assign, "=" });
                index++;
            }
        }
        else if (static_cast<unsigned char>(content[index] == '!'))
        {
            if (index + 1 < content.size() && static_cast<unsigned char>(content[index + 1] == '='))
            {
                tokens.push_back({ tokentype::noteq, "!=" });
                index += 2;
            }
            else
            {
                tokens.push_back({ tokentype::neg, "!" });
                index++;
            }
        }
        else if (static_cast<unsigned char>(content[index] == '<'))
        {
            if (index + 1 < content.size() && static_cast<unsigned char>(content[index + 1] == '='))
            {
                tokens.push_back({ tokentype::lesseq, "<=" });
                index += 2;
            }
            else
            {
                tokens.push_back({ tokentype::less, "<" });
                index++;
            }
        }
        else if (static_cast<unsigned char>(content[index] == '>'))
        {
            if (index + 1 < content.size() && static_cast<unsigned char>(content[index + 1] == '='))
            {
                tokens.push_back({ tokentype::greatereq, ">=" });
                index += 2;
            }
            else
            {
                tokens.push_back({ tokentype::greater, ">" });
                index++;
            }
        }
        else if (static_cast<unsigned char>(content[index] == '('))
        {
            tokens.push_back({ tokentype::lparen, "(" });
            index++;
        }
        else if (static_cast<unsigned char>(content[index] == ')'))
        {
            tokens.push_back({ tokentype::rparen, ")" });
            index++;
        }
        else if (static_cast<unsigned char>(content[index] == '{'))
        {
            tokens.push_back({ tokentype::lbrace, "{" });
            index++;
        }
        else if (static_cast<unsigned char>(content[index] == '}'))
        {
            tokens.push_back({ tokentype::rbrace, "}" });
            index++;
        }
        else if (static_cast<unsigned char>(content[index] == '['))
        {
            tokens.push_back({ tokentype::lbracket, "[" });
            index++;
        }
        else if (static_cast<unsigned char>(content[index] == ']'))
        {
            tokens.push_back({ tokentype::rbracket, "]" });
            index++;
        }
        else if (static_cast<unsigned char>(content[index] == ','))
        {
            tokens.push_back({ tokentype::comma, "," });
            index++;
        }
        else if (static_cast<unsigned char>(content[index] == ';'))
        {
            tokens.push_back({ tokentype::semi, ";" });
            index++;
        }
        else if (static_cast<unsigned char>(content[index] == '+'))
        {
            tokens.push_back({ tokentype::plus, "+" });
            index++;
        }
        else if (static_cast<unsigned char>(content[index] == '-'))
        {
            tokens.push_back({ tokentype::minus, "-" });
            index++;
        }
        else if (static_cast<unsigned char>(content[index] == '*'))
        {
            tokens.push_back({ tokentype::star, "*" });
            index++;
        }
        else if (static_cast<unsigned char>(content[index] == '/'))
        {
            tokens.push_back({ tokentype::slash, "/" });
            index++;
        }
        else if (static_cast<unsigned char>(content[index] == '@'))
        {
            tokens.push_back({ tokentype::at, "@" });
            index++;
        }
        else
        {
            error("bad token");
        }
    }

    return tokens;
}

static constexpr std::array<std::string_view, 6> fn_regs = { "%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9" };
static constinit std::vector<token> tokens = {};
static std::unordered_map<std::string, std::size_t> locals = {}; // name, offset
static constinit std::size_t token_index = 0, local_index = 0, label_index = 0, string_index = 0;
static std::stringstream data;
static constinit std::size_t flag_comparator = 0;
static constinit bool get_address_instead_value = false;

static int align_16(std::size_t s) noexcept;

static void parse_factor() noexcept, parse_term() noexcept, parse_expr() noexcept;
static void parse_comparison() noexcept;
static void parse_stmt() noexcept, parse_var() noexcept, parse_block() noexcept, parse_asm_block() noexcept, parse_func() noexcept;
static void parse_program() noexcept;

static token peek() noexcept, consume() noexcept;
static bool expect(tokentype t) noexcept, accept(tokentype t) noexcept;

static int align_16(std::size_t s) noexcept
{
    return (s + 15) & ~15;
}

static token peek() noexcept
{
    return tokens[token_index];
}

static token consume() noexcept
{
    return tokens[token_index++];
}

static bool expect(tokentype t) noexcept
{
    if (peek().type == t)
    {
        consume();
        return true;
    }

    error(peek().value);
    return false;
}

static bool accept(tokentype t) noexcept
{
    if (peek().type == t)
    {
        consume();
        return true;
    }

    return false;
}

static void parse_factor() noexcept
{
    if (accept(tokentype::number))
    {
        auto value = tokens[token_index - 1].value;
        std::cout << '\t' << "movq $" << value << ", %rax" << '\n';
        return;
    }

    if (accept(tokentype::string))
    {
        auto value = tokens[token_index - 1].value;
        std::size_t newlabel = string_index++;
        data << ".str" << newlabel << ':' << '\n';
        for (std::size_t i = 0; i < value.size(); i++)
            data << '\t' << ".quad " << static_cast<int>(value[i]) << '\n';
        data << '\t' << ".quad " << 0 << '\n';
        std::cout << '\t' << "leaq " << ".str" << newlabel << "(%rip)" << ", %rax" << '\n';
        return;
    }

    if (accept(tokentype::identifier))
    {
        auto id = tokens[token_index - 1].value;

        if (accept(tokentype::lparen))
        {
            int argc = 0;
            if (peek().type != tokentype::rparen)
            {
                do
                {
                    parse_expr();
                    std::cout << '\t' << "pushq %rax" << '\n';
                    argc++;
                } while (accept(tokentype::comma));
            }

            expect(tokentype::rparen);

            std::size_t args_stack = (argc > 6) ? (argc - 6) : 0;

            if ((args_stack * 8) % 16 != 0) std::cout << '\t' << "subq $8, %rsp" << '\n';

            for (int i = argc - 1; i >= 0; i--) if (i < 6) std::cout << '\t' << "popq " << fn_regs[i] << '\n';

            std::cout << '\t' << "callq " << id << '\n';

            if ((args_stack * 8) % 16 != 0) std::cout << '\t' << "addq $8, %rsp" << '\n';
        }
        else if (accept(tokentype::lbracket))
        {
            auto var = locals.find(id);

            std::cout << '\t' << "movq -" << var->second << "(%rbp), %rax" << '\n';
            std::cout << '\t' << "pushq %rax" << '\n';

            parse_expr(); // index -> rax

            expect(tokentype::rbracket);

            std::cout << '\t' << "popq %rcx" << '\n'; // base-pointer -> rcx

            if (get_address_instead_value)
                std::cout << '\t' << "leaq (%rcx, %rax, 8), %rax" << '\n';
            else
                std::cout << '\t' << "movq (%rcx, %rax, 8), %rax" << '\n';
        }
        else if (accept(tokentype::lbrace))
        {
            auto var = locals.find(id);

            std::cout << '\t' << "movq -" << var->second << "(%rbp), %rax" << '\n';
            std::cout << '\t' << "pushq %rax" << '\n';

            parse_expr();

            expect(tokentype::rbrace);

            std::cout << '\t' << "popq %rcx" << '\n';

            if (get_address_instead_value)
                std::cout << '\t' << "leaq (%rcx, %rax, 8), %rax" << '\n';
            else
                std::cout << '\t' << "movzbq (%rcx, %rax), %rax" << '\n';
        }
        else
        {
            auto var = locals.find(id);

            if (get_address_instead_value)
                std::cout << '\t' << "leaq -" << var->second << "(%rbp)" << ", %rax" << '\n';
            else
                std::cout << '\t' << "movq -" << var->second << "(%rbp)" << ", %rax" << '\n';
        }

        return;
    }

    if (accept(tokentype::lparen))
    {
        parse_expr();
        expect(tokentype::rparen);
        return;
    }

    if (accept(tokentype::minus))
    {
        parse_expr();
        std::cout << '\t' << "neg %rax" << '\n';
        return;
    }

    if (accept(tokentype::at))
    {
        auto oldflag = get_address_instead_value;

        get_address_instead_value = true ;

        parse_factor();

        get_address_instead_value = oldflag;

        return;
    }

    error("bad factor", tokens[token_index].value);
}

static void parse_term() noexcept
{
    parse_factor();

    while (peek().type == tokentype::star || peek().type == tokentype::slash)
    {
        auto op = consume().type;

        std::cout << '\t' << "pushq %rax" << '\n';

        parse_factor();

        std::cout << '\t' << "movq %rax, %rcx" << '\n';
        std::cout << '\t' << "popq %rax" << '\n';

        // RHS -> rax

        if (op == tokentype::star)
        {
            std::cout << '\t' << "imulq %rcx, %rax" << '\n';
        }
        else
        {
            std::cout << '\t' << "cqo" << '\n';
            std::cout << '\t' << "idivq %rcx" << '\n';
        }
    }
}

static void parse_expr() noexcept
{
    parse_term();

    while (peek().type == tokentype::plus || peek().type == tokentype::minus)
    {
        auto op = consume().type;

        std::cout << '\t' << "pushq %rax" << '\n';

        parse_term();

        std::cout << '\t' << "movq %rax, %rcx" << '\n';
        std::cout << '\t' << "popq %rax" << '\n';

        if (op == tokentype::plus)
        {
            std::cout << '\t' << "addq %rcx, %rax" << '\n';
        }
        else
        {
            std::cout << '\t' << "subq %rcx, %rax" << '\n';
        }
    }
}

static void parse_comparison() noexcept
{
    parse_expr();

    while (
    peek().type == tokentype::equals ||
    peek().type == tokentype::less ||
    peek().type == tokentype::greater ||
    peek().type == tokentype::lesseq ||
    peek().type == tokentype::greatereq ||
    peek().type == tokentype::noteq)
    {
        auto op = consume().type;

        std::cout << '\t' << "pushq %rax" << '\n';

        parse_expr();

        std::cout << '\t' << "movq %rax, %rcx" << '\n';
        std::cout << '\t' << "popq %rax" << '\n';

        if (op == tokentype::equals)
        {
            flag_comparator = 0;
        }
        else if (op == tokentype::less)
        {
            flag_comparator = 1;
        }
        else if (op == tokentype::greater)
        {
            flag_comparator = 2;
        }
        else if (op == tokentype::lesseq)
        {
            flag_comparator = 3;
        }
        else if (op == tokentype::greatereq)
        {
            flag_comparator = 4;
        }
        else if (op == tokentype::noteq)
        {
            flag_comparator = 5;
        }

        std::cout << '\t' << "cmpq %rcx, %rax" << '\n';
    }
}

static void parse_stmt() noexcept
{
    if (accept(tokentype::identifier))
    {
        auto id = tokens[token_index - 1].value;

        if (accept(tokentype::assign))
        {
            parse_expr();
            expect(tokentype::semi);

            auto var = locals.find(id);
            std::cout << '\t' << "movq %rax, -" << var->second << "(%rbp)" << '\n';
        }
        else if (accept(tokentype::lparen))
        {
            int argc = 0;

            if (peek().type != tokentype::rparen)
            {
                do
                {
                    parse_expr();
                    std::cout << '\t' << "pushq %rax" << '\n';
                    argc++;
                } while (accept(tokentype::comma));
            }

            expect(tokentype::rparen);
            expect(tokentype::semi);

            for (int i = argc - 1; i >= 0; i--)
            {
                if (i < 6) std::cout << '\t' << "popq " << fn_regs[i] << '\n';
            }

            std::cout << '\t' << "callq " << id << '\n';
        }
        else if (accept(tokentype::lbracket))
        {
            auto var = locals.find(id);

            std::cout << '\t' << "movq -" << var->second << "(%rbp), %rax" << '\n';
            std::cout << '\t' << "pushq %rax" << '\n';

            parse_expr();

            expect(tokentype::rbracket);

            std::cout << '\t' << "popq %rcx" << '\n';
            std::cout << '\t' << "leaq (%rcx, %rax, 8), %rax" << '\n';
            std::cout << '\t' << "pushq %rax" << '\n';

            expect(tokentype::assign);

            parse_expr();

            std::cout << '\t' << "popq %rcx" << '\n';
            std::cout << '\t' << "movq %rax, (%rcx)" << '\n';

            expect(tokentype::semi);
        }
        else if (accept(tokentype::lbrace))
        {
            auto var = locals.find(id);

            std::cout << '\t' << "movq -" << var->second << "(%rbp), %rax" << '\n';
            std::cout << '\t' << "pushq %rax" << '\n';

            parse_expr();

            expect(tokentype::rbrace);

            std::cout << '\t' << "popq %rcx" << '\n';
            std::cout << '\t' << "leaq (%rcx, %rax), %rax" << '\n';
            std::cout << '\t' << "pushq %rax" << '\n';

            expect(tokentype::assign);

            parse_expr();

            std::cout << '\t' << "popq %rcx" << '\n';
            std::cout << '\t' << "movb %al, (%rcx)" << '\n';

            expect(tokentype::semi);
        }
        else
        {
            error("bad identifier pattern");
        }
    }

    if (accept(tokentype::kw_if))
    {
        std::size_t newlabel = label_index++;

        expect(tokentype::lparen);
        parse_comparison();
        expect(tokentype::rparen);

        // negations of logic to jump over
        if (flag_comparator == 0)
        {
            std::cout << '\t' << "jne " << ".l" << newlabel << '\n';
        }
        else if (flag_comparator == 1)
        {
            std::cout << '\t' << "jge " << ".l" << newlabel << '\n';
        }
        else if (flag_comparator == 2)
        {
            std::cout << '\t' << "jle " << ".l" << newlabel << '\n';
        }
        else if (flag_comparator == 3)
        {
            std::cout << '\t' << "jg " << ".l" << newlabel << '\n';
        }
        else if (flag_comparator == 4)
        {
            std::cout << '\t' << "jl " << ".l" << newlabel << '\n';
        }
        else if (flag_comparator == 5)
        {
            std::cout << '\t' << "je " << ".l" << newlabel << '\n';
        }

        parse_block();

        std::cout << ".l" << newlabel << ':' << '\n';
    }

    if (accept(tokentype::kw_while))
    {
        std::size_t startlabel = label_index++;
        std::size_t endlabel = label_index++;

        std::cout << ".l" << startlabel << ':' << '\n';

        expect(tokentype::lparen);
        parse_comparison();
        expect(tokentype::rparen);

        // negations of logic to jump over
        if (flag_comparator == 0)
        {
            std::cout << '\t' << "jne " << ".l" << endlabel << '\n';
        }
        else if (flag_comparator == 1)
        {
            std::cout << '\t' << "jge " << ".l" << endlabel << '\n';
        }
        else if (flag_comparator == 2)
        {
            std::cout << '\t' << "jle " << ".l" << endlabel << '\n';
        }
        else if (flag_comparator == 3)
        {
            std::cout << '\t' << "jg " << ".l" << endlabel << '\n';
        }
        else if (flag_comparator == 4)
        {
            std::cout << '\t' << "jl " << ".l" << endlabel << '\n';
        }
        else if (flag_comparator == 5)
        {
            std::cout << '\t' << "je " << ".l" << endlabel << '\n';
        }

        parse_block();

        std::cout << '\t' << "jmp " << ".l" << startlabel << '\n';

        std::cout << ".l" << endlabel << ':' << '\n';
    }

    if (accept(tokentype::kw_return))
    {
        if (!accept(tokentype::semi))
        {
            parse_expr();
            expect(tokentype::semi);
        }

        std::cout << '\t' << "leave" << '\n' << '\t' << "retq" << '\n';
    }

    if (accept(tokentype::kw_asm))
    {
        parse_asm_block();
    }
}

static void parse_var() noexcept
{
    if (accept(tokentype::kw_i64))
    {
        do
        {
            expect(tokentype::identifier);

            auto id = tokens[token_index - 1].value;

            local_index += 8;
            locals[id] = local_index;

            expect(tokentype::assign);

            parse_expr();

            std::cout << '\t' << "movq %rax, -" << local_index << "(%rbp)" << '\n';
        } while (accept(tokentype::comma));

        expect(tokentype::semi);
    }
}

static void parse_block() noexcept
{
    expect(tokentype::lbrace);

    while (peek().type != tokentype::rbrace)
    {
        if (peek().type == tokentype::kw_i64)
        {
            parse_var();
        }
        else
        {
            parse_stmt();
        }
    }

    expect(tokentype::rbrace);
}

static void parse_asm_block() noexcept
{
    expect(tokentype::lbrace);
    
    while (peek().type != tokentype::rbrace)
    {
        expect(tokentype::string);

        std::string str = tokens[token_index - 1].value, newstr;

        std::size_t i = 0;
        while (i < str.size())
        {
            if ((i + 1 < str.size()) && (str[i] == '\\' && str[i + 1] == 'n'))
            {
                newstr += '\n';
                i = i + 2;
                continue;
            }
            if ((i + 1 < str.size()) && (str[i] == '\\' && str[i + 1] == 't'))
            {
                newstr += '\t';
                i = i + 2;
                continue;
            }

            newstr += str[i];
            i = i + 1;
        }

        if (!newstr.empty()) std::cout << newstr;
    }

    expect(tokentype::rbrace);
}

static void parse_func() noexcept
{
    expect(tokentype::kw_fun);
    expect(tokentype::identifier);

    auto id = tokens[token_index - 1].value;
    std::stringstream args, body;
    int argc = 0;

    expect(tokentype::lparen);

    if (!accept(tokentype::rparen))
    {
        do
        {
            expect(tokentype::kw_i64);
            expect(tokentype::identifier);

            auto arg = tokens[token_index - 1].value;

            local_index += 8;
            locals[arg] = local_index;

            args << '\t' << "movq " << fn_regs[argc++] << ", -" << local_index << "(%rbp)" << '\n';
        } while (accept(tokentype::comma));

        expect(tokentype::rparen);
    }

    auto oldbuf = std::cout.rdbuf(body.rdbuf());
    parse_block();
    std::cout.rdbuf(oldbuf);

    std::cout << id << ':' << '\n';
    std::cout << '\t' << "pushq %rbp" << '\n';
    std::cout << '\t' << "movq %rsp, %rbp" << '\n';

    std::size_t total_index = align_16(local_index);

    std::cout << '\t' << "subq $" << total_index << ", %rsp" << '\n';

    std::cout << args.str();
    std::cout << body.str();

    locals.clear();
    local_index = 0;
}

static void parse_program() noexcept
{
    while (token_index < tokens.size())
    {
        if (peek().type == tokentype::kw_fun)
        {
            parse_func();
        }
        else if (peek().type == tokentype::kw_asm)
        {
            consume();
            parse_asm_block();
        }
        else
        {
            error("expected `fun' or `asm'");
        }
    }

    std::cout << data.str();
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        return 1;
    }

    std::fstream input(argv[1], std::ios::in);

    std::istreambuf_iterator<char> beg(input), end;
    std::string content(beg, end);

    tokens = tokenize(content);

    parse_program();

    return 0;
}
