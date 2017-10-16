#include <math.h>
#include <expreval.h>
#include "compiler_helper.h"

static compiler init_compiler();

cfunction0 compile0(const char *expression)
{
    compiler c = init_compiler();

    cfunction0 f = compiler_CompileFunction(c, (char*)expression);

    compiler_Free(c);

    return f;
}

cfunction1 compile1(const char *expression)
{
    compiler c = init_compiler();
    compiler_MapArgument(c, "i", 0);

    cfunction1 f = compiler_CompileFunction(c, (char*)expression);

    compiler_Free(c);

    return f;
}

static compiler init_compiler()
{
    compiler c = compiler_New();
    compiler_RegisterFunction(c, "sin", sin, 1);
    compiler_RegisterFunction(c, "asin", asin, 1);
    compiler_RegisterFunction(c, "cos", cos, 1);
    compiler_RegisterFunction(c, "acos", acos, 1);
    compiler_RegisterFunction(c, "tan", tan, 1);
    compiler_RegisterFunction(c, "atan", atan, 1);
    compiler_RegisterFunction(c, "pow", pow, 2);
    compiler_RegisterFunction(c, "sqrt", sqrt, 1);
    compiler_SetConstant(c, "PI", 3.1415926535897932384626);

    return c;
}
