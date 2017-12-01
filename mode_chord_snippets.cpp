/*
This mode inserts a snippet.

WARNING:
This custom extension provided "as is" without warranty of any kind,
 either express or implied, including without limitation any implied warranties of condition,
 uninterrupted use, merchantability, fitness for a particular purpose, or non-infringement.
*/
#if !defined(_MODE_CHORD_SNIPPETS_CPP)
#define _MODE_CHORD_SNIPPETS_CPP

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

CUSTOM_COMMAND_SIG(nj_mode_enter_chord_snippets)
CUSTOM_DOC("Activates 'chord snippets' mode.")
{
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(NJ_CURRENT_MODE);
}

NJ_MODE_BIND_DECLERATION(NJ_CURRENT_MODE){
    begin_map(context, NJ_MODE_MAPID(NJ_CURRENT_MODE));
    inherit_map(context, mapid_nomap);
    bind(context, 'T', MDFR_NONE, nj_chord_snippet_todo_then_insert);
    bind(context, 'N', MDFR_NONE, nj_chord_snippet_note_then_insert);
    bind(context, 'H', MDFR_NONE, nj_chord_snippet_hack_then_insert);
    bind(context, 'I', MDFR_NONE, nj_chord_snippet_important_then_insert);
    
    bind(context, '[', MDFR_NONE, nj_chord_snippet_long_braces_then_insert);
    bind(context, '{', MDFR_NONE, nj_chord_snippet_long_braces_semicolon_then_insert);
    bind(context, ']', MDFR_NONE, nj_chord_snippet_long_braces_semicolon_then_insert);
    bind(context, 'b', MDFR_NONE, nj_chord_snippet_long_braces_break_then_insert);
    bind(context, '}', MDFR_NONE, nj_chord_snippet_long_braces_break_then_insert);
    bind(context, 'c', MDFR_NONE, nj_chord_snippet_case_then_insert);
    bind(context, 'i', MDFR_NONE, nj_chord_snippet_if_then_insert);
    bind(context, 'e', MDFR_NONE, nj_chord_snippet_else_then_insert);
    bind(context, 'E', MDFR_NONE, nj_chord_snippet_else_if_then_insert);
    bind(context, 'f', MDFR_NONE, nj_chord_snippet_for_then_insert);
    bind(context, 'w', MDFR_NONE, nj_chord_snippet_while_then_insert);
    bind(context, 's', MDFR_NONE, nj_chord_snippet_switch_then_insert);
    bind(context, 'r', MDFR_NONE, nj_chord_snippet_return_then_insert);
    
    bind(context, '0',  MDFR_NONE, nj_chord_snippet_zero_struct_then_prev);
    bind(context, '=',  MDFR_NONE, nj_chord_snippet_equals_semicolon_then_prev);
    bind(context, '*',  MDFR_NONE, nj_chord_snippet_eol_block_then_insert);
    bind(context, ';',  MDFR_NONE, nj_chord_snippet_eol_semicolon_then_prev);
    bind(context, '\\', MDFR_NONE, nj_chord_snippet_eol_backslash_then_prev);
    bind(context, '/',  MDFR_NONE, nj_toggle_comment_line_then_prev);
    bind(context, '#',  MDFR_NONE, nj_include_gaurd_file_then_prev);
    bind(context, key_esc, MDFR_NONE, nj_mode_enter_normal);
    end_map(context);
}


