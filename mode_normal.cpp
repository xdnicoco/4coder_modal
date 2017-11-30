/*
This mode is used to navigate, make changes to the text, delete and edit whitespace, call buffer commands etc.
This is the default mode the editor starts in, and it is essential to the modal system to have it.

WARNING:
This custom extension provided "as is" without warranty of any kind,
 either express or implied, including without limitation any implied warranties of condition,
 uninterrupted use, merchantability, fitness for a particular purpose, or non-infringement.
*/

#if !defined(_MODE_NORMAL_CPP)
#define _MODE_NORMAL_CPP

struct NJ_MODE_STATE_DECLERATION(NJ_CURRENT_MODE) {};

#define NJ_MODE_PRINT_ENTER_HOOK
NJ_MODE_PRINT_ENTER_FUNCTION(NJ_CURRENT_MODE,
                             0x050f15, // color_bg
                             0x030439, // color_bar
                             0x020f2f, // color_bar_hover
                             0x051449, // color_bar_active
                             0x104489, // color_mode
                             0x0d6695, // color_mark
                             0x03cf0c, // color_pop1
                             0xff0000  // color_pop2
                             );
#undef NJ_MODE_PRINT_ENTER_HOOK

CUSTOM_COMMAND_SIG(nj_mode_enter_normal)
CUSTOM_DOC("Activates 'normal' mode.")
{
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(NJ_CURRENT_MODE);
}

NJ_MODE_BIND_DECLERATION(NJ_CURRENT_MODE){
    begin_map(context, NJ_MODE_MAPID(NJ_CURRENT_MODE));
    inherit_map(context, mapid_movements);
    bind(context, 'e', MDFR_NONE, delete_char);
    bind(context, 'E', MDFR_NONE, nj_snipe_token_or_word);
    
    bind(context, 'i', MDFR_NONE, nj_mode_enter_insert);
    bind(context, 'I', MDFR_NONE, nj_mode_enter_chord_insert_single);
    bind(context, 'a', MDFR_NONE, nj_insert_after);
    bind(context, 'A', MDFR_NONE, nj_seek_eol_then_insert);
    bind(context, 'o', MDFR_NONE, nj_newline_then_insert_after);
    bind(context, 'O', MDFR_NONE, nj_newline_then_insert_before);
    
    bind(context, 'd', MDFR_NONE, duplicate_line);
    bind(context, 'D', MDFR_NONE, delete_line);
    bind(context, 'x', MDFR_NONE, cut);
    bind(context, 'X', MDFR_NONE, nj_cut_line);
    bind(context, 'x', MDFR_ALT,  nj_cut_token_or_word);
    bind(context, 'c', MDFR_NONE, copy);
    bind(context, 'C', MDFR_NONE, nj_copy_line);
    bind(context, 'c', MDFR_ALT,  nj_copy_token_or_word);
    bind(context, 'v', MDFR_NONE, paste);
    bind(context, 'V', MDFR_NONE, paste_next_and_indent);
    bind(context, 'p', MDFR_NONE, paste_and_indent);
    bind(context, 'P', MDFR_NONE, paste_next_and_indent);
    
    bind(context, 'g', MDFR_NONE, nj_mode_enter_chord_goto);
    bind(context, 'G', MDFR_NONE, nj_seek_end_of_file);
    
    bind(context, 'z', MDFR_NONE, undo);
    bind(context, 'Z', MDFR_NONE, redo);
    bind(context, 'u', MDFR_NONE, undo);
    bind(context, 'U', MDFR_NONE, redo);
    
    bind(context, 'f', MDFR_NONE, interactive_open_or_new);
    bind(context, 'F', MDFR_NONE, open_in_other);
    bind(context, 's', MDFR_NONE, save);
    
    bind(context, 'K', MDFR_NONE, kill_buffer);
    bind(context, 'u', MDFR_CTRL, interactive_kill_buffer);
    bind(context, 'b', MDFR_NONE, interactive_switch_buffer);
    
    bind(context, ' ', MDFR_NONE, set_mark);
    bind(context, '_', MDFR_NONE, write_character);
    
    bind(context, 'L', MDFR_ALT, to_lowercase);
    bind(context, 'U', MDFR_ALT,  to_uppercase);
    
    bind(context, 't', MDFR_NONE, list_all_substring_locations_case_insensitive);
    bind(context, 'T', MDFR_NONE, list_all_locations_of_identifier);
    
    bind(context, 'r', MDFR_NONE, nj_mode_enter_chord_replace_single);
    bind(context, 'R', MDFR_NONE, nj_mode_enter_replace);
    
    bind(context, 'q', MDFR_NONE, nj_start_recording_keyboard_macro);
    bind(context, 'Q', MDFR_CTRL, nj_finish_recording_keyboard_macro);
    bind(context, 'Q', MDFR_NONE, nj_play_keyboard_macro);
    
    bind(context, key_back, MDFR_SHIFT, nj_backspace_line);
    
    bind(context, '\n', MDFR_NONE,  newline_or_goto_position);
    bind(context, '\n', MDFR_SHIFT, newline_or_goto_position_same_panel);
    
    bind(context, '\t', MDFR_NONE,  word_complete);
    bind(context, '\t', MDFR_CTRL,  auto_tab_range);
    bind(context, '\t', MDFR_SHIFT, auto_tab_line_at_cursor);
    
    bind(context, '!', MDFR_NONE, nj_execute_any_cli);
    bind(context, '!', MDFR_CTRL, execute_previous_cli);
    
    bind(context, '~', MDFR_NONE, nj_execute_arbitrary_command);
    bind(context, ':', MDFR_NONE, nj_execute_arbitrary_command);
    bind(context, '.', MDFR_NONE, nj_toggler);
    
    bind(context, '[',  MDFR_NONE, highlight_surrounding_scope);
    bind(context, ']',  MDFR_NONE, highlight_prev_scope_absolute);
    bind(context, '\'', MDFR_NONE, highlight_next_scope_absolute);
    bind(context, '{',  MDFR_NONE, place_in_scope);
    bind(context, '}',  MDFR_NONE, scope_absorb_down);
    
    bind(context, '\t', MDFR_NONE,  word_complete);
    
    bind(context, '1', MDFR_NONE, open_file_in_quotes);
    bind(context, '2', MDFR_NONE, nj_open_matching_file_cpp_same_panel);
    bind(context, '@', MDFR_NONE, open_matching_file_cpp);
    bind(context, '3', MDFR_NONE, nj_mode_enter_chord_snippets);
    bind(context, '4', MDFR_NONE, nj_mode_enter_chord_case);
    bind(context, '5', MDFR_NONE, nj_decrement_digit_decimal);
    bind(context, '%', MDFR_NONE, nj_decrement_digit_hexadecimal);
    bind(context, '6', MDFR_NONE, nj_increment_digit_decimal);
    bind(context, '^', MDFR_NONE, nj_increment_digit_hexadecimal);
    bind(context, '8', MDFR_NONE, nj_mode_enter_chord_settings);
    end_map(context);
}

