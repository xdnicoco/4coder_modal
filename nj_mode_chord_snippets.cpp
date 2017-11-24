#if !defined(NJ_MODE_CHORD_SNIPPETS_CPP)
#define NJ_MODE_CHORD_SNIPPETS_CPP

struct NJ_MODE_STATE_DECLERATION(NJ_CURRENT_MODE) {};

#define NJ_MODE_PRINT_ENTER_HOOK
NJ_MODE_PRINT_ENTER_FUNCTION(NJ_CURRENT_MODE,
                             0x050f15, // color_bg
                             0x863430, // color_bar
                             0x874440, // color_bar_hover
                             0x895450, // color_bar_active
                             0xe02420, // color_mode
                             0xa01410, // color_mark
                             0x030c7f, // color_pop1
                             0x50080f  // color_pop2
                             );
#undef NJ_MODE_PRINT_ENTER_HOOK


static void
nj_write_named_comment_string(Application_Links *app, char *type_string){
    char space[512];
    String str = make_fixed_width_string(space);
    
    char *name = 0;
    int32_t name_len = 0;
    if (get_current_name(&name, &name_len)){
        append(&str, " // ");
        append(&str, type_string);
        append(&str, "(");
        append(&str, make_string(name, name_len));
        append(&str, "): ");
    }
    else{
        append(&str, "// ");
        append(&str, type_string);
        append(&str, ": ");
    }
    
    write_string(app, str);
}

CUSTOM_COMMAND_SIG(nj_write_todo_then_insert)
CUSTOM_DOC("At the end of the line, insert a '// TODO' comment, includes user name if it was specified in config.4coder, then activate insert mode."){
    exec_command(app, seek_end_of_textual_line);
    nj_write_named_comment_string(app, "TODO");
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}

CUSTOM_COMMAND_SIG(nj_write_note_then_insert)
CUSTOM_DOC("At the end of the line, insert a '// SNIPPETS' comment, includes user name if it was specified in config.4coder, then activate insert mode."){
    exec_command(app, seek_end_of_textual_line);
    nj_write_named_comment_string(app, "SNIPPETS");
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}

CUSTOM_COMMAND_SIG(nj_write_hack_then_insert)
CUSTOM_DOC("At the end of the line, insert a '// HACK' comment, includes user name if it was specified in config.4coder, then activate insert mode."){
    exec_command(app, seek_end_of_textual_line);
    nj_write_named_comment_string(app, "HACK");
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}

CUSTOM_COMMAND_SIG(nj_write_important_then_insert)
CUSTOM_DOC("At the end of the line, insert a '// IMPORTANT' comment, includes user name if it was specified in config.4coder, then activate insert mode."){
    exec_command(app, seek_end_of_textual_line);
    nj_write_named_comment_string(app, "IMPORTANT");
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}

CUSTOM_COMMAND_SIG(nj_write_zero_struct_then_prev)
CUSTOM_DOC("At the end of the line, insert a ' = {0};', then return to the previous mode."){
    exec_command(app, seek_end_of_textual_line);
    write_string(app, make_lit_string(" = {0};"));
    exec_command(app, nj_activate_previous_mode);
}
CUSTOM_COMMAND_SIG(nj_open_long_braces_then_insert)
CUSTOM_DOC("At the cursor, insert a '{' and '}' separated by a blank line, then activate insert mode."){
    char text[] = "{\n\n}";
    int32_t size = sizeof(text) - 1;
    long_braces(app, text, size);
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}
CUSTOM_COMMAND_SIG(nj_open_long_braces_semicolon_then_insert)
CUSTOM_DOC("At the cursor, insert a '{' and '};' separated by a blank line, then activate insert mode."){
    char text[] = "{\n\n};";
    int32_t size = sizeof(text) - 1;
    long_braces(app, text, size);
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}
CUSTOM_COMMAND_SIG(nj_open_long_braces_break_then_insert)
CUSTOM_DOC("At the cursor, insert a '{' and '} break;' separated by a blank line, then activate insert mode."){
    char text[] = "{\n\n} break;";
    int32_t size = sizeof(text) - 1;
    long_braces(app, text, size);
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}

CUSTOM_COMMAND_SIG(nj_open_long_braces_case_then_insert)
CUSTOM_DOC("At the cursor, insert a 'case :{' and '} break;' separated by a blank line, then activate insert mode."){
    char text[] = "case : {\n\n} break;";
    int32_t size = sizeof(text) - 1;
    long_braces(app, text, size);
    
    View_Summary view = get_active_view(app, AccessProtected);
    view_set_cursor(app, &view, seek_line_char(view.cursor.line, view.cursor.character + 3), 1);
    
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}

