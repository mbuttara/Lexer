#include <iostream>
#include <cstdarg>

#include "codegen.h"

std::ostream *gout;
std::string program_name;

void codegen_error(char const * fmt, ...)
{
   va_list args;
   va_start(args, fmt);
   vfprintf(stderr, fmt, args);
   std::cerr << std::endl;
   va_end(args);
   exit(1);
}


void gen_binop(parse_tree *tree)
{
   parse_tree *op = tree->children[0];
   parse_tree *left = tree->children[1];
   parse_tree *right = tree->children[2];
   std::string typestr = tree->type->to_string();
   gen_expr(left);
   gen_expr(right);
   codegen_error("Can't generate binary operator %s tree: %s",
                  op->to_string().c_str(),
                 tree->to_string().c_str());

}

void gen_expr(parse_tree *tree)
{
   // deleted stuff. This whole file is useless.
}

void gen_assign(parse_tree *tree)
{
   parse_tree *lval = tree->children[0];
   parse_tree *rval = tree->children[1];
   gen_expr(rval);
   codegen_error("Not sure where to go from here with %s and %s",
                 lval->to_string().c_str(),
                 rval->to_string().c_str());
}

void gen_global_var(parse_tree *tree)
{
   // something happens here.
}

void gen_preamble()
{
   (*gout) << ".class      public " << program_name << std::endl;
   (*gout) << ".super      java/lang/Object " << std::endl;
   (*gout) << "" << std::endl;
}

void gen_init_boilerplate()
{
   (*gout) << ".method                  public <init>()V" << std::endl
         << "   .limit stack          1" << std::endl
         << "   .limit locals         1" << std::endl
         << "   aload_0" << std::endl
         << "   invokespecial         java/lang/Object/<init>()V" << std::endl
         << "   return" << std::endl
         << ".end method" << std::endl;
}

void gen_main_block(parse_tree *tree)
{

}