static bool32 nj_recording_macro = false;
struct NJ_Input_Node {
    User_Input input;
    NJ_Input_Node *n;
};
struct NJ_Macro_Register {
    bool32 initialized;
    NJ_Input_Node root;
};
NJ_Macro_Register nj_macro_registers[((uint8_t)'~' - (uint8_t)'!')] = {0};

//
// HACK(NJ): Crahes when other command (like I-Search) tries to get user-input.
//

CUSTOM_COMMAND_SIG(nj_start_recording_keyboard_macro)
CUSTOM_DOC("Starts to record a keyboard macro.") {
    Query_Bar query_bar = {0};
    char query_bar_space[1];
    query_bar.prompt = make_lit_string("Record to register: ");
    query_bar.string = make_fixed_width_string(query_bar_space);
    
    if(query_user_string(app, &query_bar)) {
        NJ_Input_Node *current_node = 0;
        int32_t current_register = query_bar.string.str[0] - '!';
        
        if(current_register < ArrayCount(nj_macro_registers) && current_register >= 0){
            if(nj_macro_registers[current_register].initialized){
                current_node = nj_macro_registers[current_register].root.n;
                while(current_node){
                    NJ_Input_Node *temp = current_node->n;
                    memory_free(app, current_node, sizeof(NJ_Input_Node));
                    current_node = temp;
                }
                
                nj_macro_registers[current_register].initialized = false;
            }
            end_query_bar(app, &query_bar, 0);
            
            Query_Bar info_bar = {0};
            char current_info_space[256] = {0};
            info_bar.prompt = make_fixed_width_string(current_info_space);
            append_ss(&info_bar.prompt, make_lit_string("Recording keyboard macro to register ["));
            append_ss(&info_bar.prompt, query_bar.string);
            append_ss(&info_bar.prompt, make_lit_string("]"));
            start_query_bar(app, &info_bar, 0);
            
            nj_recording_macro = true;
            while(nj_recording_macro){
                User_Input in = get_user_input(app, EventOnAnyKey, 0);
                
#if 0
                { // NOTE(NJ): debugging data
                    char msg[256];
                    sprintf(msg, "Input: key = %c, command = %p\n", in.key.character, in.command.command);
                    print_message(app, msg, str_size(msg));
                }
#endif
                if(in.command.command)
                {
                    if(!nj_macro_registers[current_register].initialized)
                    {
                        nj_macro_registers[current_register].initialized = true;
                        nj_macro_registers[current_register].root.input = in;
                        current_node = &nj_macro_registers[current_register].root;
                    }
                    else {
                        // TODO(NJ): Maybe a better way to allocate memory?
                        current_node->n = (NJ_Input_Node *)memory_allocate(app, sizeof(NJ_Input_Node));
                        current_node->n->n = 0;
                        current_node->n->input = in;
                        current_node = current_node->n;
                    }
                    
                    in.command.command(app);
                }
            }
            end_query_bar(app, &info_bar, 0);
        }
        else {
            print_message(app, literal("Register ["));
            print_message(app, query_bar.string.str, query_bar.string.size);
            print_message(app, literal("] is invalid.\n"));
        }
        
    }
}

