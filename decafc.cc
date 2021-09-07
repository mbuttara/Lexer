#include <fstream>
#include "parsetree.h"
#include "semantics.h"
#include "codegen.h"


// prototypes:
extern FILE *yyin;
int yylex(); // this is only needed for the --lex option I think.
void main_lex();
void main_tree();
void main_sem();
void main_gen();

// Globals controlling behavior:
bool semantic_checks = false;

// type to represent the user's choice: lexical analysis, tree generation,
// semantic checks, code generation.
enum action {LEX, TREE, SEM, GEN};

// CITE: https://docs.microsoft.com/en-us/troubleshoot/cpp/
//               declare-pointers-to-functions
// DESC: How to declare an array of pointers to functions
void (*functions[])() = {main_lex, main_tree, main_sem, main_gen};

void usage(char **argv, int code = 1)
{
   std::cerr << "Usage: " << argv[0] << " [options] file" << std::endl;
   std::cerr << "Options:" << std::endl <<
"  --help                   Display this usage information" << std::endl <<
"  --lex                    Display the tokens" << std::endl <<
"  --tree                   Show a traversal of the parse tree" << std::endl <<
"  --sem                    Perform semantic checks" << std::endl <<
"  --gen (default)          Generate JVM assembly code" << std::endl;
   std::cerr << "At most one of --lex, --tree, --sem, or --gen may be given."
             << std::endl;
   exit(code);
}

int main(int argc, char **argv)
{
   action act = GEN;
   bool act_given = false;
   std::string filename = "";
   if (argc < 2) usage(argv);

   // process command line arguments
   for (int i = 1; i < argc; i++)
   {
      std::string arg = argv[i];

      // if you give an action, it has to be legit.
      if (   arg.size() >= 2 && arg.substr(0,2) == "--"
          && arg != "--help" && arg != "--lex" && arg != "--tree"
          && arg != "--sem" && arg != "--gen")
      {
         usage(argv);
      }
      // You can only give one action option.
      if (act_given && (   arg == "--lex" || arg == "--tree"
                        || arg == "--sem" || arg == "--geb"))
      {
         usage(argv);
      }
      if (arg == "--help") usage(argv, 0);
      else if (arg == "--lex") act = LEX, act_given = true;
      else if (arg == "--tree") act = TREE, act_given = true;
      else if (arg == "--sem") act = SEM, act_given = true;
      else if (arg == "--gen") act_given = true;
      else act_given = true, filename = arg;
   }

   // error if a filename wasn't given
   if (filename == "") usage(argv);

   // open the file, error if couldn't open.
   yyin = fopen(filename.c_str(), "r");
   if (!yyin)
   {
      std::cout << filename
           << ": no such file or file can't be opened for reading."
           << std::endl;
      exit(1);
   }

   // determine the program_name from the filename
   // (for code generation)
   size_t pos = filename.find_last_of('.');
   program_name = pos == std::string::npos ? filename : filename.substr(0,pos);

   // do the chosen function
   functions[act]();
}

void main_lex()
{
   current_line = 1;
   while (yylex())
   {
      std::cout << *mytok << std::endl;
      delete mytok;
   }
}

void main_tree()
{
   current_line = 1;
   yyparse();
   traverse_tree(top);
}

void main_sem()
{
   semantic_checks = true;
   current_line = 1;
   current_scope = nullptr;
   open_scope();

   // add in primive types
   current_scope->add("double", semantics_double_type);
   current_scope->add("int", semantics_int_type);
   open_scope();  // this would allow redefinition of built-ins.
                  // (Not possible in our grammars)
   yyparse();
   traverse_tree(top);
}

void main_gen()
{
   main_sem();
   std::string fname = program_name + ".j";
   gout = new std::ofstream(fname.c_str());
   if (!*gout)
   {
      std::cerr << "Can't open " << fname << " for writing" << std::endl;
      exit(1);
   }
   std::cerr << "Going to preamble" << std::endl;
   gen_preamble();

   delete gout;
}