CUSTOM_COMMAND_SIG(nj_open_long_braces_if_then_insert)
CUSTOM_DOC("At the cursor, insert a 'if(){' and '}' separated by a blank line, then activate insert mode."){
    char text[] = "if()\n{\n\n}";
    int32_t size = sizeof(text) - 1;
    long_braces(app, text, size);
    
    View_Summary view = get_active_view(app, AccessProtected);
    view_set_cursor(app, &view, seek_line_char(view.cursor.line, view.cursor.character + 1), 1);
    
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}

CUSTOM_COMMAND_SIG(nj_open_long_braces_else_if_then_insert)
CUSTOM_DOC("At the cursor, insert a 'else if(){' and '}' separated by a blank line, then activate insert mode."){
    char text[] = "else if()\n{\n\n}";
    int32_t size = sizeof(text) - 1;
    long_braces(app, text, size);
    
    View_Summary view = get_active_view(app, AccessProtected);
    view_set_cursor(app, &view, seek_line_char(view.cursor.line, view.cursor.character + 6), 1);
    
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}

CUSTOM_COMMAND_SIG(nj_open_long_braces_else_then_insert)
CUSTOM_DOC("At the cursor, insert a 'else {' and '}' separated by a blank line, then activate insert mode."){
    char text[] = "else\n{\n\n}";
    int32_t size = sizeof(text) - 1;
    long_braces(app, text, size);
    
    View_Summary view = get_active_view(app, AccessProtected);
    view_set_cursor(app, &view, seek_line_char(view.cursor.line+2, 0), 1);
    
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}

CUSTOM_COMMAND_SIG(nj_open_long_braces_for_then_insert)
CUSTOM_DOC("At the cursor, insert a 'for(;;){' and '} ' separated by a blank line, then activate insert mode."){
    char text[] = "for(;;)\n{\n\n}";
    int32_t size = sizeof(text) - 1;
    long_braces(app, text, size);
    
    View_Summary view = get_active_view(app, AccessProtected);
    view_set_cursor(app, &view, seek_line_char(view.cursor.line, view.cursor.character + 2), 1);
    
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}

CUSTOM_COMMAND_SIG(nj_open_long_braces_while_then_insert)
CUSTOM_DOC("At the cursor, insert a 'while(){' and '} ' separated by a blank line, then activate insert mode."){
    char text[] = "while()\n{\n\n}";
    int32_t size = sizeof(text) - 1;
    long_braces(app, text, size);
    
    View_Summary view = get_active_view(app, AccessProtected);
    view_set_cursor(app, &view, seek_line_char(view.cursor.line, view.cursor.character + 4), 1);
    
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}

CUSTOM_COMMAND_SIG(nj_open_long_braces_switch_then_insert)
CUSTOM_DOC("At the cursor, insert a 'switch(){' and '} ' separated by a blank line, then activate insert mode."){
    char text[] = "switch()\n{\ncase : {\n\n} break;\n}";
    int32_t size = sizeof(text) - 1;
    long_braces(app, text, size);
    
    View_Summary view = get_active_view(app, AccessProtected);
    view_set_cursor(app, &view, seek_line_char(view.cursor.line, view.cursor.character + 5), 1);
    
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}

CUSTOM_COMMAND_SIG(nj_write_eol_block_then_insert)
CUSTOM_DOC("At the end of the line under the cursor, insert a ' /*  */', then activate insert mode."){
    exec_command(app, seek_end_of_textual_line);
    write_string(app, make_lit_string(" /*  */"));
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}
CUSTOM_COMMAND_SIG(nj_write_eol_semicolon)
CUSTOM_DOC("At the end of the line under the cursor, insert a ';'."){
    exec_command(app, seek_end_of_textual_line);
    write_string(app, make_lit_string(";"));
}
CUSTOM_COMMAND_SIG(nj_write_eol_semicolon_then_prev)
CUSTOM_DOC("At the end of the line under the cursor, insert a ';', then return to the previous mode."){
    exec_command(app, nj_write_eol_semicolon);
    exec_command(app, nj_activate_previous_mode);
}

CUSTOM_COMMAND_SIG(nj_write_eol_backslash)
CUSTOM_DOC("At the end of the line under the cursor, insert a '\\'."){
    exec_command(app, seek_end_of_textual_line);
    write_string(app, make_lit_string("\\"));
}
CUSTOM_COMMAND_SIG(nj_write_eol_backslash_then_prev)
CUSTOM_DOC("At the end of the line under the cursor, insert a '\\', then return to the previous mode."){
    exec_command(app, nj_write_eol_backslash);
    exec_command(app, nj_activate_previous_mode);
}

CUSTOM_COMMAND_SIG(nj_comment_line)
CUSTOM_DOC("At the beggining of the line under the cursor, insert a '// ', then move the cursor to the next line."){
    exec_command(app, seek_beginning_of_textual_line);
    write_string(app, make_lit_string("// "));
    exec_command(app, seek_end_of_textual_line);
    exec_command(app, move_right);
}