CUSTOM_COMMAND_SIG(nj_finish_recording_keyboard_macro)
CUSTOM_DOC("Finishes to record a keyboard macro."){
    if(nj_recording_macro){
        nj_recording_macro = false;
    }
}

static void nj_play_keyboard_macro_from_register(Application_Links *app, int32_t current_register){
    if(nj_macro_registers[current_register].initialized){
        NJ_Input_Node *current_node = &nj_macro_registers[current_register].root;
        while(current_node){
            // HACK(NJ): Currently emulating write_character and nj_chord_goto_seek_line
            // TODO(NJ): Find a way to change the user input returned from get_command_input;
            if(current_node->input.command.command == write_character){
                uint8_t character[4];
                uint32_t length = to_writable_character(current_node->input, character);
                write_character_parameter(app, character, length);
            }
#if 0
            else if(current_node->input.command.command == nj_chord_goto_seek_line) {
                if(char_is_numeric((char)current_node->input.key.character))
                {
                    char input_character[] = {(char)current_node->input.key.character, 0};
                    i32_4tech input_digit = str_to_int_c(input_character);
                    
                    NJ_MODE_STATE(chord_goto).line_input = NJ_MODE_STATE(chord_goto).line_input*10 + input_digit;
                    nj_chord_goto_apply_seek(app);
                }
            }
#endif
            else {
                current_node->input.command.command(app);
            }
            current_node = current_node->n;
        }
    }
}

CUSTOM_COMMAND_SIG(nj_play_keyboard_macro)
CUSTOM_DOC("Querys for a macro register number and number of times to play it, then plays the macro the number of times queryed.")
{
    if(!nj_recording_macro)
    {
        Query_Bar register_bar;
        char register_bar_space[1];
        register_bar.prompt = make_lit_string("Play macro from register: ");
        register_bar.string = make_fixed_width_string(register_bar_space);
        
        if(query_user_string(app, &register_bar)) {
            int32_t current_register = register_bar.string.str[0] - '!';
            
            if(current_register < ArrayCount(nj_macro_registers) && current_register >= 0){
                Query_Bar times_bar;
                char times_bar_space[256];
                times_bar.prompt = make_lit_string("How many times do you want to play the macro? ");
                times_bar.string = make_fixed_width_string(times_bar_space);
                if(query_user_number(app, &times_bar)) {
                    int32_t times = str_to_int_s(times_bar.string);
                    for(int32_t i = 0; i < times; ++i) {
                        nj_play_keyboard_macro_from_register(app, current_register);
                    }
                }
            }
            else {
                print_message(app, literal("Register ["));
                print_message(app, register_bar.string.str, register_bar.string.size);
                print_message(app, literal("] is invalid.\n"));
            }
        }
    }
}

static void nj_execute_a_cli_command(Application_Links *app, String cmd, String output_buffer_name)
{
    String hot_directory = make_fixed_width_string(hot_directory_space);
    hot_directory.size = directory_get_hot(app, hot_directory.str, hot_directory.memory_size);
    
    uint32_t access = AccessAll;
    View_Summary view = get_active_view(app, access);
    
    exec_system_command(app, &view, buffer_identifier(output_buffer_name.str, output_buffer_name.size), hot_directory.str, hot_directory.size, cmd.str, cmd.size, CLI_OverlapWithConflict | CLI_CursorAtEnd);
    lock_jump_buffer(output_buffer_name.str, output_buffer_name.size);
}

