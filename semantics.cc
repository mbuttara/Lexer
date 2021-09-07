#include <iostream>  // temporary
#include <cassert>
#include <cstdarg>
#include "semantics.h"
#include "token.h"

/* *** variables *** */

symtab *current_scope = nullptr;
semantics *current_function;
semantics *current_interface;
semantics *current_class;

void semantic_assert(bool condition, char const * fmt, ...)
{
   if (condition) return;
   std::cerr << "line " << current_line << ": semantic error, ";
   va_list args;
   va_start(args, fmt);
   vfprintf(stderr, fmt, args);
   std::cerr << std::endl;
   va_end(args);
   exit(1);
}

/* *** semantics constructors and instance methods *** */

s_var::s_var(std::string name, s_type *type) : name(name), type(type) {}

std::string s_var::to_string() const
{
   return name;
}

s_type::s_type(std::string name) : name(name) {}

s_prim::s_prim(std::string name) : s_type(name) {}

std::string s_prim::to_string() const
{
   return name;
}

/* *** Global types builtin. *** */
s_prim *semantics_int_type = new s_prim("int");
s_prim *semantics_double_type = new s_prim("double");

/* *** symtab *** */

symtab::symtab(symtab *outer) : outer(outer)
{
   static int seq = 1;
   name = "symtab-" + std::to_string(seq++);
}

std::string symtab::to_string() const
{
   return name;
}

semantics *symtab::lookup_local(std::string key) const
{
   // p has type iterator to map of string and semantics*.
   // auto is much easier here. Thank you C++11.
   auto p = table.find(key);
   return p == table.end() ? nullptr : p->second;
}

semantics *symtab::lookup(std::string key) const
{
   semantics *p = lookup_local(key);
   return     p ? p :
          outer ? outer->lookup(key) :
                  nullptr;
}

void symtab::add(std::string key, semantics *value)
{
   assert(!lookup_local(key));
   table[key] = value;
}

void symtab::replace(std::string key, semantics *value)
{
   assert(lookup_local(key));
   table[key] = value;
}

/* *** functions *** */

void open_scope()
{
   current_scope = new symtab(current_scope);
}

symtab *close_scope()
{
   symtab *old = current_scope;
   current_scope = current_scope->outer;
   return old;
}