CUSTOM_COMMAND_SIG(nj_comment_line_then_prev)
CUSTOM_DOC("At the beggining of the line under the cursor, insert a '// ', then move the cursor to the next line and return to the previous mode."){
    exec_command(app, nj_comment_line);
    exec_command(app, nj_activate_previous_mode);
}

CUSTOM_COMMAND_SIG(nj_include_gaurd_file)
CUSTOM_DOC("Insert a c include gaurd around the current buffer, using the current buffer file name."){
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    char file_path_[256];
    String file_path = make_fixed_width_string(file_path_);
    
    copy(&file_path, make_string(buffer.file_name, buffer.file_name_len));
    String file_name = front_of_directory(file_path);
    
    String ext = file_extension(file_name);
    print_message(app, ext.str, ext.size);
    char gaurd_head[512];
    char gaurd_tail[256];
    
    char include_gaurd_[128];
    String include_gaurd = make_fixed_width_string(include_gaurd_);
    copy_partial(&include_gaurd, file_name);
    for(int i = 0; i < include_gaurd.size; ++i){
        if(char_is_lower(include_gaurd.str[i])){
            include_gaurd.str[i] = char_to_upper(include_gaurd.str[i]);
        }
        else if(!char_is_upper(include_gaurd.str[i])){
            include_gaurd.str[i] = '_';
        }
    }
    include_gaurd.str[include_gaurd.size] = 0;
    
    sprintf(gaurd_head, "#if !defined(%s)\n#define %s\n\n",
            include_gaurd.str, include_gaurd.str);
    sprintf(gaurd_tail, "\n\n#endif // %s", include_gaurd.str);
    
    buffer_replace_range(app, &buffer, 0, 0, gaurd_head, str_size(gaurd_head));
    buffer_replace_range(app, &buffer, buffer.size, buffer.size, gaurd_tail, str_size(gaurd_tail));
    
    view_set_cursor(app, &view, seek_line_char(3, 0), 1);
}
CUSTOM_COMMAND_SIG(nj_include_gaurd_file_then_prev)
CUSTOM_DOC("Insert a c include gaurd around the current buffer, using the current buffer file name, then return to the previous mode."){
    exec_command(app, nj_include_gaurd_file);
    exec_command(app, nj_activate_previous_mode);
}

#define nj_bind_mode_keys_chord_snippets(context) \
begin_map(context, mapid_chord_snippets); \
inherit_map(context, mapid_nomap); \
bind(context, 'T', MDFR_NONE, nj_write_todo_then_insert); \
bind(context, 'N', MDFR_NONE, nj_write_note_then_insert); \
bind(context, 'H', MDFR_NONE, nj_write_hack_then_insert); \
bind(context, 'I', MDFR_NONE, nj_write_important_then_insert); \
\
bind(context, '[', MDFR_NONE, nj_open_long_braces_then_insert); \
bind(context, '{', MDFR_NONE, nj_open_long_braces_semicolon_then_insert); \
bind(context, ']', MDFR_NONE, nj_open_long_braces_semicolon_then_insert); \
bind(context, 'b', MDFR_NONE, nj_open_long_braces_break_then_insert); \
bind(context, '}', MDFR_NONE, nj_open_long_braces_break_then_insert); \
bind(context, 'c', MDFR_NONE, nj_open_long_braces_case_then_insert); \
bind(context, 'i', MDFR_NONE, nj_open_long_braces_if_then_insert); \
bind(context, 'e', MDFR_NONE, nj_open_long_braces_else_then_insert); \
bind(context, 'E', MDFR_NONE, nj_open_long_braces_else_if_then_insert); \
bind(context, 'f', MDFR_NONE, nj_open_long_braces_for_then_insert); \
bind(context, 'w', MDFR_NONE, nj_open_long_braces_while_then_insert); \
bind(context, 's', MDFR_NONE, nj_open_long_braces_switch_then_insert); \
\
bind(context, '0',  MDFR_NONE, nj_write_zero_struct_then_prev); \
bind(context, '*',  MDFR_NONE, nj_write_eol_block_then_insert); \
bind(context, ';',  MDFR_NONE, nj_write_eol_semicolon_then_prev); \
bind(context, '\\', MDFR_NONE, nj_write_eol_backslash_then_prev); \
bind(context, '/',  MDFR_NONE, nj_comment_line_then_prev); \
bind(context, '#',  MDFR_NONE, nj_include_gaurd_file_then_prev); \
bind(context, key_esc, MDFR_NONE, nj_mode_enter_normal); \
end_map(context);


#endif // NJ_MODE_CHORD_SNIPPETS_CPP