CUSTOM_COMMAND_SIG(nj_execute_any_cli)
CUSTOM_DOC("Queries for a system command, runs the system command as a CLI and prints the output to a buffer named after the command."){
    Query_Bar bar_cmd = {0};
    
    bar_cmd.prompt = make_lit_string("Shell: ");
    bar_cmd.string = make_fixed_width_string(command_space);
    if (!query_user_string(app, &bar_cmd)) return;
    
    char output_buffer_name_space[256] = {"*shell: "};
    String output_buffer_name = make_string_cap(output_buffer_name_space, str_size(output_buffer_name_space), 256);
    append_partial_ss(&output_buffer_name, bar_cmd.string);
    
    for(int i = 0; i < output_buffer_name.size; ++i) {
        if(output_buffer_name.str[i] == '\\' ||
           output_buffer_name.str[i] == '/') {
            output_buffer_name.str[i] = '#';
        }
        else if(output_buffer_name.str[i] == '"') {
            output_buffer_name.str[i] = '\'';
        }
    }
    
    append_s_char(&output_buffer_name, '*');
    
    nj_execute_a_cli_command(app, bar_cmd.string, output_buffer_name);
}

CUSTOM_COMMAND_SIG(nj_execute_arbitrary_command)
CUSTOM_DOC("Execute a 'long form' command.")
{
    // NOTE(allen): This isn't a super powerful version of this command, I will expand
    // upon it so that it has all the cmdid_* commands by default.  However, with this
    // as an example you have everything you need to make it work already. You could
    // even use app->memory to create a hash table in the start hook.
    Query_Bar bar = {0};
    char space[1024];
    bar.prompt = make_lit_string("Command: ");
    bar.string = make_fixed_width_string(space);
    
    if (!query_user_string(app, &bar)) return;
    
    // NOTE(allen): Here I chose to end this query bar because when I call another
    // command it might ALSO have query bars and I don't want this one hanging
    // around at that point.  Since the bar exists on my stack the result of the query
    // is still available in bar.string though.
    end_query_bar(app, &bar, 0);
    
    if (match_ss(bar.string, make_lit_string("load project"))){
        load_project(app);
    }
    else if (match_ss(bar.string, make_lit_string("open all code")) ||
             match_ss(bar.string, make_lit_string("open *"))){
        open_all_code(app);
    }
    else if (match_ss(bar.string, make_lit_string("open all code recursive"))){
        open_all_code_recursive(app);
    }
    else if(match_ss(bar.string, make_lit_string("close all code"))){
        close_all_code(app);
    }
    else if (match_ss(bar.string, make_lit_string("dos lines")) ||
             match_ss(bar.string, make_lit_string("dosify"))){
        eol_dosify(app);
    }
    else if (match_ss(bar.string, make_lit_string("nix lines")) ||
             match_ss(bar.string, make_lit_string("nixify"))){
        eol_nixify(app);
    }
    else if (match_ss(bar.string, make_lit_string("remap")) ||
             match_ss(bar.string, make_lit_string("mode"))){
        remap_interactive(app);
    }
    else if (match_ss(bar.string, make_lit_string("new project"))){
        setup_new_project(app);
    }
    else if (match_ss(bar.string, make_lit_string("delete file")) ||
             match_ss(bar.string, make_lit_string("rm"))){
        delete_file_query(app);
    }
    else if (match_ss(bar.string, make_lit_string("rename file")) ||
             match_ss(bar.string, make_lit_string("mv"))){
        rename_file_query(app);
    }
    else if (match_ss(bar.string, make_lit_string("mkdir"))){
        make_directory_query(app);
    }
    else if (match_ss(bar.string, make_lit_string("wq"))){
        exec_command(app, save);
        exec_command(app, exit_4coder);
    }
    else if (match_ss(bar.string, make_lit_string("w"))){
        exec_command(app, save);
    }
    else if (match_ss(bar.string, make_lit_string("q"))){
        exec_command(app, exit_4coder);
    }
    else{
        bool32 command_sig_found = false;
        replace_char(&bar.string, ' ', '_');
        for(int32_t i = 0;
            i < command_one_past_last_id;
            ++i)
        {
            if(match_sc(bar.string, fcoder_metacmd_table[i].name))
            {
                command_sig_found = true;
                exec_command(app, fcoder_metacmd_table[i].proc);
                break;
            }
        }
        if(!command_sig_found)
        {
            print_message(app, literal("unrecognized command: "));
            print_message(app, bar.string.str, bar.string.size);
            print_message(app, literal("\n"));
        }
    }
}

