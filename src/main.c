/*!
@file main.c
@brief A simple test program for the C library code.
*/

#include "stdio.h"

#include "verilog_parser.h"
#include "verilog_ast_common.h"
#include "verilog_preprocessor.h"
#include "verilog_ast_util.h"

const char* port_direction_to_string(ast_port_direction direction);
const char* port_type_to_string(ast_net_type type);
const char* port_expression_type_to_string(ast_expression_type type);
const char* port_primary_type_to_string(ast_primary_type type);
const char* port_primary_value_type_to_string(ast_primary_value_type type);
const char* port_representation_to_string(ast_number_representation type);
void process_port(ast_port_declaration *port);
void process_expression(ast_expression *expr, int is_low);

int main(int argc, char ** argv)
{
    if(argc < 2)
    {
        printf("ERROR. Please supply at least one file path argument.\n");
        return 1;
    }
    else
    {

        int F = 0;
        for(F = 1; F < argc; F++)
        {
            
            // Initialise the parser.
            verilog_parser_init();

            // Setup the preprocessor to look in ./tests/ for include files.
            ast_list_append(yy_preproc -> search_dirs, "./tests/");
            ast_list_append(yy_preproc -> search_dirs, "./");
            printf("%s ", argv[F]);fflush(stdout);

            // Load the file.
            FILE * fh = fopen(argv[F], "r");

            verilog_preprocessor_set_file(yy_preproc, argv[F]);
            
            // Parse the file and store the result.
            int result = verilog_parse_file(fh);

            // Close the file handle
            fclose(fh);
            
            if(result == 0)
            {
                printf(" - Parse successful\n");
            }
            else
            {
                printf(" - Parse failed\n");
                if(argc<=2) return 1;
            }

            verilog_source_tree * ast = yy_verilog_source_tree;

            verilog_resolve_modules(ast);

            int m;
            for (m = 0; m < ast -> modules -> items; m++)
            {
                ast_module_declaration * module = ast_list_get(ast->modules,m);
                printf("Module %s\n", module->identifier->identifier);
                int n;

                for (n = 0; n < module->module_ports->items; n++) {
                    printf("number is %d \n", n);
                    ast_port_declaration * port = ast_list_get(module->module_ports,n);
                    process_port(port);
                    printf("\n");
                }
            }
            printf("Over!!\n");
        }
    }
    return 0;
}

const char* port_primary_value_type_to_string(ast_primary_value_type type) {
    switch (type)
    {
    case PRIMARY_NUMBER:
        return "number";
    case PRIMARY_IDENTIFIER:
        return "identifier";
    case PRIMARY_CONCATENATION:
        return "concatenation";
    case PRIMARY_FUNCTION_CALL:
        return "function call";
    case PRIMARY_MINMAX_EXP:
        return "minmax";
    case PRIMARY_MACRO_USAGE:
        return "macro usage";
    
    default:
        return "None";
    }
}

const char* port_primary_type_to_string(ast_primary_type type) {
    switch (type)
    {
    case CONSTANT_PRIMARY:
        return "constant primary";
    case PRIMARY:
        return "primary";
    case MODULE_PATH_PRIMARY:
        return "module path priamry";
    default:
        return "None";
    }
}

const char* port_direction_to_string(ast_port_direction direction) {
    switch (direction)
    {
    case PORT_INOUT:
        return "inout";
    case PORT_INPUT:
        return "input";
    case PORT_OUTPUT:
        return "output";
    default:
        return "unknown";
    }
}

const char* port_expression_type_to_string(ast_expression_type type) {
    switch (type)
    {
    case PRIMARY_EXPRESSION:
        return "  Primary";
    case UNARY_EXPRESSION:
        return "  Unary";
    case BINARY_EXPRESSION:
        return "  Binary";
    case RANGE_EXPRESSION_INDEX:
        return "  Range Index";
    case RANGE_EXPRESSION_UP_DOWN:
        return "  Range UpDown";
    case MINTYPMAX_EXPRESSION:
        return "  MintypeMax";
    case CONDITIONAL_EXPRESSION:
        return "  Conditional";
    default:
        return "  None";
    }
}

const char* port_representation_to_string(ast_number_representation type) {
    switch (type)
    {
    case REP_BITS:
        return " bits";
    case REP_INTEGER:
        return " integer";
    case REP_FLOAT:
        return " float";
    default:
        return " None";
    }
}

const char* port_type_to_string(ast_net_type type) {
    switch (type)
    {
    case NET_TYPE_WIRE:
        return "Wire";
    case NET_TYPE_SUPPLY0:
        return "Supply0";
    case NET_TYPE_SUPPLY1:
        return "Supply1";
    case NET_TYPE_TRI:
        return "Tri";
    case NET_TYPE_TRIAND:
        return "TriAnd";
    case NET_TYPE_TRIREG:
        return "TriReg";
    case NET_TYPE_WAND:
        return "Wand";
    case NET_TYPE_TRIOR:
        return "TriOr";
    case NET_TYPE_WOR:
        return "Wor";
    default:
        return "None";
    }
}

void process_expression(ast_expression *expr, int is_low) {
    if (is_low) {
        printf("        Port low expression type is %s \n", port_expression_type_to_string(expr->type));
    } else {
        printf("        Port high expression type is %s \n", port_expression_type_to_string(expr->type));
    }

    if (is_low) {
        printf("        Port low primary type is %s \n", port_primary_type_to_string(expr->primary->primary_type));
    } else {
        printf("        Port high primary type is %s \n", port_primary_type_to_string(expr->primary->primary_type));
    }

    if (is_low) {
        printf("        Port low primary-value type is %s \n", port_primary_value_type_to_string(expr->primary->value_type));
    } else {
        printf("        Port high primary-value type is %s \n", port_primary_value_type_to_string(expr->primary->value_type));
    }

    if (is_low) {
        printf("        Port low number-representation type is %s \n", port_representation_to_string(expr->primary->value.number->representation));
    } else {
        printf("        Port high number-representation type is %s \n", port_representation_to_string(expr->primary->value.number->representation));
    }

    ast_number *number = expr->primary->value.number;
    if (is_low) {
        printf("        Port low number value is %s \n", number->as_bits);
    } else {
        printf("        Port high number value is %s \n", number->as_bits);
    }
}


void process_port(ast_port_declaration *port) {
    int m;
    for (m=0;m < port->port_names->items; m++) {
        ast_identifier name = ast_list_get(port->port_names, m);
        printf("Port name is %s\n", name->identifier);

    }
    printf("    Port direction is %s\n", port_direction_to_string(port->direction));
    printf("    Port type is %s \n", port_type_to_string(port->net_type));
    printf("    Port is reg: %d \n", port->is_reg);

    if (port->range != NULL) {
        process_expression(port->range->lower, 1);
    }
    if (port->range != NULL) {
        process_expression(port->range->upper, 0);
    }

}