static void
nj_chord_snippet_named_comment_string(Application_Links *app, char *type_string){
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

CUSTOM_COMMAND_SIG(nj_chord_snippet_equals_semicolon_then_prev)
CUSTOM_DOC("At the end of the line, insert a ' = ;', then return to the previous mode."){
    seek_end_of_textual_line(app);
    write_string(app, make_lit_string(" = ;"));
    move_down_textual(app);
    nj_activate_previous_mode(app);
}

CUSTOM_COMMAND_SIG(nj_chord_snippet_todo_then_insert)
CUSTOM_DOC("At the end of the line, insert a '// TODO' comment, includes user name if it was specified in config.4coder, then activate insert mode."){
    seek_end_of_textual_line(app);
    nj_chord_snippet_named_comment_string(app, "TODO");
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}

CUSTOM_COMMAND_SIG(nj_chord_snippet_note_then_insert)
CUSTOM_DOC("At the end of the line, insert a '// NOTE' comment, includes user name if it was specified in config.4coder, then activate insert mode."){
    seek_end_of_textual_line(app);
    nj_chord_snippet_named_comment_string(app, "NOTE");
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}

CUSTOM_COMMAND_SIG(nj_chord_snippet_hack_then_insert)
CUSTOM_DOC("At the end of the line, insert a '// HACK' comment, includes user name if it was specified in config.4coder, then activate insert mode."){
    seek_end_of_textual_line(app);
    nj_chord_snippet_named_comment_string(app, "HACK");
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}

CUSTOM_COMMAND_SIG(nj_chord_snippet_important_then_insert)
CUSTOM_DOC("At the end of the line, insert a '// IMPORTANT' comment, includes user name if it was specified in config.4coder, then activate insert mode."){
    seek_end_of_textual_line(app);
    nj_chord_snippet_named_comment_string(app, "IMPORTANT");
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}

CUSTOM_COMMAND_SIG(nj_chord_snippet_zero_struct_then_prev)
CUSTOM_DOC("At the end of the line, insert a ' = {0};', then return to the previous mode."){
    seek_end_of_textual_line(app);
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    int32_t pos = view.cursor.pos;
    char c = buffer_get_char(app, &buffer, pos);
    if(c == ';'){
        buffer_replace_range(app, &buffer, pos, pos+1, 0, 0);
    }
    write_string(app, make_lit_string(" = {0};"));
    move_down_textual(app);
    nj_activate_previous_mode(app);
}

inline void nj_move_cursor_to_relative_line_then_char(Application_Links *app, int32_t rel_line, int32_t rel_char) {
    View_Summary view = get_active_view(app, AccessProtected);
    view_set_cursor(app, &view, seek_line_char(view.cursor.line + rel_line, 0), 1);
    view_set_cursor(app, &view, seek_line_char(view.cursor.line, view.cursor.character + rel_char), 1);
}
CUSTOM_COMMAND_SIG(nj_chord_snippet_return_then_insert)
CUSTOM_DOC("At the end of the line, insert a 'return();', then activate insert mode."){
    seek_end_of_textual_line(app);
    char str[] = "\nreturn();";
    
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    buffer_replace_range(app, &buffer, view.cursor.pos, view.cursor.pos, str, str_size(str));
    
    nj_move_cursor_to_relative_line_then_char(app, 1, 7);
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}

CUSTOM_COMMAND_SIG(nj_chord_snippet_long_braces_then_insert)
CUSTOM_DOC("At the cursor, insert a '{' and '}' separated by a blank line, then activate insert mode."){
    seek_end_of_textual_line(app);
    char text[] = " {\n\n}";
    long_braces(app, text, str_size(text));
    nj_move_cursor_to_relative_line_then_char(app, 1, 0);
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}
CUSTOM_COMMAND_SIG(nj_chord_snippet_long_braces_semicolon_then_insert)
CUSTOM_DOC("At the cursor, insert a '{' and '};' separated by a blank line, then activate insert mode."){
    seek_end_of_textual_line(app);
    char text[] = " {\n\n};";
    long_braces(app, text, str_size(text));
    nj_move_cursor_to_relative_line_then_char(app, 1, 0);
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}
CUSTOM_COMMAND_SIG(nj_chord_snippet_long_braces_break_then_insert)
CUSTOM_DOC("At the cursor, insert a '{' and '} break;' separated by a blank line, then activate insert mode."){
    seek_end_of_textual_line(app);
    char text[] = " {\n\n} break;";
    long_braces(app, text, str_size(text));
    nj_move_cursor_to_relative_line_then_char(app, 1, 0);
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}

CUSTOM_COMMAND_SIG(nj_chord_snippet_case_then_insert)
CUSTOM_DOC("At the cursor, insert a 'case :{' and '} break;' separated by a blank line, then activate insert mode."){
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    seek_end_of_textual_line(app);
    
    char *text;
    if(buffer_get_char(app, &buffer, view.cursor.pos - 1) == '\n' && buffer_get_char(app, &buffer, view.cursor.pos) == '\n') {
        text = "case : {\n\n} break;";
    }
    else {
        text = "\ncase : {\n\n} break;";
    }
    long_braces(app, text, str_size(text));
    
    nj_move_cursor_to_relative_line_then_char(app, 0, 5);
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}

CUSTOM_COMMAND_SIG(nj_chord_snippet_if_then_insert)
CUSTOM_DOC("At the cursor, insert a 'if(){' and '}' separated by a blank line, then activate insert mode."){
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    seek_end_of_textual_line(app);
    
    char *text;
    if(buffer_get_char(app, &buffer, view.cursor.pos - 1) == '\n' && buffer_get_char(app, &buffer, view.cursor.pos) == '\n') {
        text = "if() {\n\n}";
    }
    else {
        text = "\nif() {\n\n}";
    }
    long_braces(app, text, str_size(text));
    
    nj_move_cursor_to_relative_line_then_char(app, 0, 3);
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}

CUSTOM_COMMAND_SIG(nj_chord_snippet_else_if_then_insert)
CUSTOM_DOC("At the cursor, insert a 'else if(){' and '}' separated by a blank line, then activate insert mode."){
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    seek_end_of_textual_line(app);
    
    char *text;
    if(buffer_get_char(app, &buffer, view.cursor.pos - 1) == '\n' && buffer_get_char(app, &buffer, view.cursor.pos) == '\n') {
        text = "else if() {\n\n}";
    }
    else {
        text = "\nelse if() {\n\n}";
    }
    long_braces(app, text, str_size(text));
    
    nj_move_cursor_to_relative_line_then_char(app, 0, 8);
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}

CUSTOM_COMMAND_SIG(nj_chord_snippet_else_then_insert)
CUSTOM_DOC("At the cursor, insert a 'else {' and '}' separated by a blank line, then activate insert mode."){
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    seek_end_of_textual_line(app);
    
    char *text;
    if(buffer_get_char(app, &buffer, view.cursor.pos - 1) == '\n' && buffer_get_char(app, &buffer, view.cursor.pos) == '\n') {
        text = "else {\n\n}";
    }
    else {
        text = "\nelse {\n\n}";
    }
    long_braces(app, text, str_size(text));
    
    nj_move_cursor_to_relative_line_then_char(app, 1, 0);
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}

CUSTOM_COMMAND_SIG(nj_chord_snippet_for_then_insert)
CUSTOM_DOC("At the cursor, insert a 'for(;;){' and '} ' separated by a blank line, then activate insert mode."){
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    seek_end_of_textual_line(app);
    
    char *text;
    if(buffer_get_char(app, &buffer, view.cursor.pos - 1) == '\n' && buffer_get_char(app, &buffer, view.cursor.pos) == '\n') {
        text = "for(;;) {\n\n}";
    }
    else {
        text = "\nfor(;;) {\n\n}";
    }
    long_braces(app, text, str_size(text));
    
    nj_move_cursor_to_relative_line_then_char(app, 0, 4);
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}

CUSTOM_COMMAND_SIG(nj_chord_snippet_while_then_insert)
CUSTOM_DOC("At the cursor, insert a 'while(){' and '} ' separated by a blank line, then activate insert mode."){
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    seek_end_of_textual_line(app);
    
    char *text;
    if(buffer_get_char(app, &buffer, view.cursor.pos - 1) == '\n' && buffer_get_char(app, &buffer, view.cursor.pos) == '\n') {
        text = "while() {\n\n}";
    }
    else {
        text = "\nwhile() {\n\n}";
    }
    int32_t size = str_size(text);
    long_braces(app, text, size);
    
    nj_move_cursor_to_relative_line_then_char(app, 0, 6);
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}

CUSTOM_COMMAND_SIG(nj_chord_snippet_switch_then_insert)
CUSTOM_DOC("At the cursor, insert a 'switch(){' and '} ' separated by a blank line, then activate insert mode."){
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    seek_end_of_textual_line(app);
    
    char *text;
    if(buffer_get_char(app, &buffer, view.cursor.pos - 1) == '\n' && buffer_get_char(app, &buffer, view.cursor.pos) == '\n') {
        text = "switch() {\ncase : {\n\n} break;\n}";
    }
    else {
        text = "\nswitch() {\ncase : {\n\n} break;\n}";
    }
    int32_t size = str_size(text);
    long_braces(app, text, size);
    
    nj_move_cursor_to_relative_line_then_char(app, 0, 7);
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}

CUSTOM_COMMAND_SIG(nj_chord_snippet_eol_block_then_insert)
CUSTOM_DOC("At the end of the line under the cursor, insert a ' /*  */', then activate insert mode."){
    seek_end_of_textual_line(app);
    write_string(app, make_lit_string(" /*  */"));
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(insert);
}
CUSTOM_COMMAND_SIG(nj_chord_snippet_eol_semicolon)
CUSTOM_DOC("At the end of the line under the cursor, insert a ';'."){
    seek_end_of_textual_line(app);
    write_string(app, make_lit_string(";"));
    move_down_textual(app);
}
CUSTOM_COMMAND_SIG(nj_chord_snippet_eol_semicolon_then_prev)
CUSTOM_DOC("At the end of the line under the cursor, insert a ';', then return to the previous mode."){
    nj_chord_snippet_eol_semicolon(app);
    nj_activate_previous_mode(app);
    move_down_textual(app);
}

CUSTOM_COMMAND_SIG(nj_chord_snippet_eol_backslash)
CUSTOM_DOC("At the end of the line under the cursor, insert a '\\'."){
    seek_end_of_textual_line(app);
    write_string(app, make_lit_string("\\"));
    move_down_textual(app);
}
CUSTOM_COMMAND_SIG(nj_chord_snippet_eol_backslash_then_prev)
CUSTOM_DOC("At the end of the line under the cursor, insert a '\\', then return to the previous mode."){
    nj_chord_snippet_eol_backslash(app);
    nj_activate_previous_mode(app);
}

CUSTOM_COMMAND_SIG(nj_toggle_comment_line)
CUSTOM_DOC("Toggles '// ' at the beggining of the line under the cursor, then move the cursor to the next line."){
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    int32_t pos = seek_line_beginning(app, &buffer, view.cursor.pos);
    if(buffer_get_char(app, &buffer, pos) == '/' &&
       buffer_get_char(app, &buffer, pos + 1) == '/') {
        buffer_replace_range(app, &buffer, pos, pos + 2, 0, 0);
    }
    else {
        write_string(app, make_lit_string("// "));
    }
    move_down_textual(app);
}

CUSTOM_COMMAND_SIG(nj_toggle_comment_line_then_prev)
CUSTOM_DOC("Toggles '// ' at the beggining of the line under the cursor, then move the cursor to the next line and return to the previous mode."){
    nj_toggle_comment_line(app);
    nj_activate_previous_mode(app);
}

CUSTOM_COMMAND_SIG(nj_include_gaurd_file)
CUSTOM_DOC("Insert a c include gaurd around the current buffer, using the current buffer file name."){
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    String file_name = front_of_directory(make_string(buffer.file_name, buffer.file_name_len));
    
    const int32_t include_gaurd_size = 128;
    char include_gaurd_[include_gaurd_size];
    String include_gaurd = make_fixed_width_string(include_gaurd_);
    copy_partial(&include_gaurd, file_name);
    for(int i = 0; i < include_gaurd.size; ++i){
        if(char_is_lower(include_gaurd.str[i])){
            include_gaurd.str[i] = char_to_upper(include_gaurd.str[i]);
        }
        else if(!char_is_alpha_numeric(include_gaurd.str[i])){
            include_gaurd.str[i] = '_';
        }
    }
    include_gaurd.str[include_gaurd.size] = 0;
    
    char gaurd_head[include_gaurd_size*2+32];
    sprintf(gaurd_head, "#if !defined(_%s)\n#define _%s\n\n", include_gaurd.str, include_gaurd.str);
    
    char gaurd_tail[include_gaurd_size+16];
    sprintf(gaurd_tail, "\n\n#endif // _%s", include_gaurd.str);
    
    Partition *part = &global_part;
    Temp_Memory temp = begin_temp_memory(part);
    Buffer_Edit edits[2];
    
    char *str = 0;
    char *base = (char*)partition_current(part);
    
    str = push_array(part, char, str_size(gaurd_head));
    memcpy(str, gaurd_head, sizeof(gaurd_head));
    edits[0].str_start = (int32_t)(str - base);
    edits[0].len = str_size(gaurd_head);
    edits[0].start = 0;
    edits[0].end = 0;
    
    str = push_array(part, char, str_size(gaurd_tail));
    memcpy(str, gaurd_tail, str_size(gaurd_tail));
    edits[1].str_start = (int32_t)(str - base);
    edits[1].len = str_size(gaurd_tail);
    edits[1].start = buffer.size;
    edits[1].end = buffer.size;
    
    buffer_batch_edit(app, &buffer, base, part->pos, edits, ArrayCount(edits), BatchEdit_Normal);
    end_temp_memory(temp);
    
    view_set_cursor(app, &view, seek_line_char(3, 0), 1);
}
CUSTOM_COMMAND_SIG(nj_include_gaurd_file_then_prev)
CUSTOM_DOC("Insert a c include gaurd around the current buffer, using the current buffer file name, then return to the previous mode."){
    nj_include_gaurd_file(app);
    nj_activate_previous_mode(app);
}

#endif // _MODE_CHORD_SNIPPETS_CPP