void nj_ocd(Application_Links *app, char *seek, int32_t len){
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    int32_t min_line_index;
    int32_t max_line_index;
    if(view.cursor.line > view.mark.line){
        min_line_index = view.mark.line;
        max_line_index = view.cursor.line;
    } else {
        min_line_index = view.cursor.line;
        max_line_index = view.mark.line;
    }
    
    int32_t line_count = max_line_index - min_line_index + 1;
    Buffer_Seek *positions = (Buffer_Seek *)malloc(line_count*sizeof(Buffer_Seek));
    
    int32_t max_position = 0;
    int32_t min_position = INT32_MAX;
    for(int32_t line_index = 0; line_index < line_count; ++line_index)
    {
        int32_t abs_line_index = min_line_index + line_index;
        int32_t start = buffer_get_line_start(app, &buffer, seek_line_char(abs_line_index, 0).pos);
        int32_t end = buffer_get_line_end(app, &buffer, seek_line_char(abs_line_index, 0).pos) + 1;
        
        int32_t line_size = end - start;
        Partition *part = &global_part;
        String current_line = {0};
        read_line(app, part, &buffer, abs_line_index, &current_line);
        
        i32_4tech position = find_substr_s(current_line, 0, make_string(seek, len));
        
        if(position < line_size-1){
            *(positions + line_index) = seek_line_char(abs_line_index, position);
            if(position > max_position){
                max_position = position;
            }
            else if (position < min_position){
                min_position = position;
            }
        }
        else {
            (positions + line_index)->type = 0;
        }
    }
    
    if (max_position - min_position + 1 <= app->memory_size){
        char *mem = (char*)app->memory;
        for(int32_t c_index = 0; c_index < (max_position - min_position + 1); ++c_index){
            *(mem + c_index) = ' ';
        }
        
        for(int32_t line_index = 0; line_index < line_count; ++line_index)
        {
            Buffer_Seek *position = positions + line_index;
            
            //
            // char msg_pos[256];
            // sprintf(msg_pos, "inserting space in position: L#%d C#%d T#%d\n", position->line, position->character, position->type);
            // print_message(app, msg_pos, str_size(msg_pos));
            //
            
            if(position->type != 0){
                int32_t num_spaces = max_position - position->character;
                
                Partial_Cursor cursor;
                if(buffer_compute_cursor(app, &buffer, *position, &cursor)){
                    buffer_replace_range(app, &buffer, cursor.pos + 1, cursor.pos + 1, mem, sizeof(char)*num_spaces);
                }
            }
        }
    }
}

CUSTOM_COMMAND_SIG(nj_ocd_equals)
CUSTOM_DOC("Aligns the first '=' character to be on the same column of every line in the range."){
    nj_ocd(app, "=", 1);
}

CUSTOM_COMMAND_SIG(nj_ocd_arbitrary)
CUSTOM_DOC("Queries for an arbitrary character, then aligns it to be on the same column of every line in the range."){
    Query_Bar align;
    char align_space[1024];
    align.prompt = make_lit_string("OCD by: ");
    align.string = make_fixed_width_string(align_space);
    
    if (query_user_string(app, &align)) {
        nj_ocd(app, align.string.str, align.string.size);
    }
}

void nj_replace_rectangle_function(Application_Links *app, char *str, int32_t len){
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    Range range = get_range(&view);
    int32_t min_line_index;
    int32_t max_line_index;
    if(view.cursor.line > view.mark.line){
        min_line_index = view.mark.line;
        max_line_index = view.cursor.line;
    } else {
        min_line_index = view.cursor.line;
        max_line_index = view.mark.line;
    }
    int32_t min_char_index;
    int32_t max_char_index;
    if(view.cursor.character > view.mark.character){
        min_char_index = view.mark.character;
        max_char_index = view.cursor.character;
    } else {
        min_char_index = view.cursor.character;
        max_char_index = view.mark.character;
    }
    
    int32_t line_count = max_line_index - min_line_index + 1;
    int32_t edit_count = 0;
    
    Partition *part = &global_part;
    Temp_Memory temp = begin_temp_memory(part);
    
    Buffer_Edit *edits = push_array(part, Buffer_Edit, line_count);
    
    for(int32_t i = 0; i < line_count; ++i) {
        Buffer_Seek min_seek = seek_line_char(min_line_index + i, min_char_index);
        Buffer_Seek max_seek = seek_line_char(min_line_index + i, max_char_index);
        
        Partial_Cursor min_cursor, max_cursor;
        if(buffer_compute_cursor(app, &buffer, min_seek, &min_cursor) &&
           buffer_compute_cursor(app, &buffer, max_seek, &max_cursor))
        {
            Buffer_Edit new_edit;
            new_edit.str_start = 0;
            new_edit.len = len;
            new_edit.start = min_cursor.pos;
            new_edit.end = max_cursor.pos;
            edits[edit_count++] = new_edit;
            
            Assert(edit_count <= line_count);
        }
    }
    
    buffer_batch_edit(app, &buffer, str, len, edits, edit_count, BatchEdit_Normal);
    end_temp_memory(temp);
}

CUSTOM_COMMAND_SIG(nj_delete_rectangle)
CUSTOM_DOC("Deletes the range in a rectangular fashion."){
    nj_replace_rectangle_function(app, 0, 0);
}

