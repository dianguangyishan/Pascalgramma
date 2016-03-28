#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "trie.h"
trie_tree_t *trie;
void tokenIdentify(char *token, FILE *fout);
int main(int argc, char* argv[])
{
    FILE *fin = NULL;
    FILE *fout = NULL;
    char ch = 0;
    int dot = 0;
    char buff[256] ;
    char last=0;
    int nbuff = 0;
    int i = 0;
    char *filename = argv[1];
    //char filename2[100] = "testout.txt";
    char filename2[100];
    memset(filename2,0,100);
    if(filename == NULL)
    {
        filename = "testin.txt";
        strcpy(filename2, "testout.txt");
    }
    else
        for (i=strlen(filename);i>0;i--)
        {
            if(filename[i] == '\\')
            {
                strncpy(filename2,filename,i+1);
                strcat(filename2,"testout.txt");
                break;
            }

        }
    trie = trie_tree_create();
    memset(buff, 0, 255);
    if(!(fin = fopen(filename,"r")))
        printf("open file %s failed",filename);
    if(!(fout = fopen(filename2,"w")))
        printf("open output file failed");
    //if(!(fitem = fopen("fileitem.txt","w")))
    //    printf("open output file failed");
    while((ch = getc(fin)) != EOF)
    {
        if(ch == '\'')
        {
            while((ch = getc(fin)) != '\'')
            {
                buff[nbuff] = ch;
                nbuff++;
            }
            last=buff[nbuff-1];
            buff[nbuff] = 0;
            nbuff = 0;
            fprintf(fout,"(string, '%s')\n",buff);
        }
        else if(isalpha(ch))//标识符
        {
            do
            {
                buff[nbuff] = ch;
                nbuff++;
                ch = getc(fin);
            }while(isalpha(ch) || isdigit(ch));
            if(ch != EOF)fseek(fin, -1, SEEK_CUR);
            last=buff[nbuff-1];
            buff[nbuff] = 0;
            nbuff = 0;
            tokenIdentify(buff,fout);
        }
        else if(isdigit(ch))
        {
            do
            {
                buff[nbuff] = ch;
                nbuff++;
                ch = getc(fin);
            }while(isdigit(ch));
            if(ch == '.')
            {
                dot = 1;
                ch = getc(fin);
                if(ch == '.')
                {
                    dot = 0;
                    fseek(fin, -1, SEEK_CUR);
                }
                else
                {
                    buff[nbuff] = '.';
                    nbuff++;
                    do
                        {
                            buff[nbuff] = ch;
                            nbuff++;
                            ch = getc(fin);
                        }while(isdigit(ch));
                }
            }
            while(ch =='e')
            {
                buff[nbuff] = ch;
                nbuff ++;
                ch = getc(fin);
                if(ch == '+'||ch == '-'||isdigit(ch))
                {
                    do
                    {
                        buff[nbuff] = ch;
                        nbuff++;
                        ch = getc(fin);
                    }while(isdigit(ch));
                }
            }
            if(ch != EOF)fseek(fin, -1, SEEK_CUR);
            last=buff[nbuff-1];
            buff[nbuff] = 0;
            nbuff = 0;
            if(dot == 1)
            {
                fprintf(fout,"(real, %s)\n",buff);
                dot = 0;
            }
            else
            {
                fprintf(fout,"(inter, %s)\n",buff);
            }
        }
        else
        {
            switch(ch)
            {
            case '+':
                if(isdigit(last) || isalpha(last))
                    fprintf(fout,"(+, +)\n");
                else
                    fprintf(fout,"(+1, +)\n");
                break;
            case '-':
                if(isdigit(last) || isalpha(last))
                    fprintf(fout,"(-, -)\n");
                else
                    fprintf(fout,"(-1, -)\n");
                break;
            case '*':
                fprintf(fout,"(*, *)\n");
                break;
            case '/':
                fprintf(fout,"(/, /)\n");
                break;
            case '=':
                fprintf(fout,"(=, =)\n");
                break;
            case '<':
                ch = getc(fin);
                switch(ch)
                {
                case '=':
                    last = '=';
                    fprintf(fout,"(<=, <=)\n");
                    break;
                case '>':
                    last = '>';
                    fprintf(fout,"(<>, <>)\n");
                    break;
                default:
                    last = '<';
                    fprintf(fout,"(<, <)\n");
                    if(ch != EOF)fseek(fin, -1, SEEK_CUR);
                    break;
                }
            case '>':
                ch = getc(fin);
                switch(ch)
                {
                case '=':
                    last = '=';
                    fprintf(fout,"(<=, <=)\n");
                    break;
                default:
                    last= '>';
                    fprintf(fout,"(>, >)\n");
                    if(ch != EOF)fseek(fin, -1, SEEK_CUR);
                    break;
                }
            case ',':
                last = ',';
                fprintf(fout, "(,, ,)\n");
                break;
            case ':':
                ch = getc(fin);
                switch(ch)
                {
                case '=':
                    last = '^';
                    fprintf(fout,"(:=, :=)\n");
                    break;
                default:
                    last= ':';
                    fprintf(fout,"(:, :)\n");
                    if(ch != EOF)fseek(fin, -1, SEEK_CUR);
                    break;
                }
                break;
            case '^':
                last = '^';
                fprintf(fout,"(^, ^)\n");
                break;
            case '(':
            case ')':
            case '[':
            case ']':
            case ';':
                last = ch;
                fprintf(fout, "(%c, %c)\n",ch,ch);
                break;
            case '{':
                while((ch = getc(fin)) != '}');
                if(ch != EOF)fseek(fin, -1, SEEK_CUR);
                break;
            case '.':
                ch = getc(fin);
                switch(ch)
                {
                case '.':
                    fprintf(fout, "(.., ..)\n");
                    last = '.';
                    break;
                default:
                    fprintf(fout, "(., .)\n");
                    last = '.';
                    if(ch != EOF)fseek(fin, -1, SEEK_CUR);
                    break;
                }
                break;
            default:
                break;
            }
        }
    }
    fclose(fin);
    fclose(fout);
    printf("\n%d,%d\n", trie->size,trie->capacity);
    free(trie);
    system("pause");
    return 0;
}
void tokenIdentify(char *token,FILE *fout)
{
    if(!strcmp(token, "var"))fprintf(fout,"(var, %s)\n",token);
    else if(!strcasecmp(token, "string"))fprintf(fout,"(string, %s)\n",token);
    else if(!strcasecmp(token, "type"))fprintf(fout,"(type, %s)\n",token);
    else if(!strcasecmp(token, "array"))fprintf(fout,"(array, %s)\n",token);
    else if(!strcasecmp(token, "for"))fprintf(fout,"(for, %s)\n",token);
    else if(!strcasecmp(token, "if"))fprintf(fout,"(if, %s)\n",token);
    else if(!strcasecmp(token, "while"))fprintf(fout,"(while, %s)\n",token);
    else if(!strcasecmp(token, "with"))fprintf(fout,"(with, %s)\n",token);
    else if(!strcasecmp(token, "until"))fprintf(fout,"(until, %s)\n",token);
    else if(!strcasecmp(token, "to"))fprintf(fout,"(to, %s)\n",token);
    else if(!strcasecmp(token, "set"))fprintf(fout,"(set, %s)\n",token);
    else if(!strcasecmp(token, "then"))fprintf(fout,"(then, %s)\n",token);
    else if(!strcasecmp(token, "program"))fprintf(fout,"(program, %s)\n",token);
    else if(!strcasecmp(token, "record"))fprintf(fout,"(record, %s)\n",token);
    else if(!strcasecmp(token, "repeat"))fprintf(fout,"(repeat, %s)\n",token);
    else if(!strcasecmp(token, "packed"))fprintf(fout,"(packed, %s)\n",token);
    else if(!strcasecmp(token, "of"))fprintf(fout,"(of, %s)\n",token);
    else if(!strcasecmp(token, "not"))fprintf(fout,"(not, %s)\n",token);
    else if(!strcasecmp(token, "nil"))fprintf(fout,"(nil, %s)\n",token);
    else if(!strcasecmp(token, "procedure"))fprintf(fout,"(procedure, %s)\n",token);
    else if(!strcasecmp(token, "file"))fprintf(fout,"(file, %s)\n",token);
    else if(!strcasecmp(token, "label"))fprintf(fout,"(label, %s)\n",token);
    else if(!strcasecmp(token, "goto"))fprintf(fout,"(goto, %s)\n",token);
    else if(!strcasecmp(token, "function"))fprintf(fout,"(function, %s)\n",token);
    else if(!strcasecmp(token, "else"))fprintf(fout,"(else, %s)\n",token);
    else if(!strcasecmp(token, "downto"))fprintf(fout,"(downto, %s)\n",token);
    else if(!strcasecmp(token, "case"))fprintf(fout,"(case, %s)\n",token);
    else if(!strcasecmp(token, "const"))fprintf(fout,"(const, %s)\n",token);
    else if(!strcasecmp(token, "end"))fprintf(fout,"(end, %s)\n",token);
    else if(!strcasecmp(token, "mod"))fprintf(fout,"(mod, %s)\n",token);
    else if(!strcasecmp(token, "and"))fprintf(fout,"(and, %s)\n",token);
    else if(!strcasecmp(token, "div"))fprintf(fout,"(div, %s)\n",token);
    else if(!strcasecmp(token, "or"))fprintf(fout,"(or, %s)\n",token);
    else if(!strcasecmp(token, "in"))fprintf(fout,"(in, %s)\n",token);
    else if(!strcasecmp(token, "uses"))fprintf(fout,"(uses, %s)\n",token);
    else
    {
        fprintf(fout,"(indentifier, %s)\n",token);
        trie_tree_insert(trie, token);
    }
}