CUSTOM_COMMAND_SIG(nj_replace_rectangle)
CUSTOM_DOC("Replaces the range in a rectangular fashion."){
    Query_Bar with;
    char with_space[1024];
    with.prompt = make_lit_string("With: ");
    with.string = make_fixed_width_string(with_space);
    
    if (query_user_string(app, &with)) {
        nj_replace_rectangle_function(app, with.string.str, with.string.size);
    }
}

CUSTOM_COMMAND_SIG(nj_write_arrow)
CUSTOM_DOC("Writes \"->\" under the cursor."){
    write_string(app, make_lit_string("->"));
}

CUSTOM_COMMAND_SIG(nj_toggler)
CUSTOM_DOC("Replaces the character under the cursor with a complementory character, for example replaces 0 with 1, replaces . with ->, replaces lowercase withh uppercase and vice versa etc."){
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    int32_t pos = view.cursor.pos;
    char c = buffer_get_char(app, &buffer, pos);
    switch(c){
        case '.': {
            buffer_replace_range(app, &buffer, pos, pos + 1, "->", 2);
        } break;
        
        case '-': {
            if(buffer_get_char(app, &buffer, pos + 1) == '>'){
                buffer_replace_range(app, &buffer, pos, pos + 2, ".", 1);
            }
            else
            {
                buffer_replace_range(app, &buffer, pos, pos + 1, "+", 1);
            }
        } break;
        
        case '+': {
            buffer_replace_range(app, &buffer, pos, pos + 1, "-", 1);
        } break;
        
        case '=': {
            if(buffer_get_char(app, &buffer, pos + 1) == '=')
            {
                buffer_replace_range(app, &buffer, pos, pos + 2, "!=", 2);
            }
            else if(buffer_get_char(app, &buffer, pos - 1) == '='){
                buffer_replace_range(app, &buffer, pos - 1, pos + 1, "!=", 2);
            }
            else if(buffer_get_char(app, &buffer, pos - 1) == '!')
            {
                buffer_replace_range(app, &buffer, pos - 1, pos + 1, "==", 2);
            }
            else if(buffer_get_char(app, &buffer, pos - 1) == '<')
            {
                if(buffer_get_char(app, &buffer, pos - 2) == '<')
                {
                    buffer_replace_range(app, &buffer, pos - 2, pos + 1, ">>=", 3);
                }
                else
                {
                    buffer_replace_range(app, &buffer, pos - 1, pos + 1, ">=", 2);
                }
            }
            else if(buffer_get_char(app, &buffer, pos - 1) == '>')
            {
                if(buffer_get_char(app, &buffer, pos - 2) == '>')
                {
                    buffer_replace_range(app, &buffer, pos - 2, pos + 1, "<<=", 3);
                }
                else
                {
                    buffer_replace_range(app, &buffer, pos - 1, pos + 1, "<=", 2);
                }
            }
        } break;
        
        case '!': {
            if(buffer_get_char(app, &buffer, pos + 1) == '=')
            {
                buffer_replace_range(app, &buffer, pos, pos + 2, "==", 2);
            }
        } break;
        
        case '<': {
            if(buffer_get_char(app, &buffer, pos - 1) == '<')
            {
                buffer_replace_range(app, &buffer, pos - 1, pos + 1, ">>", 2);
            }
            else if(buffer_get_char(app, &buffer, pos + 1) == '<')
            {
                buffer_replace_range(app, &buffer, pos, pos + 2, ">>", 2);
            }
            else if(buffer_get_char(app, &buffer, pos + 1) == '=')
            {
                buffer_replace_range(app, &buffer, pos, pos + 2, ">=", 2);
            }
            else
            {
                buffer_replace_range(app, &buffer, pos, pos + 1, ">", 1);
            }
        } break;
        
        case '>': {
            if(buffer_get_char(app, &buffer, pos - 1) == '-'){
                buffer_replace_range(app, &buffer, pos - 1, pos + 1, ".", 1);
            }
            else if(buffer_get_char(app, &buffer, pos - 1) == '>')
            {
                buffer_replace_range(app, &buffer, pos - 1, pos + 1, "<<", 2);
            }
            else if(buffer_get_char(app, &buffer, pos + 1) == '>')
            {
                buffer_replace_range(app, &buffer, pos, pos + 2, "<<", 2);
            }
            else if(buffer_get_char(app, &buffer, pos + 1) == '=')
            {
                buffer_replace_range(app, &buffer, pos, pos + 2, "<=", 2);
            }
            else
            {
                buffer_replace_range(app, &buffer, pos, pos + 1, "<", 1);
            }
        } break;
        
        case '0': {
            buffer_replace_range(app, &buffer, pos, pos + 1, "1", 1);
        } break;
        
        case '1': {
            buffer_replace_range(app, &buffer, pos, pos + 1, "0", 1);
        } break;
        
        case '|': {
            if(buffer_get_char(app, &buffer, pos - 1) == '|'){
                buffer_replace_range(app, &buffer, pos - 1, pos + 1, "&&", 2);
            }
            else if(buffer_get_char(app, &buffer, pos + 1) == '|'){
                buffer_replace_range(app, &buffer, pos, pos + 2, "&&", 2);
            }
            else
            {
                buffer_replace_range(app, &buffer, pos, pos + 1, "&", 1);
            }
        } break;
        
        case '&': {
            if(buffer_get_char(app, &buffer, pos - 1) == '&'){
                buffer_replace_range(app, &buffer, pos - 1, pos + 1, "||", 2);
            }
            else if(buffer_get_char(app, &buffer, pos + 1) == '&'){
                buffer_replace_range(app, &buffer, pos, pos + 2, "||", 2);
            }
            else
            {
                buffer_replace_range(app, &buffer, pos, pos + 1, "|", 1);
            }
        } break;
        
        case '\\': {
            buffer_replace_range(app, &buffer, pos, pos + 1, "/", 1);
        } break;
        
        case '/': {
            buffer_replace_range(app, &buffer, pos, pos + 1, "\\", 1);
        } break;
        
        case ' ': {
            buffer_replace_range(app, &buffer, pos, pos + 1, "_", 1);
        } break;
        
        case '_': {
            buffer_replace_range(app, &buffer, pos, pos + 1, " ", 1);
        } break;
        
        default: {
            if(char_is_upper(c))
            {
                char c_new[1] = {char_to_lower(c)};
                buffer_replace_range(app, &buffer, pos, pos + 1, c_new, 1);
            }
            else if(char_is_lower(c))
            {
                char c_new[1] = {char_to_upper(c)};
                buffer_replace_range(app, &buffer, pos, pos + 1, c_new, 1);
            }
        }break;
    }
}

CUSTOM_COMMAND_SIG(nj_increment_digit_decimal)
CUSTOM_DOC("Increment the digit under the cursor, when arriving to 9 loops back to 0."){
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    int32_t pos = view.cursor.pos;
    char c = buffer_get_char(app, &buffer, pos);
    if(char_is_numeric(c)){
        i32_4tech digit = hexchar_to_int(c)+1;
        while(digit >= 10) digit -= 10;
        char c_new[1] = {int_to_hexchar(digit)};
        buffer_replace_range(app, &buffer, pos, pos + 1, c_new, 1);
    }
}

CUSTOM_COMMAND_SIG(nj_decrement_digit_decimal)
CUSTOM_DOC("Decrement the digit under the cursor, when arriving to 0 loops back to 9."){
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    int32_t pos = view.cursor.pos;
    char c = buffer_get_char(app, &buffer, pos);
    if(char_is_numeric(c)){
        i32_4tech digit = hexchar_to_int(c)-1;
        while(digit < 0) digit += 10;
        char c_new[1] = {int_to_hexchar(digit)};
        buffer_replace_range(app, &buffer, pos, pos + 1, c_new, 1);
    }
}

CUSTOM_COMMAND_SIG(nj_increment_digit_hexadecimal)
CUSTOM_DOC("Increment the digit under the cursor, when arriving to 9 loops back to 0."){
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    int32_t pos = view.cursor.pos;
    char c = buffer_get_char(app, &buffer, pos);
    if(char_is_hex(c)){
        i32_4tech digit = hexchar_to_int(c)+1;
        while(digit >= 16) digit -= 16;
        char c_new[1] = {int_to_hexchar(digit%16)};
        buffer_replace_range(app, &buffer, pos, pos + 1, c_new, 1);
    }
}

CUSTOM_COMMAND_SIG(nj_decrement_digit_hexadecimal)
CUSTOM_DOC("Decrement the digit under the cursor, when arriving to 0 loops back to 9."){
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    int32_t pos = view.cursor.pos;
    char c = buffer_get_char(app, &buffer, pos);
    if(char_is_hex(c)){
        i32_4tech digit = hexchar_to_int(c)-1;
        while(digit < 0) digit += 16;
        char c_new[1] = {int_to_hexchar(digit)};
        buffer_replace_range(app, &buffer, pos, pos + 1, c_new, 1);
    }
}

CUSTOM_COMMAND_SIG(nj_open_matching_file_cpp_same_panel)
CUSTOM_DOC("If the current file is a *.cpp or *.h, attempts to open the corresponding *.h or *.cpp file in the current view."){
    uint32_t access = AccessAll;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    Buffer_Summary new_buffer = {0};
    if (get_cpp_matching_file(app, buffer, &new_buffer)){
        view_set_buffer(app, &view, new_buffer.buffer_id, 0);
    }
}

CUSTOM_COMMAND_SIG(nj_seek_end_of_file)
CUSTOM_DOC("Places the cursor at the end of the buffer."){
    uint32_t access = AccessProtected;
    
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    view_set_cursor(app, &view, seek_pos(buffer.size), 1);
}

CUSTOM_COMMAND_SIG(nj_seek_beginning_of_file)
CUSTOM_DOC("Places the cursor at the beggining of the buffer."){
    View_Summary view = get_active_view(app, AccessProtected);
    
    view_set_cursor(app, &view, seek_pos(0), 1);
}

CUSTOM_COMMAND_SIG(nj_backspace_line)
CUSTOM_DOC("Concatinates the current line with the one before it, adding a space between them."){
    seek_beginning_of_textual_line(app);
    backspace_char(app);
    
    write_string(app, make_lit_string(" "));
}


CUSTOM_COMMAND_SIG(nj_backspace_word)
CUSTOM_DOC("Delete characters between the cursor position and the first alphanumeric or cammel boundary to the left.")
{
    uint32_t access = AccessOpen;
    
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    if (buffer.exists){
        int32_t pos2 = 0, pos1 = 0;
        
        pos2 = view.cursor.pos;
        exec_command(app, seek_alphanumeric_or_camel_left);
        refresh_view(app, &view);
        pos1 = view.cursor.pos;
        
        buffer_replace_range(app, &buffer, pos1, pos2, 0, 0);
    }
}

CUSTOM_COMMAND_SIG(nj_delete_word)
CUSTOM_DOC("Delete characters between the cursor position and the first alphanumeric or cammel boundary to the right."){
    uint32_t access = AccessOpen;
    
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    if (buffer.exists){
        int32_t pos2 = 0, pos1 = 0;
        
        pos1 = view.cursor.pos;
        exec_command(app, seek_alphanumeric_or_camel_right);
        refresh_view(app, &view);
        pos2 = view.cursor.pos;
        
        buffer_replace_range(app, &buffer, pos1, pos2, 0, 0);
    }
}

CUSTOM_COMMAND_SIG(nj_cut_line)
CUSTOM_DOC("Cuts the line under the cursor."){
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    view_set_mark(app, &view, seek_line_char(view.cursor.line, 0));
    view_set_cursor(app, &view, seek_pos(seek_line_end(app, &buffer, view.cursor.pos) + 1), 1);
    
    exec_command(app, cut);
}

CUSTOM_COMMAND_SIG(nj_copy_line)
CUSTOM_DOC("Copies the line under the cursor."){
    uint32_t access = AccessProtected;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    view_set_mark(app, &view, seek_line_char(view.cursor.line, 0));
    view_set_cursor(app, &view, seek_pos(seek_line_end(app, &buffer, view.cursor.pos) + 1), 1);
    
    copy(app);
}


CUSTOM_COMMAND_SIG(nj_select_token_or_word)
CUSTOM_DOC("Select a single, whole token on or to the left of the cursor."){
    uint32_t access = AccessProtected;
    
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    int32_t pos1 = buffer_boundary_seek(app, &buffer, view.cursor.pos+1, 0, BoundaryToken | BoundaryWhitespace);
    int32_t pos2 = buffer_boundary_seek(app, &buffer, pos1,            1, BoundaryToken | BoundaryWhitespace);
    
    view_set_mark(app, &view, seek_pos(pos1));
    view_set_cursor(app, &view, seek_pos(pos2), 1);
}

CUSTOM_COMMAND_SIG(nj_snipe_token_or_word)
CUSTOM_DOC("Delete a single, whole token on or to the left of the cursor."){
    uint32_t access = AccessOpen;
    
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    int32_t pos1 = buffer_boundary_seek(app, &buffer, view.cursor.pos+1, 0, BoundaryToken | BoundaryWhitespace);
    int32_t pos2 = buffer_boundary_seek(app, &buffer, pos1,            1, BoundaryToken | BoundaryWhitespace);
    
    Range range = make_range(pos1, pos2);
    buffer_replace_range(app, &buffer, range.start, range.end, 0, 0);
}

CUSTOM_COMMAND_SIG(nj_cut_token_or_word)
CUSTOM_DOC("Cuts a single, whole token on or to the left of the cursor."){
    exec_command(app, nj_select_token_or_word);
    exec_command(app, cut);
}

CUSTOM_COMMAND_SIG(nj_copy_token_or_word)
CUSTOM_DOC("Copies a single, whole token on or to the left of the cursor."){
    exec_command(app, nj_select_token_or_word);
    exec_command(app, copy);
}

#endif // _MODE_NORMAL_CPP
